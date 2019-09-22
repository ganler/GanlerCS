# Variant

> `std::variant` is a library addition in C++17 for sum types, and `std::visit` is one of the ways to process the values in a `std::variant`.

### Problems of Union

- The biggest one is that they don’t respect object lifetimes in the sense that <u>constructors or destructors are not called when you change the contained type</u>.
- Undefined behavior when viewed in a wrong perspective.

### Safer -> Variant

- Using `std::get<>` to get it in compile time.

- If `std::get<>` is used mistakenly, u'll receive an exception.

#### How it works

> `std::variant` stores some information about <u>the currently “active”</u>(Using *index*) type. While that means it needs some extra memory (a byte or so), this extra memory is well spent. In the interface of `std::variant`, the *index* is the number that defines which of the alternative types are stored in the variant.

> Of course, it can be accessed by index like `std::get<I>`;

#### Usage

```c++
int main() {
  using MyVariant = std::variant<std::vector<int>, std::vector<double>>;

  //construct the vector<int> in place with an initializer list
  // Initialize the first type.
  MyVariant v{std::in_place_index_t<0>{}, {1, 2, 5, 13}};
  auto numbers = std::get<0>(v);

  //replace with a vector<double> with the same values
  v.emplace<1>(std::begin(numbers), std::end(numbers));
}
```

#### Access by type

- `std::get<T>`
- `.emplace<T>`
- `std::in_place_type_t<T>`
- free func: `std::holds_alternative<T>`

#### Visit

```c++
// Bad usage.
using MyVariant = std::variant<A, B, /*and more...*/>;
std::ostream& operator<< (std::ostream& os, MyVariant const& v) {
  switch(v.index()) {
    case 0:
      os << std::get<0>(v);
      break;
    case 1:
      os << std::get<1>(v);
      break;
    // and so on...
  }
  return os;
}
```

```c++
// good
std::ostream& operator<< (std::ostream& os, MyVariant const& v) {
  std::visit([&os](auto const& e){ os << e; }, v);
  return os;
}
```

#### New Polymorphism —— inherited from variant

```c++
// 

#include <variant>
#include <iostream>

class t1
{
public:
    void print()
    {
        std::cout << "t1\n";
    }
};

class t2
{
public:
    void print()
    {
        std::cout << "t2\n";
    }
};

using var_t = std::variant<t1, t2>;

class var
{
private:
    var_t mv;
public:
    template <typename ... Args>
    var(Args&& ... args) : mv(std::forward<Args>(args)...){}
    void print()
    {
        std::visit([](auto&& x){ x.print(); }, mv);
    }
};

int main()
{
    var w;
    int c;
    while(std::cin >> c)
    {
        if(c == 1)
        {
            w = std::in_place_type_t<t1>{};
            w.print();
        } else
        {
            w = std::in_place_type_t<t2>{};
            w.print();
        }
    }
}
```

```c++
// Say goodbye 2 virtual function.

#include <variant>
#include <iostream>

class t1
{
public:
    void print()
    {
        std::cout << "t1\n";
    }
};

class t2
{
public:
    void print()
    {
        std::cout << "t2\n";
    }
};

using var_t = std::variant<t1, t2>;

class var
{
private:
    var_t mv;
public:
    template <typename ... Args>
    var(Args&& ... args) : mv(std::forward<Args>(args)...){}
    void print()
    {
        return std::visit([](auto&& x){ x.print(); }, mv);
    }
};

int main()
{
    var w;
    int c;
    while(std::cin >> c)
    {
        if(c == 1)
        {
            w = std::in_place_type_t<t1>{};
            w.print();
        } else
        {
            w = std::in_place_type_t<t2>{};
            w.print();
        }
    }
}
```

- Variant优点：必要时候才使用虚表，而用虚函数只要用了，就是一张虚表。（至少GCC9.2还是那么愚）

```c++
// 虚函数多态
// 还是用了虚表...

#include <variant>
#include <iostream>

class t
{
public:
    virtual void print() = 0;
};

class t1 : public t
{
public:
    virtual void print()
    {
        std::cout << "t1\n";
    }
};

class t2 : public t
{
public:
    virtual void print()
    {
        std::cout << "t2\n";
    }
};

int main()
{
    t1 foo;
    t* x = &foo;
    x->print();
}
```

![](https://i.loli.net/2019/09/22/vY7rfVctIXSjmzw.png)

```c++
#include <variant>
#include <iostream>

class t1
{
public:
    void print()
    {
        std::cout << "t1\n";
    }
};

class t2
{
public:
    void print()
    {
        std::cout << "t2\n";
    }
};

using var_t = std::variant<t1, t2>;

class var
{
private:
    var_t mv;
public:
    template <typename ... Args>
    var(Args&& ... args) : mv(std::forward<Args>(args)...){}
    void print()
    {
        return std::visit([](auto&& x){ x.print(); }, mv);
    }
};

int main()
{
    var w;
    int c;
    w = std::in_place_type_t<t1>{};
    w.print();
}
```

![](https://i.loli.net/2019/09/22/N8y4JqTOQMbPxEC.png)

`Variant YES!!!!`

### Ref

https://arne-mertz.de/2018/05/modern-c-features-stdvariant-and-stdvisit/

