# C++ vptr, vtbl and dynamic binding

#### Binding

- Static binding: 固定地址 -> call
- Dynamic binding: call -> `(*(p->vptr)[n])(p)`

> call虚函数会通过Dynamic binding。 

#### vptr & vtbl

![](https://i.loli.net/2019/09/23/FPo6AjHJaC5Bsu1.png)

> 只要出现了虚函数重载现象，每个类就会建立一个新vtbl；
>
> vptr指向对应的vtbl，vtbl指向对应的虚函数 。
>
> `(*(p->vptr)[n])(p)`
>
> > 编译器知道什么函数是动态binding，所以对于可以不动态binding的函数，就不会动态binding；

#### 动态绑定的条件

- 指针
- 虚函数
