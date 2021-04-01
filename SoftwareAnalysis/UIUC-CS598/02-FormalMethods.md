## SAT: Boolean Satisfiability Problem

### What is SAT

**SAT Problem**: Given a well-formed formula $\alpha$ in propositional logic, decide **whether there exists a satisfying solution** for $\alpha$.

简单来说就是看看一个formula有没有可能成为true。

### Big idea of solution

* **Proof by search:** Enumerate all interpretations (i.e., build a truth table), and check that they satisfy the formula.
* **Proof by deduction:** 就把一个formula变化（e.g., 取反），拆解，简化。

### NNF: Negation Normal Form

* Only leverage the most basic literal: $\neg, \and, \or$
* You can convert complex forms into NNF with **De Morgan's Laws**.

### DNF: Disjunctive Normal Form

$$
\textrm{Formula}=\textrm{Clause}\or\textrm{Formula}\\
\textrm{Clause}=\textrm{Literal}|\textrm{Literal}\and\textrm{Clause}
$$

* Disjunction of conjunction of literals.
* Easy to ensure satisfiability.
* May incur exp. increase in formula size.

### CNF

* SAT solvers use CNF as their input language.
* Linear increase in formula size.

## SMT: Satisfiability Modulo Theories

### What is SMT

SAT是指命题逻辑公式可满足性判定问题，而SMT是指另外一类公式的可满足性判定问题。

* SAT是只出现一些变量，变量的否定和逻辑连接词；
* SMT可以有额外的非逻辑符号（一阶逻辑）；

$$
\textrm{SAT}+\textrm{Theory Solvers}=\textrm{SMT}
$$

### Non-logic symbols: Example

* Constants;
* N-ary Functions: x + y;
* N-ary Predicates: x > y;

## Extra Resources

* [SAT/SMT by examples](https://sat-smt.codes/SAT_SMT_by_example.pdf)
* http://resources.mpi-inf.mpg.de/departments/rg1/conferences/vtsa08/slides/barret2_smt.pdf
* https://www.iitg.ac.in/pbhaduri/GIAN-CPS/Doc/Tut-1.pdf
* https://zhuanlan.zhihu.com/p/151725312

