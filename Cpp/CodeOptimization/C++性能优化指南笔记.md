# C++性能优化指南笔记

## 绪论

#### 减少内存的分配和复制

一半的计算程序对应就只是几个指令；

<u>而每次调用内存管理器的开销却是上千个指令。</u>

对缓存复制函数调用也会消耗数千个CPU周期；

> 所以：
>
> - 少分配内存；
> - 少复制内存；

> 尽量使用栈上的内存（连续的），少使用堆上的内存（不连续）；只有连续的内存访问才有可能在一个周期内完成，不连续的内存访问会消耗6~10个周期；

## 影响优化的行为

- 计算模型与现代计算机架构的复杂性；
- 指令执行速率很快（并发性是原因之一），**内存硬件的处理速度很慢**；
- **冯诺依曼瓶颈**；

> 内存顺序访问才能在一个周期完成；否则一般为6~10个周期；
>
> 多个活动（多个线程）会争夺内存总线的访问；所以多核处理器的数量能保证内存总线的通信数据量是饱和的。(每byte到CPU核心的速率大概是20~80ns)；
>
> <u>核心增加再多，也无法使主存接口变快；反而核越多，等待内存访问机会的核心也就越多。</u>
>
> 这种隐式限制被称之为"**内存墙**"。

- 内存访问的单位不一定是1字节：

  > 内存访问的单位至少是1字节，大部分处理器都是一次性向内存索取64字节。超级计算机和图形处理器还可以获得更多。
  >
  > 由此，内存对齐问题也随之产生了。因为内存不对齐，而是挤在一起的话，可能导致处理器索取内存的时候多次索取。
  >
  > > 假设有2份连续的数据: `[63bytes, 5bytes]`，并且CPU要访问后面的那个5bytes，当不对齐的时候，处理器要先取前64bytes（63+1），然后再访问后64bytes，这样夹缝中的`5bytes`导致CPU2次取值。内存对其后就没这事了。
  > >
  > > 所以设置结构体的时候，注意一下数据成员的顺序和大小。

- 多级缓存和内存

> 一般的桌面级计算机；
>
> `一级缓存(Haswell分一级数据缓存和一级指令缓存) -> 二级缓存 -> 三级缓存 -> 主存 -> 磁盘的虚拟内存页`
>
> 上层比下层的容量更小，但更快，当然更贵。

- 统一字节序；
- 获取磁盘上的内存块 -> 内存(约10 ms)，这个值比较稳定；
- **页抖动**：一个程序访问许多离散的地址，当地址相差较远的时候，没有足够的缓存来保存刚刚程序中的数据，会导致严重的性能衰退。
- 计算比做决定更快(但我做个一个用`if`取反和用`!`的benchmark，release情况下发现是谁先执行谁最快，debug的情况下if更快)；
- 调用操作系统的开销最昂贵；
- 显示的加一些标识符以帮助编译器优化。

##  测量性能

#### 工具

- Profile；

- 拿C++写；

#### 规则和定律

- 90/10规则 - 优化热点代码
- 阿姆达尔定律

>性能提升比例：
>
>$$
>S_T=\frac{1}{(1-P)+\frac{P}{S_P}}
>$$

> 优化耗时严重的重要代码(这不是废话吗)

- 记录实验笔记
- 计时：

```bash
/usr/bin/time ./你的程序
```

## 优化算法

#### 排序

- 基于比较的排序最优平均时间复杂度是$O(n\log_2 n)$
- 非比较的如桶排序可到达：$n\log_r n$，r是桶的个数；
- Flash sort，但前提是需要知道**待排数组的区间和待排数组的长度**；

> 思路很有意思，线性预测数字所在的桶的位置；
>
> 对于排序后是等差的数列，复杂度是$O(n)$
> $$
> Predict_i=(r-1)\frac{a_i-a_{\min}}{a_{\max}-a_{\min}}
> $$

- 要对输入数据集的特点作了解，并根据其特点选择合适的排序方式；
- 普适性较强：Timsort混合型排序（最好情况的复杂度是$O(n)$ ，$O(n)$额外空间）。
- `std::sort`一般的实现：内省排序（堆排+快排）（稳定$O(n\log n)$，$O(1)$额外空间）；

> 不过我们考虑非常大的数据集，$O(n)$的额外空间任意带来非常大的常数(内存是很慢的)；

#### 批量处理

- 缓冲区：缓冲区满了再输出；
- 建堆(一次性用$O(n)$建立个堆，之后插入和查找都是$O(\log n)$，建堆不是排序，建堆+排序是$nO(\log n)$)；
- 多线程/并行指令等；

