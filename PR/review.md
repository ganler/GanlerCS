[TOC]

## Intro

- Preprocessing 
- Feature Extraction
- Classification

## Bayesian

### Theory

最小化风险：
$$
R(c_i|x)=\sum_{j=1}^N\lambda_{ij}P(c_j|x)
$$
表示，猜测x是$c_i$ 类，获得的损失。$\lambda_{ij}$把j类当成i类造成的损失。
$$
P(c_j|x)=\frac{p(c_j, x)}{p(x)}=\frac{p(x|c_j)p(c_j)}{\sum p(x|c_i)p(c_i)}
$$
目标，寻找判定准则h：
$$
R(h)=\mathbb E_x[R(h(x)|x)]
$$

正态分布贝叶斯决策：

- 把每个类的分布看做正态分布；

1. $\Sigma_i=\sigma^2I$，各类特征独立，方差相等；

$$
g_i(x)=-\frac{1}{2\sigma^2}||x-\mu_i||^2
$$

2. $\Sigma_i=\Sigma$，各类协方差矩阵相同：

线性判别
$$
\begin{aligned}
g_i(x)&=-\frac{1}{2}(x-\mu_i)^T\Sigma^{-1}(x-\mu_i)+\ln p(w_i)\\
&=\Sigma^{-1}\mu_i x+\left(-\frac{1}{2}\mu_i^T\Sigma^{-1}\mu_i+\ln p(w_i)\right)\\
&= w_i^Tx+b_i
\end{aligned}
$$

3. 各类协方差不同；

### Exercise

