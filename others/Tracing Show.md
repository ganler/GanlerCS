# Tracing Show

## General

- **惊群问题**是[计算机科学](https://zh.wikipedia.org/wiki/计算机科学)中，当许多进程等待一个事件，事件发生后这些进程被唤醒（Notify ALL），但只有一个进程能获得CPU执行权，其他进程又得被阻塞，这造成了**严重的系统上下文切换代价**。[[1\]](https://zh.wikipedia.org/wiki/惊群问题#cite_note-1)[[2\]](https://zh.wikipedia.org/wiki/惊群问题#cite_note-2)
- 大页抖动（GB level):
  - 普通的malloc/jemalloc容易发生抖动
  - 用透明巨页：缺页中断会更少
  - 处理数据量up后，出现数秒卡顿问题：
    - 猜测：
      - 错误代码
      - 锁
      - 磁盘问题
      - 线程调度
      - 内存碎片（bodyinfo）
    - Drsnoop -> bg_updator（加载模型的线程）
      - 内核代码分析：
        - do_page_fault
        - 拿锁
        - 分配透明大页
      - 参数：`/sys/kernel/mm/transparent_hugepage/defrag`
        - always
        - defer
        - madvise
        - never
  - 内存排布
    - embedding：
      - vector向量
      - bias 标量
      - 方法1：分开
        - 内存对其
        - 指令优势
      - 2：[vec]\[bias]
        - cache friendly
  - 缓存池对象
    - 带过期时间的对象缓存池（kv-based）
    - 访问：map（O(1)）
    - 过期队列：queue
    - 过期的操作：remove k~v