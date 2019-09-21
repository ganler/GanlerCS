# RGB2GRAY OPTIMIZATION

## Environment

- Intel(R) Core(TM) i7-4770HQ CPU @ 2.20GHz
- Compiler: clang version 7.0.1
- OS: OS X



## Implementation

### OpenCV

```c++
time_analysis([&](){// Function to measure clk cycles.
  /* ===key=== */
  cv::cvtColor(x, foo, cv::COLOR_BGR2GRAY);
}, t);
```

（640x）循环10000次，平均每次使用298,518个时钟周期。

对10000x10000的照片，循环10次，平均每次使用89,319,591个时钟周期。

### Naive

```c++
time_analysis([&](){// 遍历更新
		if (src.isContinuous() && dst.isContinuous())
		{
				cols = cols * rows;
				rows = 1;
		}
		for (int row = 0; row < rows; ++row)
		{
        const uchar* src_ptr = src.ptr<uchar>(row);
        uchar* dst_ptr = dst.ptr<uchar>(row);
        for (int col = 0; col < cols; ++col, src_ptr += 3)
            dst_ptr[col] = static_cast<uint8_t>(
            std::round(0.114 *src_ptr[0] + 0.587 * src_ptr[1] + 0.299 * src_ptr[2]));
		}
}, t);
```

只使用`-O2`，循环10000次，平均每次使用3,221,122个时钟周期。

使用`-O3 -march=native`来激活AVX向量化，平均每次使用3,266,843个时钟周期。没有性能提升，说明并未激活向量化。十有八九是OpenCV动态链接库的原因，于是我另外写了一个未使用第三方库的rgb2gray，相同图像尺度下循环10000次测得平均每次使用2,549,572时钟周期（汇编后也发现有ymm寄存器），而相同程序的程序用`-O2`(汇编后只有xmm寄存器)，得到的结果是2,678,240时钟周期。说明这个自动向量化的结果有点弱鸡…

### float2int

整数运算比浮点数运算快，由于我们最后需要的结果还是一个整数，那么我们可以在一开始的时候就不适用浮点数，而使用整数。

```c++
constexpr uint16_t SCALE_SZ = 8;
constexpr uint16_t r_ = 0.299 * (1 << SCALE_SZ);
constexpr uint16_t g_ = 0.587 * (1 << SCALE_SZ);
constexpr uint16_t b_ = 0.114 * (1 << SCALE_SZ);
cv::Mat dst;
dst.create(src.size(), CV_8UC1);
int rows = src.rows, cols = src.cols;

time_analysis([&](){ 
  if (src.isContinuous() && dst.isContinuous())
  {
    cols = rows * cols;
    rows = 1;
  }
  for (int row = 0; row < rows; ++row)
  {
    const uchar* src_ptr = src.ptr<uchar>(row);
    uchar* dst_ptr = dst.ptr<uchar>(row);
    for (int col = 0; col < cols; ++col)
    {
      dst_ptr[col] = static_cast<uint16_t>(
        b_ * src_ptr[0] + g_ * src_ptr[1] + r_ * src_ptr[2] + (1 << (SCALE_SZ-1))
      ) >> SCALE_SZ;
      src_ptr += 3;
    }
  }
}, t);
```

10000loop取average 429,378，great，相对naive提升了差不多10倍了。























![img](https://s2.ax1x.com/2019/05/14/EI1FeK.png)

![img](https://s2.ax1x.com/2019/05/14/EI1PL6.png)