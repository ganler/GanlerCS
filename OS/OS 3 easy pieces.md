# OS 3 easy pieces

## Visualization

### Process

#### 分离策略和机制

即将**高层策略**与**底层实现**分开设计。

#### 进程创建

- 加载所有对应的代码和静态数据到进程对应的地址空间中。（lazily执行）
- 创建运行时栈，为运行时栈分配内存
- 必要的话，分配一定的堆内存
- 其他初始化任务（如I/O）

#### 进程状态

- 运行
- 堵塞
- 就绪

#### PCB

> 以xv6为例

- 上下文`context`（各种寄存器）
- 进程地址空间（初始位置与大小）
- 内核栈的栈底地址
- 进程状态
- pid
- 父进程
- chan, killed
- Open file
- 当前路径
- Trap frame

> 除了三大状态（三大状态的特点是会多次出现）还有**初始状态**（正在建立），**僵尸状态**（进程退出但未清理）。

#### API

- `fork`：创建子进程，执行当前父进程fork后的代码。

> 子进程并不是完整的拷贝父进程，其从fork返回的值是不同的。

```c++
#include <cstdio>
#include <unistd.h>

// rc == 0 => 当前进程就是子进程
// rc <  0 => 失败

int main()
{
    printf("Now --> pid:%d\n", (int)getpid());
    int rc = fork();
    if( rc < 0 ) // fork failed
        printf("fork failed\n");
    else if( rc == 0 ) // child process
        printf("--> Child: %d\n", (int) getpid());
    else
        printf("--> Father( %d ) of %d\n", (int) getpid(), rc);
}
```

- `wait`

```c++
#include <cstdio>
#include <unistd.h>
#include <sys/wait.h>

// rc == 0 => 当前进程就是子进程
// rc <  0 => 失败

int main()
{
    printf("Now --> pid:%d\n", (int)getpid());
    int rc = fork();
    if( rc < 0 ) // fork failed
        printf("fork failed\n");
    else if( rc == 0 ) // child process
        printf("--> Child: %d\n", (int) getpid());
    else
    {
        int wc = wait(NULL); // 等待子进程执行完毕。
        printf("--> Father( %d ) of %d\n", (int) getpid(), rc);
    }
}
```

- `exec`

> fork后，可调用exec去执行别的程序。这样不会产生新的子进程。
>
> 调用exec后，当前子进程的程序，静态数据，堆，栈等都会被覆盖（重新初始化），然后执行exec对应的程序。
>
> <u>对exec的成功调用永远都不会返回。</u>

```c++
#include <cstdio>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

// rc == 0 => 当前进程就是子进程
// rc <  0 => 失败

int main()
{
    printf("Now --> pid:%d\n", (int)getpid());
    int rc = fork();
    if( rc < 0 ) // fork failed
        printf("fork failed\n");
    else if( rc == 0 ) // child process
    {
        printf("--> Child: %d\n", (int) getpid());
        char* args[3] = {
                strdup("wc"),
                strdup("../util.hpp"),
                nullptr
        };
        execvp(args[0], args);
        printf("child is doing sth\n");
    }
    else
    {
        int wc = wait(NULL); // 等待子进程执行完毕。
        printf("--> Father( %d ) of %d\n", (int) getpid(), rc);
    }
}
```

#### Shell

- shell也是一个程序，我们通过传入shell命令，shell收到命令后fork再exec（的某个变体），再调用wait等待进程结束。
- **重定向的实现**：调用exec前关闭stdio，打开目标文件，然后输出到该文件上。

```c++
#include <fcntl.h>

// ...

close(STDOUT_FILENO);
open(output_file, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

// ...
```

- UNIX管道，使用了pipe()系统调用。一个**进程的输出**被链接到了一个内核管道上（队列），另外一个**进程的输入**也被链接到了一个内核管道上。然后相互match，以实现管道。`grep -r main ./* | wc -l`。

#### 受限直接执行

虚拟化机制的2大重点：

- 控制权（保证操作系统自生的控制权）
- 高性能（尽量接近zero-overhead）

> 直接运行协议（无限制）

|                              OS                              |          Program          |
| :----------------------------------------------------------: | :-----------------------: |
| 在进程列表上创建条目<br />为程序分配内存<br />加载程序和静态数据<br />根据argc/argv设置运行时栈<br />清空寄存器<br />call main |                           |
|                                                              | 执行main<br />return main |
|               释放进程内存，并从进程列表中移除               |                           |

> 直接运行的优点是快速，但这样无法进行受限操作（因为操作系统代码无法控制）。

