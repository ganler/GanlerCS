# 【PyTorch Learning Note】FNN and CNN

上一篇文章我们已经实现了`全连接层`，`线性回归`和`logistic`回归。本文将实现<u>前向神经网络</u>和<u>卷积神经网络</u>（CNN）。其中对BN层进行了比较详细的介绍。



## Feedforward Neural Network

> In BP way.

```python
import torch
from torch import nn
import torchvision
import torchvision.transforms as transforms

# DEVICE CONFIRM
device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')

# PARAMETERS
input_size = 28 * 28
hidden_size = 500
output_size = 10
num_epochs = 5
batch_size = 100
learningRate = 0.001

# MINST DATASET
train_dataset = torchvision.datasets.MNIST(root='../../data',
                                           train=True,
                                           transform=transforms.ToTensor(),
                                           download=True)

test_dataset = torchvision.datasets.MNIST(root='../../data',
                                          train=False,
                                          transform=transforms.ToTensor())

# Data loader
train_loader = torch.utils.data.DataLoader(dataset=train_dataset,
                                           batch_size=batch_size,
                                           shuffle=True)

test_loader = torch.utils.data.DataLoader(dataset=test_dataset,
                                          batch_size=batch_size,
                                          shuffle=False)


# FULLY CONNECTED LAYER
class NN(nn.Module):
    def __init__(self, input_size, hidden_size, output_size):
        # 构造函数
        super(NN, self).__init__()
        self.input2hidden = nn.Linear(input_size, hidden_size)
        self.actFunc = nn.ReLU()
        self.hidden2output = nn.Linear(hidden_size, output_size)

    def forward(self, x):
        out = self.hidden2output(self.actFunc(self.input2hidden(x)))
        return out


# MODEL AND OTHER FUNCS
model = NN(input_size, hidden_size, output_size).to(device)
lossFunc = nn.CrossEntropyLoss()
opt = torch.optim.Adam(model.parameters(), lr=learningRate)

# TRAINING
total_step = len(train_loader)
for epoch in range(num_epochs):
    for i, (images, labels) in enumerate(train_loader):
        # Move tensor to the configured device
        images = images.reshape(-1, 28 * 28).to(device)
        labels = labels.to(device)

        # Forward
        data_out = model(images)
        loss = lossFunc(data_out, labels)

        # Backward and optimize
        opt.zero_grad()
        loss.backward()
        opt.step()
        if (i + 1) % 100 == 0:
            print(f"[{epoch+1}/{num_epochs}]|[{i+1}/{total_step}] , the loss: {loss.item()}")

# TESTING
with torch.no_grad():
    correct = 0
    total = 0
    for images, labels in test_loader:
        images = images.reshape(-1, 28 * 28).to(device)
        labels = labels.to(device)
        data_out = model(images)
        _, predicted = torch.max(data_out, 1)
        total += labels.size(0)
        correct += (predicted == labels).sum().item()

    print(f"after our training , our model has a accuracy of {100*float(correct)/total}%")
```



## CNN

