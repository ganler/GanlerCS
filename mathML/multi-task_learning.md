# MTL

[TOC]

**Aim**: To improve the generalization of models by weighting the training information between the main task and the auxiliary tasks.

- Multiple loss functions.

## Hard parameter sharing

Just use some shared parameters.

- Help overcome overfitting. 

## Soft parameter sharing

Every task has its own parameters. But parameters at the same layer have constraints with each other to represent the similarity(L2 or trace norm).

- Implicit data augementation: Each task have its **sample noise** to some direction, when we are doing multi-stask learning, the noise will be offset by average(just like bagging).
  - Good for tasks with limited samples and <u>high dimensions?</u>.
- A new way of regularization.

## Block-Sparse Regularization

> Force the models to consider part of the features only.
>
> The **premise** is different tasks must be related.

For K tasks and D-dim parameters, we have $A(D\times K)$.

To make `lock-Sparse`, we need let some rows of A to be 0(Or simply use L1 regularization);

- **Group Lasso**: L1/L2 Norm. Divide parameters into groups. MSE + Group-L1.

## Regularization way for learning task relationship

- Relavance of tasks is the key point. Some times we can give them some hints to tell the relavance of tasks(To avoid **negative transfer**).
- Use clustering. 

$$
\Omega(A)= \lambda_1\Omega_{mean}(A)+\lambda_2\Omega_{between}(A)+\lambda_3\Omega_{within}(A)
$$

---

## Deep Relation Network

![](https://i.loli.net/2019/11/23/G35PyAWeIQiMVZK.png)



- Shared convolutional layers;
- Set priorities for parameters in task layers.

## Fully-Adaptive Feature Sharing

Starting from a simple structure, widen the models greedily to make similiar models cluster.

> 贪心方法无法全局最优，每个分支一个任务无法学习任务之间的复杂关系。

![](https://i.loli.net/2019/11/23/Zbxvkr2tgqYP43N.png)

## Cross-Stitch Networks 💖💖💖

==> Soft parameter sharing

![](https://i.loli.net/2019/11/23/TVwnNYE5GMmIOgu.png)

## Weighting Losses with uncertainty

<u>不考虑学习共享的结构，考虑每个任务的不确定性。</u>

通过优化loss（Gaussian likelihood with task-dependant uncertainty），调节不同tasks之间的相似性。

![](https://i.loli.net/2019/11/23/tBpnWycaY1GMxOh.png)

## Tensor Factorisation for MTL

## Sluice Network

![A Sluice Network with two tasks](https://github.com/sebastianruder/sluice-networks/raw/master/sluice_network.png)

## learning what to share(outperform hard parameter sharing)

## Auxiliary Task

- Related task
- Adversarial

## Ref

https://zhuanlan.zhihu.com/p/59413549