![](https://i.loli.net/2019/12/17/5yDJZIgvUftWRaB.png)
$$
p(w_1|x)=\frac{p(w_1, x)}{p(x)}=\frac{0.2*0.9}{0.6}=0.3\\
p(w_2|x)=\frac{p(w_2, x)}{p(x)}=\frac{0.1*0.4}{0.6}=0.07
$$
所以应该认为是第一类；

![](https://i.loli.net/2019/12/17/JAq7RkQ25cSlZT6.png)
$$
R(c_1|x)=0*0.3+6*0.07=0.42\\
R(c_2|x)=1*0.3=0.3
$$
所以$c_1$风险大，选2；

![](https://i.loli.net/2019/12/17/9fJ6ONguK4rx8T2.png)

$$
p(w_1)=0.5\%\\
p(w_2)=99.5\%\\
p(x|w_2)=1\%\\
p(x|w_1)=95\%\\
p(w_1|x)=\frac{p(w_1, x)}{p(x)}=\frac{p(x|w_1)p(w_1)}{p(x)}=0.005\times 0.95\\
p(w_2|x)=\frac{p(w_2,x)}{p(x)}=\frac{p(x|w_2)p(w_2)}{p(x)}=0.995\times 0.01
$$

## Maximum Likehood & Bayesian Parameter Estimation

$$
P(c_j|x)=\frac{p(c_j, x)}{p(x)}=\frac{p(x|c_j)p(c_j)}{\sum p(x|c_i)p(c_i)}
$$

分母不用管，分子要求：

1. $p(x|c_j)$ 有了数据集之后，走参数估计，求出「概率分布函数」；
2. $p(c_j)$ 数据集类别比例，有数据集直接求；

参数估计一般2条路：

- MLE -> 求参数值
- Baysian -> 求参数分布

### MLE 参数估计

> 特点：
>
> - 参数是固定值，但我们不晓得；
> - 最佳参数可以通过优化似然得到；

贝叶斯理论要求我们求$P(x)=\sum_i P(x|c_i)P(c_i) $；所以我们要求$P(x)$（一个概率分布）。

然后机器学习嘛，learn from data，我们需要有data；

首先我们要假设c类下的真实数(可用数据集$D_c$表示)分布符合某种概率分布$P$，然后将$P$带入到$P(x|\theta_c)$，但是这个概率分布可能有一些参数，那就是$\theta_c$。假设这个值和参数向量$\theta_c$唯一有关，那么我们就是<u>利用训练集训练求得$\theta_c$。</u>

假设这些样本是独立同分布的，那么**似然**就是：
$$
L(D_c|\theta_c)=\prod_{x\in D_c}P(x|\theta_c)
$$
对数似然就是：
$$
LL(D_c|\theta_c)=\sum_{x\in D_c}\ln P(x|\theta_c)
$$
我们要通过设置$\theta_c$，使似然尽可能大：
$$
\hat\theta_c=\arg\max_{\theta_c}LL(D_c|\theta_c)
$$
然后最优化，求导即可解。

> 一个重点就是：如果使用MLE，那么预测的distribution要尽量符合真实的数据分布，否则凉凉。

#### 例子

$$
f(X;\theta) = \theta e^{-\theta x},{\rm if\ x\ge 0, otherwise\ 0}\\
l(\theta)=\sum \ln f(x|\theta)=n\ln \theta - \theta \sum x_i, x_i\ge 0\\
\frac{d l}{d\theta}=\frac{n}{\theta}-\sum x_i, x_i\ge 0\\
$$

求最优解即可。

### Bayesian Estimation

在BE中，其参数是一个变量，而非定值；
$$
P(c_j|x)=\frac{p(c_j, x)}{p(x)}=\frac{p(x|c_j)p(c_j)}{\sum p(x|c_i)p(c_i)}
$$
我们要求$p(x|c_j)$，即求其概率分布模型和参数$\theta$ ，这是MLE干的，而贝叶斯参数估计的特点是不求参，而是求参数的分布$p(\theta)$ 。

其参数分布与数据集有关，我们的数据集是D的时候，参数的分布就是$p(\theta|D)$。

增量学习：

一开始我们没有样本，那么参数分布直接为$p(\theta) = p(\theta|D^0)$，每次增加一个样本$x_n$，我们有：

$$
p(D^n|\theta)=p(x_n|\theta)p(D^{n-1}|\theta)\\
p(\theta|D^n)=\frac{p(x_n|\theta)p(\theta|D^{n-1})}{\int p(x_n|\theta)p(\theta|D^{n-1})d\theta}\sim p(x_n|\theta)p(\theta|D^{n-1})
$$

#### 例子

我们一开始知道我们的参数w的范围是`(0, 10]`，我们的数据是`D = {4, 7, 2, 8}`。

分布:
$$
\mathbf{distribution}\to p(x|w)\sim U(0,w)=1/w\ \ {\rm if}\ \ x\in[0,w],\rm otherwise\ 0
$$

$$
\begin{aligned}
&p(w|D^0)=p(w)=U(0,10)=&1/w&\to w\in (0, 10]\\
&p(w|D^1)\sim p(x_1|w)p(w|D^0)=&1/w^2&\to w\in [4, 10]\\
&p(w|D^n)\sim p(x_n|w)p(w|D^{n-1})=&1/w^{n+1}&\to w\in[\max{D^n},10]
\end{aligned}
$$
同上，我们要求$p(x|c_j)$，其实还是求$p(x|D)$。
$$
\begin{aligned}
&p(\theta|D)\to p(x|D) = \int p(x|\theta)p(\theta|D)d\theta\\
&p(\theta|D)=\frac{p(\theta,D)}{p(D)=\int p(D|\theta)p(\theta)d \theta}\\
&p(D|\theta)=\prod_{k=1}^np(x_k|\theta)
\end{aligned}
$$
最后一行，想知道整个数据集，就是想知道每一个样本的概率的连乘。

### 朴素贝叶斯分类器

这里$x_i$是x的第i个属性值。可直接理解公式。
$$
P(c|x)=\frac{P(c)P(x|c)}{P(x)}=\frac{P(c)}{P(x)}\prod_{i=1}^dP(x_i|c)
$$

## Linear Regression

对于相互线性可分的c类，最多$\frac{c(c-1)}{2}$可分。

### Generalized Linear Discriminant Functions 

<img src="https://i.loli.net/2019/12/22/NqBQyc2fSH7d6Ra.png" style="zoom:50%;" />

Not linearly seperable? Define a  new space:

e.g. `[x1, x2] -> [x1, x2, x1x2]`

### Fisher Linear DIscrimination

- Minimize within-class scatter.
- Maximize between-class scatter.

$$
w^*x+w_0=0\\
w^*=S_w^{-1}(\mu_1-\mu_2)\\
w_0=-w^{*\top}\frac{\sum_i^N \mu_i}{N}\\
S_w=\sum S_i=\sum \left(\sum_{x\in K_i} (x-\mu_i)(x-\mu_i)^\top\right)
$$

$\mu_i$，对于第i类，x的均值。

#### 例子

<img src="https://i.loli.net/2019/12/22/Lb4BTQOalEieHYf.png" style="zoom:50%;" />
$$
S_w={\rm diag(2,2)}\\
w^*={\rm diag(1/2,1/2)}[0,-2]^\top=[0,-1]^\top\\
w_0=-w^{*\top}\frac{m_1+m_2}{2}=1
$$
(w0是一个标量)

### Perceptron & example

$$
x_1,x_2\to c_1\\
x_3,x_4\to c_2
$$

针对于增广（负样本还得反向一下）后的形式；

<img src="https://i.loli.net/2019/12/22/L28dD3RmJHiCbMu.png" style="zoom:40%;" />

这里就是对了不管，错了就w += cx（因为wx+b对w求导就是x）

（等于0一样要更新，人脑训练，没办法这就是你同济的要求 orz）

