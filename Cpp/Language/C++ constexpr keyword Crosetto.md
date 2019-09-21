# C++ constexpr keyword Crosetto

> A talk about TMP.

#### Intro

- 对于函数：可能是compile time也可能是run time，取决于编译器和参数性质；
- 对于变量：compile time.

对于函数的constexpr可以代替递归式的tmp方法。而且一般来说会提高编译速度（不过在老编译器下有的可以constexpr函数是runtime的…所以constexpr是runtime还是compile time得看编译器如何实现）。

但有一种方法可以保证其compile time，就是用`constexpr`函数给`constexpr`变量赋值。

通过编译选项：`-fconstexpr-depth=1000`可以控制递归深度。

#### Limitations

![](https://i.loli.net/2019/07/09/5d249dd252fb781871.png)

#### Constexpr types

首先必须得有constexpr构造函数；

其前提是：数据成员必须有`constexpr`构造函数；

![](https://i.loli.net/2019/07/09/5d249f5b42bf477567.png)



#### Use noexcept to check constant expression

```c++
int main()
{
    constexpr bool is_it_constexpr = (noexcept(666));
    static_assert(is_it_constexpr, "This is not const expression.\n");
}
```

