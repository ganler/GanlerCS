> Intuition / high-level : https://www.youtube.com/watch?v=nedY9cj5RNU

### Classification

- Naive Aysnc SGD.
  - Downpour SGD.
- Async SGD with stale gradient handling.
- Sync SGD.
  - Map-Reduce
  - PS Allreduce
  - Ring Allreduce
  - Tree Allreduce
  - BytePS

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