# ModernEffectiveCpp

[TOC]



## `()`和`{}`

> `{}`是统一**初始化**。

#### 为非静态成员指定默认初始化值

> 可用等号和`{}`，不可`()`.

```c++
struct foo
{
  int x = 0; // YES
  int y {1}; // YES
  int z (0); // NO
}
```

#### 不可复制对象的初始化

> 不可复制对象如`std::atomic`，可以用大小括号初始化，但不可以用`=`初始化。

```c++
std::atomic<int> ai1 { 0};
std::atomic<int> ai2 ( 0);
std::atomic<int> ai3 = 0;    // NO
```

> 上面两个例子就说明了大括号为什么是**统一初始化**了，初始化有多种方式，有的OK，有的不OK，大括号一定OK。

#### 禁止内建类型之间隐式窄化型别转换

```c++
int a; 
double b;

int k {a + b}; // 发生了隐式窄化型别转换，NO。
// 而()和=不会进行隐式窄化型别转换检查，为的是不破坏太多遗留代码；
// 【注意】：是大括号内不能有隐式转换，大括号到接受值之间还是可以有的。
	// int i {2.3}; // i 是 2

/* // You can do like this.
    int a = 1;
    double b = 2.1;
    int c {static_cast<int>(a+b)};
*/
```

#### 大括号可以免疫解析语法

> C++中规定，一切可以被解析为声明的东西都会被解析为声明。

这就意味着，一个类Foo，只要你写成`Foo()`，这就被当做一个函数声明。

```c++
Foo f(); // 被当做一个返回值为Foo的声明，而不是初始化一个对象；
thread t(Foo()); // Foo()代表的是Foo的callableObj，其被当做形参是Foo (*)()的函数声明了。
```

> **对于希望无参构造一个对象，应该首选`{}`进行构造。**注意，大括号做的是**初始化**，不能拿其作为非构造函数的函数中传入。(但在构造函数中还是可以这么做的)

#### 大括号和`std::initializer_list`

```c++
auto x = {1, 2};
// x的类型是std::initializer_list<int>
```

> 在没有`std::initializer_list<>`作为构造函数参数的情况下，`{}`和`()`的意义相同。

#### 结论

- 大括号初始化的语境最宽泛，可以阻止隐式类型转换，还能免疫解析语法。
- 一般来说尽量使用<u>大括号初始化</u>，当然得知道当前类是否使用了`std::initializer_list`作为初始化参数。
- 语义上大括号主要是1对1的初始化。

## 优先使用`nullptr`而非`NULL`

> 当C++在只能使用指针的时候，得到了一个int类型的0整数，会将其解释为空指针（NULL）；
>
> NULL可能被当做整数用，而nullptr不会。

#### `NULL`的不确定性

`NULL`具有整数型别，不具有指针型别。为什么这么说，因为NULL是一个宏，没有精确的类型。

NULL的定义有不清楚的成分，C++标准并没有规定NULL是用哪种类型的整数0。当一个函数有int类型的重载和`void*`类型的重载的时候，一个NULL会导致编译器不知道选哪个函数。

`nullptr`有精确的类型（但不具有指针型别）：`std::nullptr_t`。**可被隐式转换为任意型别的指针。**同时`nullptr`不会被视为任意一种整形变量，而NULL则会。

同样，在使用模板的情况下，NULL对应的模板类型会被推导为`int`(或其他的整型，取决于编译器的实现)

#### 总结

- 使用nullptr；
- 避免在整形和指针型别之间重载；



## 优先使用`using`而非`typedef`

#### 表达上更加符合直觉

```c++
using Func = void (*)(int, int); // 显然这种更加直观
typedef void (* Func)(int, int);
```

#### 支持模板别名

> 即给模板起别模板别名，这个`typedef`无法做到（除非声明一个`struct`再往里面加`type`）：

```c++
template <typename T>
using MyList = std::list<T, MyAlloc<T>>; 
// MyList是别名模板
// MyList<T>是类型，等同于std::list<T, MyAlloc<T>>.
// using 可用于类型之间和模板之间，模板到类型，只需要一个模板参数即可。
```

#### `typename`显示声明其后是一个类型

> C++规定，带依赖的型别前，必须加上一个`typename`；

考虑：

