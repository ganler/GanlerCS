# Compiler Design

[TOC]

> 7个大题（后面几个应该会综合考）：
>
> 1. 👌 RE->NFA->DFA->Minimized DFA
> 2. 👌 LL1(1)
> 3. 👌 算符优先文法
> 4. 👌 LR(0)
> 6. 👌 制导翻译结合中间代码生成，语句翻译
> 7. 👌 运行时存储空间组织：display表
> 8. 👌 代码优化：基本块、如何优化，etc.
>
> > 其他注意事项：
>>
> > 多步推导用$\Rightarrow$，单步产生式用$\to$ .

## Quick Pass

> - **<u>编译程序是对高级语言的翻译</u>**。
> - LR 分析法在自左至右扫描输入串时就能发现错误，但「不能准确地指出出错地点」。
> - 传名 -> 名称替换(define)
> - 最左推导：每次替换最左边的非终结符；
> - 四元式的等号也不能忽略：`(:=, T, -, result )`

## Intro

### 编译 vs 解释

- 解释：
  - 逐句分析并执行；
  - 源程序的执行系统；
  - 效率低，但易于差错；
- 编译：
  - 对全部程序进行分析，产生机器码，并执行；
  - **<u>源程序的转换系统</u>**；
  - 效率高，只需要分析一次，但runtime error需要在运行整个程序确定。（我觉得最后这条是瞎说... 编译器一般都配有强大的静态分析功能，而解释器... 至于runtime的事情，这俩不都一样吗... 顶多是定位的区别... ）

### Steps in compiling

<img src="https://i.loli.net/2019/12/28/bEa5e3wgL8CVkqK.png" style="zoom: 33%;" />

### Differences between One Pass & Multiple Pass

- 一遍：多占内存，少占时间；
- 多遍：多占时间，少占内存；

---

## Lexical & Syntax

### Concepts

#### 句型

假定G是一个文法，S是它的开始符号，如果 $S \overset{*}\Rightarrow\alpha$，则称α是文法G的一个句型

#### 句子

仅仅由终结符组成的句型都是句子。

#### 语言

文法G产生句子的全体；

#### 判定与证明二义性

如果一个文法的句子（句型）存在两棵「**不同的**」语法树，那么，该**<u>句子</u>**是二义性的。

> $$
> \begin{aligned}
> &S\to lrznb|A\\
> &A\to lrznb
> \end{aligned}
> $$
>
> 「lrznb」对应了2个语法树，该句是二义性的。
>
> <u>如果一个**文法**能搞出二义性的句子，那么这个**文法**也是有二义性的。</u>

### 💖 RE To NFA

> JHF：“弟弟们稍稍走开，我来教！”
>
> > 盲猜就是RE->NFA->DFA->最小化

<img src="https://i.loli.net/2019/12/28/uNCLrpzXEU7s8Ix.png" style="zoom: 33%;" />

### 💖 NFA To DFA

> 子集构造法。

<img src="https://i.loli.net/2019/06/05/5cf72f296fe8252959.png" style="zoom: 33%;" />

> 具体例子：https://www.youtube.com/watch?v=--CSVsFIDng
>
> 1. 画出NFA
> 2. 建立表格x-axis=>「输入符号」，y-axis=>「状态」
> 3. 填好NFA表
> 4. 建立DFA表，一开始只有初始状态
> 5. 填表，看有没有新的状态产生，若有则增加y-axis的状态，并继续填表，直到没有新的状态产生。

<img src="https://i.loli.net/2019/12/28/4myogOBYXpUE7iS.png" style="zoom:40%;" />

<img src="https://i.loli.net/2019/12/28/dlOm4cYI8zsfWrL.png" style="zoom:40%;" />

### 💖 Minimization Of DFA

> Since we have the DFA currently, what we need to do is to minimize it. There're several methods. However, I am only keen on the Myhill-Nerode(MN) theory.
>
> https://www.youtube.com/watch?v=UiXkJUTkp44&list=PLBlnK6fEyqRgp46KUv4ZY69yXmpwKOIev&index=25

- Input: DFA
- Output: Minimized DFA

