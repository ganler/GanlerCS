# Residual block and its implementation

> The implementation is based on **PyTorch**.

## Before beginning

### Prerequisites

- Understanding how CNN works.
- Be able to program with PyTorch with ease.

> ResNet = Residual Network(残差网络).

## How it works

> **Deep**
>
> > 网络越深，其表达能力越强。

<--!more-->

> **Q1**: *Is learning better networks as easy as stacking more layers?*
>
> > No. 
> >
> > - Vanishing/exploding gradients.(梯度弥散/梯度爆炸)
> >
> >   > 反向传播(Backpropogation)的时候，由链式法则，可知道，对于深度网络（Net代表整个网络，Net(x)就是整个网络的预测值）：
> >   > $$
> >   > dw_n={\rm learning\_rate}\times Error\times \frac{\partial Layers}{\partial w_n}\\
> >   > Error = y_{label}-y_{outcome}=y_{label}-Net(x)\\
> >   > $$
> >   > 对于Net，其中会有很多激活函数，如果激活函数是Sigmoid，那么其中会有很多$Sigmoid'(x)\in[-1/4,1/4]$，那么对于深层网络而言，反向传播的更新值会很小很小。那么反向传播算法（对于靠前的网络层的权值）就没有太多用了。
> >
> > - shattered gradient problem
> >
> >   >  [If resnets are the answer, then what is the question?](http://link.zhihu.com/?target=https%3A//arxiv.org/abs/1702.08591) 
>
> **Solution: Skip connection**
>
> > ![img](https://s1.ax1x.com/2018/11/22/FPOMuQ.png)
> >
> > > Recommended article:[article in ZhiHu](https://zhuanlan.zhihu.com/p/28124810)
> >
> > **Forward:**
> > $$
> > {\rm ResNet\_BasicBlock}(x) = Layers\_in\_Block(x)+x\\
> > same\ as\ H(x)= F(x)+x\ in\ paper.
> > $$
> >
> > > 也可以是${\rm ResNet\_BasicBlock}(x) = Layers\_in\_Block(x)+W_sx$，但在论文中，作者进行了很多尝试，发现效果还是直接x好。
> >
> > **Backward:**
> > $$
> > dw_n=1+learning\_rate\times Error\times\frac{\partial Layers\_in\_Block}{\partial w}
> > $$
> >

**注意**： Residual Block的input和output是相同的rows和cols，但channel可以任意改变。

#### In addition:

> deep networks with skip-connections have the following problems:
> • Feature reuse problem: upper layers might not learn useful representations given previous activations;
> • Widening is more effective than deepening: there is no benefit from increasing depth;
> • Actual depth is not clear: it might be determined by the shortest path.
>
> 最新出炉的CVPR2018中有一篇论文（reference 2）介绍了由ResNet启发得到的新的Dirac Net。旨在抗衡上述问题，用尽量浅层的网络训练出效果更好的模型。

## Implementation of Residual Block

> A simple residual block demo. A ResNet consists of a lot of residual blocks.

```python
from torch import nn

class ResidualBlock(nn.Module):
    def __init__(self, in_c, out_c, stride=1, downsample=True):
        super(ResidualBlock, self).__init__()
        self.ConvLayer1 = nn.Sequential(
	        nn.Conv2d(in_c, out_c, kernel_size=3, stride=stride),
			nn.BatchNorm2d(out_c),
            nn.ReLU()
        )
        self.ConvLayer2 = nn.Sequential(
        	nn.Conv2d(out_c, out_c, kernel_size=3, stride=stride),
			nn.BatchNorm2d(out_c),
        )
        self.LastReLU = nn.ReLU()

    def forward(self, x):
        out = ConvLayer2(ConvLayer1(x))
        if self.downsample:
            out += x
        return self.LastReLU(out)
```

## Reference

1. Dirac Net: https://arxiv.org/pdf/1706.00388.pdf
2. ResNet: https://arxiv.org/abs/1512.03385

