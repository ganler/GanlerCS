# 【PyTorch Learning Note】Autograd



## Autograd简介

`autograd`是`PyTorch`中最核心的一个包。其主要是面对BP算法而设计的。

> ​	大多数反向传播过程需要手动实现。这对于像线性回归等较为简单的模型来说，还可以应付，但实际使用中经常出现非常复杂的网络结构，此时如果手动实现反向传播，不仅费时费力，而且容易出错，难以检查。`torch.autograd`就是为方便用户使用，而专门开发的一套自动求导引擎，它能够根据输入和前向传播过程自动构建**计算图**，并执行反向传播。

#### Variable

`autograd`中的核心数据结构是`Variable`。`Variable`封装了`tensor`，并记录对`tensor`的操作记录用来构建计算图。`Variable`的数据结构如图所示，主要包含三个很重要和有用的**属性**：

> - `data`：保存`Variable`所包含的`Tensor`。
>
> - `grad`：保存`data`对应的梯度，`grad`也是variable，而不是tensor，它与`data`形状一致。
>
> - `grad_fn`： 指向一个`Function`，记录`Tensor`的操作历史，即它是什么操作的输出，用来构建计算图。如果某一个变量是由用户创建，则它为叶子节点（没有子节点的节点），对应的`grad_fn`等于`None`。
>
>   > 简而言之，可以认为是对上一个操作的记录（如`x.grad_fn`就是对应的是(最新的那个)得到x的那个式子）。

`Variable`的构造函数需要传入`Tensor`，同时有两个可选参数：

> - `requires_grad (bool)`：是否要对该`Variable`求梯度。
> - `volatile (bool)`：意为“挥发”，设置为`True`，则构建在该variable之上的图都不会求导，专为<u>推理阶段</u>设计。

​	Variable提供了大部分tensor支持的函数，但其不支持部分`inplace`函数，因这些函数会修改`Tensor`自身，而在BPNN中，`Variable`需要缓存原来的`Tensor`来计算反向传播梯度。如果想要计算各个`Variable`的梯度，只需调用根节点`Variable`的`backward`方法，`autograd`会自动沿着计算图反向传播，计算每一个叶子节点的梯度。

> `variable.backward(grad_variable=None, retain_graph=None, create_graph-None)`
>
> - **grad_variables**：形状与`variable`一致，对于`y.backward()`，`grad_variables`相当于链式法则${dz \over dx}={dz \over dy} \times {dy \over dx}$中的$\textbf {dz} \over \textbf {dy}$。`grad_variables`也可以是`Tensor`或序列。
> - **retain_graph**：反向传播需要缓存一些中间结果，反向传播之后，这些缓存就被清空，可通过指定这个参数不清空缓存，用来多次反向传播。（下面会再介绍一遍）
> - **create_graph**：对反向传播过程再次构建计算图，可通过`backward of backward`实现求高阶导数。
>

#### 自动求导示例

通过一个简单的例子来看看`PyTorch`中的自动求导机制。

```python
import torch
from torch.autograd import Variable

### EXAMPLE 1
# x,w等都是标量
x = Variable(torch.FloatTensor([1]), requires_grad=True)
w = Variable(torch.FloatTensor([2]), requires_grad=True)
b = Variable(torch.FloatTensor([3]), requires_grad=True)


# Build a computational graph.
y = w * x + b    # y = 2 * x + 3

# Compute gradients.
y.backward()

# Print out the gradients.
print(x.grad)    # x.grad = 2
print(w.grad)    # w.grad = 1
print(b.grad)    # b.grad = 1
```

> 为什么我一开始没有用非标量，因为非标量反向求导（backward）的时候要设定输出tensor的形状。

```python
import torch
from torch.autograd import Variable

### EXAMPLE 2
a = Variable(torch.ones(3, 4), requires_grad=True)
b = Variable(torch.zeros(3, 4))
c = a.add(b)
d = c.sum()
# 👆所有项加起来是12，因为是variable相加，所以d也是variable。
# 这也说明了c.sum()是variable
# 而c.data.sum()是tensor

d.backward()	# 反向传播

print(a.grad)	# d（即sum(a+b)）对a求导，返回ones(3, 4)
print(a.requires_grad, b.requires_grad, c.requires_grad)
# (True, False, True)
# 因为我们使用了a.grad，所以a.requires_grad从False变位True。那么为什么c.requires_grad也是True呢，聪明的人知道，这是由于使用了链式法则，对a求导的时候，先会让d对c(即a+b)求导。

print(a.is_leaf, b.is_leaf, c.is_leaf)	# 判断是否为叶子节点
# (True, True, False)

# c.grad是None, 因c不是叶子节点，它的梯度是用来计算a的梯度
# 所以虽然c.requires_grad = True,但其梯度计算完之后即被释放
print(c.grad is None)	# True

# grad_fn可以查看这个variable的反向传播函数，
# c是add函数的输出，所以它的反向传播函数是AddBackward
print(c.grad_fn)
# <ThAddBackward object at 0x109217438>
```