<img src="https://www.tutorialspoint.com/automata_theory/images/dfa_minimizing_using_myphill_nerode_theorem.jpg" alt="DFA Minimizing using Myphill-Nerode Theorem" style="zoom:50%;" />

1. 列一个表， ($Q_i$, $Q_j$) 对应位置用白色块，$i=j$以及其他情况用绿色块代替（表示不用）；

<img src="https://i.loli.net/2019/04/16/5cb53d0987121.png" style="zoom:33%;" />

2. 考虑每一对 ($Q_i$, $Q_j$) ，如果$Q_i$是最终状态，而$Q_j$不是（或者相反），就在对应白色位置打钩。

<img src="https://i.loli.net/2019/04/16/5cb53e47c4b69.png" style="zoom:33%;" />

3. 考虑未被标记的($Q_i$, $Q_j$) ，如果他们接受**相同的任意输入**后可以达到被标记区域，则也将其标记。

> 比如上图的(f,b)，输入0得到(f, a)，输入1得到(f, d)，而(f,d)被mark过了，所以，(f,b)也要被mark。
>
> 而(d, c)，输入0，得到(e, e)，输入1得到(f, f)，这些不在白色范围内，所以不被mark。

4. 重复上述过程，得到最终的表格，将未标记的($Q_i$, $Q_j$) 合并：

<img src="https://i.loli.net/2019/04/16/5cb5401379900.png" style="zoom: 33%;" />

> 未被标记的有：
>
> - (b, a)
> - (d, c)
> - (e, c)
> - (e, d)

可被合并成：
$$
\{a,b\},\{c,d,e\}
$$
合并完成后，我们再进行全局的新的DFA的生成。加上未被最小化的$\{f\}$

得到最终的DFA：

<img src="https://www.tutorialspoint.com/automata_theory/images/state_diagram_of_reduced_dfa.jpg" alt="State Diagram of Reduced DFA" style="zoom:50%;" />

---

## 💖 LL(1)

### Priorities

#### 左递归问题

##### 消除直接左递归

$$
P\to Pa|b
$$

可转化为
$$
P\to bP'\\
P'\to aP'|\epsilon
$$

##### 消除间接左递归

对于确定顺序（顺序随便定义，结果长起来可能不一样）的推导式，依次展开

- 第一次展开后的结果就是**原来的推导式**
- 第N次展开（并消除左递归）只能用第N次之前的结果产生式；

> 最后得到产生式，再**直接对最后一个产生式消除左递归**就好。

形如：
$$
\begin{aligned}
S\to Qc|c\\
Q\to Rb|b\\
R\to Sa|a
\end{aligned}
$$
令顺序为RQS：

首先看前1个：$R\to Sa|a$，不含左递归，下一个；

然后看前2个（RQ）：

- 把R带入到Q：$Q\to Sab | ab | b$，暂时不含左递归，所以我们什么都不要做；

看前三个，把Q带入S：

- $S\to Sabc |abc|bc|c$，左递归出现了，消之！

$$
\begin{aligned}
&S\to abcS'|bcS'|cS'\\
&S'\to abcS'|\epsilon
\end{aligned}
$$

> 一般除此之外还得加上之前其他「无法被融合」产生式「的部分」；但本例没有，遂得最终结果。

#### 回溯问题

##### 消除Left Factoring

丢弃产生式右边的左公共的部分；
$$
P\to Ta|Tb
$$
那么我们要做的就是把T给消去（这样可以消除回溯）。

做法：
$$
P\to TP'\\P'\to a|b
$$

### 分析预测表 & 分析预测程序

> 列是栈顶的非终结符，行是读头的终结符。

#### 递归预测

```pascal
program DESCENT; // 递归下降
	begin
    	GENTEXT(TOKEN); // 获得下一个TOKEN
			PROGRAM(TOKEN); // 分析过程
			GENTEXT(TOKEN);
			if TOKEN != '$' then ERROR;
  end

// ---------
// 每个函数名都是一个非终结符，通过递归的方式去match。
// 要对每一个非终结符写一个程序。
program PROGRAM(TOKEN):
	begin
		if TOKEN != 'program' then error;
		
		GENTEXT(TOKEN);
		DECLIST(TOKEN); // 非终结符DECLIST
		
		GENTEXT(TOKEN);
		if TOKEN != ':' then error; 
		
		GENTEXT(TOKEN);
		TYPE(TOKEN);    // 非终结符
		
		// ... 用专有的程序来描述递归下降
		
	end
```

