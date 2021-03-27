# Back to Basics

## Compile a Program

* Lexical Anal.
* Syntactic Anal.
* Semantic Anal. 
* Optimization.
* Code Gen;

## Context-free Grammar

> Big idea: 产生式左边只有一个终结符；

$$
G=\langle\Sigma, N, P, S\rangle
$$

* $\Sigma$ is the **alphabet**, meaning all symbols (small cases);
* $N$ a finite & non-empty set of **non-terminal** symbols;
* $P$ is a set of production rules;
* $S$ Stands for start symbols (one of the $N$);

* $\Sigma^*$ is all possible sentences of $\Sigma$;
* $L(G)$ is a subset of $\Sigma^*$ following $G$;

## CFG: Control-flow Graph

### Basic Block

In a basic block, you can simply execute all instructions starting from the 1st line to the final line line-by-line;

### CFG

$$
G=\langle N, E\rangle
$$

N: all blocks;

E: all edges;

**Build a CFG**: Connect any BB pair if one can be following the other;

### CFG-based code coverage

A practical way to **measure code quality**;

* **Stmt coverage:** well, you need to cover all BBs (nodes)!
* **Branch coverage:** You need to cover all edges!
* **Path coverage:** You need to cover all paths (between Strt & Term BB)!

> The number of paths could be infinite (loops) or exponential (branches)!

