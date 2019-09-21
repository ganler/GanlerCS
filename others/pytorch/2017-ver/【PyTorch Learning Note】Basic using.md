# 【PyTorch Learning Note】Basic using

>  **适合人群**：
>
> 对ANN和BPNN的整体结构和思想熟悉，并对之推导有一定影响的Python入门的朋友。（有`numpy`使用经验，则食用效果更佳）。然后，建议快速跳读，留个印象即可。



## 基本操作

**张量**(tensor)：多维数组。比如三维的张量**A**中坐标为(i, j, k)的元素记作$A_{i,j,k}$

> 其实标量，向量，矩阵它们三个也是张量，标量是零维的张量，向量是一维的张量，矩阵是二维的张量。 

在Python中`[]`有多少层，就意味着有多少维，也就是说明其为几维张量。

python中对维度(ndim)的定义完全可以由print之后看"[]"有多少层决定

> [[1,2]
> [[3,4]]	                          有2层"[]"所以其ndim是2
> [1,2,3]                               ndim=1
> [[1,2,3],[1,3,4],[2,3,1]]     ndim=2

**矩阵的创建**

```python
>>> import torch
>>> # 创建一个4*3的矩阵 [torch.Tensor(rowNum, colNum)]
... mtxTensor1 = torch.Tensor(4, 3)
>>> mtxTensor1
tensor([[ 0.0000e+00, -2.5244e-29,  0.0000e+00],
        [-2.5244e-29,  4.2981e+21,  6.3828e+28],
        [ 3.8016e-39,  3.7836e-39,  0.0000e+00],
        [-2.5244e-29, -8.9071e+02,  2.7837e-40]])
```

**随机矩阵初始化**

```python
mtxTensor2 = torch.rand(4, 3)
print(mtxTensor2)

# tensor([[0.1656, 0.2028, 0.0193],
#         [0.0661, 0.2309, 0.0158],
#         [0.3768, 0.7813, 0.1680],
#         [0.7660, 0.2834, 0.1335]])
```

**其他和矩阵有关的操作**

```python
mtxTensor1.size()
# torch.Size([raw, col])
# 实际上是一个元组
```

**其他**

```python
import torch

x = torch.rand(3, 2)
y = torch.rand(3, 2)
print(x+y)

# 使用含'_'的操作将直接改变被操作对象本身
x.copy_(y)
# 改变了x，x照抄了y的内容，但是不是通过引用的方式，而是通过赋值方式

# tensor支持python的索引操作

torch.numel(x) # 返回矩阵的元素数(2×3)
```

```python
>>> torch.zeros(2, 3)
tensor([[ 0.,  0.,  0.],
        [ 0.,  0.,  0.]])

>>> torch.zeros(5)
tensor([ 0.,  0.,  0.,  0.,  0.])

# torch.ones()同
```

```python
# torch中很赞的一点
inMat = torch.rand(2, 3)
torch.zeros_like(inMat)
# 即torch.zeros_like()输入一个矩阵，返回同等size的零矩阵
# torch.ones_like()同

torch.arange(1, 4)	# [1 2 3]
# 为什么要用arange，因为arange支持步长为小数的情况
torch.range(1, 4)	# [1 2 3 4]
# 在0.5版本会被移除
torch.linspace(start, end, steps=100, out=None, dtype=None, layout=torch.strided, device=None, requires_grad=False)

torch.empty(3, 5)
# 声明一个未初始化的矩阵，弱想print这个矩阵的话，值是不确定的，和[torch.Tensor(size)]一样
```

```python
# 如果要用gpu
if torch.cuda.is_available():
    x = x.cuda()
    y = y.cuda()
    x + y
```

有额外需求朋友，欢迎狂啃[PyTorch.doc](https://pytorch.org/docs/stable/torch.html)。



## 数学运算

很多地方和`numpy`如出一辙。

```python
import torch

# 要清楚一点，numpy是针对array的，torch是针对tensor的
data = [-1, 1]
tensor = torch.FloatTensor(data)

# abs绝对值:torch.abs()
print(torch.abs(tensor))	# [1 1]

# sin(cos啥的同理)
print(torch.sin(tensor))

# mean均值
print(torch.mean(tensor))
```

**矩阵运算**

```python
import torch
import numpy as np

# torch和numpy在矩阵运算上不完全一样
data = np.array([[1, 2], [3, 4]])
tensor = torch.FloatTensor(data)

# 结果相同的情况
print("\nmatrix multiplication(matmul way):\n",
      f"\nnumpy:\n{np.matmul(data, data)}\n",
      f"\nPyTorch:\n{torch.mm(tensor, tensor)}\n")

# 结果不一的情况(！！！错误示例 ！！！)

newTensor = torch.FloatTensor([1,2,3,4])
print("\nmatrix multiplication(dot way):\n",
      f"\nnumpy:\n{data.dot(data)}\n",
      f"\ntorch:\n{newTensor.dot(newTensor)}")
# numpy的两种方法都一样，而pytorch的dot只能对一维数组使用，对于多维数组会报错
```

```python
# [out]:
matrix multiplication(matmul way):
 
numpy:
[[ 7 10]
 [15 22]]
 
PyTorch:
tensor([[ 7., 10.],
        [15., 22.]])


matrix multiplication(dot way):
 
numpy:
[[ 7 10]
 [15 22]]
 
torch:
30.0
```








> **REF.**
>
> [莫烦大佬](https://morvanzhou.github.io/tutorials/machine-learning/torch/2-01-torch-numpy/)
>
> [csdn1](https://blog.csdn.net/u014630987/article/details/78669051)
>
> [chenyuntc](https://github.com/chenyuntc/pytorch-book/blob/master/chapter3-Tensor%E5%92%8Cautograd/Autograd.ipynb)
>

> **推荐**
>
> [简书](https://www.jianshu.com/p/52684285e335)