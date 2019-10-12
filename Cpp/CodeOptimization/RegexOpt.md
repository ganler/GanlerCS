# Regex Optimization

## tips

- 不要重复构造，重复构造将带来较大开销

```c++
std::cmath match;
const std::regex re(...);
// 在外面构造
while(...)
{
  	std::regex_match("...", match, regex);
}
```

## ImplOpt

对于一个图我们可以：

#### DFS

- 时间：指数级
- 空间：$O(|targets|+|states|)$

#### Memorized DFS

> 即动态规划，记录`<const state*, const char*>`;

- 时间和空间：$O(|targets|*|states|)$

我们可以将states和char*放到一个table中：

#### BFS

> 缺点是常数上slower than dfs.

- 时间：$O(|targets|*|states|)$
- 空间：$O(|states|)$

![](https://i.loli.net/2019/10/11/RaWglsDXNBeE2UM.png)

- 把下一层都看了，然后看有没有自己能匹配的，能匹配就进入下一层。

#### 提前生成DFA

> set->{当前有的所有状态}
>
> `(state, input) -> state`

- 遍历所有可能的字典里的所有字符
- 对set里的所有state输入这个字符，得到了result set(接受这个输入后的下一个状态)

> 由形式语言自动机的知识，每个set对应的就是一个DFA的state。

![](https://i.loli.net/2019/10/11/ykuEDBXSmhUbAtG.png)

## Impl

![](https://i.loli.net/2019/10/11/RSKo38zynZ6QAVh.png)

> RE2:
>
> - Try DFA with limits;
> - If failed, try BFS;
> - Finally, memorized DFS;

### 实现capturing group: 抓到你要的东东

- DFS & Memo: `std::pair<state, std::string> => bool`
- BFS: `state -> (bool, string)`

### BackReference

### BraceQuantifiers