> Some excellent article about CNN in Chinese:
>
> [ARTICLE1](https://www.zybuluo.com/hanbingtao/note/485480)
>
> [ARTICLE2](http://www.moonshile.com/post/juan-ji-shen-jing-wang-luo-quan-mian-jie-xi#toc_1)

### 结构

[`卷积层`->`池化层`]×n次 -> `全连接层` -> `批规范层` -> `dropout层`

#### torch.nn.Conv2d(in_channels, out_channels, kernel_size, stride=1, padding=0, dilation=1, groups=1, bias=True)

$$
out(N_i, C{out_j})=bias(C{out_j})+\sum^{C_{in}-1}_{k=0}weight(C{out_j},k)\bigotimes input(N_i,k)
$$

`in_channels`指的是输入图片（数据）的depth。`out_channels`即使用的卷积核的个数，也代表着输出数据的depth。而`kernel_size`是卷积核的size。`stride`是步长，默认为1，当`stride`为(a, b)时，分别代表横纵方向的步长为a和b。`padding`表示在外围补充0的层数。`dilation`是卷积核元素之间的间距，详情点[Hier](https://github.com/vdumoulin/conv_arithmetic/blob/master/README.md)。`bias`表示是否添加偏置。

`conv1 = n.Conv2d(1, 1, (3, 3), bias=False)`就定义了一个输入和输出都是1depth，不添加偏置和padding的利用3x3卷积核的卷积层。

#### squeeze&unsqueeze

在`PyTorch`中自定义layer对输入形状都有假设：输入的不是单个数据，而是一个batch。若想输入一个数据，则必须调用`unsqueeze(0)`函数将数据伪装成batch_size=1的batch。

[hier](https://blog.csdn.net/xiexu911/article/details/80820028)====

### 卷积层操作

常用的一个计算图像经过卷积层后的尺寸公式：
$$
\begin{aligned}
&pic\ size\ before\ convolutional\ layer:\ &Before\\
&pic\ size\ after\ convolutional\ layer:\ &After\\
&num of padding:\ &Padding\\
&the\ size\ of\ the\ convolutional\ kernel:\ &Kernel\\
&stride\ of\ the\ convolutional\ kernel:\ &Stride\\
\\ &
\\
&After=\frac{Before+2\times Padding-Kernel}{Stride}+1
\\
&
\\
&if\ After\ =\ Before:\\
\\
&After=\frac{Stride+2\times Padding-Kernel}{Stride-1}
\end{aligned}
$$


```python
import torch
from torch import nn
import matplotlib.pyplot as plt
from PIL import Image
from torchvision.transforms import ToTensor, ToPILImage

lena = Image.open('img/lena.png')
# 我自己的photo是灰度图(对应的Image.mode为'L')

input = ToTensor()(lena).unsqueeze(0)
# 补充维度

# 使用锐化卷积核
kernel = torch.ones(3, 3)/-9
kernel[1][1] = 1
conv = nn.Conv2d(1, 1, (3, 3), bias=False)
conv.weight.data = kernel.view(1, 1, 3, 3)

out = conv(input)
lena_new = ToPILImage()(out.squeeze(0))

plt.figure(figsize=(10, 5))
plt.suptitle('Comparison')

plt.subplot(1,2,1)
plt.title('before')
plt.imshow(lena.convert('RGB'))
# 转化为RGB的形式才能被plt打印展示

plt.subplot(1,2,2)
plt.title('after')
plt.imshow(lena_new.convert('RGB'))

plt.show()
```

![img](https://s1.ax1x.com/2018/08/19/PfXRxJ.png)

### 池化层

##### class torch.nn.MaxPool1d(kernel_size, stride=None, padding=0, dilation=1, return_indices=False, ceil_mode=False)

`stride`是filter移动的步长，其默认值为`kernel_size`

```python
pool = nn.AvgPool2d(2, 2)
# 其中池化层函数是没有参数的
# 不信可以`print(list(pool.parameters()))`，得到的是`[]`
# 还有nn.MaxPool2d()等池化层，2,2代表的是filter的size
out = pool(out)
```

### 全连接层

```python
linear = nn.Linear(3, 4)
```

### 批规范层（Batch Normalization）

#### 标准化≠归一化

> [A great blog about normlization](https://blog.csdn.net/pipisorry/article/details/52247379)

> <u>normalization</u>最明显的好处就是让特征之间相互平等，不因为某个特征在数值上特别大或特别小而影响最后的结果。
>
> 一般我们说**标准化**，都离不开**归一化**，说到归一化，都离不开`min-max normalization`。
> $$
> x^*=\frac{x-min(x)}{max(x)-min(x)}
> $$
> 这里再介绍一个常用**标准化**方法（不是归一化）的`zero-mean normalization`:
> $$
> x^*=\frac{x-\mu}{\sigma}
> $$
> $\mu$是x的均值，$\sigma$是x的标准差，这样得到结果都聚集在0附件，标准差为1，符合正态分布。$x^*$是$x$的**标准化随机变量**。
> $$
> \mathbb E(x^*)=\mathbb E[\frac{x-\mathbb E(x)}{\sqrt\sigma}]=\frac{1}{\sqrt \sigma}(\mu-\mu)=0\\
> \mathbb D(x^*)=\mathbb D[\frac{x-\mathbb E(x)}{\sqrt\sigma}]=\mathbb D[\frac{x}{\sqrt\sigma}]=\frac{1}{\sqrt\sigma}\sqrt \sigma=1
> $$
>

#### BN(Batch Normalization)

> - 能够减少Interal Covariate Shift的问题，从而减少train的时间，使得对于深度神经网络的训练更加可行。
> - 消除梯度弥散的问题（导数值为1，这样在传播的时候，不会因为网络太深导致误差传播到前方的时候都趋近于0了）
> - 对于参数的初始化影响更小

> **brilliant article about the BN algorithm**
>
> [help you to understand quickly](https://blog.csdn.net/myarrow/article/details/51848285)
>
> [with more details](https://blog.csdn.net/hjimce/article/details/50866313)

![img](https://s1.ax1x.com/2018/08/19/PfxN8O.png)

> 	其中$\gamma$和$\beta$如果是可学习参数（一般我们都这么做），则网络可以根据反向传播来更新学习这些学习参数，若我们令之不是可学习参数，那么![\gamma^{(k)}=\sqrt{Var[x^{(k)}]}, \beta^{(k)}=E[x^{(k)}]](https://www.zhihu.com/equation?tex=%5Cgamma%5E%7B%28k%29%7D%3D%5Csqrt%7BVar%5Bx%5E%7B%28k%29%7D%5D%7D%2C+%5Cbeta%5E%7B%28k%29%7D%3DE%5Bx%5E%7B%28k%29%7D%5D)，即这个BN其实什么都没干。
>
>
>
> **对算法的解释：**
>
> 前三步就是`zero-mean normalization`。
>
> 第四步就是`zero-mean normalization`的结果反推输入的一个公式。至于为什么要反推，原因很简单，在使用反向传播算法的时候，我们需要反向传播误差，我们在BN层进行了标准化（规范化）就意味着我们改变了其他神经层的输出，所以在反向传播的时候，也需要用正向传播得到输出的方法的逆运算（即第四步），进行传播。

> 举下面引用书籍中的例子：
>
> $v=[0.0066,0.0004,0.0085,0.0093,0.0068,0.0076,0.0074,0.0039,0.0066,0.0017]^T $
>
> 在$\ell_2$规范化后（$v=v/||v||_2$），这组数变为: 
>
> $v′ =[0.3190,0.0174,0.4131,0.4544,0.3302,0.3687,0.3616,0.1908,0.3189,0.0833]^T$
>
> 显然，经过规范化作用后，原本微小的数值其尺度被“拉大”了，试想如果未做规范化的那组随机数 v 就是反向传播的梯度信息，规范化自然可起到缓解“梯度弥散”效应的作用。

> 	引用和参考于《解析卷积神经网络——深度学习实践手册》，作者：魏秀参。

> **关于训练**：
>
> 理想情况下当然是整批放到模型当中训练，使用最后训练结果对应的参数比较好。但其实这样并不实际，每次有新的数据都要从头到尾训练一遍。所以很多时候，我们都是选取小批量训练，把每个batch得到的$\mu,\gamma$保留下来，然后取平均值作为最后的结果。
>		
> 不过上面的方法好像也不是特别科学，其实最好是根据损失函数的大小，对损失较小的参数值赋予更大权重，以得到更好的参数结果。

回到`PyTorch`：

##### class torch.nn.BatchNorm2d(num_features, eps=1e-05, momentum=0.1, affine=True) 

$$
y=\frac{x-\mu}{\sigma+\epsilon}\times\gamma+\beta
$$

`affine`默认为True，意味着gamma和beta都是可学习参数。如果是False就说明他们都不是可学习参数。

`num_features`为输入的特征数量（Tensor的列数）。

`eps`即$\epsilon$。

`momentum`为动态均值和动态方差所使用的动量。默认为0.1。

```python
bn = nn.BatchNorm1d(100)
input = torch.randn(20, 100)
output = bn(input)
```

### Dropout

[understanding dropout](https://blog.csdn.net/stdcoutzyx/article/details/49022443)

##### class torch.nn.Dropout(p=0.5, inplace=False)

随机将输入张量中部分元素设置为0。对于每次前向调用，被置0的元素都是随机的。

```python
import torch
from torch import nn

dropout = nn.Dropout()
input = torch.randn(1,10)
out = dropout(input)
print(input)
print(out)
```

```python
# [out]
tensor([[ 1.0329, -0.6743,  0.2868, -1.2956,  0.6864,  0.6954,  0.2924, -0.8371,
          0.7308, -1.9913]])
tensor([[ 0.0000, -1.3486,  0.0000, -2.5912,  1.3728,  1.3907,  0.0000, -1.6743,
          0.0000, -0.0000]])
```

### 激活函数

$$
ReLu(x)=\max(0,x)
$$

> **ReLu**的一些优点：
>
> * 容易计算。
> * 减轻梯度弥散的问题：我们知道sigmoid的导数为$\sigma(1-\sigma)$，其$\le \frac{1}{4}$。如果网络层很深，则会导致误差传播到最后趋近于0。

```python
import torch
from torch import nn

relu = nn.ReLU(inplace=True)
# 如果inplace=True，那么得到的输出会直接覆盖到输入当中，以节约内存和显存
input = torch.randn(2, 3)
relu(input)
print(input)
```

### PyTorch Sequential容器

`torch.nn.Sequential`可将神经层整合到一起，示例：

```python
model = nn.Sequential(
          nn.Conv2d(1, 20, kernel_size=5, padding=2),
          nn.ReLU(),
          nn.Conv2d(20, 64, 5),
          nn.ReLU()
        )
```



### 实践

**MNIST**

```python
import torch
from torch import nn
import torchvision
from torchvision import transforms

# DEVICE DETECT
device = torch.device('cuda:0' if torch.cuda.is_available() else 'cpu')

# OPTIONS
num_epoachs = 5
num_classes = 10  # 分类数目
batch_size = 100  # 批处理数量
learning_rate = 0.001

# MNIST DATASET
train_dataset = torchvision.datasets.MNIST(root='../../data/',
                                           train=True,
                                           transform=transforms.ToTensor(),
                                           download=True)
test_dataset = torchvision.datasets.MNIST(root='../../data/',
                                          train=False,
                                          transform=transforms.ToTensor(),
                                          download=True)
# train_dataset.train_data.size(): torch.Size([60000, 28, 28])
# train_dataset.train_labels.size(): torch.Size([60000])


# DATA LOADER
train_loader = torch.utils.data.DataLoader(dataset=train_dataset,
                                           batch_size=batch_size,
                                           shuffle=True)
# shuffle=True意味着把数据打乱
test_loader = torch.utils.data.DataLoader(dataset=test_dataset,
                                          batch_size=batch_size,
                                          shuffle=True)


# CNN (using 2 conventional layers and one full connected layer)
class CNN(nn.Module):
    def __init__(self, num_classes):
        super(CNN, self).__init__()
        self.layer1 = nn.Sequential(
            nn.Conv2d(3, 16, kernel_size=5, padding=2),
            nn.BatchNorm2d(16),
            nn.ReLU(),
            nn.MaxPool2d(kernel_size=2)
        )
        # [28*28*1] -> [28*28*16]for 3 times ->
        self.layer2 = nn.Sequential(
            nn.Conv2d(16, 32, kernel_size=5, padding=2, stride=1),
            nn.BatchNorm2d(32),
            nn.ReLU(),
            nn.MaxPool2d(kernel_size=2)
        )
        self.fc = nn.Linear(7 * 7 * 32, num_classes)

    def forward(self, x):
        out = self.layer1(x)
        out = self.layer2(out)
        out = out.reshape(out.size(0), -1)  # 到了全连接层需要
        out = self.fc(out)
        return out

model = CNN(num_classes).to(device)

# LOSS FUNC AND OPTIMIZER
loss_func = nn.CrossEntropyLoss()
opt = torch.optim.Adam(model.parameters(), lr=learning_rate)

# TRAIN
total_steps = len(train_loader)
for epoach in range(num_epoachs):
    loss_acccumulation = 0
    for i, (imgs, labels) in enumerate(train_loader):
        imgs = imgs.to(device)
        labels = labels.to(device)

        output = model(imgs)
        loss = loss_func(output, labels)

        opt.zero_grad()
        loss.backward()
        opt.step()
        
        loss_accumulation += loss.item()
        if (i + 1) % 100 == 0:
            print(f"{epoach+1}/{num_epoachs}: [{i+1}/{total_steps}] -> loss:{loss_accumulation/100}")
            loss_accumulation = 0

# TEST
model.eval()
with torch.no_grad():
    total = 0
    correct = 0
    for imgs, labels in test_loader:
        imgs = imgs.to(device)
        labels = labels.to(device)
        data_out = model(imgs)
        _, predicted = torch.max(data_out, 1)
        total += imgs.size(0)
        correct = (predicted == labels).sum().item()
print(f"acurracy is {correct*100 / total}%")

# 结果可达99%+，可见LeNet在数字识别上的稳健型。
```



**CIFAR10**

> <u>classes:</u>
>
> `airplane`
>
> `automobile`
>
> `bird`
>
> `cat`
>
> `deer`
>
> `dog`
>
> `frog`
>
> `horse`
>
> `ship`
>
> `truck`

All in `3×32×32`.

> * `transforms.Compose(transforms)`用于将多个transform组合：例如`transforms.Compose([transforms.ToTensor(), transforms.CenterCrop(10)])`
>
> * `transforms.Normalize(mean, std)`对于一个(R, G, B)按照mean，std进行<u>zero-mean normalization</u>。

```python
import torch
from torch import nn
import torchvision
from torchvision import transforms

# DEVICE
device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')

# PARAMETERS
num_epoachs = 5
batch_size = 100
times4print = 10000/batch_size
num_classes = 10
learning_rate = 0.001
classes = ['plane', 'car', 'bird', 'cat', 'deer', 'dog', 'frog', 'horse', 'ship', 'truck']

# CIFAR10
train_dataset = torchvision.datasets.CIFAR10(root='../../data/',
                                             train=True,
                                             transform=transforms.ToTensor(),
                                             download=True)
test_dataset = torchvision.datasets.CIFAR10(root='../../data/',
                                            train=False,
                                            transform=transforms.ToTensor(),
                                            download=False)

# LOADER
train_loader = torch.utils.data.DataLoader(dataset=train_dataset,
                                           batch_size=batch_size,
                                           num_workers=2,
                                           shuffle=True)
test_loader = torch.utils.data.DataLoader(dataset=test_dataset,
                                          batch_size=batch_size,
                                          num_workers=2,
                                          shuffle=True)
# (3 * 32 * 32)

# NETWORK
class CNN(nn.Module):
    def __init__(self, num_classes):
        super(CNN, self).__init__()
        self.layer1 = nn.Sequential(
            nn.Conv2d(3, 16, kernel_size=3, padding=1),
            nn.BatchNorm2d(16),
            nn.ReLU(),
            nn.MaxPool2d(2, 2)
        )
        self.layer2 = nn.Sequential(
            nn.Conv2d(16, 32, kernel_size=3, padding=1),
            nn.BatchNorm2d(32),
            nn.ReLU(),
            nn.MaxPool2d(2, 2)
        )
        self.fc1 = nn.Linear(32 * 8 * 8, 64)
        self.fc2 = nn.Linear(64, num_classes)

    def forward(self, x):
        out = self.layer1(x)
        out = self.layer2(out)
        out = out.view(-1, 32 * 8 * 8)
        out = self.fc1(out)
        out = self.fc2(out)
        return out


# MODEL, LOSS FUNC AND OPTIMISER
model = CNN(num_classes)
loss_func = nn.CrossEntropyLoss()
opt = torch.optim.Adam(model.parameters(), lr=learning_rate)

# TRAIN
total_steps = len(train_loader)
for epoach in range(num_epoachs):
    loss_accumulation = 0
    for i, (imgs, labels) in enumerate(train_loader):
        imgs = imgs.to(device)
        labels = labels.to(device)

        out = model(imgs)
        loss = loss_func(out, labels)

        opt.zero_grad()
        loss.backward()
        opt.step()

        loss_accumulation += loss.item()
        if (i + 1) % times4print == 0:
            print(f"[{epoach+1}/{num_epoachs}]: < {i+1}/{total_steps} > -> loss: {loss_accumulation/times4print}")
            loss_accumulation = 0

# TEST
model.eval()
with torch.no_grad():
    class_correct = list(0. for i in range(num_classes))
    class_total = class_correct.copy()
    for imgs, labels in test_loader:
        out = model(imgs)
        _, predicted = torch.max(out, 1)
        c = (predicted == labels).squeeze()
        for i in range(num_classes):
            class_correct[labels[i]] += c[i].item()
            class_total[labels[i]] += 1
    print(f"FINAL ACCURACY: {100 * sum(class_correct)/sum(class_total)}%")
    for i in range(num_classes):
        print(f"[{classes[i]}] : {100 * class_correct[i]/class_total[i]}%")
```

> 得到的总共的准确率约为60%，可以说结果很不理想了。可见，对于复杂的图片，我们可能需要更加优秀的卷积神经网络结构。