#  Inside the C++ Object Model: Chapter 1

#### 资源：

[笔记](https://github.com/zfengzhen/Blog/blob/master/article/%E3%80%8A%E6%B7%B1%E5%85%A5%E6%8E%A2%E7%B4%A2C%2B%2B%E5%AF%B9%E8%B1%A1%E6%A8%A1%E5%9E%8B%E3%80%8B%E8%AF%BB%E4%B9%A6%E7%AC%94%E8%AE%B0.md)


####  ADT的C风格代码用C++的class方式重新封装的额外开销：

理论上是0。

#### C++对象模型

2种`class data members`: 

- `static`,

- `non-static`;

3种`class member function`: 

- `static`
- `non-static`
- `virtual`

#### 类域和对象存储

类域：`static data member`  & `static and non-static member function`

对象：`non-static data member`

> 额外说明一下虚函数：
>
> 对于有虚函数的class，**类域**会建立一个<u>虚表</u>：
>
> - 类型；(第一个slot)
> - 指向虚函数的指针；
>
> 每一个**类对象**会有一颗<u>虚指针</u>：指向虚表。
>
> > 虚指针的设定和重置由构造函数和析构函数完成。

**一个类对象的大小由非静态成员数据、是否有虚函数以及内存对齐设定来决定**。

**计算题**：

```c++
class Foo
{
public:
  Foo();
  virtual ~Foo();
  int _integer;
  static char _char;
private:
  char _pc;
}
```

分别计算以4字节和1字节进行内存对齐得到的类对象大小（sizeof值）。(假设指针大小为4字节)

- **Non-static**: int(4字节) char(1字节)
- **Virtual**: 一个虚指针
- **内存对齐**：不说了….

> 内存对齐在32位计算机上是32位（深度探索c++对象模型说的），这样可以提高bus的运输效率。

答案是12 & 9.

此外，对于一个类对象最小内存为1

#### 多态的体现

其实多态是一个很广泛概念，POD之间的隐式转换也可叫做多态。这里我们讨论class级别的多态。

有3中方法支持class级别的多态：

- `Base* ptr = new Derived();`
- Virtual function;
- `dynamic_cast<>()`

#### 小小的分析

```c++
Derived der;
Base *base = &der;
Derived* _der = &der;
```

- `base`和`_der`都指向同一字节的地址。但`_der`指向的地址包含整个`Derived`类，而`base`只包含`Base`部分。

- 也正是如此，`base`不能对`der`中对应的`Derived`类新生的members进行操作，但可以通过virtual的机制达到：

```c++
// 假设Derived 有个de_fn()的方法，而Base没有；
base->de_fn(); // 错

(static_cast<Derived*>( base ))->de_fn(); // 对，指针型的static_cast是编译期转换的

if(auto _d = dynamic_cast<Derived*>(base)) // 对，但dynamic_cast是run_time操作，会有1点点开销
  _d->de_fn();
```

- 仔细考究一下编译期类型转换的`static_cast`，我们不难理解，base的类型在编译期时已决定了下面的内容：
  - 接口(比如`base`只能用`Base`里面有的东西，不能用`Derived`里面独特的东西)
  - access level(对于虚函数，基类和派生类之间同名不同法，`base`只能用基类的"法")

#### OO & OB

OO -> 弹性；

OB -> 效率；