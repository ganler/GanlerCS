# Lecture 5

## Intel x86 Processors and history

![](https://i.loli.net/2019/04/11/5caf2af9ae5e2.png)

![](https://i.loli.net/2019/04/11/5caf2ae12bef7.png)

**<u>2018 State of art: Coffee</u>**

#### About Advanced Micro Devices(AMD)

- Followed just behind Intel;
- A little bit slower, a lot cheaper;

#### All but low-end x86 processors support x86-64

▪  But, lots of code still runs in 32-bit mode

#### Example ISAs:

 ▪ Intel: x86, IA32, Itanium, x86-64 

▪ ARM: Used in almost all mobile phones 

▪ RISC V: New open-source ISA 

#### Memory

- Code
- Data(Static data, literal)
- Stack
- (Heap)

#### Register Files

![](https://i.loli.net/2019/04/11/5caf2acc6977e.png)

![](https://i.loli.net/2019/04/25/5cc1aa868e9c0.png)



## Intructions

#### Type

- Load/Store data from registers to memory;
- Arithmetic functions;
- Transfer control
  - Jump
  - Conditional branches
  - Indirect branches

#### Addressing modes

- Simple:

```assembly
movq (%rdi), %rax
# 将mem[寄存器rdi的内容]传到寄存器rax中
```

- Complete

![](https://i.loli.net/2019/04/11/5caf2a8b8add1.png)

#### `movq`

`q` means 8 bytes(64 bits) consecutive bytes of memory at address given by register.

![](https://i.loli.net/2019/04/11/5caf2ab8d5d71.png)

> Memory2memory transfer cannot be done within a single instruction.

![](https://i.loli.net/2019/04/11/5caf2a5382ebc.png)

> Intel docs use mov Dest, Source.

#### `leaq` Src, Dst

Computing addresses without a memory reference.

Computing arithmetic expression of form `x+k*y`

```c++
long m12(long x)
{
  return x * 12;
}
/*

leaq (%rdi, %rdi, 2), %rax # t = x+2*x
salq $2, %rax							 # return t << 2

*/
```

#### Arithmetic Operations

![](https://i.loli.net/2019/04/11/5caf2a2a98154.png)

#### Translation example

```c++
long arith(long x, long y, long z) {
  long t1 = x + y;
  long t2 = z + t1;
  long t3 = x + 4;
  long t4 = y * 48;
  long t5 = t3 + t4;
  long rval = t2 * t5;
  return rval;
}
```

```assembly
arith:
	leaq (%rdi, %rsi), %rax    # t1
	addq %rdx, %rax						 # t2
	leaq (%rsi, $rsi, 2), %rdx # t4 = y * 3
	salq $4, %rdx							 # t4 = t4 << 4(*16)
	leaq 4(%rdi, %rdx), %rcx	 # t5 = (t4 + t3 = (4+x))
	imulq %rcx %rax						 # rval
	ret
```

#### C to object Code

`gcc -Og p1.c p2.c -o p`

- `-Og` means basic optimizations (New to recent version of gcc)

##### Procedures:

`text(.c)` -> `text(.asm/.s)` -> `binary(.obj)` -> `binary(.a(static libraries))`

> An assembler translates `.s` to `.o`.

##### show assembly code

`gcc -Og -S sum.c`

> Warning: Will get very different results on non-Shark
> machines (Andrew Linux, Mac OS-X, ...) due to different
> versions of gcc and different compiler settings.

#### Assembly Characteristics: Data Type

- `Integer of 1, 2, 4 or 8 bytes`
- `Floating point data of 4, 8 or 10 bytes`
- SIMD vector data type of 8, 16, 32 or 64 bytes.
- Code: Byte sequences
- No aggregate types.

#### Disassembler

For `.out` or `.o` files.

(Create `.o` files: `gcc -c sum.c`)

`objdump -d sum.o`

`gdb sum.o`

`disassemble sum.o`

# Lecture 6

![](https://i.loli.net/2019/04/11/5caf2a0d205a4.png)

![](https://i.loli.net/2019/04/11/5caf29f9da31e.png)

> `rsp`: r说明是x86-64的架构（64位寄存器是以r开头的）；`sp`指的是运行时栈栈顶寄存器的地址（也代表我们能使用的栈的大小），（对于所有x86的架构）向下递增。rbp是指基址寄存器(x86的**帧指针**，用来指向当前栈的基地址)。一般程序一开始就是：

```asm
push rbp				# 把基地址保存在栈中：背后所干的事情就是rsp -= 8 byte，栈增长8字节
								# 拆解后是：sub rsp, 8; mov [ss:rsp], rbp;
								# (后一句是把rbp存到[ss:rsp]中)
mov  rbp, rsp		# 将当前栈顶寄存器的内容（当前基地址）放到基址寄存器中 rbp << rsp;
```

### Condition codes

#### Single bit registers for flags

- **CF**: Carry flag(for **unsigned**)
- **SF**: Sign flag(for signed)
- **ZF**: Zero flag;
- **OF**: Overflow flag(for **signed**)

#### Implicitly set of arithmetic operations

> 我们知道整数转换只需要"重新解释"，而重新解释的依据就是这些single bit register中的flag。而在进行各种指令的时候，flags也会随之更新。

##### Example 1

`addq Scr, Dest` -> `t = a + b`

- CF set if <u>**unsigned** overflow</u>(carry in MSB)

- ZF set if `t == 0`

- SF set if `t < 0`(The MSB of t is `1`)

- OF set if <u>two's-complement (signed)</u> overflow

  > `(a > 0 && b > 0 && t < 0) || (a < 0 && b < 0 && t > 0)`
  >
  > Or `MSB[a] == MSB[b] && MSB[a] != MSB[t]`
  >
  > (**同号相加的结果是异号**)

##### Example 2

`cmpq Src2, Src1`

>  `cmpq b, a` -> `((b-a) = 0) ?`

- CF: unsigned overflow(出现了越界行为：越过00…00和FF…FF)
- ZF: `a == b`
- SF: `(a - b) < 0` (作为有符号数)
- OF: a，b异号然后相减溢出

##### Test

`testq Src2, Src1`

`a&q`

- ZF: `a&q == 0`
- SF: `a&q < 0`

检测rax是否是0：`testq %rax, %rax`

##### Set

![](https://i.loli.net/2019/04/11/5caf30feabdcf.png)

![](https://i.loli.net/2019/04/11/5caf33282fdf9.png)

> `movzbl`是对`%al`进行0拓展。
>
> (对无符号数是直接做0拓展，有符号数是拓展第一位)

#### Jumping

> jump是根据上述flags的结果进行跳转的。

![](https://i.loli.net/2019/04/15/5cb44873811e8.png)

```asm
absdiff:
	cmpq  %rsi, %rdi  # y : x (与函数顺序相反)
	jle   .L4 			 	# if x <= y go to .L4
	movq  %rdi, %rax  # REG[rax] = x
	subq  %rsi, %rax	# REG[rax] -= y
	ret
.L4:
	movq  %rsi, %rax  # REG[rax] = y
	subq  %rdi, %rax
	ret
# rax是用来装return value的。
```

#### Conditional Move

- 分支语句（比如conditional moves）可能会破坏`pipline`，对性能不太友好。
- Conditional move 不需要控制分支转换

就是对于分支结果进行提前计算，最后赋值给对的。

上面的可以改写为：

```asm
absdiff:
	movq   %rdi, %rax  # REG[rax] = x
	subq   %rsi, %rax  # REG[rax] -= y
	movq   %rsi, %rdx
	subq   %rdi, %rdx
	cmpq   %rsi, %rdi  # 计算比较结果
	cmovle %rdx, %rax  # if <= , result(rax) = rdx
	ret
```

> 在c++中`int a =  (xxx) ? ++a : ++a;`最后a只会被+1，而不是+2。但在汇编中不能这样。Conditional Move的本质是提前计算结果，最后一步搞定选择哪个结果，而不是跳转。但提前计算结果的代价是不能同时改变同一个变量，否则变量会被改变2次。

#### Loop

```c++
// do while

loop:
	// sth.
	if(TEST) goto loop;
```

```c++
// while

goto test;
loop:
	// sth.
test:
	if(x) goto loop;
```

#### Switch

![](https://i.loli.net/2019/04/15/5cb46b7d1bb02.png)

> 这就解释了为什么switch里面的东西只能是一种类型的，因为编译的时候，switch的可选项都会编到一个跳转表中(Jump Table)，也解释了为什么要break。`goto *JTab[x]`。

![](https://i.loli.net/2019/04/15/5cb46c8d8beb2.png)

> `jmp *.L4(,%rdi,8)`意味着跳转到`*Jtab[x]`上。
>
> 跳转表的开始是`.L4`。表接受的x值为0~6，不然就是default(`.L8`)，所以上段程序会先比较其是否在范围内：`cmpq $6, %rdi`，若不在就跳转到`default(.L8)`。

#### 寻找跳转表

![](https://i.loli.net/2019/04/15/5cb46eddd15f3.png)

