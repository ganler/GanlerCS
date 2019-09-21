# pair_and_tuple

### `std::pair`

```c++
#include <iostream>
#include <utility>			// std::make_pair
#include <functional>		// std::ref

using namespace std;

int main()
{
    string names[] = {"a", "b"};
    int scores[] = {1, 2};

    // 直接pair(scores[0], names[0])亦可。
    auto pair1 = pair<int, string>(scores[0], names[0]);
    cout << ">>> pair1 : ("
         << pair1.first << ", " << pair1.second << ")\n";

    auto pair2 = make_pair(ref(scores[1]), names[1]);
    scores[1] += 2; // 2+2 = 4
    cout << ">>> pair2 : ("
         << pair2.first << ", " << pair2.second << ")\n";
}
/*
>>> pair1 : (1, a)
>>> pair2 : (4, b)
*/
```

> `std::pair`的声明。

```c++
namespace std
{
    template<class T1, class T2>
    struct pair
    {
        using first_type = T1;
        using second_type = T2;
				T1 first;
        T2 second;
        // default为默认构造函数，即不准编译器自动生成别的构造函数。
        pair(const pair&) = default;
        pair(pair&&) = default;
        constexpr pair();
        constexpr pair(const T1&, const T2&);
        // 下面的模板是将U, V类型的数据砖到T1, T2类型。
        template<class U, class V>constexpr pair(U&&, V&&);
        template<class U, class V>constexpr pair(const pair<U, V>&);
        template<class U, class V>constexpr pair(pair<U, V>&&);
        
        pair& operator=(const pair&);
        template<class U, class V>pair& operator=(const pair<U, V>&);
		pair& operator=(pair&&) noexcept();
        template<class U, class V>pair& operator(pair<U, V>&&);
        void swap(pair&) noexcept();
    }
}
```

### `std::tuple`

```c++
#include <iostream>
#include <tuple>

using namespace std;

int main()
{
    string names[] = {"a", "b", "c"};
    int scores[] = {1, 2, 3};
    char sex[] = {'f', 'm', 'f'};

    auto stu1 = make_tuple(names[0], scores[0], sex[0]);
    cout << "> stu1 : ["
         << get<0>(stu1) << ','
         << get<1>(stu1) << ','
         << get<2>(stu1) << "]\n";

    string name;
    int score;
    char s;
    // also: tuple(names[1], scores[1], sex[1]);
    auto stu2 = tuple<string, int, char>(names[1], scores[1], sex[1]);
    tie(name, score, s) = stu2;
    cout << "> stu2 : ["
         << name << ',' << score << ',' << s << "]\n";

    // in C++17
    auto stu3 = tie(names[2], scores[2], sex[2]);
    auto [a, b, c] = stu3;
    cout << "> stu3 : [" << a << ',' << b << ',' << c << "]\n";
}
```

> 为了方便输出，可以写一个输出流：

```c++
template<class T1, class T2>
ostream& operator<<(ostream& out, const pair<T1, T2> &p)
{
    return out << '(' << p.first << ',' << p.second << ')';
}
```

> 如果我们只在乎其中的一部分数据：

```c++
string name;
int score;
char s;

tie(ignore, ignore, s) = stu_;
// 即使用std::ignore方法。
```

> 判断类型是否一致：

```c++
cout << is_same<U, V>::value << endl; // true/1 or false/0.
```

#### `make_tuple`和`tie`的区别：

> `make_tuple`是直接构造复制；`tie`是创建**左值**引用。
>
> `get<>()`返回的是引用。

```c++
auto stu3 = tie(names[2], scores[2], sex[2]);
// stu3是对这3个数据的引用。
get<1>(stu3)++; // get<1>(stu3)是score[2]的引用。

cout << scores[2] << endl;
```

> 而`auto [a, b, c] = stu`的方法和`make_tuple`一样，即**复制**（类型上与stu各个位置数据的类型是一样的）。比如说stu的score是int&类型的，那么通过改变b，就能改变score。

#### 总结

- `std::make_tuple()` 以及 `std::tie()` 均可以构造一个 `tuple`，两者的区别在于前者构造复制，后者创建引用，更准确的说法是后者创建一个**左值引用**；
- `std::get()` 会解包出来固定位置的值，其返回值是元组内部值的**引用**；
- `std::tie()` 会解包出来该元组的所有值，不需要的位置可以用占位符 `std::ignore` 代替，解包数据的数据类型是基本类型而非引用类型；
- C++17 引入的 structured binding 方法 `auto [_1, _2, _3, ...]` 在解包时的表现与 `std::tie()` 类似。