> **受限操作**：
>
> 区分用户模式和内核模式：用户模式不能完全访问资源，而内核模式可。内核通过“系统调用”向用户代码提供一些特殊的受限功能。
>
> > 要执行系统调用，程序必须执行特殊的**<u>陷阱指令</u>**。
> >
> > - 调用系统调用 -> **陷入内核(trap)**
> > - 将特权级别提升为内核模式
> > - 返回 -> **陷阱返回(return-from-trap)**
> > - 降低特权级别
>
> > x86架构在进行系统调用的时候会把寄存器push到**内核栈**上，返回时再pop。
>
> > trap后，操作系统根据不同的异常码，找到对应的内核代码并执行。这个找的过程，靠的是陷阱表(trap table)，陷阱表由操作系统在开机初始化的时候对硬件进行配置，而跳转的过程是通过CPU的一些特殊指令完成的。

> **受限执行协议（LDE）**

![](https://i.loli.net/2019/09/21/MFwcDOXHepyGgfi.png)

#### 等待系统调用的协作方式

> 即OS相信程序会定期give up。
>
> - yield
> - I/O
> - 发生错误，陷入trap
> - 其他系统调用

#### 操作系统干预的非协作方式

依靠时钟中断，即运行一段时间（一般是几毫秒就中断一次）。中断的时候，OS就可以获得操控权了。（当然操作系统也可以关中断->特权操作）

#### 上下文切换

内核栈push&pop。

![](https://i.loli.net/2019/09/21/OZchYsWv8CnGKf3.png)

> 注意，寄存器被保存和恢复有2种方式：
>
> - 硬件隐式保存（目标地址是内核栈，由硬件实现）；
> - 操作系统显式保存（目标地址是PCB的内存，linux3.6内核代码`switch_to`中的注释表示）；

```c
/*
 * Context-switching clobbers all registers, so we clobber      \
 * them explicitly, via unused output variables.                \
 * (EAX and EBP is not listed because EBP is saved/restored     \
 * explicitly for wchan access and EAX is the return value of   \
 * __switch_to())           
 */
```



> > 再次注意，每个进程有2种类型的stack，user-mode和kernel-mode，kernel-mode stack就是我们说内核栈。
> >
> > Each user thread has both a **user-mode** stack and a **kernel-mode** stack. When a thread enters the kernel, the current value of the <u>user-mode stack</u> (`SS:ESP`) and <u>instruction pointer</u> (`CS:EIP`) are saved to the thread's <u>kernel-mode stack,</u> and the CPU switches to the kernel-mode stack - with the `int $80` syscall mechanism, this is done by the CPU itself. <u>The remaining register values and flags are then also saved to the kernel stack.</u>
> >
> > When a thread context-switches, it calls into the scheduler (the scheduler does not run as a separate thread - <u>it always runs in the context of the current thread</u>). The scheduler code selects a process to run next, and calls the `switch_to()` function. This function essentially just switches the kernel stacks - it saves <u>the current value of the stack pointer into the TCB</u> for the current thread (called `struct task_struct` in Linux), and loads a previously-saved stack pointer from the TCB for the next thread. At this point it also saves and restores some other thread state that isn't usually used by the kernel - things like floating point/SSE registers. <u>If the threads being switched don't share the same virtual memory space (ie. they're in different processes), the page tables are also switched.</u>
> >
> > So you can see that the core user-mode state of a thread isn't saved and restored at context-switch time - it's saved and restored to the thread's kernel stack <u>when you enter and leave the kernel.</u> The context-switch code doesn't have to worry about clobbering the user-mode register values - those are already safely saved away in the kernel stack by that point.

#### 上下文切换开销

- 200-MHz Linux 1.3.37 ~ 系统调用 3μs 上下文切换6μs

> 理论分析：
>
> - 寄存器的保存与恢复
> - Cache刷新
> - 流水线，分支预测结果刷新
> - TLB（线程级别可不考虑）

### 进程调度

在进行上下文切换的时候，如何选择即将要切换到的进程，涉及的就是进程调度的问题了。

> 调度问题2大aim：公平+性能；

#### 指标

- 希望尽早完成（性能）：周转时间=完成时间-到达时间
- 希望尽早被响应（公平+性能）：响应时间=首次运行时间-到达时间（适用于对时间敏感的程序）

#### 策略

- FIFO
- SJF（shortest job first）：最优，平均周转时间最短（同时到达进程由开始到被调度完的时间总合/被调度进程数量）（$T=Nt_1+(N-1)t_2+\cdots+t_N$）

> 缺点：非抢占式，若有新的短时间进程来了，还是会让老进程先执行。

- STCF（最短完成时间优先Shortest Time-to-Completion First）：抢占式。同样是最优的。
- RR(Round-Robin 轮转)：解决的是响应时间的问题。（对周转时间不友好，但符合公平性）
- Overlap I/O

#### 问题

上面很多策略是假设我们知道整个运转时间的长度，但实际上我们并无法知道…所以我们应该利用最近的策略去预测未来…（多级反馈队列）

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

