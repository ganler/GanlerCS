# LBA 线性有界自动机与CSL

> [复习资料1](http://blog.forec.cn/2017/02/25/formal-languages-and-automata1/)

## 直观感觉

- 特殊的**非确定型**图灵机，主要是对读写头的移动范围加以限制；
- LBA接受的语言类小于TM；

## 概念

### 表示

> 一种特殊的非确定型图灵机；
>
> Possible to simulate k tape cells with a single tape cell, by increasing the size of the tape alphabet

$$
M=(Q,\Sigma, \Gamma, \delta, l, r, s, t, r)
$$

### 特性

- 有界；
- 线性：对于长度为n的输入，存储量是kn+b倍；
- 实际计算机都是LBA；

### 例子1

> 对于输入是$\{ww|w\in{a,b}^+\}$。
>
> - 找中心；(奇数reject)；(到左边界打右箭头，到右边界打左箭头)
> - 然后一个一个匹配；

$$
\begin{aligned}
&\sigma(q_0,\P)=(q_1,\P,R)\\
&\sigma(q_1,X)=(q_2,\overrightarrow X, R)\\
&\sigma(q_1,\overrightarrow X)=(q_10,\overrightarrow X, 0)\\
&\sigma(q_2,X)=(q_2,X,R)\\
&\sigma(q_2,$)=(q_3,$,L)\\
&\sigma(q_2,\overleftarrow X)=(q_3,\overleftarrow X,L)\\
&\sigma(q_3,X)=(q_4,\overleftarrow X,L)\\
&\sigma(q_3,\overrightarrow X)=(q_5,\overrightarrow X,R)\\
&\sigma(q_4,X)=(q_4,X,L)\\
&\sigma(q_4,\overrightarrow X)=(q_1,\overrightarrow X,R)\\
&\sigma(q_5, \overrightarrow X)=(q_5,\overrightarrow X,L)\\
&\sigma(q_5,\P)=(q_6,\P,R)\\
&\sigma(q_6,\overrightarrow X)=([q_6,X],empty,R)\\
&\sigma([q_6,X], \overrightarrow Y)=([q_6,X],\overrightarrow Y,R)\\
&\sigma([q_6,X], empty)=([q_6,X],empty,R)\\
&\sigma([q_6,X], \overleftarrow X)=(q_7,empty,L)\\
&\sigma([q_6,X], \overrightarrow Z)=(q_{10}, \overrightarrow Z,0)\\
&\sigma(q_7,\P)=(q_9,\P,0)\\
&\sigma(q_7,empty) = (q_7,empty,L)\\
&\sigma(q_7, \overrightarrow X) = (q_8, \overrightarrow X, L)\\
&\sigma(q_8, empty) = (q6,empty,R)\\
\end{aligned}
$$

### 性质

- LBA可接受上下文有关语言（上下文有关语言无$\epsilon$）；
- $L(LBA)-\epsilon=CSL$

## CSL：上下文有关语言

### 性质

- CSL类对􏱉􏰛􏱩􏰴􏱘􏱓􏱢􏰬􏱟􏱎􏰵􏱖􏱢􏱊􏰜􏱉􏰛􏱩􏰴􏱘􏱓􏱢􏰬􏱟􏱎􏰵􏱖􏱢􏱊􏰜并、链接和**正**闭包运算($\{x\}^+$)是封闭的；
- 每个CSL都是递归集；
- 存在非CSL的递归集；

### 完全图灵机(TM)

- 对于所有情况都能停机的图灵机（没有loop）；