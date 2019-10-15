

# Quaternions 四元数

[TOC]

## Complex Number 复数

#### 平方

$$
z = (a+b\bold i)\\
z^2 = a^2-b^2+2ab\bold i
$$

#### 共轭复数

$$
z = a+b\bold i\\
\bar z = a-b\bold i\\
z\bar z=a^2+b^2
$$

#### 绝对值

$$
|z|=\sqrt{z\bar z}
$$

$$
|\frac{z_1}{z_2}|=\frac{|z_1|}{|z_2|}
$$

#### 其他

$$
\bold i^0=1\\
z*\bold i^4=z
$$

#### application

二维平面上：

> 乘个i即逆时针旋转90度；

$$
z = 2+\bold i\\
z * \bold i = -1+2\bold i
$$

![](https://www.qiujiawei.com/images/2015.9/5.png)

#### 旋转数 Rotors

> 逆时针旋转$\theta$度，那么就是想象一个与x轴正向成$\theta $的单位向量，算出他对应的复数，与之相乘即可；
>
> 如逆时针旋转45度：$q=\cos 45°+\bold i \sin 45°$

$$
z = a + b\bold i\\
q = \cos \theta+\bold i \sin \theta \\
qz = (a\cos\theta - b\sin \theta)+(a\sin\theta+b\cos\theta)\bold i
$$

> 思考角度：
>
> 实数：一维数；
>
> 复数：二位数；
>
> 在三维中做旋转的话，我们需要一个”三维数“；但其实我们需要一个”四维数“；
>
> > k维的数需要$2^{k-1}$元数

## 四元数

了解了复数系统和复数平面后，我们可以额外增加2个虚数到我们的复数系统，从而把这些概念拓展到3维空间。
$$
q=s+x\bold i+y\bold j+z\bold k
$$

> 共轭四元数：
> $$
> \bar h = s-xi-yj-zk
> $$

$$
\bold i^2=\bold j^2 = \bold k^2=\bold{ijk}=-1\\
\bold{ij=k}\\
\bold{jk=i}\\
\bold{ki=j}\\
\bold{ji=-k}\\
\bold{kj=-i}\\
\bold{ik=-j}\\
$$

> 类似笛卡尔坐标系下的叉乘规则；

![](https://www.qiujiawei.com/images/2015.9/6.png)

#### 有序四元数

$$
\bold q = [s, x\bold i+y\bold j+z\bold k]
$$

> 与复数形式相呼应；

#### 四元数的乘积

> 叉乘；

#### 四元数的逆

$$
qq^{-1}=1\\
q^*qq^{-1}=q^*\\
|q|^2q^{-1}=q^*\\
q^{-1}=\frac{q^*}{|q|^2}
$$

#### 四元数的旋转

让向量v沿着旋转轴u旋转$\theta$度：
$$
q=[\cos(\frac{\theta}{2}),\sin(\frac{\theta}{2})\bold u]
$$

$$
v'=qvq^{-1}
$$

```c++
glm::vec3 myRotationAxis( ??, ??, ??);
glm::mat4 anim = glm::rotate(
  glm::mat4(1.0f), 
  glm::radians(angle),  // 角度转弧度
  myRotationAxis);
```