#### 非递归预测

> 造自动机（下面的S其实是$）

<img src="https://s2.ax1x.com/2019/12/28/lmtnOA.png" alt="image" style="zoom:50%;" />

### 求First

>  First(u)是u推导出的所有句子中首字的集合（如果$u\Rightarrow \epsilon$那么$\epsilon$ 也在First集中）；

递归的去求就完事了... 也可以用动态规划优化一下。

A的first主要看
$$
A\to uXv
$$
的u。

> 1. 首先把「产生式右部」「首终结符（如果有的话）」加入到「左部」对应的「First集」中。
> 2. 依次递归地根据「首非终结符」的First完善其他非终结符的First集。

> 计算First(X1X2...Xn)，这个很简单，如果X1无法推导出空，那么就是X1，否则「再」加上下一个非终结符X2，依次这样下去。

### 💖 求Follow

> 一个**非终结符**后继第一个字符的集合；
>
> 在Follow Set中，<u>不能存$\epsilon$。</u>

> 计算follow set；
>
> - <u>加入$到Follow(S)</u>（注意「`$`」不能缺 ），S为起始符号，\$为终止符号；
> - 对于$A\to uBv$，$Follow(B)\leftarrow First(v)-\epsilon$
> - 对于$A\to uB$ or $A\to uBv $(First(v)可有$\epsilon $)的产生式，把$Follow(A)$加入到$Follow(B)$；（对于产生式右部最后可以为非终结符的情况）
>
> <u>（算完再重复一遍，直到没有变化为止）</u>

### LL(1)的判定条件

1. 无左递归；
2. First集两两不相交；
3. 如果非终结符A它的First集含有$\epsilon$，那么要求$First(A)\cap Follow(A)=\Phi$；

> 有左递归那么这个语法树可以没完没了，所以不行；
>
> First集相交意味着，一个句型可能有多套产生式的左部，这是不行的；（当然这一点得结合分析预测程序来思考）；
>
> 至于第三点，我们说分析预测的时候不能有分歧，如果A它的First集含有$\epsilon$，那么他的「推导出的结果可能」在$First(A)/Follow(A)$，我们不能允许「可能」的结果，而应该要确定的结果，即，要么在First(A)，要么在Follow(A)，换句话说就是他们不能相交。

---

## LR

### Concepts

$$
S\overset{*}{\Rightarrow}\alpha A\delta\ 并且\ A\overset{+}{\Rightarrow}\beta
$$

其中$\beta$是短语。（一个句型的语法树的 **任意子树节点所组成的符号串** 都是该句型的短语）。

（一个非终结符推导出的叶子节点 「从左到右」 连起来就是短语）

#### 直接短语

句型**对应的子树不再有别的子树**。（就一个父节点+一堆子节点的时候的子节点）

> S -> aAp
>
> A -> b
>
> 那么b就是直接短语。

