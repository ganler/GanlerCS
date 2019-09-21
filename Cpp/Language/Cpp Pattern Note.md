# Cpp Pattern Note

[TOC]

## Copy elements from a range to another range or container

#### 预备知识

- `std::copy`
- `std::copy_if`

```c++
template< class InputIt, class OutputIt >
constexpr OutputIt copy( InputIt first, InputIt last, OutputIt d_first );
```

- `std::back_inserter`

```c++
template< class Container >
std::back_insert_iterator<Container> back_inserter( Container& c )// before c++ 20;
{
		return back_insert_interator<Container>(c);
}
// c为支持push_back的容器；
```

```c++
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <iostream>

int main()
{
    std::vector<int> v(10);                     // 建立长度为10的空数组
    std::iota(v.begin(), v.end(), 1);           // 按1~10初始化
    std::fill_n(std::back_inserter(v), 3, 6);   // 在v的back后再初始化3个6
                                                // 相当于进行了3次push_back(6)

    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';
}
```

> `std::fill(v.begin(), v.end(), val)` 在begin~end处用val初始化
>
> `std::fill_n(iter, times, val)` 在iter后times次初始化val

#### 开始

```C++
std::vector<int> add = {6, 6, 6};

std::vector<int> base(10);
std::iota(base.begin(), base.end(), 1);

// 将add加再base后
std::copy(add.begin(), add.end(), std::back_iterator(base));
```

## Count occurrences of value in a range

```c++
std::vector<int> nums(10, 3);
std::cout << std::count(nums.begin(), nums.end(), 3) << std::endl;
```

## Swap contianers

```c++
#include <vector>
#include <iostream>
#include <utility>

int main()
{
    std::vector<int> vec_a(10, 1);
    std::vector<int> vec_b(5, 0);

    std::swap_ranges(vec_b.begin(), vec_b.end(), vec_a.begin());
    std::copy(vec_a.begin(), vec_a.end(), std::ostream_iterator<int>(std::cout, " "));

    // vec_a.swap..
    // std::swap...
}
```

## Rule of 3/5([ref](https://zh.cppreference.com/w/cpp/language/rule_of_three))

```c++
#include <utility>
#include <type_traits>
#include <iostream>

template <class T>
class lrznb
{
public:
    // types:
    using value_type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
    using size_type = size_t;
    using pointer = T*;
    using type = lrznb<value_type >;
private:
    value_type* m_data;
    size_type m_size;
    lrznb(const T* __raw, std::size_t __sz)
    : m_data(new value_type[__sz]), m_size(__sz)
    {
        std::memcpy(m_data, __raw, __sz);
    };
public:
    lrznb()
    : m_data(new value_type{}), m_size(0)
    {};
    lrznb(const T* __raw)
    : lrznb(__raw, (sizeof(__raw)-std::is_same<value_type, char>::value)/ sizeof(T))
    {}
    lrznb(size_type __sz)
    : m_data(new value_type[__sz]), m_size(__sz)
    {};
    lrznb(const lrznb& __nb)
    : lrznb(__nb.m_data, __nb.m_size)
    {};
    lrznb(lrznb&& __nb) noexcept
    : lrznb(std::exchange(__nb.m_data, nullptr))
    {}

    size_type size() const noexcept
    {
        return m_size;
    }

    lrznb& operator= (lrznb __nb)
    {
        std::swap(m_data, __nb.m_data);
        std::swap(m_size, __nb.m_size);
        return *this;
    }

    void swap(type& __nb) noexcept(std::is_nothrow_swappable_v<value_type>)
    {
        using std::swap;
        if(this != &__nb)
        {
            swap(this->m_data, __nb.m_data);
            swap(this->m_size, __nb.m_size);
        }
    }
    friend inline void swap
    (type& __ljw, type& __lrz) noexcept(std::is_nothrow_swappable_v<value_type>)
    {
        __ljw.swap(__lrz);
    }
    void print()
    {
        for(size_t i = 0; i < m_size-std::is_same<value_type, char>::value; ++i)
            std::cout << m_data[i] << ' ';
        std::cout << std::endl;
    }
};

namespace std
{
    template <typename T>
    void swap(lrznb<T>& __ljw, lrznb<T>& __lrz)
    {
        __ljw.swap(__lrz);
    }
}

int main()
{
    lrznb lrz("lrznbb");
    lrznb ljw("ljwljj");
    std::swap(ljw, lrz);
    std::cout << "ljw say: ";
    ljw.print();
    std::cout << "lrz say: ";
    lrz.print();
}
```



