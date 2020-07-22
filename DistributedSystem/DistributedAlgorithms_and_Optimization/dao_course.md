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

## Lecture 2

### Scaling Types

$$
\texttt{SpeedUp}(p, n)=\frac{T_{1, n}}{T_{p, n}}
$$

> Big O: Upper bound.
>
> Big θ: Tight bound.
>
> Big $\Omega$: Lower bound.

- **Strongly Scalable**
  - $\texttt{SpeedUp}(p, n)=\Theta(p)$
  - Given more(p) processors and n workload, speedup is p.
- **Weakly Scalable**
  - $\texttt{SpeedUp}(p, np)=\Omega(1)$
  - Given more(p) processors and p-linear workload, the speedup is at least of constant.
  - When adding each processor, we can add relevantly more data to help scaling.
- **Embarassingly Parallel**
  - There's **no dependency** between the operations.

> Brent's theorem assumes optimal scheduling is NP hard.
>
> However, we can still have a polynomial time constant approximation algorithm for optimal scheduling, where these bounds still holds.

### Scheduling

#### The Simple Greedy Algorithm

Prerequisite: 

- Knowing the running time of all tasks.
- Jobs are independent.

Def: Always assign the next job to the processor that has least load.

Features:

- Online
- Greedy
- Linear approximate to the optimal algorithm

#### Other variants of scheduling

- Jobs have dependencies.
- Online / Offline.

#### Optimality of the greedy approach

- For online/dependent jobs, the optimal scheduling solution is NP hard.
- We can use approximation algorithms.
- The Simple Greedy Algorithm has an approximation ratio of 2.

$$
{\rm OPT} \ge \frac{\sum_{i=1}^N J_i}{p}\\
{\rm OPT} \ge \max_i J_i
$$

For greedy approach, let $J_t$ denote the last job executed(i.e. $k=\arg \max_j L_j$). And $L_j$ denotes the total workload of processor j.

- The processor used to have the least load when assigned with $J_t$.

$$
p(L_\max - J_t) \le \sum_{j=1}^p L_j' \le \sum_{j=1}^p L_j = \sum_{i=1}^p J_i \\
L_\max \le \frac{\sum_{i=1}^p J_i}{p} + J_t
$$

- Also, $J_t \le \max_i J_i \le \rm OPT$

$$
L_\max\le 2\times OPT
$$

#### Real World

- We may not know how long our jobs take.
  - Using the historical data to predict.

#### What if we could see the future?

 Sort jobs and execute larger jobs first.

- We can reach 3/2 approximation.
- We can reach 4/3 approximation if we use a [tighter analysis](https://theory.epfl.ch/osven/courses/Approx13/Notes/lecture2.pdf).  
- Intuition: By faking computation until the user takes an action, Spark can sort jobs and to obtain a more efficient scheduler. (lazy evaluation)