```c++
template<T> class Foo
{
		typename T::SubType* ptr;
}
```

> `模板参数::xxx`就是"带依赖的型别"；这里的`typename`是为了表示，`ptr`是一个类型，而不是一个变量。如果去掉`typename`，`T::SubType`会被当做一个静态成员。

> 对应到上文的`MyList<T>`，是一个<u>"不带依赖的型别"</u>（用别名模板来定义的）。

```c++
std::remove_const<T>::type;
// 等价于
std::remove_const_t<T>;
// 实现
template<T> using remove_const_t = typename remove_const<T>::type;
```

#### 总结

- `typedef`不支持模板化，而`using`可以；
- 善用`using`可以避免在类中加`type`的操作，因为`using`产生的直接是<u>"不带依赖的型别"</u>的型别；
- `typename`显示声明其后是一个类型，带依赖的型别前（`XX::xx`），必须加上一个`typename`；



## 优先使用**限定作用域**的枚举类别(`enum class`)

#### 区别

- `enum class` 使用限定作用域的枚举类别
- `enum` 使用非限定作用域的枚举类别

一般来说，在`{}`里面的东西，`{}`外是没法直接访问的，但这一条不适用于C++98的`enum`，即`enum`的变量，`{}`可直接访问，不需要加**作用域前缀**。

此外，`enum class`可以前置声明(只声明不定义)，而`enum`声明时必须定义；(增加了对编译的又好性)

`enum`可以自己确定较优的存储空间，而`enum class`默认是`int`，也可以自己定义：

```c++
enum class Foo : std::uint32_t
{
	good = 1;
	bad = 0; // 值也可以自己确定。
};
```

#### 不会隐式型别转换

`enum class `对应的整形要转为其他类型的时候，必须用`static_cast<>()`。

而enum可能进行隐式型别转换；

#### 还是有可以用到`enum`的地方

```c++
using Info = std::tuple<int, double, char>;
//                      id    score   sex

Info info;
auto val = std::get<1>(info); // 取得info的第二个元素

// Better approach

enum InfoField{ id, score, sex};

Info info;
auto val = std::get<score>(info);
// enum可以进行`隐式型别转换`，转为std::size_t;
// 这里如果用enum class会比较麻烦（要显式转换）。
```



## 优先使用`delete`而非设置private

删除一个类的默认拷贝/构造函数，c++98的做法是设置private，而到了c++11的时候，应该使用`delete`关键字。

#### More than private

在进行模板编程的时候，我们可以使用特例化的技巧保证只有我们期望的类型可以被编译：

```c++
// 只想让std::ostream的子类被初始化
template <typename T, typename Enable = void> class Foo
{
  	Foo(...)
    {
	      static_cast(!std::is_void<Enable>::value, "This cannot be initialized");
    }
}

template <typename T> 
class Foo<T, std::enable_if_t<std::is_base_of<std::ostream, T>::value>>
{
		// Do sth.
} 
```

```c++
template <typename T>
void foo(T);


// 删除特定类型
template <>
void foo<double>(double) = delete;

template <>
void foo<int>(int) = delete;

// --------------------------------

bool ge0(int x)
{
  	return x > 0;
}
// 但这个代码会存在隐式转换，我们可以用delete消除隐式转换

bool ge0(char) = delete;
bool ge0(double) = delete; // float也会被删除，因为float优先转double.
```

## 改写必加`override`

#### 防止编译器不检查

```c++
class Foo
{
  	virtual void doit();
}

class Bar : Foo
{
		virtual void doit(); // virtual 可忽略
}
```

Bar的doit可以改写Foo的doit以实现运行时多态，但前提是：

- 基类和子类对应的虚函数的<u>名字，类型，参数形参型别，const性，返回值，异常规格，引用饰词</u>都得一样。

> 要严格保证其一致没那么容易，而编译器并不保证一定对你类似的相位进行检查，有的编译器开`-Wall`也没用。(`clang++7.0.1`还是很智能，会提醒)

```c++
class Foo
{
    virtual void doit(int x);
};

class Bar : Foo
{
    virtual void doit(int x) const; // virtual 可忽略
};

int main()
{

}

/*

clang++ 7.0.1 :
warning: 'Bar::doit' hides overloaded virtual function [-Woverloaded-virtual]

*/
```

