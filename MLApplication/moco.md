# MOCO

[TOC]

> https://zhuanlan.zhihu.com/p/94319787
>
> https://www.bilibili.com/video/av625735603/

### Contrastive Learning: Are this 2 of the same category?

- **Give 2 "contrastive" inputs**: $(X_1 , X_2)$
- **Train outputs**: $Y$ = 0 if they are of the same category else 1.

For pairs of similar and dissimilar inputs, we take different loss functions.($L_S$ and $L_D$) 

$D_W$ is the distance function.

![[公式]](https://www.zhihu.com/equation?tex=%5Cmathcal%7BL%7D+%3D+%5Csum_%7Bi%3D1%7D%5EP+%281-Y%29L_S%28D_W%5Ei%29+%2B+YL_D%28D_W%5Ei%29)

- $L_S$: Y = 0. For similar data, we need to measure the "similarity" and make the distance "closer". So MSE is enough.
- $L_D$: Y = 1. For dissimilar we need to help them push each other away. So margin loss.

![[公式]](https://www.zhihu.com/equation?tex=%5Cmathcal%7BL%7D%28W%2CY%2CX_1%2CX_2%29+%3D+%281-Y%29%5Cfrac%7B1%7D%7B2%7D%28D_W%29%5E2+%2B+Y%5Cfrac%7B1%7D%7B2%7D%5C%7Bmax%280%2Cm-D_W%29%5C%7D%5E2)



### Contrastive Prediction Coding(CPC): How's my representation

> Big question: How to tell whether the representation in representation learning is good or bad?

![img](https://pic1.zhimg.com/80/v2-0894ab367665ad82a9a0172e1c5d7df8_1440w.jpg)

- Idea: to maximize the mutual information between "context vector" and input x.
- ![](https://www.zhihu.com/equation?tex=MI%28x%3Bc%29+%3D+%5Csum_%7Bx%2Cc%7D+p%28x%2Cc%29+%5Clog%5Cfrac%7Bp%28x%7Cc%29%7D%7Bp%28x%29%7D)

- We have input X, latent vector y, encoder F.
- Given (X, y)(random sampled), we make a discriminator D to check whether they are a pair!
- If the discriminator learns well(loss converges easily), it means that the latent vector representation form is relatively **GOOD**.

### MoCo

- Learning: Querying Dictionary(a queue as we have memory limitation). 
- Baseline for MoCo:
  - E2E
  - Memory Bank

![img](https://pic3.zhimg.com/80/v2-59041705533deb8de9036f2f1ee3f13e_1440w.jpg)