# OpenCL Notes

## 异构计算速记

#### 常用技术

- 向量化：SIMD(单instruction多data，CPU为主)，SIMT(单instruction多thread)；

- 多核：

  > 以arm cortex-A72为例：2级缓存：
  >
  > - 一级缓存 L1 Cache
  >   - 32KB数据缓存
  >   - 48KB指令缓存
  > - 512KB ~ 2MB 的二级共享缓存 L2 Cache

- 多路：多核在主板上，其之间靠共享缓存交换数据。NUMA（非一致性内存访问）特性。

#### 单核瓶颈

> 单核的好处在于降低了编程的复杂度，程序不需要对多核体系进行设计/修改。一份代码用到老。

单核提升的方法：

- **提升频率**：功耗和散热是瓶颈，功耗和频率的增长近似成3次方正比；
- **指令级并行**：
  - 超标量，无关的程序一起并行执行；
  - 流水线：
    - IF (Instruction Fetch) 取指令
    - ID (Instruction Decode) 指令解码
    - EX (Execution) 执行
    - MEM (Memory Access) 访存
    - WB (Write Back) 写回

> 指令级并行的提升需要代码优化人员，编译器作者和处理器设计者的共同努力，
>
> 硬件层面有：
>
> - 指令重排
> - 发射队列
> - 寄存器重命名

所以硬件厂商将多个处理器放到一个芯片上，通过稍微降低频率和电源，以减少多核的总功耗和散热。

> Anyway，现在来说最狠最立竿见影的技术是 **多核** 和 **向量化**。

#### OpenCL

就是拿来搞异构计算的。

> 写一个C/C++程序，去build和执行一个kernel（一个用OpenCL C写的文件）即可。

有1.0, 1.1, 1.2, 2.0, 2.1, 2.2几个版本。2.1开始用C++作为其API，之前的版本是用OpenCL C，即C语言API。

## OpenCL模型

#### 平台模型`cl::Platform`

- 一个平台由host和一些设备(`cl::Device`)组成；
- 一个设备可以拥有很多计算单元(Compute Unit, CU)；
- 一个计算单元又可以有多个处理单元(Processing Unit, PU)；

![](https://i.loli.net/2019/07/11/5d2734703b4bb54463.png)

> 在OpenCL中，一个平台模型应该只要有2中处理器(不然咋交异构哇)，这些处理器主要靠PCI-e总线和主机相连接。(如上图所示)

#### 执行模型

OpenCL的代码分成2种程序：

- 主机程序(就是host方写的程序)；
- kernel程序；

> 主机端程序在运行的时候，通过一些命令来讲kernel程序丢到各种Device上执行。

OpenCL定义了3种内核：

- OpenCL内核（OpenCL C编写，用OpenCL C编译器编译的函数）；
- 原生内核（OpenCL之外创建的函数，可以用过用过函数指针访问）；
- 内建内核（待补充）；

> IO由主机承担，这意味着用OpenCL的时候，要把数据从主机发到Device，然后又从Device中拿回来。

#### `cl::Context`上下文

上下文定义的是一个执行环境。

- 一群设备：
> `std::vector<cl::Devices> devices = context.getInfo<CL_CONTEXT_DEVICES>();`

- 内核对象：`cl::Kernel`
- 程序对象：`cl::Program`
- 存储器对象；

#### `cl::CommandQueue`命令队列

通过命令队列与设备交互；

- 内核入队；
- 存储器入队；
- 同步命令；
- 按序执行；
- 乱序执行；

#### 内核在OpenCL设备上的执行

待补充

https://downloads.ti.com/mctools/esd/docs/opencl/execution/kernels-workgroups-workitems.html

#### OpenCL存储器模型

- **主机内存**(主机)
- **全局存储器**：可以被所有的工作项读写；(主机，如果Device有缓存的话就在Device中)
- **常量存储器**：对工作项而言是只读的，主机负责初始化和分配；(同上)
- **局部存储器**：对工作组而言是局部可见的，可用来分配共享变量；(对于计算单元来讲)
- **私有存储器**：一个工作项的私有存储器对其他工作项是不可见的；(对于工作项而言)

#### 存储器对象

- **缓冲**(`cl::Buffer`)
- **图像**`cl::Image`
- **管道**；

主机和设备的交互方式：读/写/填充、映射和解映射以及拷贝；

#### 共享虚拟存储器(Shared Virtual Memory, SVM)

> OpenCL2.0

通过映射将全局存储区域映射到主机可以访问的地址空间；

- 细粒度SVM：共享发生在OpenCL缓冲对象区域，但是以字节加载/存储粒度使用；
- 粗粒度SVM：共享发生在OpenCL缓冲存储器对象区域的粒度；
- 细粒度系统SVM：待补充；