#### 缓存

- 在设计类的时候，如果输入相同可以直接返回缓存值(当且仅当计算一次消耗巨大的情况)；
- 线程池；
- 动态规划算法；

#### 其他

- 把热点语句中的`if-else`块进行优化：(最有可能的条件放在最优先的位置)
- 一次性进行大量IO效率会更高，但需要考虑当程序奔溃的时候，损失的数据会更多；
- 唤醒线程也是要花时间的，不要唤醒线程后只做一些很简单的事情；



## 栈堆

#### 栈是FIFO

```c++
#include<cstdio>

int main()
{
    int a = 1;
    int b = 2;
    printf("%d,%d",a, a=a+b ); // 3, 3
}
// 输出结果是3，3。
```

**参数的入栈顺序是<u>从右到左</u>，先入栈，先计算**，所以`a=a+b`会被提前计算；

> 但在参数较小的时候，编译器<u>直接通过寄存器传参</u>，而不通过栈，但记住，编译器的数量是有限的，可以放到寄存器就放寄存器，否则放栈中。但参数类型是浮点型，指针等等其他类型就不能使用寄存器来传递函数参数了。

```asm
main:
.LFB1:
        push    rbp					# 将原始bp入栈，用于保存bp位置
        mov     rbp, rsp		# 将rsp的值放入rbp的值
        sub     rsp, 16			# rbp向下走16字节
        ...
        pop 		rbp					# pop，获取之前的栈位置并返回
        ret
        # eax用来装返回值。
```

> `rbp`是**栈顶寄存器**地址，`rsp`是**基址寄存器**地址，代表栈的最大空间容量，其增长是向下增长。而且是在不够的时候，才向下增长。

#### 堆

比较一下malloc和new的不同。这个问题其实不难，首先明确：malloc是函数，而new是关键字。然后new作为C++中动态对象创建的基石，除了完成堆空间的分配操作以外还要完成一些初始化操作，及new的过程中会调用对象的构造函数去初始化，而malloc不会。最后要明确的是malloc分配的内存只能用free来释放，而new分配的地址只能用delete来释放，如果new分配的是数组，则需要delete[ ]来释放，否则会出现未定义行为。

- 首次适应算法：在**空闲区链首**开始向后寻找，找到第一块能满足所需大小的空闲块

- 循环首次适应算法：从**上一次找的空闲区**开始向后寻找。直到链尾都不满足，则返回链首寻找空闲块

- 最佳适应算法：每次分配内存时，都选择能满足要求，并且又是最小的空闲块。优点是：每次第一次找到的满足要求的空闲块，必然是最佳的

- 最坏适应算法：每次总是选择一个最大的空闲块分配给进程使用。优点是：产生内存碎片的几率较小

- 快速适应算法：将空闲区依据其容量大小进行分类，每一类同容量的空闲块都有自己的链表。同时在内存中设立一张管理索引表，每个表项为一种空闲块类型，并记录其链首指针。其优点是：查找效率高，不会产生内存碎片。缺点是实现复杂，系统开销大。

## 优化动态分配

> 减少动态分配以提高性能；

#### 变量存储期

- 静态存储期

> 全局静态变量是在程序开始时被执行；函数中声明的静态变量会在第一次调用函数的时候被声明。静态存储期的变量被分配在编译器预留的内存空间中——静态存储区；
>
> 在命名空间作用域定义的变量以及被声明为`static`和`extern`的变量具有静态存储期；

- `thread_local`

> 线程局部变量在进入线程时被构建，退出线程时被销毁。其内存空间由线程分配。一般来说，访问`thread_local`变量会比访问全局变量多一个周期。有的操作系统必须通过线程ID索引一张全局表来访问线程局部变量，这样的额外开销更大；

- 自动存储期

> 编译期完成内存分配。栈空间有限，若递归超过某一深度，会造成栈溢出；

- 动态存储期

> 调用内存管理器以分配内存。

#### 所有权

> 所有权是用来确定一个变量什么时候被构建，什么时候被销毁；

- 全局所有权

> `main`函数前被构建，`main`函数后被销毁；

- 词法作用域所有权

> 进入词法作用域时被构建，离开词法作用域后被销毁；

- 成员所有权

> 实例被构建时被构建，实例被销毁时被销毁；

- 动态变量所有权

> 没有预定义所有者；由编程人员管理。

#### 值对象和实体对象

值对象可copy可复制，而实体对象独一无二。

#### 智能指针

