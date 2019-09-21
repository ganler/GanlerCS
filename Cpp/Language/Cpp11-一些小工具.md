# Cpp11-一些小工具

## 时间库`chrono`

#### 让线程休眠？

```c++
#include<chrono>
#include<thread>

using namespace std;

int main()
{
    this_thread::sleep_for(chrono::seconds(4)); // std::chrono...
    this_thread::sleep_for(chrono::milliseconds(100));
}
```

> `chrono::seconds` & `chrono::milliseconds` 都是模板类`chrono::duration`的`typedef`。

#### 每一个`std::chrono::duration`对象的意义？

```c++
// 每个duration对象代表着一段时间
int main()
{
    std::chrono::seconds s_3{3}; // 3 secs.
    auto s_6 = s_3*2;
    std::cout << s_3.count() << std::endl;
    // 查看s_3有多少个秒。如果s_3是milliseconds的对象，那么就是显示有多少个毫秒。
}
```

> 注意`milli...`是毫秒，`micro...`是微秒。（想想**微**软`[Micro]soft`...）

#### 如何将不同周期的duration对象进行转换？

```c++
// std::chrono::duration_cast<to>(from);

auto x = std::chrono::duration_cast<std::chrono::minutes>(std::chrono::seconds(3));
```

#### `chrono`中的clocks

- `chrono::system_clock` 依赖系统时间的时钟
- `chrono::steady_clock` 保证先后调用的时间值不会递减
- `chrono::high_resolution_clock` 高精度时钟，其实只是`steady_clock`或者`system_clock` 的别名。

#### 获取时间段

```c++
#include<iostream>
#include<chrono>
#include<thread>

using namespace std;

int main()
{
    auto beg = chrono::high_resolution_clock::now();

    for(int i=0; i<100; i++)
    {
        this_thread::sleep_for(chrono::milliseconds(1));
    }
    
    auto end = chrono::high_resolution_clock::now();
    cout << chrono::duration<double, milli>(end-beg).count() << " ms" << endl;
}
```

#### 计时器`timer`

我们希望他用起来像这样：

```c++
int main()
{
    Timer t;
    cout << "hello world" << endl;
    cout << t.time_out() << endl;
}
```



```c++
// 设计一个timer
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;

class Timer
{
public:
    Timer() : clk_beg(high_resolution_clock::now()) {}
    void reset(){clk_beg = high_resolution_clock::now(); }

    template <typename Format = milliseconds>
    uint64_t time_out() const
    {
        return duration_cast<Format>(high_resolution_clock::now() - clk_beg).count();
    }

    uint64_t time_out_microseconds() const{ return time_out<microseconds>(); }
    uint64_t time_out_seconds() const{ return time_out<seconds>(); }
    uint64_t time_out_minutes() const{ return time_out<minutes>(); }
    uint64_t time_out_hours() const{ return time_out<hours>(); }

private:
    time_point<high_resolution_clock> clk_beg;
};

/*
用起来是这样的：

Timer t;
// code
cout << t.time_out() << " ms" << endl;

*/
```

## 数值类型和字符串互转

#### 转string？

> c++11提供了默认的`to_string`方法。
>
> （包括`to_wstring`等）

#### 其他

[My zhihu](https://zhuanlan.zhihu.com/p/37303144)

## Raw string

#### 如何告别难受的制表符，直接使用原始字符串？

```c++
#include<iostream>
using namespace std;
int main()
{
    // 一样的输出 R"yyy(xxx)yyy"， xxx就是rawstring，就连制表符和换行符都能store。
    // yyy会被忽略，只有“()”内的字符才会被理会。但前后的yyy必须一样，比如：R"test(Hello World\n)test".
    cout << "\\n\\n" << endl;
    cout << R"(\n\n)" << endl;

    cout << "大家好\n我不是raw string" << endl;
    cout << R"(大家好
我是raw string)" << endl;
}
```

## 派生类使用基类的函数

```c++
class Derived : public Base
{
	using Base::Base; // 引入基类构造函数
	using Base::Fun;  // 引入基类的Fun函数
}
```

## 算法库里面的很舒服的东西

```c++
// 判断vec是不是所有元素都是正数
bool ans = all_of(vec.begin(), vec.end(), [](int x){return x > 0; });
// 同理：any_of 任意一项满足返回true
// none_of 所有都不满足返回true

// 将vec0中所有大于0的数全都copy到vec1中
copy_if(vec0.begin(), vec0.end(), vec1.begin(), [](int i){ return i > 0; });

// iota 生成有序序列
// 生成一个1~n的数组
vector<int> a(n);
iota(a.begin(), a.end(), 1); // 从1开始

// 同时获取最大值和最小值(返回一个含2个迭代器的pair)
auto iter_pair = minmax_element(v.begin(), v.end());
cout << "min: " << *iter_pair.first << ' ' << "max: " << *iter_pair.second;

// is_sorted是否有序
bool is_sort = is_sort(v.begin(), v.end());

// is_sorted_until返回前面已经排好序的部分序列
// v = {1, 2, 3, 1}
auto pos = is_sorted_until(v.begin(), v.end());
for(auto it = v.begin(); it < pos; ++it)
    cout << *it << ' ';
// 1 2 3
```
