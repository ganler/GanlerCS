## Models

### Set your network

#### 简单的模型

> 参考自[here](https://github.com/yunjey/pytorch-tutorial/blob/master/tutorials/02-intermediate/convolutional_neural_network/main.py#L35-L56)。关于MINST的手写数字识别的网络。

```python
class ConvNet(nn.Module):
    def __init__(self, num_classes=10):
        super(ConvNet, self).__init__()
        self.layer1 = nn.Sequential(
            nn.Conv2d(1, 16, kernel_size=5, stride=1, padding=2),
            nn.BatchNorm2d(16),
            nn.ReLU(),
            nn.MaxPool2d(kernel_size=2, stride=2))
        self.layer2 = nn.Sequential(
            nn.Conv2d(16, 32, kernel_size=5, stride=1, padding=2),
            nn.BatchNorm2d(32),
            nn.ReLU(),
            nn.MaxPool2d(kernel_size=2, stride=2))
        self.fc = nn.Linear(7 * 7 * 32, num_classes)

    def forward(self, x):
        out = self.layer1(x)
        out = self.layer2(out)
        out = out.reshape(out.size(0), -1)
        out = self.fc(out)
        return out
```

#### 封装模型

> 参考自[here](https://github.com/yuanlairuci110/pytorch-best-practice-master/blob/master/models/BasicModule.py)。

```python
# 一般的模型都是直接继承nn.Module。我们可以对nn.Module进行一系列的改进，方便使用。
import torch
import time

class BasicModule(torch.nn.Module):
    '''
    封装了nn.Module,主要是提供了save和load两个方法
    '''
    def __init__(self):
        super(BasicModule,self).__init__()
        self.model_name=str(type(self))# 默认名字

    def load(self, path):
        '''
        可加载指定路径的模型
        '''
        self.load_state_dict(torch.load(path))

    def save(self, name=None):
        '''
        保存模型，默认使用“模型名字+时间”作为文件名
        '''
        if name is None:
            prefix = 'checkpoints/' + self.model_name + '_'
            name = time.strftime(prefix + '%m%d_%H:%M:%S.pth')
        torch.save(self.state_dict(), name)
        return name

'''
demo:
class net(BasicModule):
	...

net.load(xxx)
...
net.save()
# 非常舒服
'''
```

#### 稍微复杂一点的模型

> 如Residual Block，参考[我的博客](https://ganler.github.io/2018/12/03/Residual-block-and-its-implementation/)。
>
> 下面的代码参考自[here](https://github.com/yuanlairuci110/pytorch-best-practice-master/blob/master/models/ResNet34.py)。

```python
from torch import nn
from torch.nn import functional as F

class res_block(nn.Module):
    def __init__(self, in_c, out_c, stride=1, short_cut=None):
        super(res_block, self).__init__()
        self.left = nn.Sequencail(
        	nn.Conv2d(in_c, out_c, kernel_size=3, stride, bias=False),
            nn.ReLU(inplace=True),
            nn.Conv2d(out_c, out_c, 3, 1, bias=False),
            nn.BatchNorm2d(out_c)
        )
        self.right = short_cut
        
    def forward(self, x):
        out = self.left(x)
        if self.right is not None:
            out += x
        return F.relu(out)
```

再用Residual Block搭建ResNet。

```python
from util import BasicModule

class ResNet34(BasicModule):
    '''
    实现主module：ResNet34
    ResNet34包含多个layer，每个layer又包含多个Residual block
    用子module来实现Residual block，用_make_layer函数来实现layer
    '''
    def __init__(self, num_classes=2):
        super(ResNet34, self).__init__()
        self.model_name = 'resnet34'

        # 前几层: 图像转换
        self.pre = nn.Sequential(
                nn.Conv2d(3, 64, 7, 2, 3, bias=False),
                nn.BatchNorm2d(64),
                nn.ReLU(inplace=True),
                nn.MaxPool2d(3, 2, 1)
        )
        
        # 重复的layer，分别有3，4，6，3个residual block
        self.layer1 = self._make_layer( 64, 128, 3)
        self.layer2 = self._make_layer( 128, 256, 4, stride=2)
        self.layer3 = self._make_layer( 256, 512, 6, stride=2)
        self.layer4 = self._make_layer( 512, 512, 3, stride=2)

        #分类用的全连接
        self.fc = nn.Linear(512, num_classes)
    
    def _make_layer(self,  inchannel, outchannel, block_num, stride=1):
        '''
        构建包含多个residual block的layer
        '''
        shortcut = nn.Sequential(
                nn.Conv2d(inchannel,outchannel,1,stride, bias=False),
                nn.BatchNorm2d(outchannel)
        )
        layers = []
        layers.append(res_block(inchannel, outchannel, stride, shortcut))
        
        for i in range(1, block_num):
            layers.append(res_block(outchannel, outchannel))
        return nn.Sequential(*layers)
        
    def forward(self, x):
        x = self.pre(x)
        
        x = self.layer1(x)
        x = self.layer2(x)
        x = self.layer3(x)
        x = self.layer4(x)

        x = F.avg_pool2d(x, 7)
        x = x.reshape(x.size(0), -1)
        return self.fc(x)
```

### Fine-tune your network

#### 初始化权值

```python
# 在自己的网络的类中定义权值初始化函数即可：
def initialize_weights(self):
    for module in self.modules():
		if isinstance(module, nn.Conv2d):
			torch.nn.init.xavier_normal_(module.weight.data)
            if module.bias is not None:
                module.bias.data.zero_()
        elif isinstance(module, nn.BatchNorm2d):
            module.weight.data.fill_(1)
            module.weight.data.zero_()
        elif isinstance(module, nn.Linear):
            torch.nn.init.normal_(module.weight.data, 0, 0.01)
            module.bias.data.zero_()
```

- Xavier均匀分布：

  > `torch.nn.init.xavier_uniform_(tensor, gain=1)`
  >
  > $gain\times[-\sqrt{\frac{6}{a+b}}, +\sqrt{\frac{6}{a+b}}]$区间均匀分布。

- Xavier正态分布：

  > `torch.nn.init.xavier_normal_(tensor, gain=1)`
  >
  > $\mu=0, \sigma=gain\times\sqrt{\frac{6}{a+b}}$

- Kaiming均匀分布：

  > `torch.nn.init.kaiming_uniform_(tensor, a=0, mode='fan_in', nonlinear='leaky_relu')`
  >
  > $[-\sqrt{\frac{6}{(1+a^2)\times fan\_in}}, +\sqrt{\frac{6}{(1+a^2)\times fan\_in}}]$
  >
  > `fan_in`使正向传播方差一致，`fan_out`反之。

- Kaiming正态分布：

  > `torch.nn.init.kaiming_normal_(tensor, a=0, mode='fan_in', nonlinear='leaky_relu')`
  >
  > $\mu=0, \sigma=\sqrt{\frac{2}{(1+a^2)\times fan\_in}}$

> `nn.Conv2d`的权值初始化方法为正态分布初始化。

#### Finetune

> 用于transfer learning的Finetune。

```python
# 假设我们导入了一个网络的参数
pretrained_dict = torch.load(some_where)

# 假设我们现在创建了另外一个不同结构的网络
net = Net()

# 将两个网络共有的部分导入到pretrained_dict_
pretrained_dict_ = {
    k: v for k, v in pretrained_dict.items() if k in net.state_dict()
}

net.state_dict.update(pretrained_dict)
```

> 在finetune的时候，有时我们希望<u>前面的layer学习率低一点，后面的高一点</u>，这就要求我们队不同layer设置不同的学习率。
>
> 假设我们想设置2种学习率，那么，我们就需要将我们的参数分成两组，在`nn.optim`中设置其学习率。

```python
ignored_params = list(map(id, net.fc3.parameters()))
# fc3是net中的一个数据成员
base_params = filter(
    lambda p : id(p) not in ignored_params, 
    net.parameters()
)
'''
id(x)返回的是x的内存地址。上面的意思是，对于在net.parameters()中的p，过滤掉'id(p) not in ignored_params'中的p。
'''

optimizer = optim.SGD(
	[{'params': base_params}, 
     {'params': net.fc3.parameters(), 'lr': 0.01}],
    0.001, momentum=0.9, weight_decay=1e-4
)
# 即对于参数的默认学习率是0.001，但我特别安排了fc3的参数的学习率0.01
```

#### Others

> 同样，我们可以再在上述情况下加上`scheduler`。即改变学习率。

```python
>>> # Assuming optimizer uses lr = 0.05 for all groups
>>> # lr = 0.05     if epoch < 30
>>> # lr = 0.005    if 30 <= epoch < 60
>>> # lr = 0.0005   if 60 <= epoch < 90
>>> # ...
>>> scheduler = StepLR(optimizer, step_size=30, gamma=0.1)
>>> for epoch in range(100):
>>>     scheduler.step()
>>>     train(...)
>>>     validate(...)
```

> 具体内容请见后文。
