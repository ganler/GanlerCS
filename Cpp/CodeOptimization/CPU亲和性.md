# CPU亲和性

> 笔记，摘自[rootlee](http://irootlee.com/juicer_cpu_affinity/)
>
> https://toutiao.io/posts/60r22r/preview

引用一下维基百科的说法，CPU亲和性就是绑定某一进程（或线程）到特定的CPU（或CPU集合），从而使得该进程（或线程）只能运行在绑定的CPU（或CPU集合）上。CPU亲和性利用了这样一个事实：进程上一次运行后的残余信息会保留在CPU的状态中（也就是指CPU的缓存）。<u>如果下一次仍然将该进程调度到同一个CPU上，就能避免缓存未命中等对CPU处理性能不利的情况，从而使得进程的运行更加高效。</u>

<u>CPU亲和性分为两种：软亲和性和硬亲和性</u>。软亲和性主要由操作系统来实现，Linux操作系统的调度器会倾向于保持一个进程不会频繁的在多个CPU之间迁移，通常情况下调度器都会根据各个CPU的负载情况合理地调度运行中的进程，以减轻繁忙CPU的压力，提高所有进程的整体性能。除此以外，Linux系统还提供了硬亲和性功能，即用户可以通过调用系统API实现自定义进程运行在哪个CPU上，从而满足特定进程的特殊性能需求。

#### Application

在Linux中，每个进程对应一个`task_struct`，其中有个`cpus_allowed`位掩码，其<u>位数</u>和cpu核数相等(当然用`std::thread::hardware_concurrency()`是一样的)，若超频则为cpu核数的2倍。

Linux系统为我们提供了CPU亲和性相关的调用函数和一些操作的宏定义，函数主要是下面两个：

- `sched_set_affinity()` （修改位掩码）
- `sched_get_affinity()` （查看当前的位掩码）

除此之外还提供了一些宏定义来修改掩码，如`CPU_ZERO()`(将位掩码全部设置为0)和`CPU_SET()`(设置特定掩码位为1)。