在`grad_fn`中，`next_functions`（一个元组`tuple`）保存`grad_fn`的输入。（叶子节点都没有`grad_fn`）

```python
import torch
from torch.autograd import Variable

x = Variable(torch.ones(1))
b = Variable(torch.rand(1), requires_grad=True)
w = Variable(torch.rand(1), requires_grad=True)
y = w*x
z = y+b

print(z.grad_fn)	# <ThAddBackward object at 0x109217438>
print(z.grad_fn.next_functions)
# ((<ThMulBackward object at 0x1080ea358>, 0), (<AccumulateGrad object at 0x1080ea4a8>, 0))
# 如何解释这个next_functions呢？其实就是对应z=y+b相关的两个变量y和b。y是通过“乘”得到的，所以是`ThMulBackward`，而b是一个叶子节点，是直接初始化的，所以是`AccumulateGrad`（该标识也有可能是梯度累加的意思）。这里的`next`是按着传播的反方向
```



## 计算图

什么是计算图？

![img](https://pic3.zhimg.com/v2-5d56dc3930e0c68b2f1655b18265f172_b.gif)

​	在研究神经网络的时候，我们常通过计算图来使整个神经网络变得条理清晰且直观。一个计算图由**节点**（Node，通常来说里面放的是变量）和**连接线**（Edge，一般是运算操作）组成。计算图在BPNN中极为重要。我们从高数中的求多元函数偏导数开始讲解计算图是什么东东：
$$
\begin{aligned}
&e=(a+b)\times(b+1)\\
&令c=a+b\\
&d=b+1\\
&求\frac{\partial e}{\partial a}和\frac{\partial e}{\partial b}:\\
\frac{\partial e}{\partial a}&=\frac{\partial e}{\partial c}\frac{\partial c}{\partial a}+\frac{\partial e}{\partial d}\frac{\partial d}{\partial a}=b+1\\
&\frac{\partial e}{\partial b}同理
\end{aligned}
$$
<u>其对应的计算图如下：</u>

![p1](https://ws4.sinaimg.cn/large/006tKfTcly1ftwjwuupqbj319i0us4qp.jpg)

`Autograd`的牛逼之处，也在于其能自动生成`Computational Graph`。



### Buffer

在BPNN中计算梯度的时候，这些数值会在向前传播的过程中被存在buffer（缓冲区中）中，每一次反向传播完成后都会将之清空。在多次反向传播的过程中，如果要保存这些值，我们可以指定`retain_graph`来保留这些buffer。

```python
import torch
from torch.autograd import Variable

x = Variable(torch.rand(1), requires_grad=True)
# 记得设置requires_grad否则无法求导，而且
y = 2*x
z = x+y

z.backward(retain_graph=True)
x.grad
```



## PyTorch 0.4

其实在0.4版本当中，`Tensor`和`Variable`已经融合了。

> `torch.Tensor`和`torch.autograd.Variable`现在其实是同一个类! 没有本质的区别! 所以也就是说, **现在已经没有纯粹的Tensor了, 是个Tensor, 它就支持自动求导！** 现在再也不要给`Tensor`包一下`Variable`，因为它已经是天生的`Variable`了。

> 关于`PyTorch`的代码迁移，请等有一定基础后参照[这里](https://www.itency.com/topic/show.do?id=494122)。因为很多教程是基于0.3的，之所以要穿插0.3的方法，是希望读者同样能读懂0.3的代码。



> **REF.**
>
> [chenyuntc](https://github.com/chenyuntc/pytorch-book/blob/master/chapter3-Tensor%E5%92%8Cautograd/Autograd.ipynb)
>
> [yunjey](https://github.com/yunjey/pytorch-tutorial/blob/master/tutorials/01-basics/pytorch_basics/main.py)
>
> [csdn](https://blog.csdn.net/u013527419/article/details/70184690)
>
> [optimizer](https://ptorch.com/news/54.html)