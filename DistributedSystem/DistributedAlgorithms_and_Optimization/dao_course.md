# Distributed Algorithms and Optimization

[TOC]

## Lecture 1

> Link of annotation notes. https://kami.app/o23kVNyi0Iaf

### 	PRAM Model

- PRAM: Parallel RAM

- PRAM model variants: {Exclusive, Concurrent} x {Read, Write}

  - For instance, [Ex-Write, Con-Read] is RW lock or `std::shared_mutex` in C++.

  - Ways to do concurrent writes(multiple writes occurred in the same time):

    - **Undefined**: Just let UB goes everywhere.
    - **Arbitrary**: means random. We have K writes and we only apply one write randomly.
    - **Priority**
    - **Combination**: $V_{final}=f(V_0, V_1, \cdots, V_N)$

    

### Defenestration of bounds on runtime

$T_p$ means the amount of (wall-clock) time when executing the algorithm on p (homogeneous) processors.

#### Brent's Theorem

$$
\frac{T_1}{p} \le T_p \le \frac{T_1}{p} + T_\infin
$$

- In the best case, we can have linear speed up.
- $T_\infin$ can be interpreted as how parallel an algorithm can be
  - When there's no sequential part in the algorithm, $\lim_{p\to \infin} T_p=0$. Otherwise, $T_\infin$ is equal the sequential(can not be fully paralleled by infinite processors) computing time of the algorithm.
  - Supposing that each node in the DAG consumes the same time, $T_1 = N_{nodes}, T_\infin=N_{levels}$

> Upper bound prove:
>
> $m_i$ is the sequential computation time of layer i.
> $$
> T_1=\sum^N_{i=1} m_i
> $$
>
> $$
> T_p^i=\left\lceil\frac{m_i}{p}\right\rceil\le \frac{m_i}{p} + 1\\
> T_p\le \frac{T_1}{p}+N=\frac{T_1}{p}+T_\infin
> $$
>
> 

#### Exercise

The upper bound of vector sum(length = n):
$$
T_1=n\\
T_\infin = \log_2 n
$$
