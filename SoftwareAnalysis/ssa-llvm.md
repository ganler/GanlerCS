# SSA = Static Single Assignment

## What

- varaible can only be assigned once.

To convert ordinary codes into SSA form:
Make each assignment is creating a new value.

e.g., $a=b; a=b+1;$ => $a_0=b;a_1=b+1;$

## Why

- Efficient register allocation b.c. it's easy to count how many varaibles there.


## Branch? Phi Node?

[ref](https://en.wikipedia.org/wiki/Static_single_assignment_form#Converting_to_SSA)

When joining a branch:
- brach A creates value y1 assigned to x;
- brach B creates value y2 assigned to x;

x = y1 or x = y2 depends on the execution. So, can we write something like x=y1 or y2?

To resolve this, $\phi$ (Phi) function is introduced.
y3 = Phi(y1, y2), x = y3 will take the ball.

## Misc

In LLVM, registers are required to be in SSA form while memory objects don't.

sounds like you have:

immutable objects -> register
mutable objects -> memory (by stack allocation)

but there's a pass called [PromoteMemoryToRegister](http://llvm.org/docs/doxygen/html/PromoteMemoryToRegister_8cpp.html) trying to moving those memory stuff into registers.



