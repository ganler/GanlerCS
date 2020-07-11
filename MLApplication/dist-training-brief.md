> Intuition / high-level : https://www.youtube.com/watch?v=nedY9cj5RNU
>
> https://zhuanlan.zhihu.com/p/79030485

### Classification

> Sync SGD: What do we care:
>
> - Communication rounds(how many times u gonna do recv/send).
> - Total communication workload.
> - Inbound/bottleneck bandwidth.
>
> For ideal situation where machines get the same computing and communication speed, we can model like this:
>
> - For non-master mode
>
> $$
> T= R(N) \times \left\{\alpha + S\times \left(\frac{1}{B}+\frac{1}{C}\right) \right\}
> $$
>
> - N: Number of machines
> - \alpha: Comm. overhead per round.
> - S: Parameter packet size
> - B: Bandwidth
> - C: Computing speed(Say, FLOPS)

- Naive Aysnc SGD.
  
  - Downpour SGD.
  
- Async SGD with stale gradient handling.

- Sync SGD(about reduce for all machines): Non-master approach: Computing => Communication.
  - Parameter Server: Reduce + Broadcast(Master)

  <img src="https://pic1.zhimg.com/80/v2-d42c33c98d0bec0b8567373941d1c7fc_1440w.jpg" alt="img" style="zoom:50%;" />

  - Fully Connected Allreduce.
  - Ring Allreduce(Non-master, too many rounds of communications)

  $$
  2(N-1)\times\left(\alpha + \frac{S}{NB}\right)+(N-1)\times\left(\frac{S}{NC}\right)
  $$

  

  - Recursive halving and doubling(Master)

  > Power of 2:
  >
  > <img src="https://pic3.zhimg.com/80/v2-796d17b24c0ad6bf9c70e4e2e683281a_1440w.jpg" style="zoom:50%;" />
  >
  > Otherwise:
  >
  > <img src="https://pic1.zhimg.com/80/v2-88f10c69348ab90baeeba68351c292cc_1440w.jpg" alt="img" style="zoom:50%;" />

  > Limitation: The bandwidth of machines are not fully utilized.

  - Butterfly(No master):

  > <img src="https://pic2.zhimg.com/80/v2-77c19f1a584634f084dbfb25d179b5c5_1440w.jpg" alt="img" style="zoom:50%;" />

  - Segmented Ring
    - For big volume of data.
  - Double Binary Tree Allreduce(NCCL)
    - NCCL:
      - 1.x: Single machine. 
      - 2.x: Multi-node.
      - 2.4: DBT
  - BytePS: add additional bandwidth server(pure CPU).
  - Blink: For heterogeneous communication.
  - Baidu: Hierarchy Ring:

  ![img](https://pic2.zhimg.com/80/v2-83392bc108bd54ad11692abf03703de1_1440w.jpg)

  <img src="https://pic4.zhimg.com/80/v2-4ddb9b977f5418ec64b292da594e0727_1440w.jpg" alt="img" style="zoom:50%;" />

  <img src="https://pic2.zhimg.com/80/v2-c4085be63aa220ed13bf47e6fde9a6e9_1440w.jpg" alt="img" style="zoom:67%;" />

  #### OpenMPI Logic

  <img src="https://pic4.zhimg.com/80/v2-65aa14466ca96726998129e8cd40a1db_1440w.jpg" alt="img" style="zoom:50%;" />

### Async

- HOGWILD! (nips 2011)

  - lock-free && tradutional ML.
  - Memory Model: Shared. Atomic R/W.
  - Optimization Functions: Seperate them.
    - Sample: Pick one using uniformly random.
    - Calc gradient.
  - Example: [Matrix Completion](https://www.zhihu.com/question/47716840)
    - Assumption: low rank
    - What we have: X
    - What we want: completed X -> Z

  ![](https://pic1.zhimg.com/80/45dd41656dce01fe99124c71f694d30c_1440w.jpg?source=1940ef5c)

  - https://www.youtube.com/watch?v=l5JqUvTdZts
  - https://papers.nips.cc/paper/4390-hogwild-a-lock-free-approach-to-parallelizing-stochastic-gradient-descent
  - http://d0evi1.com/hogwild/

### Limitations

- Batch norm is a mess. We can only do batch norm per GPU, not per mini batch.