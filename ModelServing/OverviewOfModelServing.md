# ModelServing

https://medium.com/@vikati/the-rise-of-the-model-servers-9395522b6c58

## Why Model Serving

### Significance

- Simplify model deployment at scale.(consistent API)
- Accelerate the deeplearning application(better scheduling methods like accumulating bigger batch).

### What people used to do

Hide their model behind a Flask app.

- No consistent APIs.
- No model versioning.
- No mini-batching.

### The birth of Model Serving

At the [2017 NIPS](https://nips.cc/Conferences/2017), the developers of many of these model servers provided updates on their increasingly mature projects. In general, the model server use case is straightforward. By pointing the model server at one or more trained model files, the model server can now serve inference queries at scale. The server handles scaling, performance, and some model lifecycle management.

![](https://miro.medium.com/max/1976/1*EQKPGTl8ifI_XMQ-JndEHw.png)

Some famous model server:

- TF Serving
  - GRPC protocol.
  - 100,000 requests per second per core.
- Clipper
  - Model-agnostic.
  - REST interface.
- Model Server for Apache MXNet(MMS)
  - Nicely packaged with Docker images and automated setups.
  - A strong feature of MMS is the ability to package custom processing code in its model archive. This means that feature engineering can be transparently run in the model server.
  - MMS includes an automatic nginx-based HTTP endpoint out of the box（开箱即用）, so inference can be run easily by existing apps.
  - MMS also provides real-time metrics for monitoring utilization, latencies, and errors of the endpoint and the inference service.
- TensorRT Server

![](https://docs.nvidia.com/deeplearning/sdk/tensorrt-inference-server-guide/docs/_images/arch.png)

[[Credit]](https://docs.nvidia.com/deeplearning/sdk/tensorrt-inference-server-guide/docs/architecture.html)

- The TensorRT Inference Server architecture allows multiple models and/or multiple instances of the same model to execute in parallel on a single GPU.
- By default, if multiple requests for the same model arrive at the same time, the inference server will **serialize** their execution by scheduling only one at a time on the GPU.

![_images/multi_model_serial_exec.png](https://docs.nvidia.com/deeplearning/sdk/tensorrt-inference-server-guide/docs/_images/multi_model_serial_exec.png)

- The TensorRT inference server provides an [instance-group](https://docs.nvidia.com/deeplearning/sdk/tensorrt-inference-server-guide/docs/model_configuration.html#section-instance-groups) feature that allows each model to specify **how many parallel executions of that model should be allowed**. Each such enabled parallel execution is referred to as an *execution instance*. By default, the server gives each model a single execution instance, which means that only a single execution of the model is allowed to be in progress at a time.

![_images/multi_model_parallel_exec.png](https://docs.nvidia.com/deeplearning/sdk/tensorrt-inference-server-guide/docs/_images/multi_model_parallel_exec.png)

- To provide the current model execution capabilities shown in the above figures, the inference server uses [CUDA streams](https://devblogs.nvidia.com/gpu-pro-tip-cuda-7-streams-simplify-concurrency/) to exploit the GPU’s hardware scheduling capabilities. For example, using streams allows the GPU to execute a memory-copy for one model, a kernel for another model, and a different kernel for yet another model at the same time.

![_images/cuda_stream_exec.png](https://docs.nvidia.com/deeplearning/sdk/tensorrt-inference-server-guide/docs/_images/cuda_stream_exec.png)

