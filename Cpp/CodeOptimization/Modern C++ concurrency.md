# Modern C++ concurrency

## 有锁栈的设计



对于一个和`std::stack`一样接口的栈，如果我们将其设计为并发的：

- 直接只使用`empty()`和`size()`是没有问题的
- 但把`empty()`的结果作为向后引导的标志就有问题了：

```c++
if(!s.empty())
{
  	do_sth(s.top()); 
  	// 可能在!s.empty()之后，马上就栈空了，从而引发一些未定义行为
}
```

## 双含锁对象操作避免死锁

- 首先检查两个对象是不是一样的，若是一样的话肯定会死锁
- 按顺序锁，就是对于两把锁，给每个锁/对象一个const id，然后按顺序锁
- 同时把俩互斥量锁了，锁了后再把下面的互斥量锁了

```c++
// 第三种方法的实现
std::lock(lk1, lk2);
std::lock_guard lkl(lk1, std::adopt_lock);
std::lock_guard lkr(lk2, std::adopt_lock);
// adopt_lock	假设调用方线程已拥有互斥的所有权
// 因为除了调用lock之外还得unlock lock_guard会帮忙unlock
// std::lock还有一个好处就是要么2个都锁，要么2个都不锁
// 缺点就是不太高效(特别是在GCC上，Clang可以忽略)

// 中间可能导致锁的次数太多而gg
void lock(_L0& __l0, _L1& __l1)
{// libc++源码 (glibc++的更nb直接一个模板打一串)
    while (true)
    {
        {
            unique_lock<_L0> __u0(__l0);
            if (__l1.try_lock())
            {
                __u0.release();
                break;
            }
        }
        __libcpp_thread_yield();
        {
            unique_lock<_L1> __u1(__l1);
            if (__l0.try_lock())
            {
                __u1.release();
                break;
            }
        }
        __libcpp_thread_yield();
    }
}

// glibc++
template<typename _L1, typename _L2, typename... _L3>
    void
    lock(_L1& __l1, _L2& __l2, _L3&... __l3)
{
  while (true)
  {
    using __try_locker = __try_lock_impl<0, sizeof...(_L3) != 0>;
    unique_lock<_L1> __first(__l1);
    int __idx;
    auto __locks = std::tie(__l2, __l3...);
    __try_locker::__do_try_lock(__locks, __idx);
    if (__idx == -1)
    {
      __first.release();
      return;
    }
  }
}
```

- 一般来说直接用`std::lock`把多个锁给锁了
- diy的并发结构可以用id法

## 避免死锁

- 尽量只用一个锁
- 避免不了嵌套锁的时候：`std::recursive_mutex` 或者直接`std::lock`
- 有锁的时候尽量不要使用用户代码（万一用户代码里面有和你一样的锁呢）
- Hierarchy mutex … 和id法差不多[link](http://zhengyi.me/2018/04/11/what-is-lock-hierarchy/)

## 性能问题

#### 根据情况使用原子变量的内存栅栏参数

// TODO

#### 分批更新临界区数据

对于一个共享变量，我们搞一些`thread local`的变量来记录当前线程对这个共享变量的改变操作，然后批量更新共享变量。（比如原子计数、比如shared_ptr的原子引用计数（一个线程的引用计数是非原子的，等local的非原子引用计数为0的时候才更新原子引用计数））

#### 减少创建和销毁线程的开销

创建和销毁线程的开销有：

- 操作系统更新记录线程的表
- 操作系统要调度线程，进入内核态，还有上下文切换
- 创建栈（和动态内存分配类似，而且创建栈则更是要预留很多内存，那么线程很多的时候，内存就容易爆了）
- 初始化寄存器组（当新线程第一次得到**调度因子**时会有<u>少量调度相关的延时</u>和<u>存储原来线程寄存器数据</u>的延时，而非第一次得到**调度因子**的延迟还有<u>从内存中获取当前寄存器数据的延时</u>）
- 线程很多的时候，操作系统的调度变得更慢，而且线程很多的时候会导致高速缓存发生较为严重的抖动，从而降低性能

> 解决方法就是用线程池进行缓存。

#### 开销数据感受

- 原子操作比非原子操作慢十几倍；
- 用带线程池实现的`std::async`的效率一般比直接`std::thread`快个10倍；