> `std::shared_ptr`的引用计数是使用原子操作实现的，在进行自加和自减的时候，会调用系统的内存屏障(不只是简单的一个指令)，从而降低性能。(这样做是为了能让`std::shared_ptr`可进行多线程操作)

- 所有权单一`->` `std::unique_ptr`
- 所有权不明朗`->` `std::shared_ptr` (尽量使用`std::make_shared`，使2次内存管理器的调用简化为一次)
- 其他：原始指针

#### 动态内存为什么慢

- 寻找合适的内存块慢；
- 找不到合适的内存块，会调用系统内核，从系统的可用内存池中调用额外的大块内存(很慢)
- 对内存分配器的调用不能过于频繁，否则操作系统会视其为一种资源竞争，从而同一时间只给一个线程分配内存，让其他线程等待。

#### 使用环形缓冲区而非双端队列

使用足够大的环形队列作为缓冲区

- `empty()`： `begin()==end()`
- `full()`: `begin()==(end()+1)%MAX_SIZE`

> 参考`boost::circular_buffer`

#### 预分配和返还多余内存

- `reserve()`

- `shrink_to_fit()`

> 想不返回所有内存？先`resize()`再`shrink_to_fit`

#### 在循环外创建动态变量

```c++
// Slow

for(int i = 0; i < N; ++i)
{
  	string str;
		ReadFile(filename, str);
  	// ...
}
// 这段代码中，str会被不断重新分配内存，从而导致性能下降


// Fast
// 这样的话，原来被分配的内存只要充足，就不会重新分配
string str;
for(int i = 0; i < N; ++i)
{
		str.clear();
  	ReadFile(filename, str);
  	// ...
}
```

#### 写时复制

COW(copy on write)：默认使用浅拷贝，只有当有更改需求的时候才进行拷贝。

> `std::string`禁止了COW，而是SSO，即立即复制。

#### 使用`std::string_view`

主要是避免`substring`拷贝。

#### RVO比`std::move`更加高效

#### 移动父类

```c++
class Base{};

class Derived : public Base
{
		Derived(Derived&& rhs):
		move(rhs), 
		... // Other moves.
		{}
}
```

> 一般来说在move中swap这里，我们要先把自己的动态变量delete弄成nullptr(释放好)，否则额外的拷贝会损害性能。

#### 尽量不要strlen

> 使用`std::string`的时候没这个问题，因为`std::string::length()`是动态更新的。但是似乎迭代器版本的总归要快一些。

```c++
// 对于char[]，尽量用range-based for或者for(int i = 0, l = strlen(str); ...)

#include <cstring>

static void no_strlen(benchmark::State& state) {
  for (auto _ : state) {
    std::string str = "asdnu nasdio aasdn noasdi s";
    for(auto& x : str)
        if(x == ' ')
            x = '-';
    benchmark::DoNotOptimize(str);
  }
}
// Register the function as a benchmark
BENCHMARK(no_strlen);

static void use_strlen(benchmark::State& state) {
  for (auto _ : state) {
    char str[] = "asdnu nasdio aasdn noasdi s";
    for(int i = 0; i < strlen(str); ++i)  // 这个strlen会重复计算
        if(str[i] == ' ')
            str[i] = '-';
    benchmark::DoNotOptimize(str);
  }
}
BENCHMARK(use_strlen);
```