那么加工override后，就表明了我们的意图：**重写**，此时如果找不到被重写的函数的基类函数的话，就会报错。

```c++
class Foo
{
    virtual void doit(int x);
};

class Bar : Foo
{
    virtual void doit(int x) override; // virtual 可忽略
};

int main()
{

}
```

#### 函数类型

```c++
// > 左值指的是可以获取内存的变量
class Vec
{
  	// A: 当对象是左值，则调用此函数
  	dataType& data() &
    { return values; } // 用于赋值
  	
  	// B: 当对象是右值，则调用此函数
  	dataType data() &&
    { return std::move(values); } // 用于把自身数据move给对方
}

/*

Vec v;

auto d1 = w.data(); // A

auto d2 = Vec().data(); // B

*/
```



## 优先选用`const_iterator`

> 如题即可：

```c++
auto it = std::find(v.cbegin(), v.cend(), value); // 返回一个iterator
```

- 对于最通用的代码，使用非成员函数的`begin()`和`cbegin()`等。

> 在C++11中没有`cbegin()`的非成员函数版本，C++14才添加，但在C++11的代码中可以这么写。

```c++
template<Container C>
auto cbegin(const C& container) -> decltype(std::begin(container))
{
  	return std::begin(container);
}
```

## 为不会发生异常的函数加上`noexcept`

> `noexcept` 运算符进行编译时检查，若表达式声明为不抛出任何异常则返回 true 。

```c++
int f() throw();  // C++98 : 开解至f的调用方，在执行一些动作后，程序终止；
int f() noexcept; // C++11 : 程序终止之前，栈只是可能会被开解；
```

> 带有`noexcept`的函数，优化器可以再异常传出之前，保持栈的开解状态；也不需要在异常溢出的前提下，保证所有对象以构造顺序的逆序进行析构。

> 因为在调用noexcept函数时不需要记录exception handler所以编译器可以生成更efficient的code（但实际编译器是否优化noexcept不一定，但理论上noexcept给了编译器更多优化的机会）。另外编译器在编译一个非noexcept的函数时有可能会生成<u>很多冗余的代码</u>，这些代码虽然只在出错的时候执行，但还是会对instruction cache造成影响，进而影响程序整体的performance。

```c++
// 在未加noexcept的时候，如果a在构造的时候发生了异常，a会被安全析构；
// 但也会新生成一些代码去调用a的析构函数；
void foo
{
		A a;
		do_with(a);
}
```



- `noexcept`意味着有更多的被优化的机会；
- **每个函数都考虑noexcept会很麻烦，只在明显的时候使用**；
- `noexcept`函数在出现异常的情况（不管是当前函数还是调用的函数）下直接`std::terminate`；
- `noexcept`在`move`, `swap`, 释放和析构函数处最有价值；
- `noexcept`在移动构造函数上很有用。如果移动构造函数不是`noexcept`的话，那么很多时候，逻辑上可以move，最后还是copy了。
- 对于有`alloc`的成员，默认不用`noexcept`。
- move constructor/assignment operator 如果不会抛出异常，一定用noexcept。
- destructor一定用noexcept。

> 因为如果destructor抛出异常，程序99%会挂掉，你用noexcept基本没任何坏处。

> `noexcept`主要是给底层库作者抠性能用的，业务代码过多使用`noexcept`很有可能导致不易修改的缺点。

- <u>像`vector`这样的容器，如果其装载的对象的移动构造函数不是`noexcept`的时候，`push_back`等函数并不会移动，而是拷贝。</u>

```c++
template<typename T, size_t N>
void swap(T (&a)[N], T (&b)[N]) noexcept(noexcept(swap(*a, *b)))
{
  	// ...
}
```



## 尽量使用`constexpr`

- 使用于函数和变量；
- 对于`constexpr`函数，输入一个编译期常量表达式，返回一个编译期常量表达式。输入运行时值，则输出运行时值。

> C++11不支持`constexpr void` ，而C++14支持。

- `constexpr`对象都具备`const`属性；
- 对于使用到了堆内存(new过的)的对象，永不能使用`constexpr`；

> 目前来说`std::vector`不能是`constexpr`，但以后可能是(C++20/23)。
>
> 关于一个对象是不是`constexpr`，主要看其构造函数是否是`constexpr`的。



