# Lecture 10

#### Memory aliasing 内存别名使用

不同指针指向同一地址；编译器在进行安全优化的时候必须考虑内存别名使用，从而会减少一些优化；

```c++
void twiddle(int* lhs, int* rhs)
{
  	*lhs += *rhs; // 3 次内存引用
	  *lhs += *rhs; // 3 次
}

// 并不会优化成下面这个函数，因为当lhs和rhs指向同一地址的时候，上面的代码*lhs成为了原来的4倍
void twiddle2(int* lhs, int* rhs)
{
  	*lhs += 2* *rhs; // 3 次内存引用(读rhs，读lhs，写lhs)
}
```

#### 循环展开

#### 减少内存引用

对指针/引用进行操作时，要假想他每次都要从地址中load新值。

```c++
void add_sum(const vector<double>& v, double& ret)
{
    for(const auto x : v)
        ret += x;
}

void add_sum_opt(const vector<double>& v, double& ret)
{// 10%左右的提升
    double tmp = 0;
    for(const auto x : v)
        tmp += x;
    ret += tmp;
}
```

```assembly
# Bad
L3:
	vaddsd	(%rax), %xmm0, %xmm0
	addq	$8, %rax
	vmovsd	%xmm0, (%rsi)
	cmpq	%rdx, %rax
	jne	L3
L5:
	ret
	
# Good (循环中的代码更少)
L9:
	vaddsd	(%rax), %xmm0, %xmm0
	addq	$8, %rax
	cmpq	%rdx, %rax
	jne	L9
L8:
	vaddsd	(%rsi), %xmm0, %xmm0
	vmovsd	%xmm0, (%rsi)
	ret
```

