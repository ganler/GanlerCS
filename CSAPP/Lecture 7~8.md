# Lecture 7

## X86-64 Stack

- Grows toward lower addresses;
- Stack pointer: `%rsp`(Lowest stack address)

- Current stack frame: `%rbp`
- When coming into a new frame, we push `%rbp` to the stack, and copy `%rsp`(current stack frame) to `%rbp`;

```assembly
push %rbp;
move %rbp %rsp;
```

### Passing control

#### Call label

- Push return address on stack;
- Jump to label;

#### Return address

- Address right after the call instruction.

#### Procedure return

- Pop address from stack;
- Jump to address;

> 在调用call label的时候，会在栈上push一个return value的位置。然后再去开一个新的stack frame(如果有必要的话)。在新的frame里面将返回值存在`%rax`中，最后从`%rax`中取值，将结果放到原来的return value处。

### Passing data

#### Passing parameters via registers and stack

![](https://i.loli.net/2019/04/24/5cbfc4ecd8175.png)



### Managing local data

#### Things need to be stored

- Parameters
- Local variable
- Return pointer

![](https://i.loli.net/2019/04/24/5cbfc86e9bb11.png)



### X86-64/Linux Stack Frame

![](https://i.loli.net/2019/04/24/5cbfc89b04dcb.png)



### X86-64/Linux Registers

![](https://i.loli.net/2019/04/24/5cbfc9e629f22.png)

![](https://i.loli.net/2019/04/24/5cbfca0c01d01.png)

> 在调用函数的时候，寄存器可以最多帮忙存6个参数；(调用函数也不一定真的会增加stack frame)



## 拓展阅读：地址空间布局随机化

> Windows Vista、Linux 2.6.12、Mac OS X 10.7、iOS 4.3以及Android 4.0均从此版本开始支持ASLR.

为了防止蠕虫攻击，在对程序建立栈的时候，在当前位置前随机分配一段内存空间（只是程序前加了个bias，其中的各数据区的bias依旧是固定的），以使得栈首位置难以预测。

> 现代linux内核每次加载程序时会将程序加载到内存的随机位置，以防止缓冲区益处的代码注入攻击。

> http://www.arkteam.net/?p=2728

```c++
#include <stdio.h>
#include <stdlib.h>

int z;
int i=1;
const int c=1;

int main(int argc, char *argv[])
{
  printf("Stack:\t%p\n", (void*)&argc);
  printf("heap:\t%p\n", (void*) malloc(1));
  printf("code:\t%p\n", (void*) main);
  printf("data:\t%p (zero)\n", (void*) &z);
  printf("data:\t%p (initialized)\n", (void*) &i);
  printf("data:\t%p (constant)\n", (void*) &c);
  return 0;
}
```



## Lecture 8: Data

![](https://i.loli.net/2019/04/25/5cc1b3c95b0f7.png)

注意`int (*A3)[3]`，因为`*`的优先级很低，所以先被认为是`int[3]`，那么`A3`就成为了指向`int[3]`的指针了。



### 优化struct的内存对齐

struct的内存对齐默认和其最大的数据成员对齐：

```c++
// 都是4字节内存对齐

struct bad
{// 12字节
	char a[3]; // 3
	int b;		 // 4
	char c;    // 1
};

struct good
{// 8字节
  int b;     // 4
	char a[3]; // 3
	char c;    // 1
};
```

> 建议：对`struct`中的数据成员进行大小排序排列；

### 浮点数

#### 发展历程

- MMX(64)：MM寄存器
- SSE(128)：XMM寄存器
- AVX(256)：YMM寄存器(`-mavx2`，一般加`-march=native`即可，会自动优化)
- AVX512

![](https://i.loli.net/2019/04/25/5cc1b7859b848.png)

> 注意在256的寄存器上，低128可以被用来使用SSE指令。

YMM之间或者YMM和内存之间可以进行**不做任何转换**的浮点数**传送**指令。

GCC只用标量传输操作**从内存传输数据到XMM**或者反过来。

`vmovaps`和`vmovapd`。

对于这些情况，序复制整个寄存器还只复制低位值既不会影响性能，也不会影响执行速度。

上面指令中的`“a”`代表对齐，如果不是16字节对齐的话，他们将引起异常。