## 保证const成员函数的线程安全性

> 考虑缓存计算的情况；

- 对于const成员，其中可能会发生改变的量，要加上`mutable`关键词；
- 对于单个变量或内存区域的操作，使用`std::atomic`较好（性能）.
- 而其他的情况，可能还是得使用`std::mutex`；(含锁的对象只可move，不可copy)。



## 特种成员函数的生成机制

- 特种成员函数：默认构造函数，析构函数，复制构造函数，复制赋值函数，移动构造函数，移动赋值函数；
- 移动操作是：可移动的部分移动，对于无法移动的型别部分将通过复制完成"移动"；
- 复制操作是独立的，声明了复制构造函数，不声明复制赋值函数，编译器会自动给生成，反之亦然；
- 移动操作是彼此不独立的，两者声明其一，编译器便不会再帮忙合成另外一个；

> 理由是这样的，编译器生成的移动构造函数和移动赋值函数的功能是一样的，自己生成的也应该是一样的。一旦声明其一就意味着"我的移动构造/赋值和编译器的意图不一样"，那么如果编译器再给合成一个"与预期意图不一样的函数"是没必要的。

- 一旦显示声明了复制操作，该类便不再生成移动操作;

> 依据是：一旦显示声明了复制操作，就说明，默认的"按成员复制"的操作不符合预期意图，那么这样，编译器也将热内"按成员移动"会不符合预期意图，遂不再自动生成；

- 一旦显示声明了移动操作，该类不再生成复制操作；

#### Rule of 3

根据上面的各种情况，出现了一种指导原则叫三法则：

如果要显示定义，下面三者应该同时被显示定义：

- 析构函数；
- 复制构造函数；
- 复制赋值函数；

> 一般来说，这种情况意味着该类要在复制和析构的时候对资源进行管理。STL中的很多动态容器都是符合三定律的；
>
> **推论**：如果用户声明了析构函数，就不会生成移动操作，但仍可以生成复制操作；
>
> 这是为了兼容98的代码。

#### 移动操作的生成条件

- 该类未声明任何复制操作
- 该类未声明任何移动操作
- 该类未声明任何析构函数

#### `=default`支持默认的结果

```c++
class Base
{
public:
  	virtual ~Base() = default;		// 虚析构
  	Base(Base&&) = default;				// 移动支持
  	Base& operator=(Base&&) = default;
  
  	Base(const Base&) = default;
  	Base& operator=(const Base&) = default;
}
```

## 理解`std::move`和`std::forward`

- 上述两个操作都只是编译时的类型转换；
- 保持常量的正确性(对于想要移动的东西不要声明为常量)

```c++
void foo(const string& txt)
{
  	auto str = std::move(txt); 
  	// 不是移动，而是拷贝，因为移除reference后还有const，这样的类型会被拷贝赋值函数调用。
  	// ...
}
```

- `std::forward()`源码

```c++
template <typename T>
	void relay(T&& t)
{
  // lref: T -> lref, t -> rref !!! => that's what perfect forwarding is solving.
  // rref: T -> rref, t -> rref
  	foo(std::forward<T>(t));
}
```



```c++
// 上面的代码具体意思就是如果我们传进来的T为左值类型，则返回的类型为左值，如果我们传进来的类型为右值类型，则返回的类型为右值。

template<typename T>
constexpr T&& forward(std::remove_reference_t<T>& param) noexcept
{// T was lref, T&& is also lref.
  	return static_cast<T&&>(param);
}
// C++不允许reference to reference（引用的引用），所以会进行引用折叠
// 即A_ref B_ref中，任意一项为左值引用，那么最后就是左值引用，否则右值引用：
// int& && 和int&& &都是左值引用，int && && 才是右值引用。

template<typename T>
constexpr T&& forward(std::remove_reference_t<T>&& param) noexcept
{// T was rref, T&& is also rref.
  	static_assert(std::is_lvalue_reference<T>::value, ""template argument"
		    " substituting _Tp is an lvalue reference type"");
  	return static_cast<T&&>(param);
}
```

- move是强制将类型转化为右值类型，而forward是有条件地进行转换。

- 一般与变长参数模板使用；
