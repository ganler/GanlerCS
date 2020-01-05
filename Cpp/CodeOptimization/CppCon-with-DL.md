# CppCon-With-DL

# CppCon 2017: Peter Goldsborough “A Tour of Deep Learning With C++”

### Hierarchy(OSI model)

- Task：What we are doing?
- Model：What model should we use?
- Layer：What layers in the model?
- Graph：How the graph is built within the layer?
  - We can think about how to place these operations onto different devices.
  - Parallelism
    - Data Parallelism: Parameter Server
      - We have different machines host the same models.
      - And we use a parameter server to do the synchronization.
        - Local machines tell the PS the gradients.
        - PS updates the parameters.
        - Send it back to local machines.
      - 1-N model. So the transmission
        - Send: O(1) => broadcasting
        - Receive: O(N)(it depends on the time of proceesing and transmission)
          - But I think that, fork-join can make it O(log N)
    - Model Parallelism: 
      - The computation graph is split onto different machines.
      - What we need to do is layer-layer synchronization.
      - The only choice for super big models.
      - 1-1-1.... The transmission. 
  - Static Graph:
    - The graph is defined once.
    - All super parameters must be defined ahead of time.
    - Bad for control flow.
      - You have to add operations to the graph.
    - You can compile them.
    - TF/Caffe2
  - Dynamic graph:
    - Defined by running.
    - You have to trace the input & output.
    - PyTorch.

- Operator: How to represent the node in the graph?
  - Hardware-dependent abstraction.
  - Customized operators.
  - e.g. Map a 3D cov(both data & kernel) to a matrix mulplication(im2col).

<img src="https://s2.ax1x.com/2020/01/05/lDEwtg.png" alt="image" style="zoom: 50%;" />



- Kernel: Impl code for hardwares.
  - Forward.
  - Backward.
  - Quanatization(speedup!!!).
    - Mapping.
    - Gemmlowp
      - GEMM(General matrix to matrix multiply)
  - cuDNN/cuBLAS
  - MKL

- Hardware: The hardware impl of the kernel.
  - CPU
  - GPU
    - Multi-GPU: NVLink 12x faster than PCIe.
  - ASIC(Good at one thing.)

### GAN

- Random values => **Generator** => New Images.
- **Discriminator** => judge the quality and feedback.

Finally we got the models and what we need to do is to give some random values to generator.