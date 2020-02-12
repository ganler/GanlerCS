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
  - GRPC protocol
  - 100,000 requests per second per core.
- Clipper
  - Model-agnostic
  - REST interface
- Model Server for Apache MXNet
- DeepDetect
- TensorRT Server