```c++
#include <utility>

/*
 [三法则]：拷贝构造函数、复制运算符和析构函数。
 // 通过可复制柄管理不可复制资源的类，必须声明复制赋值和复制构造函数为私有并不提供其定义，或定义它们为被删除。
 // 这是另一种三规则：删除一者并保留另一者为隐式定义很可能导致错误。
 [五法则]：在此基础上+移动构造函数和移动赋值函数。
 */


class rule_of_three
{
    char* cstring;
    rule_of_three(const char* s, size_t sz) : cstring(new char[sz])
    {// 直接拷贝
        std::memcpy(cstring, s, sz);
    }

public:
    rule_of_three(const char* s = "")
    : rule_of_three(s, std::strlen(s) + 1)
    {}
    rule_of_three(const rule_of_three& other)
    : rule_of_three(other.cstring)
    {}
    ~rule_of_three()
    {
        delete []cstring;
    }
    rule_of_three& operator=(rule_of_three other)
    {
        std::swap(cstring, other.cstring);
        return *this;
    }
};

class rule_of_five
{
    char* cstring;
    rule_of_five(const char* s, size_t sz) : cstring(new char[sz])
    {
        std::memcpy(cstring, s, sz);
    }
public:
    rule_of_five(const char* s = "")
    : rule_of_five(s, std::strlen(s) + 1)
    {}
    rule_of_five(const rule_of_five& other)
    : rule_of_five(other.cstring)
    {}
    rule_of_five(rule_of_five&& other) noexcept
    : cstring(std::exchange(other.cstring, nullptr)) // 返回前者
    {}
    rule_of_five& operator=(rule_of_five other)
    {
        std::swap(cstring, other.cstring);
        return *this;
    }
  	friend void swap(rule_of_five& first, rule_of_five& second)
    {
      	std::swap(first.p, second.p);
    }
};
// 多用0规则吧
```

## Delegate behavior to derived classes

