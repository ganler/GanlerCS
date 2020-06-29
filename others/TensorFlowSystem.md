[TOC]

## TF 2 Peformace Profiler

> https://www.youtube.com/watch?v=pXHAQIhhMhI

- TensorBoard -> Overview Page

![image-20200617142850975](/Users/bytedance/Desktop/Dropbox/main/GanlerCS/others/image-20200617142850975.png)

- I/O Pipeline Analyzer: Telling whether your app is input bound.
- TensorFlow Stats: 
- Trace Viewer: Timeline

https://blog.tensorflow.org/2020/04/introducing-new-tensorflow-profiler.html

https://www.tensorflow.org/tensorboard/tensorboard_profiling_keras

- Optimize `dataset.map`:
  - Sequential map to parallel map.
  - `num_parallel_calls=tf.data.experimental.AUTOTUNE`

## XLA

- XLA: Accekerated Linear Algebra
  - JIT compiler(Training + Inference)
  - AOT: Ahead-Of-Time(Usually For Inference): https://www.tensorflow.org/xla/tfcompile
    - TensorFlow Graph + `tfcompile` = C++ Headers + Libraries(Quite like TVM, aha.)
    - Creates executable with minimal TF Runtime needed.
    - Commonly used for mobile devices inference graph.
    - Mainly for CPU. 

## GTT: Graph Trainsform Tool

- Remove training ops.
- Optimization for inference.
  - Given inputs & outputs, remove unreachable nodes. (**Traverse from the output node.**)
  - Op Fuse.
  - Quantinization.

## TF Serving

![image-20200617171624898](/Users/bytedance/Desktop/Dropbox/main/GanlerCS/others/image-20200617171624898.png)

