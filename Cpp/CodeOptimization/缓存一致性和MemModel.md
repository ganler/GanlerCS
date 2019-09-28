## Latency data

> [SRC](https://gist.github.com/hellerbarde/2843375)

![pic](https://cloud.githubusercontent.com/assets/1489514/19324275/be41885c-908e-11e6-95a2-68fa75333c38.jpg)

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

## Memory Model

> 待续。