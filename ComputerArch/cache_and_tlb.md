# Cache&TLB

[TOC]

## 分类

- Instruction Cache
- Data Cache
- TLB(for MMU)

![img](https://pic3.zhimg.com/80/v2-0e77de9fe46e80c179204da1bf9ad6b2_hd.jpg)

## Cache Line

- X86 ~ DDR3/4 ~ 64 bytes.
- 旧架构的 ARM ~ 32 bytes.

> CPU从主存取数据的最小单位是cache line
>
> 主存从硬盘拿数据的最小单位是4MB

### 替换策略 Replacement Policy

- FIFO
- LRU（在FIFO的基础上，会将最近使用cache line放到队列的头头）

### 一致性

#### 读

- 数据不在Cache ~ Cache Miss ~ 从下一级存储中将其存入当前Cache （若满了就换掉victim）
- 命中：Cache Hit

#### 写

- **Write through**: 直接写到主存
- **Write back**: 写到Cache（此时Cache的数据与Memory不一样，Cache变成dirty的了），当DMA访问这段数据的时候需要通过缓存一致性去取最新的数据。

> 补充一下DMA相关的内容：
>
> DMA传输常使用在将一个内存区从一个设备复制到另外一个。当中央处理器初始化这个传输动作，传输动作本身是由**DMA控制器**来实行和完成。典型的例子就是移动一个外部内存的区块到芯片内部更快的内存去。
>
> 个人电脑的ISA DMA一般有8个DMA通道：
>
> - 其中7个可以让CPU利用
> - 每个DMA
>   - 一个16位地址寄存器
>   - 一个16位计数寄存器
> - 初始化数据传输时，设备驱动程序一起设置DMA通道的地址和计数寄存器&传输方向（R / W），然后指示DMA开始这个工作。
> - **传输结束时通过中断的方式来通知中央处理器。**
>
> **缓存一致性的问题**：
>
> DMA直接访问的是内存，当CPU访问内存某个地址时，会先将其写入缓存，在Write之后，内存数据并未更新，若此时发生DMA，那么DMA会读到未更新的数据。（DMA写内存也是同样的道理）
>
> **解决方案**：
>
> - **缓存同调系统**：外部设备写内存的时候，用一个信号来通知缓存控制器已过期/应更新；（通过信号和缓存一起同步）
> - **非同调系统**：用软件方法完成，OS在读取缓存的时候，禁止DMA；（会造成DMA系统的负担）
>
> > 除了与硬件交互相关外，DMA也可为昂贵的内存耗费减负。比如大型的拷贝行为或[scatter-gather](https://zh.wikipedia.org/w/index.php?title=Vectored_I/O&action=edit&redlink=1)操作，从中央处理器到专用的DMA引擎。[Intel](https://zh.wikipedia.org/wiki/Intel)的高端服务器包含这种引擎，它被称为[I/O加速技术](https://zh.wikipedia.org/wiki/I/O加速技术)（IOAT，Linux内核则是自2006年开始使用，但此特性据称由于缺乏性能优势和造成数据损坏的可能性，在之后被禁用）

多核且每个核有独立Cache的情况下，要看一些缓存一致性协议，如MESI。

> **提速技术**：
>
> 在Cache Miss的情况下，我们往往会消耗很多时间去从Memory中拿数据，这个时间往往足够CPU取个百把条指令，这个时候我们会：
>
> - 乱序执行：提前执行后面的不相关的指令；
> - 超线程：多套寄存器，当前这个线程停下来去取数据了，那么CPU马上执行另外一个线程；

## Great Article About Virtual Memory

https://courses.cs.washington.edu/courses/cse351/18au/lectures/23/code/vm_overview.pdf

访问内存时，我们需要将虚拟地址翻译为物理地址，这个时候我们需要用到一个叫做**MMU**的硬件。

- 每个进程有自己的页表；

- 查看页表内的内容：

  - **PTBR**：Page Table Base Register(用来定位页表的第一个PTE)；
- **PTE**：页表的每个索引条目；
  

### TLB: The Translation Lookaside Buffer

A cache for the page table.

Each block is a **PTE**. 如果要找的entry不在TLB，那么会发生TLB miss，然后TLB会从内存里面找PTE。

![](https://i.loli.net/2019/10/31/dca8IyF95J16sLv.png)

![](https://i.loli.net/2019/10/31/wjIOkK6RaWCmQcA.png)

![](https://i.loli.net/2019/10/31/rzQSvKcqWgRMTyN.png)




## Ref

> https://zhuanlan.zhihu.com/p/31875174