# Inside the C++ Object Model: Chapter 2

> 构造语义学

## 默认构造函数的操作

#### 基础

<u>对于POD类型不提供默认初始化</u>；对于其他对象对应的构造函数都使用<u>其对应的默认构造函数初始化</u>。

> 一个类中若没有"含non-trivial"的构造函数，那么该类的构造函数同样也是"non-trivial"的（就是什么都不干的意思）。

> 全局和静态变量的内存会保证在使用前被清零。而堆内存的变量不会这样。

#### 再考虑派生类对象

<u>派生类对象的基类若有默认构造函数，则基类部分的数据成员按基类的默认构造函数的方式初始化。</u>

#### 考虑虚函数&虚基类

对于含有虚函数（继承自虚基类）的对象，指向虚表的指针会被初始化。

## 复制构造函数的操作

#### 复制构造函数被调用有三种情况

- **Explicit** :明确的一个对象的内容作为另外一个对象的初始值。如X xx = x或X xx(x)。
- **Parameter**:对象作为参数传递给函数时。
- **Return**:类对象作为函数返回值时。

#### 基础：Default memberwise initialization

在用户没有提供构造函数的情况下，编译器会自动生成默认复制构造函数：

这样的构造函数分2种：bitwise copy和memberwise copy(copy constructor)：

[Blue](https://www.zhihu.com/question/30856753)

#### Memberwise

data member:

- Non-obj(POD):逐个拷贝；(bitwise copy)
- obj：递归调用默认拷贝函数；

#### Bitwise

然而，当出现了以下几种情况就是不能使用Bitwise Copy（即简单来说，使用Bitwise Copy会出错，所以得为类再合成一个拷贝构造函数）

1. <u>Class内部含有一个Member Class Object，如上文的Y，Y内部含有一个Copy Constructor，无论是自定义还是编译器为其生成的</u>

2. <u>Class继承的基类还有一个Copy Constructor</u>

3. <u>Class含有virtual function</u>

4. <u>class继承的基类中，有出现virtual base class的情况</u>

> 优先调用数据成员能被用的默认拷贝函数(无论是自己设置的还是基类复制构造函数提供的)
>
> 其次含虚函数（或虚基类）则说明该对象有虚指针，虚指针需要被适当初始化；

## 类成员的初始化列表

```c++
class A{
	int a;
  double b;
  obj c;
  A::A(int x, double y, obj z):a(x), b(y), c(z){} // 初始化列表
}
```

说到类成员的初始化列表必然想起一个经常出现的面试题：成员初始化列表的顺序是按照成员变量在类中声明的顺序。如果成员初始化列表的顺序和成员变量在类中声明的顺序不一致时某些编译器会提示警告。编译器将成员初始化列表的代码插入到构造函数的最开始位置，优先级跟调用类类型的成员变量的默认构造函数是一致的，都是跟类类型成员变量在类中的声明次序相关。

类成员初始化必须使用成员初始化列表的四种方式：

- 初始化一个引用类型的成员变量
- 初始化一个const的成员变量
- 调用基类的构造函数，且基类的构造函数采用成员初始化列表的方式
- 调用类成员的构造函数，且类成员的构造函数采用成员初始化列表的方式

[src](http://kuring.me/post/inside_the_c++_object_model_chapter_2/)