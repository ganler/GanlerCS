## linux下的random_device:
* <linux下获取真随机数的途径>：linux内核将机器的物理硬件产生的噪声等（例如发生的键盘敲击，中断次数，频率，温度等等）放入系统的熵池`/dev/random` 和`/dev/urandom` 中放的就是熵池中的部分随机数据，`/dev/random` 里只放入熵池中达到一定熵值的情况，例如达到比较的高的熵值的情况，那个时候数据最混乱，所以读取`/dev/random` 中的随机数据时会阻塞（熵池中随机性也就是整体熵值达到一定程度才会在这里面有随机数据），但他的随机性能比`/dev/urandom` 更好点，`/dev/urandom` 已经可以满足对加密要求非常高需要随机数的场景了。
* 使用c++11随机数生成器random_device

```c++
#include <iostream>
#include <random>       // a must ;

using namespace std;

int main{
// initialize.
    random_device rd;

    for(int n=0; n<200 ;n++){
        cout << rd() << endl;
    }
}
// can also be used in VS2013;
```
官方代码示例：
```c++
// random_device_engine.cpp 
// cl.exe /W4 /nologo /EHsc /MTd 
#include <random> 
#include <iostream> 
using namespace std;

int main() 
{ 
    random_device gen; 
 
    cout << "entropy == " << gen.entropy() << endl; 
    cout << "min == " << gen.min() << endl; 
    cout << "max == " << gen.max() << endl; 
 
    cout << "a random value == " << gen() << endl; 
    cout << "a random value == " << gen() << endl; 
    cout << "a random value == " << gen() << endl; 
}
```

## random库的常见用法
* **组件**
  * 随机数引擎（产生特定类型的随机数）
  * 随机数分布引擎（产生特定范围，概率的随机数）
* **随机数常用类**：
  * `default_random_engine`       非负随机数（一种随机数引擎类，返回unsigned类型）
  * `uniform_int_distribution`    指定范围的非负整数
  * `uniform_real_distribution`   指定范围的随机实数
  * `bernoulli_distributuin`      伯努利分布（指定概率的随机分布）
* **BASIC**

```c++
#include <iostream>
#include <random>     // a must
using namespace std;

int main(){
    default_random_engine e;      // engine -> initialize e;
    // set its range & type(in "<>");
    // 但如果不是整数类型，而是小数的话，应该把"uniform_int_distribution"的"int"->"real"。
    uniform_int_distribution<int> u(-10, 10);
    // cout
    for(int i=0; i<10; i++) cout << u(e) << endl;
}
```
`bernoulli_distribution`是一个分布类，而不是一个模板类。其构造函数只有一个参数，表示返回true的概率，默认0.5。
```c++
...
int main(){
    double posi = 0.6;
    // 必须是double或者float类型，不然用int得到的都是0；
    default_random_engine e;
    bernoulli_distribution u(posi);
    for(int i = 0; i < 5; i++)cout << u(e) << endl;
}
...
```



**最常用的方法**

```c++
#include<iostream>
#include<random>

using namespace std;

int main(){
    random_device rd;
    default_random_engine e{rd()};
    uniform_int_distribution<int> u{0, 100};// {}和()都可以
    for (int i=0; i < 10; ++i)
        cout << u(e) << endl;
}
// about why: 
// "the performance of many implementations of random_device degrades sharply once the entropy pool is exhausted. For practical use random_device is generally only used to seed a PRNG such as mt19937"
```

