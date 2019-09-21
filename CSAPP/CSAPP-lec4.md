# CSAPP Lec 4

## Fractional Binaray Numbers

$$
FBN=\sum_{k=-j}^i b_k\times 2^k
$$

#### 缺点

- 只能表示被$2^k$整除的数。
- 范围有限(不能表示很小或者很大的数字)

## IEEE Floating Point

### IEEE 754

- Hard to make fast in hardware.
- Nice standards for rounding, overflow, underflow.

#### Numeric form

$$
(-1)^s\times M\times 2^E
$$

- $M\in [1.0, 2.0)​$

#### Encoding

- MSB: s
- Exp: about E
- Frac: about M

> `float_32`: `1 8 23`
>
> `double_64`: `1 11 52`

![image-20190407185207140](/Users/ganler-mac/Library/Application Support/typora-user-images/image-20190407185207140.png)

> 除了单精度和双精度，也有半精度和四精度。

### Three kinds of floating point numbers

> The difference is on `exp`.

- Normalized: $exp \neq 0 \ or\ 11\cdots1$；
- Denormalized: $0$
- Special: $11\cdots1$

#### `Normalized`

##### <u>E = exp - Bias</u>

- `exp`: unsigned value of exp filed.
- Bias = $2^{k-1}-1$, where k is number of exponent bits(单精度的情况下是8).
  - Single: 127
  - Double: 1023

###### <u>M = 1.xxxx</u>

- Significand coded with implied leading 1: M = `1.xxxxx`
- `xxxx`是frac的bits$\in [00\cdots0, 11\cdots]\sim[0,1-\epsilon] $，so，$M\in[1, 2-\epsilon]$
- Get extra leading code for "free".(因为M的leadingcode默认是1)

$$
v = (-1)^s\ M\ 2^E\\
E=\exp - bias
$$

#### `denormalized`

When `exp = 0`；
$$
v= (-1)^s M 2^E\\
E=1-bias
$$
按照`Normalized`的框架来说，其实就是把`exp=0`看做`exp=1`。

$M=0.xxxx\in [0, 1-\epsilon]$

- 当frac全0且exp全0的时候，浮点数才为0. 整个浮点数也全0；(but +0 & -0)
- exp全0，而frac不全0的时候，此时v非常接近0.

#### `special values`

When `exp = 11...1`

- `exp = 11…1，frac = 00…0` 
  - 表示无穷；
  - overflow
  - 1.0/0 = $+\infin$
- `exp = 11…1，frac ≠ 00…0` 
  - `Not-a-number`(NaN)
  - E.g. `sqrt(-1)`

### float numbers in C

![image-20190407193031976](/Users/ganler-mac/Desktop/local store/md_imgstore/image-20190407193031976.png)



![image-20190408091640378](/Users/ganler-mac/Desktop/local store/md_imgstore/image-20190408091640378.png)

![image-20190408093407411](/Users/ganler-mac/Library/Application Support/typora-user-images/image-20190408093407411.png)

#### Special Properties of IEEE Encoding

- FP Zeros Same as Integer Zero.

- Can use Usigned Integer Comparison.

  - First sign bits.
  - -0 = 0.
  - NaNs.
    - Will be greater than any other values
  - Otherwise OK

  ## Rounding

  - Towards Zero
  - Rounding Down
  - Rounding Up
  - Nearst Even

  > 遵循四舍六入，当为5的时候，靠近最近的偶数。

#### 以二进制的方式思考Nearst even

- 偶数：末尾为0
- Half way(十进制的half-way是5，2进制的则是1)：rounding position右边的数字为$100_{\cdots2}$

那么就是rounding possision右边一位

- <u>0 直接截断</u>
- 1
  - 右边是1，左边+1
  - 右边是0
    - 左边是0，直接截断
    - 左边是1，左边+1

![image-20190408095948511](/Users/ganler-mac/Library/Application Support/typora-user-images/image-20190408095948511.png)

![image-20190408100107591](/Users/ganler-mac/Library/Application Support/typora-user-images/image-20190408100107591.png)

> 对于GRS是2个1以上的情况，就进位。

#### FP multiplication

##### Basic

- $s_1*s_2$
- $M_1*M_2$
- $E_1+E_2$

##### Fixing

- $if\ M \ge 2$，:$M>>1,++E$ (小数点对齐)
- $E$如果跑出了界限，直接overflow即可
- 对M使用round，来适应`frac`的精度

#### FP addition

全都将小数点调至1.xxxx并小数点对其，然后再相加。

![image-20190410081654350](/Users/ganler-mac/Desktop/local store/md_imgstore/image-20190410081654350.png)

![image-20190410082014056](/Users/ganler-mac/Desktop/local store/md_imgstore/image-20190410082014056.png)