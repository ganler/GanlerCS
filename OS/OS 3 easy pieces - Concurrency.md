# OS 3 easy pieces - Concurrency

[TOC]

![](book.jpg)

## Concurrency

### Intro

#### 线程和进程的区别

线程之间共享地址空间（地址空间保持不变，即不用切换页表），可以访问相同的数据（当然也有`thread_local`的数据）。

> 进程：PCB（操作系统内核的一种数据结构）
>
> 线程：TCB（1KB的样子）

#### 临界区

访问共享资源的一段**代码**。

#### 竞态条件

存在多个线程大概同时进入临界区，且存在试图**更新共享数据的情况**。

#### 原子操作

一系列操作要么已经发生了，要么没有发生，无法观察其中间态。

### 锁

使有且仅有一个线程进入临界区。

#### 评价锁

- 性能
- 公平性（是否能雨露均沾）

#### 实现（包括同步原语）

> 同步原语保证该操作是原子的。

- 单核——禁止中断
- 自旋锁（抢不到锁就自旋）
  - TAS（只能用来抢锁）
  - CAS（有额外的expected值，可对应更多的情况）
  - 链接加载和条件存储（就是检查自从上一次更新链接地址起，链接地址是否被更改，若没有被更改则说明没人抢锁，那么锁就归自己了，抢到后把链接地址的值设置为占用即可）
  - FAA（自增，使用ticket-turn的方法保证雨露均沾）

#### 解决自旋过多的问题

- yield（立即停止对当前线程的调度）
- 睡觉并丢到队列里（这样也保证了公平性，不过还是需要锁来帮忙，即锁住队列）
  - Solaris:
    - Park & Unpark & Setpark（防止并发执行Park-Unpark操作时，Unpark先，最后Park了）
  - Linux:
    - Futex
    - 二阶段锁：
      - 快速自旋有限次（一般一次就好
      - 然后再上Futex

#### 读写锁

- 记录读者数量；
- 一个同步原语；

```c++
// 读写锁的计数器
#include <shared_mutex>

class counter
{
public:
    std::size_t get() const {
        std::shared_lock l(sm_);
        return cnt_;
    }
    void inc(){
        std::unique_lock l(sm_);
        ++cnt_;
    }
    void reset(){
        std::unique_lock l(sm_);
        cnt_ = 0;
    }
private:
    mutable std::shared_mutex sm_;
    std::size_t               cnt_;
};
```

读写锁的实现

```c++
class rwlock
{// 如果readlock对应的临界区事情不是很多的话，就没必要rwlock了。
public:
    void read_lock()
    {
// atomic cannot help. Cuz when wm_.lock is going to be happened. Interrupt may occur.
        std::lock_guard l(rm_);
        if(++rcnt_ == 1)
            wm_.lock();
    }
    void read_unlock()
    {
        std::lock_guard l(rm_);
        if(--rcnt_ == 0)
            wm_.unlock();
    }
    void write_lock()
    {
        wm_.lock();
    }
    void write_unlock()
    {
        wm_.unlock();
    }
private:
    mutable std::mutex  wm_;
    mutable std::mutex  rm_;
    mutable std::size_t rcnt_;
};
```

### Concurrent Data Structure

#### 原子计数优化

损失准确性进行优化

> 每个线程建立一个local的计数，当local的操作累积到一定值的时候再对原子计数进行更新；

#### 引用原子计数优化

当local的原子计数为0的时候再更新原子计数。（没有准确性的损耗）

#### 大锁

> 比如LINUX的内核锁BKL；

#### 细粒度锁

- 过手锁（频繁释放和获取锁，消耗较大）
- 并发哈希表：每个桶一个锁

### Condition Variable

#### wait一定要加锁

notify如果只考虑自身的线程安全的话，反正C++标准是保证的，可以直接把notify放临界区外。Linux的pthread据李润中说也是保证线程安全的。

#### Linux的管道是producer/consumer模型

```shell
grep foo f.txt | wc -l
# 两个进程并发执行，前者是producer，标准输出重定向到管道，由后者consumer使用
```

#### 覆盖条件

以并发动态内存分配为例，当内存不够的时候就`wait_until(有内存)`，而当真正有内存的时候，我们使用条件变量不知道应该`notify which thread`。简单的做法就是`notify_all`（但性能不会很好）；

## 其他

#### 9.24 凌晨和李润中讨论条件变量的结果

- 先notify再wait，wait能不能接受到这个notify —— 不能

> 因为在pthread_cond_t和C++中condition_variable的实现中，首先wait需要拿到一把锁，然后看看条件是否达成，达不成就unlock，并进入休眠队列。notify的话就是唤醒某一线程抢一下锁然后查看一下结果，如果OK，就继续，不行就unlock。
>
> **结论**：
>
> - notify after unlock
> - wait操作将临界区分成2段

- notify操作的线程安全性：

> 在C++中：
>
> `notify_one()`/`notify_all()` 的效果与 `wait()`/`wait_for()`/`wait_until()` 的三个原子部分的每一者（解锁+等待、唤醒和锁定）以能看做原子变量[修改顺序](https://zh.cppreference.com/w/cpp/atomic/memory_order#.E4.BF.AE.E6.94.B9.E9.A1.BA.E5.BA.8F)单独全序发生：顺序对此单独的 condition_variable 是特定的。譬如，这使得 `notify_one()` 不可能被延迟并解锁正好在进行 `notify_one()` 调用后开始等待的线程。
>
> 通知线程不必保有等待线程所保有的同一互斥上的锁；实际上这么做是劣化，因为被通知线程将立即再次阻塞，等待通知线程释放锁。然而一些实现（尤其是许多 pthread 的实现）辨识此情形，在通知调用中，直接从条件变量队列转移等待线程到互斥队列，而不唤醒它，以避免此“急促并等待”场景。
>
> 然而，在要求精确调度事件时，可能必须在处于锁下时通知，例如，在若满足条件则线程将退出程序，导致析构通知线程的 condition_variable 的情况下。互斥解锁之后，但在通知前的虚假唤醒可能导致通知在被销毁对象上调用。

