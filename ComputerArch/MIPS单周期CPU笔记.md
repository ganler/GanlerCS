# MIPS单周期CPU笔记

[TOC]

## 单周期组合逻辑控制器

### 指令汇总

- 单周期；
- 32位（4字节）指令；
- 寄存器的地址用5位($2^5=32$)表示，内存的地址用32位表示，但其中寄存器的值都是32位的；
- 所有指令的前6位是指令码；
- R类型后五位是功能码；
- 对ALU来说，所有数值都要<u>扩展到32位</u>才能计算；
- PC指的是程序计数器，存放<u>下一条指令</u>所在单元的地址的地方。

![](https://i.loli.net/2019/04/14/5cb337ec4a93a.png)

![](https://i.loli.net/2019/04/14/5cb3317116927.png)



#### `Addu rd, rs, rt`

![](https://i.loli.net/2019/04/14/5cb331becbd5a.png)

- rd 目的地址；
- 5~0代表的是功能码；

```c++
REG[rd] = REG[rs] + REG[rt];
PC += 4;
```



#### `Subu rd, rs, rt`

无符号减法，同上，功能码为100011；

```c++
REG[rd] = REG[rs] - REG[rt];
PC += 4;
```



#### `Ori rt, rs, immediate`

立即或运算，rt <- rs or imm16

![](https://i.loli.net/2019/04/14/5cb3367ba8db7.png)

```c++
REG[rt] = REG[rs] ^ [16位符号扩展 imm16];
PC += 4;
```



#### `Sll rd, rt, sa`

rt对应的寄存器左移sa位(sa位长5位)后赋值给rs对应的寄存器。

![](https://i.loli.net/2019/04/14/5cb3375edc3a5.png)

```c++
REG[rd] = REG[rt] << sa;
PC += 4;
```



#### `Beq rs, rt, offset`

分支指令：

![](https://i.loli.net/2019/04/14/5cb339cbb2ad3.png)

```c++
if(REG[rs] == REG[rt])
  PC += ([14位符号拓展 offset 00]->共32位);
else
  PC += 4;
```



#### `J target`

无条件跳转；

![](https://i.loli.net/2019/04/14/5cb339d8cecfe.png)

```c++
PC += [(PC+4)[31:28] instr_index 00]
```



#### `Lw rt, offset(base)`

读取存储器

![](https://i.loli.net/2019/04/14/5cb33bb97801d.png)

```c++
REG[rt] = MEM[REG[base] + [16位符号拓展 offset]];
PC += 4
```



#### `Sw rt, offset(base)`

将数据存到存储器中；

```c++
MEM[REG[base] + [16拓展位 offset]] = REG[rt]
PC += 4
// 上面反过来
```

### 基本部件和数据流图

![](https://i.loli.net/2019/04/15/5cb4876b75a9a.png)

- 指令存储器(`IMEM`)
- 计算单元`ALU`
- 程序计数器(`PC`)
- `NPC`(将PC值＋4)
- `DMEM`主存
- 寄存器组(`RegFile`)，一般是SRAM实现的。

> - `Rd` ：存储结果(输出寄存器)
> - `Rdc` ：
> - `Rsc`
> - `Rtc`
> - `Rs`：输入寄存器
> - `Rt`：输入寄存器

- 18位立即数位拓展器`Ex18`
- 16位立即数位拓展器`Ex16`
- 位移拓展器`Ex5`
- jump拓展器；
- `ADD`：计算分支指令的位置；
- `MUX1` 从0（Jump）， 1（直接+4或者Beq）中选择指令；
- `MUX2` 在 ALU的结果 和 内存取值 中选择；
- `MUX3` 在 Rs 和 位移拓展器`Ex5` 中选择；
- `MUX4` 在 Rt 和 `Ex16` 中选择；
- `MUX5` 在直接+4和Beq中选择。