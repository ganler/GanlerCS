# GCC Optimization

[Link](https://wiki.gentoo.org/wiki/GCC_optimization#cite_note-1)

## My practice

#### Faster code

> Some of them are aggressive codes that are not suitable for systemwise building.

`-Wall -march=native -Ofast -frename-registers -funroll-loops -fwhole-program -flto`

> `-funroll-all-loops`
>
> `-fforce-mem`
>
> `-fforce-addr`

For undangerous flags:

`-Wall -Ofast -march=native -pipe`

#### Faster compile time

`-pipe`

#### Smaller code size

`-fomit-frame-pointer`

## Flags

- 不要重复声明`CXXFLAGS`；

```bash
CFLAGS="-march=native -O2 -pipe"
CXXFLAGS="${CFLAGS}"
```

- the global CFLAGS configured in `/etc/portage/make.conf` will be applied to every package on the system so administrators typically only set general, widely-applicable options. 

### `-march`

> **to create code tailor-made to the system**

`-march`接的东西是ISA(指令集)。

一般加了`-march-native -O2`之后，很多操作可能本来可以使用AVX YMM但最后还是使用了SSE XMM指令。所以应该加上`-ftree-vectorize`，`-O3`或`-Ofast`等。

- `-ftree-vectorize`( `-O3` and `-Ofast`中会默认开启)会尝试向量化。

> The reason it isn't enabled at `-O2` is that <u>it doesn't always improve code, it can make code slower as well, and usually makes the code larger;</u> it really depends on the **loop** etc.

- 查看指令:

```bash
# mac:
sysctl -a | grep machdep.cpu.features

# linux
cat /proc/cpuinfo | grep flags | uniq
```

```bash
# -c : 不要做任何linking
# -Q : 展示各个option是enable还是disable
gcc -c -Q -march=native --help=target

# show the compiler directives for building the header file
# -### 不在屏幕上打印
gcc -### -march=native /usr/include/stdlib.h
```

> 使用march=native/mtune=native的时候不要使用distcc（快速、免费的分布式 C/C++ 编译方法）。分布式编译意味着有不同的电脑，使用自动=native可能会导致结果不符合预期。
> -march不保证和之前版本的CPU兼容，-mtune产生的代码会更加通用（尽管他会为了特定CPU调整代码，但他不考虑指令集和ABI的兼容性）。
> Do not use -mcpu on x86 or x86-64 systems, as it is deprecated for those arches.
> Only non-x86/x86-64 CPUs (such as SPARC, Alpha, and PowerPC) may require -mtune or -mcpu instead of -march.(GCC's behavior and flag naming is not consistent across architectures)

### -O

#### Warning
Using `-O3` or `-Ofast` may cause some packages to break during the compilation.

#### options
 -O0, -O1, -O2, -O3, -Os, -Og, and -Ofast

- `-O1`: Basic optimization.(Faster and smaller code fastly).
- `-O2`: The recommended level of optimization unless the system has special needs. SSE or AVX may be be utilized at this level but no YMM registers will be used unless -ftree-vectorize is also enabled.
- -Ofast := -O3, -ffast-math, -fno-protect-parens, -fstack-arrays.

For speed, I use:
```bash
-Wall -march=native -Ofast -frename-registers -funroll-loops -fwhole-program -flto
```

#### -O的flag最多是3

```c++
case OPT_LEVELS_3_PLUS:
      enabled = (level >= 3);
      break;
case OPT_LEVELS_3_PLUS_AND_SIZE:
      enabled = (level >= 3 || size);
      break;
```

### -pipe

This flag has no effect on the generated code, but it makes the compilation process faster. It tells the compiler to use pipes instead of temporary files during the different stages of compilation, which uses more memory. On systems with low memory, GCC might get killed. In those cases do not use this flag.

### -fomit-frame-pointer
> Reduce generated code size.
> 建议显示声明；
> 使用这个后可能无法debug。

> Do *not* combine `-fomit-frame-pointer` with the similar flag `-momit-leaf-frame-pointer`. Using the latter flag is discouraged, as `-fomit-frame-pointer` already does the job properly. Furthermore, `-momit-leaf-frame-pointer` has been shown to negatively impact code performance.

### -msse, -msse2, -msse3, -mmmx, -m3dnow

> These enables the Streaming SIMD Extension.
>
> Note that **pni** is just a different name for SSE3.