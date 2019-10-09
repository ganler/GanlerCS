# Kalman Filter

## 原理

### 依据

- 历史数据：用矩阵A来描述上一次数据到这一次数据的转换状态
- 控制方式：u是控制单元，其将控制当时的x的变化，然后用矩阵B来表示当时u的转换
- 噪声：假设其符合均值为0，协方差矩阵为Q

$$
\rm \bold {x_k=A_kx_{k-1}+B_k u_k+w_k}
$$

> x是真实的结果，而我们往往得不到真实的结果，只能得到其观测值（传感器测得）z。
> $$
> \rm\bold{z_k=H_kx_j+v_k}
> $$
> H矩阵是把真实空间的结果映射到观测空间的转换矩阵，v是噪声，满足均值为0的正态分布（协方差矩阵为R）。

### 已知的数据

- 观测值z
- 控制规则u
- A：如果是(距离/速度)模型的话，那么x'=x+dx
- B: v'=adt+v
- H: 一般H直接被设置为单位矩阵

### 不知道的

- x
- 各种噪声

![preview](https://pic1.zhimg.com/v2-75fbd11f4815c8aeb24c95f0a58d8820_r.jpg)

> 引自：https://zhuanlan.zhihu.com/p/73814069

## 计算

### 预测-校正

#### 根据k-1后验估计得到k先验估计

$$
\rm\bold{\hat x_k'=A_k\hat x_{k-1}+B_k u_k}
$$

- $\rm\bold{\hat x_k’}$为k时刻的先验估计——预测子，其误差为先验误差：$\rm\bold{e_k'\equiv x_k-\hat x_k'}$
- $\rm\bold{\hat x_{k-1}’}$为k-1时刻的后验证估计——校正子，后验误差：$\rm\bold{e_k=x_k-\hat x_k}$

然后更新先验误差和后验误差的协方差矩阵$P_k',P_k$。

然后我们的目标就希望**<u>后验误差尽量小</u>**。（后验协方差矩阵的迹尽量小）

> 有3个用正态分布来描述误差的矩阵：
>
> - Q: 过程噪声
> - R: 观测噪声
> - P': 预测估计协方差矩阵

#### 通过k先验估计得到k后验估计

$$
\rm\bold{\hat x_k=\hat x_k'+K_k(z_k-H\hat x_k')}
$$

- $K_k$是卡尔曼增益；

我们需要找到一个最优的卡尔曼增益，使协方差矩阵的迹尽量小。

> 略去推导。

$$
\rm\bold{K_k=P_k'H^T(HP_k'H^T+R)^{-1}}
$$

**预测：**

![](https://www.zhihu.com/equation?tex=%5Cmathbf%7B%5Chat%7Bx%7D%27%7D_k+%3D+%5Cmathbf%7BA%7D+%5Cmathbf%7B%5Chat%7Bx%7D%7D_%7Bk-1%7D+%2B+%5Cmathbf%7BB%7D+%5Cmathbf%7Bu%7D_k+%5C%5C+%5Cmathbf%7BP%7D%27_k+%3D+%5Cmathbf%7BA%7D+%5Cmathbf%7BP%7D_%7Bk-1%7D+%5Cmathbf%7BA%7D%5ET+%2B+%5Cmathbf%7BQ%7D%E3%80%82)

> 协方差的变化很简单：
> $$
> Cov(P)=\Sigma\\
> Cov(AP)=A\Sigma A^T
> $$
> 

**校正：**

![](https://www.zhihu.com/equation?tex=%5Cmathbf%7BK%7D_k+%3D+%5Cmathbf%7BP%27%7D_k+%5Cmathbf%7BH%7D%5ET+%28%5Cmathbf%7BH%7D+%5Cmathbf%7BP%27%7D_k+%5Cmathbf%7BH%7D%5ET+%2B+%5Cmathbf%7BR%7D%29%5E%7B-1%7D%2C+%5C%5C+%5Cmathbf%7B%5Chat%7Bx%7D%7D_%7Bk%7D+%3D+%5Cmathbf%7B%5Chat%7Bx%7D%E2%80%99%7D_%7Bk%7D+%2B+%5Cmathbf%7BK%7D_k+%28%5Cmathbf%7Bz%7D_k+-++%5Cmathbf%7BH%7D+%5Cmathbf%7B%5Chat%7Bx%7D%E2%80%99%7D_%7Bk%7D%29%2C+%5C%5C+%5Cmathbf%7BP%7D_k+%3D+%28%5Cmathbf%7BI%7D+-+%5Cmathbf%7BK%7D_k+%5Cmathbf%7BH%7D%29+%5Cmathbf%7BP%7D%27_k+%E3%80%82)

> 引自：https://zhuanlan.zhihu.com/p/73814069

初始时给定一个$\hat x_0,P_0$即可。

## OpenCV Usage

### Data Members

```c++
Mat statePre;           //!< 预测子 (x'(k)): x(k)=A*x(k-1)+B*u(k)  
Mat statePost;          //!< 校正子 (x(k)): x(k)=x'(k)+K(k)*(z(k)-H*x'(k))  
Mat transitionMatrix;   //!< 根据历史数据-转换(A)  
Mat controlMatrix;      //!< 控制模型-转换 (B) (如果没有控制就没用)

Mat measurementMatrix;  //!< 测量值到真实值的转换 (H)  

Mat processNoiseCov;    //!< process noise covariance matrix (Q)  
Mat measurementNoiseCov;//!< measurement noise covariance matrix (R)  

Mat errorCovPre;        //!< 先验协方差 (P'(k)): P'(k)=A*P(k-1)*At + Q)*/  
Mat gain;               //!< 增益 (K(k)): K(k)=P'(k)*Ht*inv(H*P'(k)*Ht+R)  
Mat errorCovPost;       //!< 后验协方差 (P(k)): P(k)=(I-K(k)*H)*P'(k)  

// temporary matrices  
Mat temp1;  
Mat temp2;  
Mat temp3;  
Mat temp4;  
Mat temp5; 
```