![](https://i.loli.net/2019/07/05/5d1f105e6487146210.png)

#### Log

在log的时候，我们经常需要打印时间，但是获取时间的速度是缓慢的，所以我们可以在一个时间点尽量多的写入多行信息。(不要一个信息对应一个时间点，而是一个时间点对应多行信息)

#### 虚函数的坏处

> 首先每个对象会多一个vptr(一般在实例的第一个字段以提高效率)；
>
> vptr会指向一个虚表vtable(加载一次不连续地址)；这个vtable记录了指向类中可见的每个虚函数前面所关联的函数体（即多个函数体），那么这就意味着我们又得再跳转一次（又加载一次不连续地址）；

- 有前后相关的程序，破坏流水线；
- not cache-friendly（2次加载不连续地址）；
- 难以内联；

#### 多重基础

> 调用多重基础的函数一般要在this类实例指针中加上一个偏移量来组成指向多重继承类实例的指针。对于加上偏移量再去call的这种事情，在桌面级(x86等架构)上，依旧是一个指令的事情，但是在一些嵌入式机器上就可能会变成2条指令（先加再call）。(使用静态成员函数就没这屁事了，所以应该尽量使用静态成员函数)

#### 把虚析构函数移动到基类中

任何一个有继承类的析构函数都应该是虚函数。这样，除了delete当前类，派生类也会被delete。

```c++
#include <iostream>

struct A
{
    virtual ~A(){ std::cout << "Delete A\n"; };
};

struct B : A
{
    ~B(){ std::cout << "Delete B\n"; };
};

int main()
{
    // Without a virtual ~A(), the B part wouldn't be deleted.
    // With a virtual ~A(), ~B() first, then ~A().
    A* ptr = new B(); 
    delete ptr;
}
```

#### 如果不改变，就尽量使用range-based for

```c++
#include <string>
#include <cstdlib>
#include <utility>

void range_for(const std::string& s)
{// 一次解引用
    for(const auto x : std::as_const(s))
        putchar(x);
// range-based for，在一开始就会把迭代范围确定好：begin(s)和end(s)
// 所以迭代次数是确定的
}

void non_range_for(const std::string& s)
{// 两次解引用
    for(int i = 0; i < s.length(); ++i)
        putchar(s[i]);
 // 迭代次数不确定，看s.length()，所以不会优化成上述版本
}
```

#### 优化二分查找

- 对有序的字符串做二分查找的时候 不用binary_search 用自己实现的二分搜索 加上strcmp，因为strcmp的返回值包含的信息更多（对于定长情况还可以`std::char_traits<CharT>::compare`）。
- 其他类似的情况 只要我们可以通过一次遍历得到 greater less equal这些信息 我们都可以通过替换标准库的二分查找实现以提高性能。

## 额外

#### 系统级别优化

> [blog](https://toutiao.io/posts/60r22r/preview)

- CPU亲和性(特定情况下使用)
- 尽量不使用系统调用；
- 数据集中存放(尽量使用连续存储)
- Cache Line对齐

```c++
#define CACHE_LINE_SIZE 64

#ifndef _WIN32
#define CACHE_LINE_ALIGN __declspec (align(CACHE_LINE_SIZE))
#else
#define CACHE_LINE_ALIGN __attribute__((aligned(CACHE_LINE_SIZE)))
#endif

// 使用c++11语法更加简单：alignas(CACHE_LINE_SIZE) XXX x.
// https://stackoverflow.com/questions/20791428/what-is-the-recommended-way-to-align-memory-in-c11
```

- 数组按行访问

#### Jumps/branches are expensive. Minimize their use whenever possible.

#### Avoid/reduce the number of local variables.

• Local variables are normally stored on the stack. However if there are few enough, they can instead be
stored in registers. In this case, the function not only gets the benefit of the faster memory access of data
stored in registers, but the function avoids the overhead of setting up a stack frame.
• (Do not, however, switch wholesale to global variables!)

#### Reduce the number of function parameters.

• For the same reason as reducing local variables – they are also stored on the stack.

```c++
#include <iostream>
#include <array>
#include <numeric>

#include "util.hpp" // class Timer

using namespace std;

void foo_(std::array<uint64_t, 1000> arr = {})
{
    arr.fill(1);
    cout << accumulate(arr.begin(), arr.end(), 0) << ' ';
}

void foo()
{
    std::array<uint64_t, 1000> arr;
    arr.fill(1);
    cout << accumulate(arr.begin(), arr.end(), 0) << ' ';
}

int main()
{
    int SZ = 100000;

    for (int i = 0; i < SZ; ++i) {
        foo_(); // warm up for the CPU.
    }

    Timer t;
    for (int i = 0; i < SZ; ++i) {
        foo_(); // about 94 ms
    }
    cout << endl << t.time_out() << endl;

    t.reset();
    for (int i = 0; i < SZ; ++i) {
        foo(); // about 73 ms
    }
    cout << endl << t.time_out() << endl;
  
}
```

> Never pass huge things to functions.

#### Try to avoid casting where possible.

• Integer and floating point instructions often operate on different registers, so a cast requires a copy.
• Shorter integer types (char and short) still require the use of a full-sized register, and they need to be padded to 32/64-bits and then converted back to the smaller size before storing back in memory. (However, this cost must be weighed against the additional memory cost of a larger data type.)

#### Restricted pointer array

When declaring a pointer array which you are sure will not be subject to pointer aliasing — namely there will be no other pointer pointing to the same memory address –, you can declare that pointer as a restrict pointer, as below:

- GCC: double* __restrict__ my_pointer_array
- Intel compiler: double* restrict my_pointer_array

This will let the compiler know that <u>it can change order of certain operations involving my_pointer_array</u> to make your code faster without changing some read/write order that may change your results. If you want to use the restricted qualifier with the intel compiler the flag -restrict must be passed as an argument to the compiler.