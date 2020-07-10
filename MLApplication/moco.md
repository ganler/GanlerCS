# MoCo

[TOC]

> Ref. [[1](https://zhuanlan.zhihu.com/p/94319787), [2](https://www.bilibili.com/video/av625735603/)]

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

#### Big Idea

- Learning: Querying Dictionary(a queue as we have memory limitation). 
  - <u>Contrastive Learning as Dictionary Look-up</u>
- Baseline for MoCo:
  - E2E
  - Memory Bank

![img](https://pic3.zhimg.com/80/v2-59041705533deb8de9036f2f1ee3f13e_1440w.jpg)

- The FIFO Q is our dictionary. Usually, the Q size >> mini-batch size. 

![image.png](https://i.loli.net/2020/07/08/IQVqDK4dO2h7igk.png)

> 作者提出建立dictionary依赖两个必要条件：*1. **large**，dictionary的大小需要足够大，才能对高维、连续空间进行很好的表达*；2. **consistent**，*dictionary的key需要使用相同或者相似的encoder进行编码，这样query和key之间的距离度量才能够一致并且有意义*。

- E2E: Good consistency but cannot scale. (consistent but not large)
- Memory bank (large but not consistent)

#### Contrastive Loss Function

对一张图片而言，我们希望Contrastive Loss具有如下属性：

1. 该图片经过key encoder和query encoder编码以后得到的两个向量 ![[公式]](https://www.zhihu.com/equation?tex=k_%2B%2C+q) 尽可能接近
2. 该图片经过query encoder得到的向量 ![[公式]](https://www.zhihu.com/equation?tex=q) ，和dictionary中所有其他的图片的经过key encoder编码后得到的向量![[公式]](https://www.zhihu.com/equation?tex=%5C%7Bk_i%2C%5C++i%5Cin%7B0%2C1%2C...%2CK%7D%5C%7D) 尽可能正交(不接近)

![](https://www.zhihu.com/equation?tex=%5Cmathcal%7BL%7Dq+%3D+-+%5Clog+%5Cfrac%7B%5Cexp%28q%5Ccdot+k%2B%2F%5Ctau%29%7D%7B%5Csum_%7Bi%7D%5EK%5Cexp%28q%5Ccdot+k_i%2F%5Ctau%29%7D+)