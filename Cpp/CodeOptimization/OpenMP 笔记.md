# OpenMP 笔记

[TOC]

## 表格初始化

#### Normal

```c++
#include <cmath>
#include <iostream>
#include <iomanip>
#include "util.hpp"

#define PRINTSZ 25

int main()
{
    constexpr int sz = 262144;
    double table[sz];

    Timer t;

    for (int i = 0; i < sz; ++i)
        table[i] = std::sin(2 * M_PI * i / sz);
    cout << left << setw(PRINTSZ) << ">>> Naive:" << t.time_out_microseconds() << " μs"  << endl;
    t.reset();


// SIMD
#pragma omp simd
    for (int i = 0; i < sz; ++i)
        table[i] = std::sin(2 * M_PI * i / sz);
    cout << left << setw(PRINTSZ) << ">>> OMP simd:" << t.time_out_microseconds() << " μs" << endl;
    t.reset();


// For omp.
#pragma omp parallel for
    for (int i = 0; i < sz; ++i)
        table[i] = std::sin(2 * M_PI * i / sz);
    cout << left << setw(PRINTSZ) << ">>> OMP for:" << t.time_out_microseconds() << " μs"  << endl;
    t.reset();


#pragma omp target teams distribute parallel for map(from:table[0:262144])
    for (int i = 0; i < sz; ++i)
        table[i] = std::sin(2 * M_PI * i / sz);
    cout << left << setw(PRINTSZ) << ">>> OMP multi-device:" << t.time_out_microseconds() << " μs"  << endl;
    t.reset();
}
```

```c
>>> Naive:               4406 μs
>>> OMP simd:            3330 μs
>>> OMP for:             1839 μs
>>> OMP multi-device:    1211 μs
```

## 语法学习

- 以`#pragma omp parallel`开头

#### `parallel construct`

> The parallel construct starts a parallel block. It creates a *team* of N threads (where N is determined at runtime, usually from the number of CPU cores, but may be affected by a few things), all of which execute the next statement (or the next block, if the statement is a {…} -enclosure). **After the statement, the threads join back into one.**

```c++
 #pragma omp parallel
  {
    // Code inside this region runs in parallel.
    printf("Hello!\n");
  }
// Hello会被跑核数遍；
```

#### `Parallelism conditionality`

```c++
extern int parallelsim_enabled; // 0，可以是动态变量

#pragma omp for if(parallelsim_enabled)
for(...);
```

#### `loop construct`

```c++
#pragma omp for // 只切割任务，并不执行多线程操作
for(...);
// #pragma omp parallel for // 切割任务并且循环
```

> The `for` construct splits the for-loop so that each thread in the current team handles a different portion of the loop.

> 切割for循环的任务并且并行；

#### 确定线程数

```c++
#progma omp paralel num_threads(3)
{
  ...
}
```

#### Scheduling

> schedule只能用于**循环并行构造**中，用于控制循环并行结构的任务调度。
>
> schedule的语法为：
>
> `schedule(kind, chunk_size)`
>
> > `chunk_size`是指分配给每个线程的循环迭代次数。
>
> • `static`:(default) Iterations are divided into chunks of size chunk_size. Chunks are assigned to threads in the team in round-robin fashion in order of thread number.
>
> • `dynamic`: Each thread executes a chunk of iterations then requests another chunk until no chunks remain to be distributed.
> • `guided`: Each thread executes a chunk of iterations then requests another chunk until no chunks remain to be assigned. The chunk sizes start large and shrink to the indicated chunk_size as chunks are scheduled.
> • `auto`: The decision regarding scheduling is delegated to the compiler and/or runtime system.
>
> • `runtime`: The schedule and chunk size are taken from the run-sched-var ICV

```c++
// 如果不指定schedule的话，openmp会把所有的线程都用上
/* STATIC */
#include <omp.h>
#include <iostream>
#define COUNT 4*3

int main() {
// 每个线程分4次，12/4=3总共使用了3线程，如果不指定，那么直接把所有线程都用上
#pragma omp parallel for schedule(static, 4) 
    for (int i = 0; i < COUNT; i++)
        printf("Thread: %d, Iteration: %d\n", omp_get_thread_num(), i);
//        std::cout << "Thread:" << omp_get_thread_num() << ", Iteration:" << i << '\n';
// 发现如果不用printf会乱序...
}

/* DYNAMIC */
// This is most useful when used in conjunction with the ordered clause, or when the different iterations in the loop may take different time to execute.
#pragma omp parallel for schedule(dynamic)
// 简单通俗的理解就是：先给你们这些各分配一个任务，你们搞完了再找我要，我再给。
// 可限定每个线程一次分配的任务数: schedule(dynamic, 4)
// 为了速度，尽量用static
```

**3种scheduling modifiers**:

- monotonic: If a thread executed iteration i, then the thread must execute iterations larger than i subsequently.
- non-monotonic: Execution order not subject to the monotonic restriction
- simd: The `chunk_size` must be a multiple of the simd width.

```c++
#pragma omp parallel for schedule(simd:static)
```

#### 顺序执行

用以保证部分内容按顺序执行。

```c++
#include <omp.h>
#include <iostream>
#define COUNT 4*3

int main() {
#pragma omp parallel for ordered
    for (int i = 0; i < 10; ++i) {
        printf("unordered %d\n", i);
#pragma omp ordered
        printf("ordered %d\n", i);
    }
}
/*

unordered 4
unordered 0
ordered 0
unordered 6
unordered 1
unordered 8
unordered 7
unordered 9
unordered 2
ordered 1
unordered 5
ordered 2
unordered 3
ordered 3
ordered 4
ordered 5
ordered 6
ordered 7
ordered 8
ordered 9

*/
```

#### `collapse clause`

对于多层循环，使用`collapse`可使循环被多处利用；

```c++
#include <omp.h>
#include <iostream>

int main() {
#pragma omp parallel for collapse(2)
    for(int i = 0; i < 5; ++i)
        for(int j = 0; j < 5; ++j)
            printf("thread: %d ==> i: %d, j: %d\n", omp_get_thread_num(), i, j);
}
```

#### `reduction clause`

> The `reduction` clause is a special directive that instructs the compiler to generate code that accumulates values from different loop iterations together in a certain manner. It is discussed in a separate chapter later in this article. 

```c++
// 这样做的目的是保证程序的正确性。

int sum=0;
#pragma omp parallel for reduction(+:sum)
for(int n=0; n<1000; ++n) sum += table[n];
```

#### `section`

Start new teams.

```c++
#pragma omp parallel sections
    {
        std::cout << "section 1" << std::endl; // Run once
#pragma omp section
        std::cout << "section 2" << std::endl; // Run once
#pragma omp section
        std::cout << "section 3" << std::endl; // Run once
    }
```

or

```c++
 #pragma omp parallel // starts a new team
 {
   //Work0(); // this function would be run by all threads.
   
   #pragma omp sections // divides the team into sections
   { 
     // everything herein is run only once.
     { work1(); }
     #pragma omp section
     { work2();
       work3(); }
     #pragma omp section
     { work4(); }
   }
   
   //Work5(); // this function would be run by all threads.
 }
```

