# Symbolic Execution in KLEE

Also see [Harvard252-SymbolicExecution](../Harvard252/lec13-symbolicExe.md) to get a big idea by looking at the examples.

## Symbolic Execution for Testsuite Generation

- Use symbolic values for inputs;
- Execute program symbolically on the symbols;
- Collect the path constraints (PCs, at each program point, PC is $\text{symbolState } \and \text{ PathCondition}$);
- Given the PCs, use SMT/SAT solvers to check the accessbility of each branch;

Then you gotta know at each program point, what the preconditions is. And along with your assertion (what we wanna check. say if we want "n > 0", then add this clause onto it when SMT.).

BTW, in the beginning, PC is true and we add conditions when meeting additional branches.

## Challenge: Path explosion

Goal: exploit possible new states in the program!

Interleaving 2 heuristics searching:

- Random searching (pick a sub-branch with equal probability);
- Coverage-Optimized searching: try to cover new codes;
  - When picking a branch, we prioritize branch with lower coverage (min distance to an uncovered instruction);

## Challenge: Optimize SMT queries

The goal is to simplify the formular sent to the SMT solver;

Simplify the expressions:

e.g., x + 1 == 10 -> x == 9;

Look for sort path:

- Counter-example cache;
- Superset of (un)sat constraints;
- Subset of sat constraints;

# Concolic Execution -> Hybrid Concolic Testing [ICSE'07]

## Concolic execution

Concolic = concrete + symbolic

1. Generate a random input to dive into the program execution tree;
2. Collect the PCs along the path;
3. In the next iteration, negate the last conjunction to obtain a new constraint;
4. Solve the constraints to get inputs matching the branch decisions except the last one;
5. Goto step 3 until no path left;

Concolic in a nutshell: starting from a path by a random input, try new paths by collecting and mutating observed constraints.

But we still need some kind of random inputs since the path generated by concolic execution highly depends on the starting point.

## Hybrid Concolic Execution

- Try random testing first;
- If the coverage converges, try concolic testing;

Thus, we achieve deep and broad search!

---

## Some trivial comments

Compared with guided unitest generation which lies in the part of "test program generation", concolic testing looks at how to generate test data which is preferred to be of better code coverage when shipping to the (test) program.

> 2ce: concolic execution heavily relies on SMT solving (slow!), making it hardly deployable for huge projects. (then what about employing caching along the commit history?)
