## Latency data

> [SRC](https://gist.github.com/hellerbarde/2843375)

![](https://cloud.githubusercontent.com/assets/1489514/19324275/be41885c-908e-11e6-95a2-68fa75333c38.jpg)

> Cache size:
>
> - L1: 64K
> - L2: 512K
> - L3: 1~2MB

## 共享存储器多处理器(SMP)

> [Ref](https://github.com/GHScan/TechNotes/blob/master/2017/Memory_Model.md)

SMP是通过共享地址空间进行通讯的多处理器系统（对应集群的概念）

- **NUMA**：大量的处理器通过**<u>互连网络连接</u>**在一起，每个节点访问本地存储器时延迟较短，访问其他节点有不同的延迟
- **UMA**：所有的处理器通过总线连接在一起，访问一个共享的存储器，<u>每个节点的存储器访问延迟相同</u>。目前常见的桌面多核处理器就是这种结构。

## 现代处理器中的加速技术

- **流水线**
- **动态分支预测**
- **动态多发射**
- **乱序执行**：将无关指令提前且并行执行
  - **寄存器重命名**：CPU通过寄存器重命名的方式使得物理寄存器数目超过指令集中的逻辑寄存器，缓解了如x86指令集中的寄存器不足的问题。*寄存器重命名另一大作用是，避免由于假数据依赖(False Data Dependence)导致的停顿；具体来说，寄存器重命名解决了WAW Hazard和WAR Hazard*。
- `推断执行(Speculative Execution)`：支持动态分支预测和乱序执行的处理器，需要保留一个`重排序缓冲区(Reorder Buffer)`，用来对乱序执行的指令进行`顺序提交(In-Order Commit)`。重排序缓冲区为推断失败时的撤销提供了解决方案，只需要清空分支指令后的所有指令即可。另外，顺序提交也为`精确异常(Precise Exception)`提供了基础，这是操作系统中断和`缺页故障(Page Fault)`处理的关键。推断执行的指导思想是“加速大概率事件”
- `写缓冲区(Writer Buffer)`：CPU在写存储器时，不直接访问存储器或Cache，而是将要写的数据放入一个写缓冲区，然后继续执行后面的指令，这缓解了<u>写存储器</u>导致的停顿（写存储器比较慢）
- `硬件多线程(Hardware Multithreading)`：上面列举的优化策略都旨在改进`指令级并行(Instruction-Level Parallelism)`，另一种提高CPU利用率、掩盖停顿的做法是硬件多线程，<u>即为每个CPU准备两套寄存器和指令计数器，同时从两个逻辑线程取指令并轮询发射，由于这两个线程的指令间没有依赖，并不会引入额外的停顿。</u>支持推断执行的硬件多线程技术又叫`同时多线程(Simultaneously Multithreading)

## 一致性问题

> 当一个变量被多个cache共享的时候，一旦发生改变，如何使cache保持同步，这就是一致性问题。
>
> 在486，Pentium的时代，通过总线lock来让某个核心占据一条cache line，bus仲裁器走round robin(无优先级轮流)来决定当前访问权。这样很低效，因为其他core职能等待。

现在的目的是要制造一种假象，看起来是原子地修改多个Cache，也就是让Cache看起来是强一致的。

### MSI

> 给Cache line 3个状态：
>
> - Modified：被修改过，但未返回主存（下次read的时候激活）
> - Shared：大家share一个cache line，目前来说内容都与主存一致（下次write的时候凉凉）
> - Invalid：表示当前cache不能用

- Read Miss导致cache读入存储器的数据，即变量被加载：Shared。如果这个cache line已经是modified了，那么就找到最新cache line，将modified的结果更新回存储器；
- Write Miss导致Cache以Modified的状态读cache。如果这个cache已经是被shared了，那么告诉shared："It's invalid"。如果那个cache已经是modified（刚写过），那么就对人家发送`Read-invalid`，目标处理器接受到消息后将持有的cache line标记为invalid，同时更新自己本身。
- Shared变量被Modified后：Invalidate，此时其他的Cache需要重复再去读这个变量，达到一致。

### MESI(Modified|Exclusive|Shared|Invalid)

> 给Cache line4个状态，新增Exclusive：独有的shared（对其write不用发invalid）。

#### General Idea

- 加载一个变量（一条cache line）：Exclusive

> 从shared分离出，独占状态。独占的时候不需要玩各种valid啥的，这样提高性能也降低总线容量。
>
> *(即，相比Shared到Modified状态的迁移，第一次写独占Cache Line时进行的是Exclusive到Modified的迁移，不必发送Invalidate Message了)*

- 变量被第二次加载：Shared
- 存在CPU修改某Shared变量：Modified
- Shared变量被Modified后：Invalidate，此时其他的Cache需要重复再去读这个变量，达到一致。

![](https://i.loli.net/2019/09/27/zgEm13YFbKQGuwd.png)

![img](https://pic1.zhimg.com/80/v2-a5c24a9cde82007fa62de9b66cf03a60_hd.jpg)

> 对于一个cache line，一旦有人是M，那么其他人统统应该I。我I的时候，别想干嘛干嘛。S的时候比人也可S。E的话，因为是独占，所以不和I之外的状态共享。

### MESIF

新增forwarding：

对于S，如果有一个新的core想有这份copy，应该由哪个core来响应？

**如果每个持有该cacheline的core都来应答就会造成冗余的数据传输**, 所以我们在这么多core中选一个设置为F，叫F传输就好了。

![img](https://pic4.zhimg.com/80/v2-f1777664f679c926364ad6c9438e1db7_hd.jpg)

## TAS&CAS

> 原理：当2个core CAS的时候：
>
> 两者都是Shared时候：需要一个M/E来invalid对方，此时双方都会向ring bus发送invalidate操作，那么在ringbus上就会根据特定的设计协议仲裁是core0,还是core1能赢得这个invalidate，胜者完成操作，失败者需要接受结果（返回起点）；

- `std::atomic_flag` -> `test_and_set()` -> `clear()`
- `std::atomic<T>::compare_exchange_weak/strong(std::ref(期待值)，新的值)`（如果他原本是期待值，那么就给他新值）

```c++
bool atomic::cas(int& expected, int new_val)
{// 只是逻辑而已，还是需要硬件(特殊指令)
  	if(this->val == expected){
      	this->val = new_val;
				return true;
    }
  	return false;
}
```

> weak版本的CAS允许错误的返回（比如在字段值和期待值一样的时候却返回了false），不过在一些循环算法中，这是可以接受的。通常它比起strong有更高的性能。

## 编译器优化

> 编译器被允许在不改变单线程程序执行结果的前提下，进行各种优化。

- 公共子表达式删除

```c++
a = b*c + h;
d = b*c * 3;
// tmp <= b*c
// a += tmp
// d *= tmp
```

- 死代码删除
- 寄存器分配（多次读优化）
- 指令调度（流水线重排——为的就是提高指令发射数量）

## Memory Model

> 用于规范多线程访问共享存储器时的行为。（决定了多线程程序的正确性）

- 单处理器（且没有DMA）的单线程程序无需考虑Memory Model
- 汇编语言的memory model由处理器规定和实现，不可移植；高级语言的memory model由编译器和目标处理器共同实现，可移植；

-  the default mode for atomic loads/stores in C++11 is to enforce *sequential* consistency.

### CPPREFERENCE

- byte为最小可寻址内存单位
- 内存位置为标量对象或非零长位域的最大相接序列（不满8bit则延续）
- 线程与数据竞争：
  - 对一个正在写的内存位置进行操作，会触发数据竞争，除法
    - 同线程或同一信号处理函数
    - 2个都是原子操作
    - 使用memory order的`happens-before`

#### 内存顺序 memory_order

| 值                     | 解释                                                         |
| ---------------------- | ------------------------------------------------------------ |
| `memory_order_relaxed` | 宽松操作：没有同步或顺序制约，**<u>仅对此操作要求原子性</u>**（见下方[宽松顺序](https://zh.cppreference.com/w/cpp/atomic/memory_order#.E5.AE.BD.E6.9D.BE.E9.A1.BA.E5.BA.8F)）。 |
| `memory_order_consume` | 有此内存顺序的加载操作，在其影响的内存位置进行*消费操作*：当前线程中依赖于当前加载的该值的读或写不能被重排到此加载前。其他释放同一原子变量的线程的对数据依赖变量的写入，为当前线程所可见。在大多数平台上，这只影响到编译器优化（见下方[释放消费顺序](https://zh.cppreference.com/w/cpp/atomic/memory_order#.E9.87.8A.E6.94.BE.E6.B6.88.E8.B4.B9.E9.A1.BA.E5.BA.8F)）。 |
| `memory_order_acquire` | 有此内存顺序的加载操作，在其影响的内存位置进行*获得操作*：当前线程中读或写不能被重排到此加载前。其他释放同一原子变量的线程的所有写入，能为当前线程所见（见下方[释放获得顺序](https://zh.cppreference.com/w/cpp/atomic/memory_order#.E9.87.8A.E6.94.BE.E8.8E.B7.E5.BE.97.E9.A1.BA.E5.BA.8F)）。 |
| `memory_order_release` | 有此内存顺序的存储操作进行*释放操作*：当前线程中的读或写不能被重排到此存储后。当前线程的所有写入，可见于获得该同一原子变量的其他线程[释放获得顺序](https://zh.cppreference.com/w/cpp/atomic/memory_order#.E9.87.8A.E6.94.BE.E8.8E.B7.E5.BE.97.E9.A1.BA.E5.BA.8F)），并且对该原子变量的带依赖写入变得对于其他消费同一原子对象的线程可见（见下方[释放消费顺序](https://zh.cppreference.com/w/cpp/atomic/memory_order#.E9.87.8A.E6.94.BE.E6.B6.88.E8.B4.B9.E9.A1.BA.E5.BA.8F)）。 |
| `memory_order_acq_rel` | 带此内存顺序的读修改写操作既是*获得操作*又是*释放操作*。当前线程的读或写内存不能被重排到此存储前或后。所有释放同一原子变量的线程的写入可见于修改之前，而且修改可见于其他获得同一原子变量的线程。 |
| `memory_order_seq_cst` | 有此内存顺序的加载操作进行*获得操作*，存储操作进行*释放操作*，而读修改写操作进行*获得操作*和*释放操作*，再加上存在一个单独全序，其中所有线程以同一顺序观测到所有修改（见下方[序列一致顺序](https://zh.cppreference.com/w/cpp/atomic/memory_order#.E5.BA.8F.E5.88.97.E4.B8.80.E8.87.B4.E9.A1.BA.E5.BA.8F)）。 |

#### 例子

- Happens-before: `release - acquire`

## Atomic

### Lock-free atomic

- Run-time: `is_lock_free` （因为是平台相关的——alignment）
- C++17 =>(compile time) `is_always_lock_free`
- alignment must be power of 2

#### Alignments matters

```c++
struct M1{ // lock free when it's 16 byte alignment.
  				 // 16 bytes => %mmx
  	long x, y;
}

struct M2{ // lock free;
  	long x;
  	int y; // aligned to 16 bytes
}

struct M2{ // NOT lock free; => aligned to 12 bytes != 16
  	int x, y, z;
}
```

### False sharing

> It happends when 2 (atomic) **<u>*writting*</u>** goes on the same cache line.
>
> (So reading to shared variable is faster than writing.)

```c++
// Double-checked locking pattern.
// Optimized in hardware.
bool cas_strong(T& old_, T new_)
{
  	T tmp = val; // atomic value
  // >>> First check.(Not locked)
  	if(tmp != old_)
    {
      	old_ = tmp;
      	return false;
    } // tmp == old_
  
  	LOCK;
  	tmp = val;
  	if(tmp != old_)
    {
      	old_ = tmp;
      	return false;
    } // tmp == old_
		val = new_;
  	return true;
}

bool cas_weak(T& old_, T new_)
{// in x86 it doesn't.
  	T tmp = val;
  	if(tmp != old_)
    {
      	old_ = tmp;
      	return false;
    }
  	TIMED_LOCK;
  	if(TIME_OUT)
      	return false; // Cannot get the lock in time;
  	if(tmp != old_)
    {
      	old_ = tmp;
      	return false;
    }
  	val = new_;
  	return true;
}
```

### 2 std::CAS 

- `compare_exchange_strong` ---- true when success
- `compare_exchange_weak` ---- maybe false when success

## Lock-Free QUEUE

```c++
struct node{
  	int   val;
  	node* next;
};

std::atomic<node*> head;

template<typename T>
void push_front(T&& x)
{
  	node* new_ = new node;
  	new_->val = std::forward<T>(x);
  	node* old_ = head;
		do{  new_->next = old_;   }
  	while(!head.compare_exchange_strong(old_, new_));
}
```

### Memory Barrier

> It controls how changes to memory made by one CPU become visible to other CPUs.

In C++11 `memory barrier => memory_order`

#### Relaxed Memory Barrier

Reorder whatever the compiler & processor they want.

#### Acquire Memory Barrier

- Half barrier.

> All operations behind the barrier become visible(in order).
>
> 即，acquire之后的操作，一定是放在acquire之后，不能重排到前面。
>
> - 后面的不可以到前面
> - 前面的可以到后面

#### Release Memory Barrier

> - 前面的不可以到后面
> - 后面的可以到前面

#### Acquire + Release Memory Barrier

- Thread 1 writes sth to **x** => release
- Thread 2 reads sth from **x** => acquire

Things before `x.release_store` will be seen after `x.acquire_load`.

#### Acq_rel Memory Barrier

2 together. No operator can move across the barrier.

> 之前的在之前发生，之后的在之后 发生。

#### Seq_cst Memory Barrier

2个Seq_cst之间的操作是固定的；

#### Back2CAS

```c++
bool cas_strong( // Double check implementation.
  T& old_,
  T  new_,
  memory_order on_success, 
  memory_order on_failure) // they're seq_cst by default.
{
  	T tmp = v.load(on_failure);
  	if(tmp != old_)
    {
      	old_ = tmp;
      	return false;
		}
  	LOCK;
  	tmp = v;
  	if(tmp != old_)
    {
      	old_ = tmp;
      	return false;
		}
  	v.store(new_, on_success);
  	return true;
}
```

#### Why change memory_order

- <u>*Performance*</u>（It depends on platform => memory barriers are very expensive on ARM.But for other architecuture it's more expensive）
  -  <u>**On X86**</u>. 
    - <u>**All loads are aquire loads so *acquire* loads are free.**</u>
    - **<u>All stores are release stores so *release* stores are free.</u>** 
    - **<u>All read-modify-write are *acq-rel*</u>**
    - **<u>No difference between *acq-rel* & *seq-cst* on X86</u>**
- Expressing intent（表达意图）
- As programmers we address to audiences

> - Relaxed **write** can be as fast as non-atomic **write**.(10~100 faster than seq_cst)
> - `seq_cst` is used when there's multiple atomic vars. Or the programmer don't know how to deal with memory order.

## Others

- X86 atomic operations are slower 特别是write
- Atomic比mutex快10倍（spinlock可调优，常识是当线程多余核数时spinlock会很低效）