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