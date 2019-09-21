# PyTorch Learning Note: Brief Intro & Install

[PyTorch](http://www.pytorchtutorial.com/goto/http://pytorch.org/) 是 [Torch](http://www.pytorchtutorial.com/goto/http://torch.ch/) 在 Python 上的衍生. 因为 [Torch](http://www.pytorchtutorial.com/goto/http://torch.ch/) 是一个使用 [Lua](http://www.pytorchtutorial.com/goto/https://www.lua.org/) 语言的<u>神经网络库</u>, Torch 很好用, 但是 Lua 又不是特别流行, 所有开发团队将 Lua 的 Torch 移植到了更流行的语言 Python 上。

> 关于各平台的比较与选择可以参考[这里](https://www.zhihu.com/question/59274399/answer/163537353)。个人觉得，对于想深入了解并且想在其中create的朋友们，`PyTorch`是一个非常好的框架。而对于有速度要求的可以考虑`caffe2`。但`caffe2`已经能做到将代码并入`PyTorch`了。

在神经网络领域，`PyTorch`可以用来替换`numpy`。



## PyTorch 和 Tensorflow 

> （摘自莫凡的教程）
>
> ​	据 `PyTorch` 自己介绍, 他们家的最大优点就是<u>建立的神经网络是动态的</u>, 对比静态的 `Tensorflow`, 他能更有效地处理一些问题, 比如说 RNN 变化时间长度的输出。 而我认为, 各家有各家的优势和劣势, 所以我们要以中立的态度。两者都是大公司, Tensorflow 自己说自己在分布式训练上下了很大的功夫, 那我就默认 Tensorflow 在这一点上要超出 PyTorch, 但是 Tensorflow 的静态计算图使得他在 RNN 上有一点点被动 (虽然它用其他途径解决了), 不过用 PyTorch 的时候, 你会对这种动态的 RNN 有更好的理解。
>
> 而且 Tensorflow 的高度工业化, 它的底层代码… 你是看不懂的. PyTorch 好那么一点点, 如果你深入 API, 你至少能比看 Tensorflow 多看懂一点点 PyTorch 的底层在干嘛。
>
> 最后我的建议就是:
>
> - 如果你是学生, 随便选一个学, 或者稍稍偏向 PyTorch, 因为写代码的时候应该更好理解。懂了一个模块, 转换 Tensorflow 或者其他的模块都好说.
> - 如果是上班了, 跟着你公司来, 公司用什么, 你就用什么, 不要脱群。



## 安装

最开始`PyTorch`是不支持`win`的，只支持`OSX`和`Linux`。这里提供`OSX`的安装方法（无GPU版本）：

```shell
conda create -n pytorch python=3.6
# 上面一句运行完后可能需要等待一段时间来下载
source activate pytorch
pip3 install torch torchvision
```

只要能在`Anaconda`下输入`import torch`，并且不报错，就说明你成功了。



## Numpy & Torch

> ​	Torch 自称为神经网络界的 Numpy, 因为他能将 torch 产生的 tensor 放在 GPU 中加速运算 (前提是你有合适的 GPU), 就像 Numpy 会把 array 放在 CPU 中加速运算。所以神经网络的话, 当然是用 Torch 的 tensor 形式数据最好咯。 就像 Tensorflow 当中的 tensor 一样。

`numpy`和`PyTorch`能做到较好的兼容，以方便使用者自由和转换。（`numpy array` & `torch tensor`）

```python
import torch
import numpy as np

# 用numpy建立数据
np_data = np.arange(6).reshape((2, 3))
# 把np的数据转化为torch的tensor
torch_data = torch.from_numpy(np_data)
# 把torch的tensor转化为np的array
tensor2array = torch_data.numpy()

print(f"[numpy data]:\n{np_data}\n\n [torch data]:\n{torch_data}\n\n [tensor2array]:\n{tensor2array}")
```

```python
# out:
[numpy data]:
[[0 1 2]
 [3 4 5]]

 [torch data]:
tensor([[0, 1, 2],
        [3, 4, 5]])

 [tensor2array]:
[[0 1 2]
 [3 4 5]]
```

重点记住:

**array2tensor:	**`torch.from_numpy()`

**tensor2array:	**`torch.numpy()`



> **REF.**
>
> [莫凡的PyTorch](https://morvanzhou.github.io/tutorials/machine-learning/torch/1-1-why/)