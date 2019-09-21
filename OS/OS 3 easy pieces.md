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

#### 上下文切换开销

- 200-MHz Linux 1.3.37 ~ 系统调用 3μs 上下文切换6μs

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

#### wait和notify一定要加锁

#### Linux的管道是producer/consumer模型

```shell
grep foo f.txt | wc -l
# 两个进程并发执行，前者是producer，标准输出重定向到管道，由后者consumer使用
```

#### 覆盖条件

以并发动态内存分配为例，当内存不够的时候就`wait_until(有内存)`，而当真正有内存的时候，我们使用条件变量不知道应该`notify which thread`。简单的做法就是`notify_all`（但性能不会很好）；