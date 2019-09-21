# IBM CPP11

[TOC]

#### `__cplusplus`

用于混用C与C++头文件。

```c++
#ifdef __cplusplus
extern "C"{// extern "C"可以抑制C++对函数名等名称进行重整
#endif
	/* codes */
#idef __cplusplus
}
#endif
```

整数值，以界定标准

```c++
#if __cplusplus < 201103L
		#error "Should use C++11 implementation."
#endif
```

#### 就地对非静态成员成员变量初始化

> 仍可以用`std::initializer_list`。

#### 外部模板以减少目标文件大小

```c++
// test1.cpp
#include "test.h"
template void fun<int>(int); // 显式地实例化
void test1() { fun(3); }

// test2.cpp
#include "test.h"
extern template void fun<int>(int); // 显式地实例化
void test1() { fun(3); }
```

#### 引用折叠规则

有`&`就是`&`，其他类型的引用组合都是`&&`；

因为引用折叠，完美转发可以写成：

```c++
template<typename _Tp> // Forward lvalue
    constexpr _Tp&&
    forward(typename std::remove_reference<_Tp>::type& __t) noexcept
    { return static_cast<_Tp&&>(__t); }

template<typename _Tp> // Forward rvalue
    constexpr _Tp&&
    forward(typename std::remove_reference<_Tp>::type&& __t) noexcept
    {
      static_assert(!std::is_lvalue_reference<_Tp>::value, "template argument"
		    " substituting _Tp is an lvalue reference type");
      return static_cast<_Tp&&>(__t);
    }
```

例子，写一个简单的对象池：

```c++
std::list<T*> cache; // not thread-safe

template <typename T, typename ... Args>
	T* acquire_obj(Args&& ... args)
{
  	if(! cache.empty())
    {
      	auto ret = cache.back();
      	cache.pop_back();
      	ret->reset(std::forward<Args>(args)...);
      	return ret;
    }
  	return new T(std::forward<Args>(args)...);
}
```

如果不走

#### inline namespace

将当前namespace的东西暴露在上层；方便版本控制；

```c++
namespace mlib
{
  inline namespace v1{
    /* use v1 */
  }
  namespace v2{ /* use v2 */}
}
```

#### SFINEA

```c++
#include <iostream>
#include <type_traits>

template <typename T, typename Enable = void>
class foo
{
public:
    foo(...)
    {
        static_assert(!std::is_void<Enable>::value, "Not allowed.");
    }
};

template <typename T>
class foo<T, std::enable_if_t<std::is_floating_point<T>::value>>
{
public:
    foo(...)
    {
        std::cout << "OK" << std::endl;
    }
};

int main()
{
    foo<double > f_d;
//    foo<int> f_i; // Compile error.
}
```

#### 对齐

> 个人还是喜欢`alignas`...

```c++
// GCC等编译器私货
#pragma pack(push, 4)
struct data
{
  // ...
}
#pragma pop()
```

```c++
// 标准

#include <iostream>
#include <memory>
 
template <std::size_t N>
struct MyAllocator
{
    char data[N];
    void* p;
    std::size_t sz;

    MyAllocator() : p(data), sz(N) {}
    template <typename T>
    T* aligned_alloc(std::size_t align_sz = alignof(T))
    {
        if (std::align(align_sz, sizeof(T), p, sz))
        {
            T* result = reinterpret_cast<T*>(p);
            p = (char*)p + sizeof(T);
            sz -= sizeof(T);
            return result;
        }
        return nullptr;
    }
};
 
int main()
{
    MyAllocator<64> a;
 
    // allocate a char
    char* p1 = a.aligned_alloc<char>();
    if (p1)
        *p1 = 'a';
    std::cout << "allocated a char at " << (void*)p1 << '\n';
 
    // allocate an int
    int* p2 = a.aligned_alloc<int>();
    if (p2)
        *p2 = 1;
    std::cout << "allocated an int at " << (void*)p2 << '\n';
 
    // allocate an int, aligned at 32-byte boundary
    int* p3 = a.aligned_alloc<int>(32);
    if (p3)
        *p3 = 2;
    std::cout << "allocated an int at " << (void*)p3 << " (32 byte alignment)\n";
}
```

内存对齐 -> cache friendly, SIMD...

[动态对齐内存分配](http://www.songho.ca/misc/alignment/dataalign.html)

[OpenCV内存分配](https://codeyarns.com/2017/02/28/aligned-memory-allocation/): 16字节对齐，并且保存一个PLUS空间(大小为一个指针的大小+对齐大小)

> > 这个alignment做的太诡异了….
>
> 一个`(void*)ptr`指向aligned mem，一块mem是以32byte为单位的，返回的ptr地址后5位被擦除，以展示是32byte对齐。
>
> free的时候直接`free( ((void**) ptr)[-1] )`
>
> > `[ptr[-1]]:[ptr->mem]`