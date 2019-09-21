# Template Notes

## TMP Overview

Template meta-programming (TMP) refers to uses of the C++ template system to perform computation at **compile-time** within the code. It can, for the most part, be considered to be "**programming with types**" — in that, largely, the "values" that TMP works with are specific C++ types. Using types as the basic objects of calculation allows the full power of the type-inference rules to be used for general-purpose computing.

#### Compile time programming

- Old way

```c++
#define myvar 17

#if myvar % 2
   cout << "Constant is odd" << endl;
#else
   cout << "Constant is even" << endl;
#endif
```

## Important points

- 人为指定的参数放前面，推导参数放后面；

```c++
template<typename Ret, typename Acc>
Ret c_style_cast(Acc val)
{
  return (Ret)(val);
}
// Acc是可推导类型，可推导类型模板参数放后面，人为指定的模板参数放在前面。
```

- 模板参数除了类型外（包括基本类型、结构、类类型等），也可以是一个整型数（Integral Number）。这里的整型数比较宽泛，包括<u>布尔、不同位数、有无符号的整型，甚至包括指针</u>（浮点数不可以）。
- `typename` 相当于声明一个"类型变量"；
- 但是模板也和宏有很大的不同，否则此文也就不能成立了。模板最大的不同在于它是“可以运算”的。

## type_traits

> 代码复杂度：对于一个函数而言：`count(if, while, for, &&, ||, ...)`。

#### 常量

`std::integeral_constant<type, VALUE>::type`

```c++
// 计算0!+1!+2!+...+n!
#include <iostream>
#include <type_traits>

template <int N>
struct factor : std::integral_constant<int, N * factor<N-1>::value> {};

template <>
struct factor<0> : std::integral_constant<int, 1> {};

template <int N>
struct factor_sum : std::integral_constant<int, factor<N>::value + factor_sum<N-1>::value> {};

template <>
struct factor_sum<0> : std::integral_constant<int, 1>{};

int main()
{
    std::cout << factor_sum<3>::value << std::endl;
}
```

对于一个模板类：

- 值类型`using value_type = T;`
- 模板类型`using type = XXX<T>;`

- `using std::true_type = std::integeral_constant<bool, true>`

- `std::is_base_of<BASE, DERIVED>`

```c++
// 在创建类型的时候得remove_reference
// 甚至remove_cv
template<typename T>
typename std::remove_cv<std::remove_reference<T>::type>::type* create()
{
    return new (typename std::remove_cv<std::remove_reference<T>::type>::type) ();
}

// 简化的写法是
template<typename T>
typename std::decay<T>::type* create() // 移除引用和cv符
{
  	return new (typename std::decay<T>::type)();
}
```

```c++
// 延迟计算：
template<typename F>
struct delay_func
{
		using func_type = std::decay<F>::type;
  	delay_func(F& f) : m_f(f){}
  	void run()
    {
      	m_f();
    }
  	func_type m_f;
}
```

- `std::conditional<TRUEorFALSE, A, B>` true的时候type是A，false的时候type是B；

- 返回调用类型的traits

```c++
template<typename F, typename Arg>
decltype(auto) Func(F f, Arg arg)
{
		return f(r);
}
```

- 可变参数模板：

折叠表达式必须出现在函数参数的末尾（这里的末尾并不是指模板末尾）；

```c++
template <typename ... Args> // ... T 打包
void print(Args ... args)    // T ... 拆包
{
    std::cout << sizeof...(args) << std::endl; // 打印参数个数
}
```

- 递归展开

```c++
void print()
{}

template <typename T, typename ... Args>
void print(T head, Args ... args)
{
    std::cout << "parameter: " << head << std::endl;
    print(args ... );
}
```

```c++
template<typename...> struct Tuple {};
template<typename T1, typename T2> struct Pair {};
 
template<class ...Args1> struct zip {
    template<class ...Args2> struct with {
        typedef Tuple<Pair<Args1, Args2>...> type;
//        Pair<Args1, Args2>... 是包展开
//        Pair<Args1, Args2> 是模式
    };
};
 
typedef zip<short, int>::with<unsigned short, unsigned>::type T1;
// Pair<Args1, Args2>... 展开成
// Pair<short, unsigned short>, Pair<int, unsigned int> 
// T1 是 Tuple<Pair<short, unsigned short>, Pair<int, unsigned>>
 
typedef zip<short>::with<unsigned short, unsigned>::type T2;
// 错误：包展开所含的参数包有不同长度
```

## codes

```c++
// TMP is functional programming.

#include <cstddef>
#include <iostream>
#include <vector>
#include <algorithm>

// Old cpp way. =====================
template <std::size_t X, std::size_t Y>
struct greatest_common_divisor
{
    static constexpr auto result = greatest_common_divisor<Y, X % Y>::result;
};

template <std::size_t X>
struct greatest_common_divisor<X, 0>
{
    static constexpr auto result = X;
};

template <std::size_t Y>
struct greatest_common_divisor<0, Y>
{
    static constexpr auto result = Y;
};

// Modern cpp way =====================
template <typename _Tp>
constexpr _Tp modern_greatest_common_divisor(_Tp x, _Tp y)
{
    return y == 0 ? x : modern_greatest_common_divisor(y, x % y);
}

// Left and right fold.
// if we combine the first element with the result of recursively combining the rest,
// this is called a right fold. instead, if we combine the result of recursively
// combining all elements but the last one, with the last element, this operation is
// called a left fold.

/*  fold expressions */
// C++17 fold expressions are implemented as so-called variadic templates (available since C++11)
// This arbitrary number of arguments is held in a so-called parameter pack.

// (... operator parampack)  -> left fold
// (parampack operator ...)  -> right fold
// (initvalue operator ... operator parampack) // left fold with an init value.
// (parampack operator ... operator initvalue) // right fold with an init value.

template <typename ... PACK> // left fold.
int subtract_fold(int minuend, PACK ... pack)
{
    return (minuend - ... - pack);
}

template <typename ... PACK>
bool all_true(bool lmost, PACK&& ... pack)
{
    return (lmost && ... && pack); // brackets are in need.
}

template <typename ... PACK>
bool some_true(bool lmost, PACK&& ... pack)
{
    return (lmost || ... || pack);
}

int main()
{
    std::cout << greatest_common_divisor<40, 10>::result << '\n';
    std::cout << modern_greatest_common_divisor(40, 10) << '\n';

    std::vector<int> numbers(10, 0);
    int temp = 0;
    std::generate(numbers.begin(), numbers.end(), [&temp]() noexcept { return ++temp; });
/*
template <typename Iter, typename Gen>
void generate(Iter beg, Iter end, Gen g)
{
    while(beg != end)
        *beg++ = g();
}
*/
    std::cout << subtract_fold(100, 50, 40, 10) << " is 0\n";
    std::cout << all_true(1, 1, 1, 1, 1, 1) << '\n';
    std::cout << some_true(0, 1, 0, 1, 0, 0, 0) << '\n';
}
```