使用CRTP达到静态多态。[[More stuff for reading](https://accu.org/index.php/journals/296)]

> This approach avoids the costs associated with using `virtual` functions for run-time polymorphism, typically implemented with a [virtual function table](http://en.wikipedia.org/wiki/Virtual_method_table) (a dynamic dispatch mechanism).

```c++
#include <iostream>

// 基类实现public使用接口do_sth，隐藏(private)子类的实现do_sth_impl
// 子类将do_sth_impl实现并且置于public，方便基类的do_sth可调用

template <typename derived>
class base
{
    void do_sth_impl() {}
public:
    void do_sth()
    {
        static_cast<derived*>(this)->do_sth_impl();
    }
};

class foo : public base<foo>
{
public:
    void do_sth_impl()
    {
        std::cout << "I\'m foo" << std::endl;
    }
};

class bar : public base<bar>
{
public:
    void do_sth_impl()
    {
        std::cout << "I\'m bar" << std::endl;
    }
};

template <typename derived>
void do_it(base<derived>& d)
{
    d.do_sth(); // 后面有static_cast所以不能用const
}

int main()
{
    foo f;
    bar b;
    do_it(f);
    do_it(b);
}
```

## Lexicographic ordering

```c++
#include <utility>
#include <iostream>

class three_nums
{
public:
    three_nums(int a, int b, int c)
    : x(a), y(b), z(c)
    {}

    friend bool operator < (const three_nums& l, const three_nums& r)
    {
        return std::tie(l.x, l.y, l.z) < std::tie(r.x, r.y, r.z);
    }

private:
    int x;
    int y;
    int z;
};

int main()
{
    three_nums p = {1, 2, 3};
    three_nums q = {1, 2, 4};
    if(p < q)
        std::cout << "q is greater" << std::endl;
    else
        std::cout << "p is greater" << std::endl;
}
```

## ADL

可以简单地说：如果一个函数没有被名字空间修饰，那么可以把它称作是**未限定的（unqualified）**。

当一个函数是未限定的的时候，**编译器会根据实参的类型去其所在的名字空间里面去找对应的函数原型！**

当然，这条**名字**查找规则适用于所有**未限定的**函数！

```c++
#include <iostream>

namespace ns
{
    struct Foo
    {
        void say()
        {
            std::cout << "Balabala" << std::endl;
        }
    };

    void help_foo_say(Foo& f)
    {
        f.say();
    }
}

/*
我们在ns中声明了class Foo和函数help_foo_say
首先我们总结一下：
- Foo是ns命名空间的
- help_foo_say是ns命名空间的
*/

int main()
{
    ns::Foo f; // 合情合理
    help_foo_say(f); 
  	// 为什么这个函数不用加ns::呢？因为c++的ADL(参数依赖查找)机制规定，对于当前命名空间内的函数，除了会在当前命名空间中寻找函数存在，还会在参数所在的命名空间内寻找。
  	// 使用help_foo_say的时候，我们在“全局域”，而参数f是ns命名空间内的，所以它就会在ns和全局域查找。
  	// 如果在全局域再设置个help_foo_say函数会因为ambiguous的原因报错。
  	// 为什么要这么做呢，大家想想std::cout << 的“<<”，如果没有这个机制，我们就得
/*
#include <iostream>

int main()
{
    std::operator<<(std::operator<< (std::cout, "Hello World"), "\n");
}
*/
    // 嗯如果是这样的话，现在我们就不会知道c++这门语言存在过了。
}
```

## PIMPL

> 问题：**c++的make是根据文件时间戳来的**，假设我有一个`A.hpp`和3个`.cpp`：`A.cpp, B.cpp & C.cpp`，这几个`.cpp`都include了`A.hpp`，这就意味着，我们一旦想修改`A.cpp`，给其增加一个private成员，我们同事要修改A.hpp。而B，C因为用不到其私有成员，所以讲道理我们不用编译它，然而，“c++的make是根据文件时间戳来的”，这就意味着，一旦`.hpp`在上一次和A，B，C一同编译的过程中对应的时间戳不一样，我们就需要重新编译ABC，三个文件还行，如果是几千个文件那就麻烦了。
>
> 我们希望不去动`A.hpp`，而只是修改`A.cpp`，那么我们可以对其私有/保护成员定义一个结构体，并且用智能指针指向这些结构体。这样，我们以后只需要在`.cpp`中对私有/保护成员进行操作即可。
>
> [推荐阅读](https://goodspeedlee.blogspot.com/2016/01/c-pimpl.html)

#### 优点

Remove compilation dependencies on internal class implementations and improve compile times.

```c++
// Foo.hpp
#include<memory>

class Foo
{
public:
    Foo();
    ~Foo();
    Foo(Foo&&) noexcept;
    Foo& operator=(Foo&&) noexcept;

private:
    struct impl;
    std::unique_ptr<impl> pimpl;
};

// Foo.cpp

Foo::Foo() : pimpl(std::make_unique<impl>()) {} // Important.
Foo::~Foo() = default;
Foo::Foo(Foo &&) noexcept = default;
Foo& Foo::operator=(Foo&&) noexcept = default;

struct Foo::impl
{// do what you want in Foo.cpp and ignore Foo.hpp.
    int data;
    bool happy;
};
```

## Visual constructor

> virtual构造函数是不允许的，因为“**<u>构造函数是在对象完全构造之前运行的</u>，在构造函数运行的时候，对象的动态类型还不完整 - - -《C++ Primer》**”，无法实现多态。
>
> 虚拟构造器是一种机制，通过此机制可以实现虚函数的动态绑定的多态功能，而这种机制正是设计模式中的工厂模式：通过传入不同类型的参数构造产生出不同类型的对象。

```c++
class Base{
public:
		virtual Base* clone() const = 0;
  	virtual ~Base(){};
}

class Derived_2 : public Base
{
public:
		Derived* clone() const override
		{
				return new Derived_2(*this);
		}
}

class Derived_1 : public Base
{
public:
		Derived* clone() const override
		{
				return new Derived_1(*this);
		}
}

void foo(std::unique_ptr<Base> ori)
{
  	std::unique_ptr<Base> copy{ ori->clone() };
	  // ...
}
```

## Create a thread

```c++
#include <iostream>
#include <thread>
#include <string_view>
#include <functional>
#include <string>

using namespace std;

void say_what(string_view __x)
{
    cout << __x << endl;
}

int main(){
    string str = "hello world";
    thread t1{say_what, ref(str)}; // Use ref or cref.
  // A thread must be either joined or detached before destruction, or std::terminate will be called.
    t1.join();
}

// Or
#include <iostream>
#include <future>

using namespace std;

int num1()
{
    return 1;
}

int main(){
    future<int> ans = async(num1);
    cout << ans.get() << endl;
}
```

## Check existence of a key

```c++
#include <map>
#include <iostream>

using namespace std;

int main()
{
		map<char, int> _map = {{'a', 1}, {'b', 2}};
  	cout << ((_map.count('a')) ? "existed" : "not existed") << endl; // 返回_map中'a'元素的数量，对unordered系列，复杂度为常数。
}
/*
#include <unordered_set>
#include <iostream>

using namespace std;

int main()
{
    unordered_set<char> _map = {'a','b'};
    cout << ((_map.count('a')) ? "existed" : "not existed") << endl;
}
*/
```

## Erase and remove

```c++
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<int> v = {1, 2, 4, 2, 4, 5, 2, 4};
    v.erase(
            remove(v.begin(), v.end(), 2),
            v.end()
            );
    for(auto x : v)
        cout << x << ' ';
    cout << endl;
}
```

> `remove_if`有更多的个性化操作。后面可以接一个lambda.

## optional

> 当你想在一个类型多加一个表示"nothing"的值。

```c++
#include <optional>
#include <iostream>
#include <string>

std::optional<std::string> magic(bool tx)
{
    if(tx)
        return "lrznb\n";
    return {};
}

int main()
{
    std::cout << sizeof(std::optional<uint16_t>) << std::endl;
    std::cout << magic(true).value_or("empty\n");
    std::cout << magic(false).value_or("empty\n");
  	// 取值和指针类似
/*
		auto x = magic(true);
		std::cout << *x;
		x.reset(); // destroy it.
		// make_optional
		// 对于“nothing”的返回值，可用std::optional::nullopt作为比较项；
*/
}
```

> 缺点是存储空间翻倍。

## Dynarray

> 与vector不同，dynarray是fixed size的allocator。

## Read line-by-line

```c++
#include<string>
#include<iostream>
#include <sstream>

int main()
{
    std::istringstream stream("a\n"
                              "hello\n"
                              "world\n"); // 看作cin一样的东西即可
    std::string line;
    while(std::getline(stream, line))
    {
        std::cout << line << std::endl;
    }
}
```



## Read a line of values

```c++
#include <vector>
#include <sstream>
#include <iterator>
#include <iostream>

int main()
{
    std::istringstream stream("1 2 3 4");
    std::vector<int> vec;
    std::copy(std::istream_iterator<int>{stream},
              std::istream_iterator<int>{}, // 默认构造函数意味着stream的结尾
              std::back_inserter(vec));
    // Copy接受的都是iterator
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(std::cout, " "));
}
```



## Overload operator `<<`

```c++
#include <iostream>

class foo
{
private:
  	int x;
public:
  	// 为了重载函数能读到foo的内部成员
  	friend ostream& operator << (std::ostream&, foo const&);
};

std::ostream& operator << (std::ostream& stream, foo const& f)
{// return type.
  	return stream << "Hello : " << f.x;
}
```



## Weak reference to break circular dependencies

```c++
#include <iostream>
#include <memory>

using namespace std;

class Foo;
class Bar;

class Foo
{
public:
    Foo(const shared_ptr<Bar>& sptr) : bar_sptr{sptr}{}
private:
    shared_ptr<Bar> bar_sptr;
};

class Bar
{
public:
    Bar(const weak_ptr<Foo>& wptr) : foo_wptr{wptr}{}

    void do_sth()
    {
        if(!foo_wptr.expired()) // 没有被销毁
            auto sptr = foo_wptr.lock(); // 使用对应的shared_ptr
    }

private:
    weak_ptr<Foo> foo_wptr;
};
```

> 避免循环引用的方法就是将其中之一设置为`std::weak_ptr`。

## Choose a random element

```c++
#include <iostream>
#include <vector>
#include <random>

using namespace std;

int main()
{
    // Random device
    // Random engine
    // Random distribution

    vector<int> vec(10);
    iota(vec.begin(), vec.end(), 0);

    random_device device;
    mt19937 engine{device()};
    uniform_int_distribution<int> range(0, vec.size()-1);

    for (int i = 0; i < 5; ++i) {
        cout << vec[range(engine)] << endl;
    }
}
```

## Flip a coin

```c++
#include <iostream>
#include <random>

using namespace std;

int main()
{
    // Random device
    // Random engine
    // Random distribution

    random_device device;
    mt19937 engine{device()};

    bernoulli_distribution coin{0.3};

    cout << (coin(device) ? "I love you" : "I hate you") << endl;
}
```



## Seed sequence

```c++
#include<iostream>
#include<random>
#include <vector>

using namespace std;

int main()
{
    random_device rd;
    seed_seq seq{rd(), rd(), rd(), rd(), rd()};
    vector<uint32_t > vec(10);
    seq.generate(vec.begin(), vec.end());
    for(auto x : vec)
        cout << x << ' ';
}
```



## SFINAE

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

```c++
#include <iostream>
#include <type_traits>
#include <cmath>

template <typename T>
std::enable_if_t<std::is_integral<T>::value, bool> equal(T lhs, T rhs)
{
    return lhs == rhs;
}

template <typename T> /* lhs: left hand side */
std::enable_if_t<std::is_floating_point<T>::value, bool> equal(T lhs, T rhs)
{
    return std::fabs(lhs - rhs) < 1e-5;
}

int main()
{
    std::cout << equal(1, 1) << std::endl;
    std::cout << equal(1.0000001, 1.) << std::endl;
}
```

