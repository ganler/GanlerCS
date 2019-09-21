# CSAPP

## Lecture1: Bits, Bytes and Integers

- Pointers in x86-64 framework has a size of **8 bytes.**
- In C/C++ shift amount < 0 or $\ge$ word size is "undefined behavior".

> 在补码中最高位是1的意思是要$-2^{w-1}$

$$
Unsigned:\\
B2U(X)=\sum_{i=0}^{w-1}x_i*2^i\in[0,2^w-1]\\
Two's\ complement\\
B2T(X)=\sum_{i=0}^{w-2}x_i*2^i-x_{w-1}*2^{w-1}\in [2^{w-1},2^{w-1}-1]
$$

> 补码转换：
>
> - 负数：绝对值取反加一(十进制转二进制)

- C does not mandate using two’s complement，but most machines do.

> 这里的意思是C不规定2进制一定要是补码，但是很多架构都默认用补码表示。

- 补码设计的好处：直接转换(比如signed转unsigned，只要改变解释的意义即可)

  > 数值上给人的感觉是进行一个映射，但实际上在计算机中并没有转换进制位，只是重新解释了一下。

  > 所以在C中（对于int的）casting不转换位数，只转换"解释意义"。

**Casting Surprises** 

- **Expression Evaluation** 
  - If there is a mix of unsigned and signed in single expression, ***signed values implicitly cast to unsigned*** 

#### 符号位扩展

补码：即把符号位复制粘贴到拓展位。

![image-20190406114726326](/Users/ganler-mac/Desktop/local store/md_imgstore/image-20190406114726326.png)

> 在正数的情况下理所当然；
>
> 在负数的情况下考虑取反加一的规则，就能理解了。

![image-20190406115107967](/Users/ganler-mac/Desktop/local store/md_imgstore/image-20190406115107967.png)

> 这样来说，把小size的整数转到大size的整数有个额外操作：将之前的位和符号位一致。

#### 截断Truncation

<u>直接对应截断</u>即可：

比如w+k转w位，只要最后w位即可。

![image-20190406115423065](/Users/ganler-mac/Desktop/local store/md_imgstore/image-20190406115423065.png)

#### 加法

![image-20190406140837369](/Users/ganler-mac/Desktop/local store/md_imgstore/image-20190406140837369.png)

#### 乘法

![image-20190406140956110](/Users/ganler-mac/Desktop/local store/md_imgstore/image-20190406140956110.png)

![image-20190406141810086](/Users/ganler-mac/Library/Application Support/typora-user-images/image-20190406141810086.png)

#### Shift

如果是乘以$2^w$则没什么好说的，有符号和无符号是一样的；

如果是除以$2^w$，则无符号是一般的做法，而有符号：

> 除的操作是向右shift然后在符号位前进行位拓展，和之前说的一样，有符号数的位拓展就是把符号位copy过去。
>
> 因为在补码的环境里面，最高位的意义是$-2^{w-1}$ 。

#### 除法

![image-20190406143039965](/Users/ganler-mac/Desktop/local store/md_imgstore/image-20190406143039965.png)

> 计算机支持向下取整，而当我们要计算向上取整的时候，就需要进行一些转化。

#### 取负号Negation

$-x=\sim x+1$

取反加一。(负数补码算绝对值也是这样的)

如果是unsigned变负数的话即：$-x_w=2^w-x_w$

当补码最小值(100…00)取反加一的时候，这种操作是越界的，并且这样之后的结果还是自己本身。

`size_t`：位数大于等于地址线宽度

> `size_t`和`unsigned int`有所不同,**size_t的取值range是目标平台下最大可能的数组尺寸**,一些平台下size_t的范围小于int的正数范围,又或者大于`unsigned int`.
>
> 最典型的,在x64下,int还是4,但size_t是8.这意味着你在x64下最大可能开辟的数组尺寸是2^64. 如果你使用int或者unsigned int,那么在x64下如果你的代码中全部使用uint作为数组的尺寸标记, 那么你就会失去控制$2^{32}$尺寸以上的数组的机会.虽然现在在x64上开辟一个大于$2^{32}$大小的连续数组依然是个不大可能的事情,但是..........
>
> “640K内存对于任何人来说都足够了”----比尔盖茨
>
> > [知乎  KE meng](https://www.zhihu.com/question/24773728/answer/28920149)

#### Representations in memory, pointers, strings

- system provides <u>private address spaces</u> to each “process”

![image-20190406145235404](/Users/ganler-mac/Desktop/local store/md_imgstore/image-20190406145235404.png)



