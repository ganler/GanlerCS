## Introduction

Followings in this note are primitives for management of threads in a block/warp;

### Hardware view

GPU -> SM (Stream Multiprocessors) -> ~8 SP (Stream Processor)

### Software/SDK view

Grid -> Blocks -> Threads (granularity = warp)

For each kernel function we define:

* \# of blocks: how many thread blocks;
* \# of threads: how many threads in one block;

* **Block-Level Scheduling:** When executing one kernel function, blocks will be scheduled to map different SMs. (blocks[software] -> SMs[hardware]) 
* Then, one thread block will be split into several warps (32 threads).

## Shared Memory [Block Level]

* Shared memory is allocated **per thread block**; (even more high-level than warp-level)
* about **100x faster** than global memory if no bank confliction;

```c++
// https://developer.nvidia.com/blog/using-shared-memory-cuda-cc/

// Call by staticReverse<<<_, _>>>
__global__ void staticReverse(int *d, int n) {
  __shared__ int s[64];  // Memory amount determined at compile time (64 `int`s per block).
  int t = threadIdx.x;
  int tr = n-t-1;
  s[t] = d[t];
  __syncthreads();
  d[t] = s[tr];
}

// Call by dynamicReverse<<<_, _, shared_mem_bytes>>
__global__ void dynamicReverse(int *d, int n) {
  extern __shared__ int s[];  // Memory amount determined at runtime.
  int t = threadIdx.x;
  int tr = n-t-1;
  s[t] = d[t];
  __syncthreads();
  d[t] = s[tr];
}
```

* Threads in **one block** may also get synchronized using `__syncthreads()`.
* **Bank conflicts:** 
  * Shared memory is divided into equally sized memory modules (banks)
  * Say that we have `N` load/store on `S` banks; If multiple load/store applied on the same bank, the accessing will be serialized (slow!). Thus, a good program should try to let multiple load/store not access the same bank. (make them more divergent).

