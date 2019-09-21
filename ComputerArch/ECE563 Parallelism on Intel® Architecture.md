# NOTES: Fundamentals of Parallelism on Intel® Architecture

[TOC]

## Modern Code

#### Parallel Programming Layers

- Cluster computing
- Multi-threading(MIMD)
- Vectorization(SIMD)

> Instructions technologies(Automatically done by processors);

### Technologies and limits

#### Clock speed and power wall

> Higher clock speed means faster execution time. 但是更高的时钟频率往往意味着更高的热量消耗，这也将导致需要更高的散热成本。所以散热会限制时钟频率；

#### Pipeline and ILP wall

> 多级流水线。但级数是有上限的。一般来说，把指令的所有步骤拆分的越多越好，但（一般来说）最多只能拆分成5级。

#### Superscalar execution and ILP wall

> 超标量技术。让多个指令一起执行。缺点是，我们说的并行指令，要是"相互独立"的指令。而这样做之前，要检查多个指令是否独立，这种操作需要消耗额外的电路和CPU操作。（额外CPU操作即为我们说的上限）（CppCon Google Tech Leader讲过一个微指令并行的操作和这个相似）

#### Out-of-Order Execution and memory wall

> [More info](<https://cseweb.ucsd.edu/classes/fa11/cse240A-a/Slides1/09_OutOfOrderExecution.pdf>)
>
> [Register Renaming](<https://zh.wikipedia.org/wiki/%E5%AF%84%E5%AD%98%E5%99%A8%E9%87%8D%E5%91%BD%E5%90%8D>)

> 乱序执行。即对顺序无关的指令做适当调度，以使之可并行执行。其中的数据冲突问题可以通过寄存器重命名技术完成。
>
> 指令依赖类型：
>
> - RAW(Read after write)
> - WAR
> - WAW

#### Parallelism: Cores and vectors

> 目前来说没有limits，但并不是CPU自动执行的，需要程序员编程是实现。

### Modern Codes

![](https://i.loli.net/2019/05/02/5ccb00ccd7d44.png)

