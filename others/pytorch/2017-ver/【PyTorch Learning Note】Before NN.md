# 【PyTorch Learning Note】Before NN

> 本节将介绍和神经网络有关的基础`PyTorch`内容。适用于至少对ANN的基本知识有一定了解的朋友。
>
> * 激励函数的了解和使用
> * 神经网络工具箱nn的使用
> * 线性回归
> * Logistic回归

## PyTorch与激励函数

```python
import torch
import torch.nn.functional as F	# 激励函数的位置

# 常用的几种激励函数
yy_relu = F.relu(xx).data.numpy()
yy_sigmoid = F.sigmoid(xx).data.numpy()
yy_tanh = F.tanh(xx).data.numpy()
yy_softplus = F.softplus(xx).data.numpy()
# yy_softmax = F.softmax(xx)
```

![ (Activation)](https://morvanzhou.github.io/static/results/torch/2-3-1.png) 



## 神经网络工具箱

说的就是一个叫做`torch.nn`的模块。

**数据类型：**`Module`。它是一个抽象概念，既可以表示神经网络中的某个层（layer），也可以表示一个包含很多层的神经网络。在实际使用中，最常见的做法是继承`nn.Module`，撰写自己的网络/层。

先不管能不能看懂，把下面的程序读完吧。

```python
import torch
from torch.autograd import Variable
from torch import nn


# 全连接层：y=Wx+b
# 其中W和b是可学习的系数，假设W是(m,n)的矩阵，全连接层把m列的向量转为n列的向量(这里指的是行向量)

class Linear(nn.Module):
    def __init__(self, rows, cols):
        super(Linear, self).__init__()
        # 等价于nn.Module.__init__()
        self.w = nn.Parameter(torch.randn(rows, cols))
        self.b = nn.Parameter(torch.randn(cols))
    def forward(self, x):
        x = x.mm(self.w)
        return x + self.b.expand_as(x)
        # 这一句没看懂不要紧，稍后解释
```

```python
if __name__ == "__main__":
    # main
    layer = Linear(4, 3)
    data_in = Variable(torch.randn(3, 4))
    print(f"输入的数据：\n{data_in}\n")
    for name, parameter in layer.named_parameters():
        print(name, parameter)

    data_out = layer(data_in)  # (3,4)的数据进去后就变成(3,3)
    print(f"\n输出的数据：\n{data_out}")
```

```python
# [out]:

输入的数据：
tensor([[ 2.6090, -0.3695, -0.2910,  0.6708],
        [-0.1158,  1.0268, -0.5773, -0.6809],
        [ 2.1536, -2.4460,  1.0354, -0.1669]])

w Parameter containing:
tensor([[ 0.0242,  0.2026,  0.6974],
        [ 0.2231,  1.5837, -0.6379],
        [-1.8405,  1.4215,  1.8168],
        [-1.2155,  0.5858,  1.0169]], requires_grad=True)
b Parameter containing:
tensor([ 0.1641, -0.7378, -0.0708], requires_grad=True)

输出的数据：
tensor([[-0.1350, -0.8149,  2.1380],
        [ 2.2804, -0.3545, -2.5478],
        [-2.0321, -2.8012,  4.7028]], grad_fn=<ThAddBackward>)
```

```python
# 关于self.b.expand_as(x)的解释
# 在命令行下运行

import torch

a = torch.Tensor([[1], [2], [3]])
# 注意是二维的
a.size()
# (3,1)
a.expand(3, 4)
# tensor([[1., 1., 1., 1.],
#         [2., 2., 2., 2.],
#         [3., 3., 3., 3.]])

# expand方法可以把列以相同元素扩张，但又不改变自身

a
# tensor([[1.],
#         [2.],
#         [3.]])

test = torch.empty(3, 4)
# 建立3x4的矩阵
a.expand_as(test)	# 按照test的形状(即3x4)扩张
# tensor([[1., 1., 1., 1.],
#         [2., 2., 2., 2.],
#         [3., 3., 3., 3.]])
```

> **解释**
>
> * `nn.Module`：在我们自定义类中，Linear继承自nn.Module。我们说了nn.Module可以表示一层或多层神经网络，这里我们设置的Linear就能表示一层神经网络或者一个线性分类器（wx+b=0），所以自然我们要让Linear继承自nn.Module，对应的有继承功能的语段就即为声明类时候的那句。而`super(Linear, self).__init()__`，即把nn.Module的构造函数搬到我们的自定义类Linear中。
>
>   > `super(class_son, self).__init__()`：`super(class_son, self)`即为`class_son`的父类。
>
> * `Parameter`：在构造函数`__init__`中必须自己定义可学习的参数，并封装成Parameter，如在本例中我们把w和b封装成parameter。对应把w和b封装的语句就是`self.w=...`和`self.b=...`。`parameter`是一种特殊的`Variable`，但其默认需要求导（requires_grad = True）。
>
>   > 在本示例中，我们使用了正态分布随机数来初始化权值（注意矩阵和向量的size）。
>
> * `forward`函数实现前向传播过程，其输入可以是一个或多个variable，对x的任何操作也必须是variable支持的操作。
>
> * 无需写反向传播函数，因其前向传播都是对variable进行操作，`nn.Module`能够利用autograd自动实现反向传播，这点比Function简单许多。
>
> * 使用时，直观上可将layer看成数学概念中的函数，调用layer(input)即可得到input对应的结果。它等价于`layers.__call__(input)`，在`__call__`函数中，主要调用的是 `layer.forward(x)`，另外还对钩子做了一些处理。所以在实际使用中应尽量使用`layer(x)`而不是使用`layer.forward(x)`，关于钩子技术将在下文讲解。
>
> * `Module`中的可学习参数可以通过`named_parameters()`或者`parameters()`返回迭代器，前者会给每个parameter都附上名字，使其更具有辨识度。



## 优化器Optimizer

> **神经网络优化器**能够优化神经网络，提高训练的速度（计算是线性回归，也是得训练的吧~）。在`PyTorch`中，可使用`torch.optim`方法优化神经网络。要使用`torch.optim`方法，首先要构造一个`optimizer`对象。这个对象能保存当前的参数，并且基于计算梯度更新参数。

#### 优化器的构建

要构造一个optimizer，你必须给它一个包含参数（必须都是Variable对象）进行优化。然后，您可以指定optimizer的参数选项，比如学习率，权重衰减等。

```python
# 梯度下降
optimizer = optim.SGD(model.parameters(), lr=0.01, momentum=0.9)
# Adam
optimizer = optim.Adam([var1, var2], lr = 0.0001)
```

#### 优化器的差别和选择

**常见的优化器**

> * SGD
> * Momentum
> * AdaGrad
> * RMSProp
> * Adam

**深入研究参考好文**

> [加速神经网络训练 (Speed Up Training)](https://morvanzhou.github.io/tutorials/machine-learning/ML-intro/3-06-speed-up-learning/)
>
> [各种优化方法总结比较（sgd/momentum/Nesterov/adagrad/adadelta）](http://blog.csdn.net/blue_jjw/article/details/50650248)

**部分优化器的介绍**

> **SGD**
>
> 梯度下降法是最基础的方法。把数据一顿吞了进行训练。所以缺点很明显，就是速度慢。但是得到的结果相对较为科学。很多时候用SGD，人们会把数据拆分成多批训练(如下图)，提高速度，但是损失了精度。
>
> ![å éç¥ç"ç½ç"è®­ç" (Speed Up Training)](https://morvanzhou.github.io/static/results/ML-intro/speedup2.png)
>
> **Momentum**
>
> > 又被称作“使用动量的随机梯度下降”
>
> ![加速神经网络训练 (Speed Up Training)](https://morvanzhou.github.io/static/results/ML-intro/speedup4.png)
>
> 大多数其他途径是在更新神经网络参数那一步上动动手脚。 传统的参数 W 的更新是把原始的 W 累加上一个负的学习率(learning rate) 乘以校正值 (dx)。 这种方法可能会让学习过程曲折无比，看起来像喝醉的人回家时，摇摇晃晃走了很多弯路。
>
> ![å éç¥ç"ç½ç"è®­ç" (Speed Up Training)](https://morvanzhou.github.io/static/results/ML-intro/speedup5.png)
>
> 所以我们把这个人从平地上放到了一个斜坡上，只要他往下坡的方向走一点点，由于向下的惯性，他不自觉地就一直往下走，走的弯路也变少了。这就是 Momentum 参数更新。
>
> > 参考[史丹利复合田](https://blog.csdn.net/tsyccnh/article/details/76270707)的解释，更为生动：
> >
> > 如果把梯度下降法想象成一个小球从山坡到山谷的过程，那么前面几篇文章的小球是这样移动的：从A点开始，计算当前A点的坡度，沿着坡度最大的方向走一段路，停下到B。在B点再看一看周围坡度最大的地方，沿着这个坡度方向走一段路，再停下。确切的来说，这并不像一个球，更像是一个正在下山的盲人，每走一步都要停下来，用拐杖来来探探四周的路，再走一步停下来，周而复始，直到走到山谷。而一个真正的小球要比这聪明多了，从A点滚动到B点的时候，小球带有一定的初速度，在当前初速度下继续加速下降，小球会越滚越快，更快的奔向谷底。momentum 动量法就是模拟这一过程来加速神经网络的优化的。
>
> $$
> v_{n+1}=\alpha v_n-learningRate*dx\\
> W_{n+1}=W_n+v_{n+1}\\
> (\alpha 是动量系数，v是速度)
> $$
>
> $\alpha$一般取0.9。这样的做法可以让早期的梯度对当前梯度的影响越来越小，如果没有衰减值，模型往往会震荡难以收敛，甚至发散。而取负号是因为这里我们求最小值。
>
> ![æºå¨å­¦ä¹ ä¸­çä¼åç®æ³](http://imgtec.eetrend.com/sites/imgtec.eetrend.com/files/201807/blog/16889-35142-9.png)
>
> **AdaGrad**
> $$
> v = v+d(x^2)\\
> W = W-learningRate*\frac{dx}{\sqrt v+\delta}\\
> \delta 为小常数，为了数值稳定(你总不能让分母为0吧\sim)，一般设置为10^{-7}\\
> $$
> **RMSProp**
>
> Momentum+AdaGrad
>
> ![å éç¥ç»ç½ç»è®­ç» (Speed Up Training)](https://morvanzhou.github.io/static/results/ML-intro/speedup7.png)
>
> > RMSProp在非凸条件下结果更好。
>
> $$
> v=\alpha v+(1-\alpha)*d(x^2)\\
> W=W-LearningRate*\frac{dx}{\sqrt v+\delta}\\
> \delta建议为10^{-6}
> $$
>
> **Adam**
>
> Adam被很多人认为是深度学习中最棒的算法。（那会有人问为什么要讲前面的算法，那当然是防止和别人讨论时因为知道的优化算法太少而尴尬啊~）
> $$
> v_1=\alpha_1 v_1+(1-\alpha_1)*dx\\
> v_2=\alpha_2 v-2+(1-\alpha_2)*d(x^2)\\
> W=W-LearningRate*\frac{v_1}{\sqrt v+\delta}\\
> \delta 建议为10^{-8}\\
> \alpha_1和\alpha_2分别建议为0.9和0.999
> $$
>

各算法在马鞍面的表现

![img](https://images2018.cnblogs.com/blog/764050/201806/764050-20180626101922973-719563842.gif)

各算法在等高线上的表现![img](https://images2018.cnblogs.com/blog/764050/201806/764050-20180626101958953-188946578.gif)

> AdaDelta暂不说，其效果和Adam差不多。

```python
LR = 0.001

# SGD：pytorch据说是用随机梯度下降干的
opt_SGD = torch.optim.SGD(xxx.parameters, lr=LR)
# Momentum
opt_Momentum = torch.optim.SGD(xxx.parameters, lr=LR, momentum=0.9)
opt_MRSprop = torch.optim.RMSprop(xxx.parameters, lr=LR, alpha=0.9)
opt_Adam = torch.optim.Adam(xxx.parameters, lr=LR, betas=(0.9, 0.999))
# 不输入lr时，默认的学习率为0.01，动量为0.9。当定义momentum为1时，得到的就是最朴素的SGD。
```

> Adam或者说最近出的AdamW是很牛逼。但是为了求稳，还是在写paper、小型数据和结果导向性的任务中使用简单一点的算法吧（比如SGD）。但也要看情况，比如马鞍面这个情况，普通的SGD直接爆炸。

我们知道，很多时候，我们训练并不是只训练一次，而是重复训练，所以每训练一次后，要用`xxxoptimizer.zero_grad()`来清空被优化过的Variable梯度，再用`xxxoptimizer.step()`来进行参数更新。



## Linear Regression

```python
### 一元一次的回归

# IMPORT ZONE
import torch
from torch import nn
import matplotlib.pyplot as plt
import numpy as np

# PARAMETERS
num_epochs = 50
learningRate = 0.001
data_rows = 1
data_cols = 1

# MODEL
model = nn.Linear(data_rows, data_cols)

# LOSS AND OPTIMIZER
lossFunc = nn.MSELoss()  # 平方损失函数
opt = torch.optim.SGD(model.parameters(), lr=learningRate)

# DATA IN NUMPY FORM
x_train = np.array([[3.3], [4.4], [5.5], [6.71], [6.93], [4.168],
                    [9.779], [6.182], [7.59], [2.167], [7.042],
                    [10.791], [5.313], [7.997], [3.1]], dtype=np.float32)
# 使用float32是为了减少计算成本
y_train = np.array([[1.7], [2.76], [2.09], [3.19], [1.694], [1.573],
                    [3.366], [2.596], [2.53], [1.221], [2.827],
                    [3.465], [1.65], [2.904], [1.3]], dtype=np.float32)

# train the model
for epoch in np.arange(num_epochs):
    # translate numpy.array into torch.FloatTensor
    data_in = torch.from_numpy(x_train)
    labels = torch.from_numpy(y_train)

    # forward
    data_out = model(data_in)
    loss = lossFunc(data_out, labels)

    # backward and optimize
    opt.zero_grad()  # reset
    loss.backward()  # error backward
    opt.step()  # using parameters

    # print outcome
    if (epoch + 1) % 5 == 0:
        print(f"Epoch {epoch+1}/{num_epochs}: Loss {loss.item()}")
	# 此过程中，误差会逐渐减小

# plot a figure
predictData = model(torch.from_numpy(x_train)).detach().numpy()
plt.plot(x_train, y_train, 'ro', label='Original Data')
plt.plot(x_train, predictData, label='Predicted Model')
plt.legend()
plt.show()

# keep your model in cktp form
torch.save(model.state_dict, 'tutorial_linear.ckpt')
```

> 我们知道，variable对于每种操作都会记录，但有的时候，我们不希望其再记录操作。比如在转化为上文的`model(torch.from_numpy(x_train))`，我们希望将之转化为numpy再去画图，这一步转化完全没有必要记录。所以我们利用`detach`函数将梯度截断，让其`volatile=True`。但就算这样，返回的variable和被detach的variable指向同一个tensor（引用）。



## Logistic Regression

> 接下来将用logistic回归分类MINST。

```python
import torch
from torch import nn
import torchvision
import torchvision.transforms as transforms

# PARAMETERS
rows = 28 * 28
cols = 10
# 此处解释一下，对于一张图片(28x28像素)，我们一般拉成行向量再来处理
# cols=10，说明每次我们干十张照片
num_epochs = 5
learningRate = 0.001
batch_size = 100

# MODEL AND METHODS
model = nn.Linear(rows, cols)
lossFunc = nn.CrossEntropyLoss()
opt = torch.optim.Adam(model.parameters(), lr=learningRate)

# MINST DATASET DOWNLOADING
train_dataset = torchvision.datasets.MNIST(root='../../data',
                                           train=True,
                                           transform=transforms.ToTensor(),
                                           download=True)

test_dataset = torchvision.datasets.MNIST(root='../../data',
                                          train=False,
                                          transform=transforms.ToTensor())

# DATA LOADER
train_loader = torch.utils.data.DataLoader(dataset=train_dataset,
                                           batch_size=batch_size,
                                           shuffle=True)

test_loader = torch.utils.data.DataLoader(dataset=test_dataset,
                                          batch_size=batch_size,
                                          shuffle=False)

# TRAINING
total_step = len(train_loader)
for epoch in range(num_epochs):
    for i, (images, labels) in enumerate(train_loader):
        # reshape it. -1 means the num of rows will be calculated automatically.
        images = images.reshape(-1, 28 * 28)

        # forward
        data_out = model(images)
        loss = lossFunc(data_out, labels)

        # backward and optimze
        opt.zero_grad()
        loss.backward()
        opt.step()

        if (i + 1) % 100 == 0:
            print(f"[{epoch+1}/{num_epochs}]:now in [{i+1}/{total_step}],and the loss is {loss.item()}")

# TESTING
with torch.no_grad():
    # 关闭计算图功能
    correct = 0
    total = 0
    for images, labels in test_loader:
        images = images.reshape(-1, 28*28)
        data_out = model(images)
        _, predicted = torch.max(data_out.data, 1)
        # max第二个参数为1，表示返回最大值索引
        # _也是一个变量，但是我们用不着用它，我们只想要predicted，所以就取了一个无聊的名字来接受最大值的值
        total += labels.size(0)
        correct += (predicted == labels).sum()
    print(f"Accuracy of the model on the 10000 test images: {100*float(correct)/total} %")

# if you want to keep the model
# torch.save(model.state.dict(), 'tutorial_logistic.ckpt')
```



>**REF**
>
>[1](https://github.com/chenyuntc/pytorch-book/blob/master/chapter4-%E7%A5%9E%E7%BB%8F%E7%BD%91%E7%BB%9C%E5%B7%A5%E5%85%B7%E7%AE%B1nn/chapter4.ipynb)
>
>[2](https://github.com/yunjey/pytorch-tutorial/blob/master/tutorials/01-basics/linear_regression/main.py#L22-L23)
>
>[3](https://morvanzhou.github.io/tutorials/machine-learning/torch/3-06-optimizer/)

