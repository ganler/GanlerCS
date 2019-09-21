# CS188: NOTE

## English Words

- self-diagnostic
- Tier by tier

## Lesson 2: Search

#### Reflex Agent

- Choose action based on current percept(e.g. memory);
- Do not consider the future consequences of their action.

Think about the `reflex` in biology.

#### Planning Agent

- Ask `what if`;
- Decisions based on consequences of actions.
- Must have a model of how the world evolves in response to actions.
- Must formulate a goal;

#### Search Problems consist of

- Search space(things needed to get the following elements.);
- A successor function;
- A start state and a goal test;

**A solution** is a sequence of actions (a plan) which transforms the start state to a goal state;

#### BFS vs DFS

BFS may means that you need more storage.

**BFS**

- Shortest path(On condition that each road weight is equal.)

**DFS**

- Less memory.(Maximum memory = the number of the layer: $n$, while BFS: $2^n$ )

#### Get both of the advantages:

Iterative deepening:

- Run DFS with depth limit 1;
- Run DFS with depth limit 2;
- Run DFS with depth limit 3;

...

> Note that the algorithm analysis talked above is when the cost of each road is equal(e.g. they're all 1.)

#### Cost-Sensitive Search

> To find the shortest path.

##### Uniform Cost Search(UCS)

```html
S -> G;

Start from S;

insert S.sons which are not in PQ and update the shortest distance.

Get the one has the minimum distance from S in PQ and see if it is G;

If not, insert all leaves of it to PQ and PQ.pop;

repeat until get G or PQ is empty;
```

> It can obviously help us find out the shortest path, as each time we extract the node from `PQ`, we get the shortest path in the left paths.

> UCS和Dijkstra本质上是一种算法，UCS是找到目标节点就结束，而Dijkstra是找到所有节点的最短路径。
>
> 为了更高的效率，我们一般不在优先队列里面去找一个节点是否存在，而是直接把没有被遍历且distance小于当前的最小distance的点直接插入到优先队列里（会导致优先队列里面有重复的节点），不过我们再对于已经被pop过一次的点，直接continue就好。

## Lecture 3: Uninformed Search

#### 2 conditions

- Informed
  - BFS(opt)：实现用的是普通队列
  - UCS(opt)：实现用的是优先队列
  - DFS
  - Depth-limited S
- Uninformed

#### 评价函数

$f$用于评价各个节点到目的地的代价。

一般f会用到启发函数$h(n)$：节点n到目的地的**代价估计**。(认为设计的)

一般代价估计会用一些距离描述：

- 曼哈顿距离
- 欧式距离

也会有$g(n)$，代表出发点到节点n的最短距离。(固定的)

#### 各种搜索方法的评价函数

- DFS: $f(n)=depth\_num-depth(n)$ 
- BFS: $f(n) = depth(n)$
- UCS: $f(n)=g(n)$
- A*: $f(n)=g(n)+h(n)$ (opt)
- Greedy first: $f(n)=h(n)$ (opt)



## Lecture 5

> 多智能体的博弈问题。对于之前的单agent问题，所有场面都是可控的。而对于多agent的博弈问题，我们只能决定我方的选择，无法左右敌方的选择，因此，就搜索树而言，我们只能在某些节点上进行人为选择，而无法在其他节点上进行选择。

零和问题：总的双方获胜收益相同的问题。(比如打比赛，有输-1，有赢1，有平局0，加起来就是0，这就是零和)。

### MinMax

#### 核心思想

首先得有一个评估函数，能评估每个节点对我方来说的受益值。

对于我们自己来说，我们肯定是选择可选节点中收益指最大的。然后因为整个搜索树有不确定的部分(由对手决定的部分)，所以我们需要帮对手决定一下走哪步，才能把整个搜索树中，我认为的收益值给计算出来。<u>MinMax的思想是假设对手是个天才，每一步都会选择我方收益最小的节点。</u>然后叶子节点（游戏结束）的受益值我们是知道的，所以通过递归我们就可以算出所有节点上，对我来说的受益值。就可以按此方法去决策。

对于平均合法行棋点是b，且搜索深度是m的游戏，该算法复杂度为$b^m$。

#### 实现

> 递归即可。

- 找到叶子节点，算出其评价值；
- 按照MinMax法则更新叶子节点的父节点平均值。

### AlphaBeta剪枝

#### 减少搜索情况

Min会选择Max节点的最小值作为路径。

考虑下面的树：

![](https://i.loli.net/2019/04/13/5cb1d371a3f81.png)



在图b，我在根节点处，要选择子节点的最大值，于是我开始DFS更新节点值，因为Min节点会选让我尽量小的值，如果在我已找到的最优解时，Min有可能在某个分支选择比我的最优解更小的值，那么我就把改节点减掉。

#### 实现

对于每个节点设定$\alpha$和$\beta$值，前者表示我当前搜到的最优解，$\beta$是敌方当前的最优解，他们的初始值为$[-\infin,+\infin]$，我会选择尽量高的子节点中的最劣解，一旦我新发现的最劣解比我目前找到的最劣解的最优解差，那么这段我都不找了。然后$\alpha$和$\beta$值是可以前后更新的，当前Min找到的$\beta$可以更新父节点的$\alpha$，Max的$\alpha$可以更新父节点的$\beta$。