> 素短语
>
> - 必须含终结符
> - 不含更小的素短语（即恰好到有最少的终结符为止）
>
> ![image](https://s2.ax1x.com/2019/12/28/lmXni9.png)
>
> 如本图，我们得到`P*P`后就已经获得「终结符」了，就无需往上跑了。

#### 句柄

最左直接短语。（直接短语中最左的那个短语，就是句柄）

#### 例子

<img src="https://img-blog.csdn.net/20161213182424840?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSVRfRFJFQU1fRVI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast" alt="这里写图片描述" style="zoom: 67%;" />

<img src="https://img-blog.csdn.net/20161213183102306?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSVRfRFJFQU1fRVI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast" alt="这里写图片描述" style="zoom: 50%;" />

> **短语**：
>
> - E+T*F
> - T*F
>
> **直接短语**：
>
> - T*F （没有多余的推导/子树）
>
> **最左直接短语（句柄）**：
>
> - T*F

#### 活前缀

令句柄即句柄之前的部分的字符串为s。那么`s[:q]`(q<len(s))都是活前缀。

### LR(0)的判定

对于每个状态**不存在**下列情况：

1. 既含**<u>移进项目</u>**又含**<u>规约项目</u>**；
2. 含有多个**<u>规约项目</u>**；
   - $A\to T\cdot ,B\to T\cdot$，两者都可规约不知道选哪个；

> 如果文法对应的自动机中不存在「移进-归约」冲突和「归约-归约」冲突则为 LR(0)文法。

#### SLR

在LR(0)基础上，不允许多个移进项目接受相同非终结符。

> 消除部分的这些冲突就是让**当前项目集中**：
>
> - **被移入** 的符号$X\to *\cdot a_? \cdots$中，$a_?$不重复（废话），然后这些$a_?$形成集合**A**
> - **被规约** 的产生式左边（$X\to *\cdot$）的follow集，得到$FOLLOW(X)$
>
> 中的元素不要重复。

> 如果文法对应的自动机中不存在「移进-归约」冲突冲突则为 SLR文法。（他本身的规定就表明不会有规约-规约的出现）

> 但还是无法解决这类冲突（需要LR(1)）：
> $A\to T\cdot,B\to T\cdot *$
>
> 如果`*`不在`A`的follow集中，满足SLR，但还是会有冲突。

#### LR(1)

LR(1)的要求就是，不仅仅看一个 **非终结符**的follow集，而是看 **非终结符 + <u>位置</u>** 来确定follow集。

### LR(0)分析表

#### 执行

<img src="https://i.loli.net/2019/12/04/GH6MI71YwRKFSXJ.png" style="zoom: 20%;" />

- **Sx**：
  - Shift。
  - 进入x状态，并将该状态push进栈；
- **Rx**：
  - Reduce
  - 按照第x个式子进行规约
  - 推完就pop状态栈，得到一个非终结符，这个时候通过call GOTO来决定进入什么状态。

#### 项集族的构建

- 初始项目集：$\rm CLOSURE(\{S'\to\cdot S\})$
- 终结项目集：$\rm CLOSURE(\{S'\to S\cdot\})$

- `CLOSURE(I)`函数->计算**项目集I的闭包**
  - 若`.`后面第一个就是非终结符 => 进行下一步推导；
- `GOTO(I, X)`函数->计算**I的后继项目集闭包**
  - 相当于一次规约（进入下一项），然后进行下一步；

### 算符优先文法

#### 判断

对于任何终结符对`(a, b)` 最多只有一种优先关系；

<img src="https://s2.ax1x.com/2019/12/28/lmylRI.png" alt="image" style="zoom:50%;" />

> 如楼上，又可以先*又可以先+所以不是算符优先文法。
>
> 判断流程也是通过一个一个推导**带终结符的产生式**，来确定算符之间的优先级，且推导完之后要求没有矛盾。

#### 构造

##### 构造FIRSTVT(P)

$$
P\to a\cdots|Qa\cdots\\
a\in FIRSTVT(P)
$$

在上面的基础上，FIRSTVT(Q)也在FIRSTVT(P)中。

##### 构造LASTVT(P)

$$
P\to \cdots a|\cdots aQ\\
a\in LASTVT(P)
$$

在上面的基础上，LASTVT(Q)也在LASTVT(P)中。

##### 构建分析过程的时候

比较符号栈最右的 **终结符** 和 输入串首部的 **终结符**：

- < 或 = ： 移入
- \> : 规约

##### ⚠️❌优先函数

> 构造见习题复习。

---

## 制导翻译

### 属性文法

- **文法符号** <-> **语义属性**：$X.a$ 文法符号X的a属性。
- **产生式** <-> **语义规则** => 文法符号属性值

> 每个文法符号联系于一组属性，且对每个产生式都给出其语义规则的文法称为**属性文法**。

### SDD & SDT

SDD (Abstraction)

- 是关于语言翻译的高层次规格说明
- 隐蔽了许多具体实现细节，使用户不必显式地说明翻译发生的顺序

SDT (Implementation)

- 可以看作是对SDD的一种补充，是SDD的具体实施方案
- 显式地指明了语义规则的计算顺序，以便说明某些实现细节

### SDD

#### 综合属性

- 「分析树结点N」上的「非终结符A的**综合属性**」只能通过「**N的子结点**」或「**N本身**」的「属性值」来定义；
- **<u>终结符可以具有综合属性</u>**。终结符的综合属性值是由「词法分析器提供的**词法值**」，因此在SDD中没有计算终结符属性值的语义规则；

> 父亲.what = 儿子.what。

<img src="https://liuyehcf.github.io/images/%E7%BC%96%E8%AF%91%E5%8E%9F%E7%90%86-%E8%AF%AD%E6%B3%95%E5%88%B6%E5%AF%BC%E7%BF%BB%E8%AF%911/fig3.jpg" style="zoom:33%;" />

#### 继承属性

- 在「分析树结点N上的**非终结符**A的**继承属性**」只能通过「**N的父结点**」、「**N的兄弟结点**」或「**N本身**」的属性值来定义。
- **<u>终结符没有继承属性</u>**。终结符从「词法分析器处」获得的「属性值」被归为「综合属性值」

<img src="https://liuyehcf.github.io/images/%E7%BC%96%E8%AF%91%E5%8E%9F%E7%90%86-%E8%AF%AD%E6%B3%95%E5%88%B6%E5%AF%BC%E7%BF%BB%E8%AF%911/fig4.jpg" style="zoom:33%;" />

#### 例子

<img src="https://liuyehcf.github.io/images/%E7%BC%96%E8%AF%91%E5%8E%9F%E7%90%86-%E8%AF%AD%E6%B3%95%E5%88%B6%E5%AF%BC%E7%BF%BB%E8%AF%911/fig5.jpg" alt="fig5" style="zoom:33%;" />

<img src="https://liuyehcf.github.io/images/%E7%BC%96%E8%AF%91%E5%8E%9F%E7%90%86-%E8%AF%AD%E6%B3%95%E5%88%B6%E5%AF%BC%E7%BF%BB%E8%AF%911/fig6.jpg" alt="fig6" style="zoom:33%;" />

<img src="https://liuyehcf.github.io/images/%E7%BC%96%E8%AF%91%E5%8E%9F%E7%90%86-%E8%AF%AD%E6%B3%95%E5%88%B6%E5%AF%BC%E7%BF%BB%E8%AF%911/fig2.jpg" alt="fig2" style="zoom:33%;" />

#### S-SDD -> SDT

由于**只使用综合属性**，那么<u>直接连接到右部末尾</u>；（如上图2、3项）

#### L-SDD -> SDT

- **继承属性**：直接插入到**被继承符号的<u>左侧</u>**；（如上图1、4项）
- **综合属性**：产生式右部最右端；（如上图2、3项）

> S-SDD: 只用综合属性，直接自底向上综合即可；
>
> L-SDD：在一个产生式所关联的各属性之间，**<u>依赖图的边可以从左到右，但不能从右到左</u>**（因此称为L属性的，左边 -> 右边，即右边只能依赖左边）
>
> > 判断是否是L-SDD：
> >
> > - 每个属性**要么是一个综合属性**；
> > - 要么是满足如下条件的**继承属性**：假设存在一个产生式$A \to X_1X_2...X_n$，其右部符号$X_i(1 \le i \le n)$的**继承属性**仅**依赖**于下列属性（都是对继承属性的限制）：
> >
> > 1. $A$的继承属性；
> > 2. 产生式中$X_i$**<u>左边</u>**的符号$X_1, X_2, ..., X_{i-1}$的属性；
> > 3. $X_i$本身的属性，但$X_i$的全部属性**不能在依赖图中形成环路**；

### 依赖图（不考）

描述节点依赖关系的「有向无环图（DAG）」；

首先我们得先有一个**<u>语法树</u>**，然后用**<u>拓扑排序</u>**规定求值顺序（所以要用 <u>已知的值</u>-><u>未知的值</u>）

> 拓扑排序就是一直删去入度为0的节点。
>
> <u>SDD不一定无环，但一定存在一个SDD的有用子类，产生无环图。</u>

<img src="https://s2.ax1x.com/2019/12/29/luSAFU.png" alt="image" style="zoom:50%;" />

### 树遍历

DFS... 没啥好说的

---

## 中间代码

### 后缀表达式（逆波兰式）的转换

#### 中缀转后缀

$$
a + b * c + (d * e + f)*g
$$

1) 先按照运算符的优先级对<u>二元运算</u>加括号，变成`((a+(b*c))+(((d*e)+f)*g))`

2) 将运算符移到括号的后面，变成`((a(bc)*)+(((de)*f)+g)*)+`

3) 去掉括号，得到`abc*+de*f+g*+`

结束！

#### 后缀转中缀（及其计算）

有一个stack存变量值，遇到符号就对stack的top2进行计算，结果给push到stack里。

> ab+cd+*
>
> - `ab+` => `e = (a+b)`
> - `ecd+` => `f = (c+d)`
> - `ef*` => `g = (e * f)`
>
> 从下往上综合就是`(a+b) * (c+d)`

### 💖 中间语言

#### DAG表示法

- 叶子节点是变量（只保留一份）
- 非叶子节点是操作;

#### 三地址码

- `x := y op z`，op是二元运算符；
- `x := op y`，op是一元运算符
  - `uminus`负号（和「减」要区分）；
  - `not`；
  - 移位运算和转换运算；
- 赋值语句`x := y`
- 无条件转移语句`goto`
- 条件转移语句
  - `if x relop y goto L`
  - `if a goto L`
- 用于过程调用，`p(a, b, c)` ，n表示参数个数

```
param a
param b
param c
call p, n
```

- `.place` 代表地址；
- `.code` 代表求值语句序列；
- `newtemp()`返回新的临时变量名；

#### 四元式

$$
\rm \mathbf{(Op, arg1, arg2, result)}
$$

如a := b * -c + b * - c的四元式：

> 四元式的等号也不能忽略；

(Uminus, c, /, T1)

(*, T1, b, T2)

(Uminus, c, / T3)

(*, T3, b, T4)

(+, T2, T4, T5)

(:=, T5, /, a)

### 💖 翻译

#### 赋值语句

> 对于$S\to id = E$生成三地址码。

- `lookup(name)`查询符号表，返回name对于的记录；
- `gen(code)`生成三地址指令；
- `newtemp`生成新的临时变量，并返回其地址；
- `emit`直接增量翻译，将三地址语句发送到输出文件；

> 增量翻译，只保存一个中间代码字符串，之后只在这个字符串的基础上进行追加。

> 下图是2pass和1pass的对比。

<img src="https://i.loli.net/2019/12/29/A8N9MIXFZyOxTU1.png" style="zoom:40%;" />

<img src="https://i.loli.net/2019/12/29/Pi4GlovnEj6sJzS.png" style="zoom:25%;" />

#### 布尔表达式

> 注意，在有限即上，&&比||优先。

- 直接算出算术表达式的结果；
- 逐步推导（优化后）；

##### 数值表示法

- `nextstat`：下一条三地址语句的地址索引；
- `emit后nextstat会自动+1`

<img src="https://i.loli.net/2019/12/29/mpcuzLJeYGdiR1K.png" style="zoom:40%;" />

> 先根据文法画出语法树，按依赖图来一次执行SDT即可；
>
> 先建立数据结构再进行分析的话都是2pass；

#### 控制语句

算了，烦，不想复习这块了...

---

## 运行时存储空间

### 💖 运行时存储器划分

- 代码
- 静态数据
- 栈
- 堆

### 💖 嵌套过程语言的栈式实现

> 形参：`void func(int a, int b)`；
>
> 实参：`func(x, y)`；

- **静态链SL**：存放的是「定义该过程」所对应的上一层过程，最近一次运行时的活动记录的起始单元。
- **动态链DL**：存放的是「调用该过程前」正在运行过程的活动记录的起始单元。过程返回时当前活动记录要被撤销，此时需要动态链信息来修改基址寄存器b的内容。（DisplayTable记录的就是从下往上的DL地址）

<img src="https://i.loli.net/2019/12/29/Sn2f5RWYtAsZBdh.png" style="zoom: 33%;" />

> 楼上的diaplay不是我干的。

---

## 代码优化

### 基本块

- 「goto语句」下面一刀切；
- 「被goto的地方」上面一刀切；
- 「halt」处停下；

<img src="https://liuyehcf.github.io/images/%E7%BC%96%E8%AF%91%E5%8E%9F%E7%90%86-%E4%BB%A3%E7%A0%81%E4%BC%98%E5%8C%961/fig1.jpg" style="zoom:33%;" />

### 基于DAG的块内优化

用DAG来表示基本块内的中间代码；

- 删除无用代码
  - 删除没有父节点的节点（如果前提假设已经是活跃变量了就别删了）；
  - 一个节点上多个变量的话只要保留一个就好；

<img src="https://liuyehcf.github.io/images/%E7%BC%96%E8%AF%91%E5%8E%9F%E7%90%86-%E4%BB%A3%E7%A0%81%E4%BC%98%E5%8C%961/fig25.jpg" alt="fig25" style="zoom:25%;" />



<img src="https://liuyehcf.github.io/images/%E7%BC%96%E8%AF%91%E5%8E%9F%E7%90%86-%E4%BB%A3%E7%A0%81%E4%BC%98%E5%8C%961/fig26.jpg" alt="fig26" style="zoom:25%;" />

### 其他

- **代码外提**：找循环不变式；
- **强度削弱**：乘除 => 加减；

>  删除归纳变量是在强度削弱以后进行。

---

## 目标代码生成

- 「活跃变量」：对于变量x和程序点p（当前所在的程序块），如果在流图中沿着从p开始的**某条**路径会引用变量x在p点的值，则称变量x在点p是**活跃(live)**的，否则称变量x在点p**不活跃(dead)**

> 就是，如果当前算出来的以后结果没人用，那么就删除当前代码。

- 「待用信息」

> 直观的理解是，当前产生的值会被后面的表达式给利用，而且如果已经被计算过了，那么本次就不用重新计算了；and，如果我现在算出一个值，他以后会被用到，那么在他不会再被用到之前，我们应该尽量将其保存在寄存器里；

<img src="https://i.loli.net/2019/12/29/chKVGpYMyCnZ5oa.png" style="zoom:40%;" />

从最后一个中间代码A = B op C开始，依次往上：

1. 把A = B op C这句话的各项的（活跃/附加）信息加入到中间代码；
2. A -> (^, ^)
3. B, C -> (i, Y)

### 寄存器和地址描述

- 寄存器描述数组 `RVALUE`
  - `RVALUE(Ri)={A, B}`：引用Ri的值的变量；
- 变量地址描述数组 `AVALUE`
  - `AVALUE(A) = Ri`，变量A的值存在Ri中；

> - 因为寄存器的分配是局限于基本块范围之内的，<u>一旦处理完基本块中所有四元式，对「现行值在寄存器中的每个变量」，如果它在「基本块之后是活跃」的，则要把它存在寄存器中的值「存放到它的主存单元」中。</u>
> - 对于`A = B`，不用单独生成目标代码，只需要在RVALUE(Ri)中追加一个A；

### 代码生成算法

> 1. 只有result才一定要用寄存器，被op的数可以不用寄存器。如`SUB R1, B`。
> 2. 活跃变量要存起来！！！

对于`A = B op C`

- 以四元式作为参数，通过`GETREG(...)`得到存放A的寄存器为`R`；
- 如果B, C都在寄存器里，则令其寄存器为B' & C'；
- 如果`B' ≠ R`，那么生成目标代码
  - `LD R, B'`
  - `op R, C'`
- 否则直接：`op R, C'`（这种情况下直接删除`AVALUE[B]`的值，因为B原来的已经不存在了）
- 令`AVALUE[A] = R, RVALUE[R] = A`
- 若B/C以后不再被用（块内不被用，同时也不是活跃变量），那么释放该变量（清除其对应的寄存器记录`AVALUE[B/C] = {}, RVALUE[Ri].pop(B/C)`）

#### 寄存器分配

A = B op C

1. 如果B独占一个寄存器，且A和B是同一标识符，或者B之后不再被用，则选Ri，over；
2. 如果有没有分配的，拿没有分配的，over；
3. 从已经分配的里面拿：
   1. <u>先把人家占用寄存器里的值放回对应内存去</u>
   2. 尽量选择以后不会被引用到的/最远被引用的

> **每个block的活跃变量得被存到内存里**；

### 例题

> 见习题。