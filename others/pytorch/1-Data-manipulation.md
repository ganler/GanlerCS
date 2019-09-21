## Data manipulation

### Folders to Dataset

假设我们采集完数据后是这样的：

```c++
<文件夹：dataset>
	- <文件夹：0>
    - <文件夹：1>
```

0和1都是folder，里面包含了原始数据。

假设我们的代码在dataset文件夹隔壁的src文件夹下：

```c++
<文件夹：project>
	- <文件夹：dataset>
	- <文件夹：src>
```

#### 训练集、验证集和测试集的划分

训练集&验证集用来调参，调到最后的模型让测试集来测试其泛化能力。

> 我们现在要做的就是，把dataset中的2个类别的数据划分为训练集，验证集和测试集。
>
> 首先我们新建立这三个数据集的文件夹，再把数据集拷到里面。

```c++
<文件夹：project>
	- <文件夹：dataset>
	- <文件夹：src>
	- <文件夹：Data>
		- <文件夹：train>
		- <文件夹：valid>
		- <文件夹：test>
```

**划分的代码**(参考自[here](https://github.com/tensor-yu/PyTorch_Tutorial/edit/master/Code/1_data_prepare/1_2_split_dataset.py))

```python
import os
import glob
import random
import shutil

dataset_dir = '../dataset/'
train_dir = '../Data/train/'
valid_dir = '../Data/valid/'
test_dir = '../Data/test/'

# 比例 proportion
train_per = 0.8
valid_per = 0.1
test_per = 0.1

# 图片格式
img_format = 'png'

def makedir(new_dir):
    if not os.path.exists(new_dir):
        os.makedirs(new_dir)

'''
首先解释一下os.walk这个函数，该函数返回当前目录下的文件名：
root是"./xxx"里的"."，dirs是当前目录下的文件夹，files是非文件夹。
os.path.join(root, dir/file)是返回当前dir或file的（相对）路径。

'''
for root, dirs, files in os.walk(dataset_dir):
    for sDir in dirs:
        # 获取root/sDir下所有的.png文件，并且返回成一个list
        imgs_list = glob.glob(os.path.join(root, sDir) + '/*.' + img_format)
        random.seed(666)
        random.shuffle(imgs_list)
        imgs_num = len(imgs_list)

        train_point = int(imgs_num * train_per)
        valid_point = int(imgs_num * (train_per + valid_per))

        for i in range(imgs_num):
            if i < train_point:
                out_dir = train_dir + sDir + '/'
            elif i < valid_point:
                out_dir = valid_dir + sDir + '/'
            else:
                out_dir = test_dir + sDir + '/'

            makedir(out_dir)
            out_path = out_dir + os.path.split(imgs_list[i])[-1]
            shutil.copy(imgs_list[i], out_path)

        print('Class:{}, train:{}, valid:{}, test:{}'.format(
            sDir,
            train_point,
            valid_point - train_point,
            imgs_num - valid_point)
        )
```

运行后是这样的：

![img](https://s2.ax1x.com/2019/01/10/FOxUne.png)

##### Dataset类

```python
class Dataset(object):
"""An abstract class representing a Dataset.
All other datasets should subclass it. All subclasses should override ``__len__``, that provides the size of the dataset, and ``__getitem__``, supporting integer indexing in range from 0 to len(self) exclusive. """
	def __getitem__(self, index): 
    	raise NotImplementedError
	def __len__(self):
		raise NotImplementedError
	def __add__(self, other):
		return ConcatDataset([self, other])
```

> 在训练时，对`dataloader`进行迭代的时候，调用的就是`__getitem__`，即每次都返回一个样本（以及其label）。至于如何使用`__getitem__`，一般是创建一个对训练集进行描述的txt文件或者csv文件作为对数据的索引。而索引文件的格式，无非就是 `位置信息+类别`。

> 说到这我觉得确实也没必要把整个数据集分成3个文件夹，其实都放到一堆，然后写3个txt/csv其实就好了。

#### 制作csv文件

> 即在三个文件夹（训练集，验证集，测试集）中建立3个对应的csv文件。

```python
import os
import pandas as pd
'''
假设我们的代码格式都是这样：
- Project
    - dataloader
        codes & files dealing with data
    - record
        model & params
    - dataset
        - class_0
        - class_1
        ...
        - class_N

假设我们的数据格式是这样：
- dataset
    - class_0
        - sample_0.png
        - sample_1.png
        ...
    - class_1
        ...
    ...
    - class_N
本.py最后生成3个csv文件，分别是train.csv, valid.csv & test.csv.
'''

# ==========================================
dict_ = {'image_name': [], 'tags': []}

dataset_loc = '../dataset/'

# csv的路径
train_csv = 'train.csv'
valid_csv = 'valid.csv'
test_csv = 'test.csv'

image_format = '.png'

split_por = [0.7, 0.15, 0.15]
# ==========================================

def csv_generate(split, shuffle=True):
    for root, s_dirs, _ in os.walk(dataset_loc, topdown=True):
        for tag, sub_dir in enumerate(s_dirs):
            class_dir = os.path.join(root, sub_dir) # class_dir是class_x的位置
            img_list = os.listdir(class_dir)      	# 其中的sample
            for i in range(len(img_list)):
                if not img_list[i].endswith(image_format):
                    continue
                dict_['tags'].append(str(tag))
                dict_['image_name'].append(os.path.join(class_dir, img_list[i]))
    # 打乱并切分
    df = pd.DataFrame(dict_).sample(frac=shuffle)
    if shuffle:
        df = df.reset_index()
    train = df.loc[0 : split[0]*len(img_list)]
    valid = df.loc[split[0]*len(img_list) : (split[0]+split[1])*len(img_list)]
    test = df.loc[(split[0]+split[1])*len(img_list):-1]
    train.to_csv(train_csv)
    valid.to_csv(valid_csv)
    test.to_csv(test_csv)

if __name__ == '__main__':
    csv_generate(split_por)
```

得到的csv文件的内容大概为：

```python
,image_name,tags
0,../Data/train/0/0-739.png,0
1,../Data/train/0/0-711.png,0
2,../Data/train/0/0-705.png,0
3,../Data/train/0/0-249.png,0
4,../Data/train/0/0-513.png,0
5,../Data/train/0/0-275.png,0
```

![img](https://s2.ax1x.com/2019/01/12/Fjggjf.png)

#### 自定义dataset设计

```python
import torch
from torchvision import transforms
import pandas as pd
from PIL import Image
import numpy as np

class CustomImgDataset(torch.utils.data.Dataset):
    def __init__(self, csv_path, transform=None):
        tmp_df = pd.read_csv(csv_path) # 将csv的位置作形参
        self.transform = transform
        self.X_train = tmp_df['image_name']
        self.y_train = tmp_df['tags'].astype(np.float32)

    def __getitem__(self, index):
        img = Image.open(self.X_train[index]).convert('RGB') # 对于灰度图是'L'
        if self.transform is not None:
            img = self.transform(img) # transform将直接覆盖原始图片
        label = torch.from_numpy(
            np.array(self.y_train[index])
        		).long()
        return img, label

    def __len__(self):
        return len(self.X_train.index)
```

#### transform

```python
# 注意PIL库读入是图片是height*width*channels
# ToTensor后得到的是channels*height*width(更加符合张量给人的感觉)

# 使用均方差归一化的时候，先要计算出各个通道的均值(mu)，和标准差sigma；
norm_mean = [r_mu, g_mu, b_mu]
norm_std = [r_sigma**2, g_sigma**2, b_sigma**2]
norm_transform = transforms.Normalize(norm_mean, norm_std)

train_transform = transforms.Compose([
    transforms.Resize(32),
    transforms.RandomCrop(32, padding=4),
    transforms.ToTensor(),
    norm_transform
])
# 这里RandomCrop是指随机裁剪图片大小，即给原图（32x32）加上4圈padding之后，再选一块32x32的区域。

valid_transform = transforms.Compose([
    transforms.ToTensor(),
    norm_transform
])
```

> 其中计算均值和方差的方法: 参考自[here](https://github.com/tensor-yu/PyTorch_Tutorial/edit/master/Code/1_data_prepare/1_5_compute_mean.py)

```python
import numpy as np
import cv2
import random
import pandas as pd

train_csv = '../Data/train/train.csv'
channel_num = 3

img_h, img_w = 32, 32
imgs = np.zeros([img_w, img_h, 3, 1])
means, stdevs = [], []

srcs = pd.read_csv(train_csv)['image_name']
random.shuffle(srcs)		# 随机打乱

# 思路：从第四维度，将img们堆叠在一起。
for cnt, src in enumerate(srcs):
    img = cv2.imread(src)
    img = cv2.resize(img, (img_h, img_w))
    img = img[:, :, :, np.newaxis]
    imgs = np.concatenate((imgs, img), axis=3)
    if(cnt%5 == 0)
    	print("now : ", cnt, '/', len(srcs))
	cnt += 1

imgs = imgs.astype(np.float32)/255.

for i in range(channel_num):
    pixels = imgs[:,:,i,:].ravel()  # 拉成一行
    means.append(np.mean(pixels))
    stdevs.append(np.std(pixels))

means.reverse() # BGR --> RGB, opencv是默认BGR的
stdevs.reverse()

print("normMean = {}".format(means))
print("normStd = {}".format(stdevs))
print("直接复制下列代码即可")
print('norm_transform = transforms.Normalize(norm_mean = {}, norm_std = {})'.format(means, stdevs))
```

> **Transform**方法的碎碎念：
>
> > 均来自`torchvision.transforms`中。

- `transforms.Normalize(mean, std)`

- `transforms.Grayscale(num_output_channels=1)`

  > 转灰度图。当`num_output_channels`是3时，保证rgb三通道的值相等。

- `transforms.LinearTransformation(transformation_matrix)`

  > 线性变换。y=kx+b。可用于白化（ZCA）。

```python
# ZCA
'''
Y = XW
X = S*lambda*S^T
W = (len(X)-1)*S*((lambda)**(1/2))*S^T
'''
from numpy import *

def zca(X):
    X -= mean(X, axis=0)
    eigVals, eigVects = linalg.eig(cov(X, rowvar = 0))
    # 在cov中已经把(n-1)给除去了
    W = eigVects.dot(linalg.inv(sqrt(diag(eigVals))).dot(eigVects.T))
    return X.dot(W)
```

- `transforms.ToTensor()`

  > 将PIL Image或ndarray转为tensor，并且归一化至[0, 1]（直接除255）。

- `transforms.ToPILImage(mode=None)`

  > mode为None的时候，为1通道。mode是3的时候转RGB。

- `transforms.Resize(size, interpolation=2)`

  > 插值方法interpolation默认是`PIL.Image.BILINEAR`双线性插值法。

- `transforms.ColorJitter(brightness=0, contrast=0, saturation=0, hue=0)`

  > 修改亮度，对比度，饱和度和色调。

- `transforms.RandomCrop(size, padding, padding_mode='constant', fill=0)`

  > 当`padding_mode = 'constant'`的时候，padding的填充值为fill（可以是直接的数值，也可以是长度为通道数的tuple）

- `transforms.CenterCrop(size)`

  > 从中心进行裁剪。

- `transforms.RadomResizeCrop(size, scale=(0.08, 1.0), ratio=(0.75, 1.333333333), interpolation=2)`

  > 将原图按随机**比例**（ratio是比例的范围）和**大下**（scale的范围）裁剪后，再上/下采样到size大小。
  >
  > Interpolation 上采样的插值方法（默认为双线性插值）

- `transforms.RandomHorizontalFlip(p=0.5)`

  > 按p的概率，随机将突破水平（Horizontal）翻转。

- `transforms.RandomVerticalFlip(p=0.5)`

- `transforms.RandomRotation(degrees, resample=False, expand=False, center=None)`

  > 按随机角度(degrees表示范围，若degrees是int类型，如30，则范围是`(-30, +30)`之间，若是sequence类型，如(-20, 60)，那么这就是范围)进行旋转。
  >
  > `center`表示是中心旋转，默认是左上角旋转。
  >
  > `resample`重采样方法选择，可选`PIL.Image.NEAREST`，`PIL.Image.BILINEAR`，`PIL.Image.BICUBIC`，默认为最近邻nearst。

  - `transforms.RandomChoice(transforms)`

    > 随机选择其中的一个trans。

### 可能是更好的Dataset管理方式

之前的方法会造成数据集的重复复制，非常麻烦，这里给出一个不用复制，直接产生csv文件的方法。

```python
import os
import pandas as pd
'''
假设我们的代码格式都是这样：
- Project
    - dataloader
        codes & files dealing with data
    - record
        model & params
    - dataset
        - class_0
        - class_1
        ...
        - class_N

假设我们的数据格式是这样：
- dataset
    - class_0
        - sample_0.png
        - sample_1.png
        ...
    - class_1
        ...
    ...
    - class_N
本.py最后生成3个csv文件，分别是train.csv, valid.csv & test.csv.
'''

# ==========================================
dict_ = {'image_name': [], 'tags': []}

dataset_loc = '../dataset/'

# csv的路径
train_csv = 'train.csv'
valid_csv = 'valid.csv'
test_csv = 'test.csv'

image_format = '.png'

split_por = [0.7, 0.15, 0.15]
# ==========================================

def csv_generate(split, shuffle=True):
    for root, s_dirs, _ in os.walk(dataset_loc, topdown=True):
        for tag, sub_dir in enumerate(s_dirs):
            class_dir = os.path.join(root, sub_dir) # class_dir是class_x的位置
            img_list = os.listdir(class_dir)      	# 其中的sample
            for i in range(len(img_list)):
                if not img_list[i].endswith(image_format):
                    continue
                dict_['tags'].append(str(tag))
                dict_['image_name'].append(os.path.join(class_dir, img_list[i]))
    # 打乱并切分
    df = pd.DataFrame(dict_).sample(frac=shuffle)
    if shuffle:
        df = df.reset_index()
    train = df.loc[0 : split[0]*len(img_list)]
    valid = df.loc[split[0]*len(img_list) : (split[0]+split[1])*len(img_list)]
    test = df.loc[(split[0]+split[1])*len(img_list):-1]
    train.to_csv(train_csv)
    valid.to_csv(valid_csv)
    test.to_csv(test_csv)

if __name__ == '__main__':
    csv_generate(split_por)
```

