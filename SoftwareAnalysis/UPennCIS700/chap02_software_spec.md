## Classification of Testing

<img src="image-20210109010058424.png" alt="image-20210109010058424" style="zoom: 33%;" />

* White-Box: Tester can leverage what is inside the program when testing; (so you can directly perform static analysis on the source codes)
* Black-Box: You cannot see what is inside; (specify the inputs and relative outputs for verification)
* [More than those two] Grey-Box: You know part of the information;

## Common forms of safety properties

* types; (statically-typed or dynamically-typed)
* type-state properties;
* assertions;
* pre- and post- conditions; (no help in writing tests but helps run them)
* loop and class invariants;
  * Find some **invariant properties**;

## Type-State Properties

refines types with finite state information;

* SOCKET -> CONNECTED;

such properties which operations are valid in each state and their effects; (also called temporal safety properties)

Example:

![image-20210109013306073](image-20210109013306073.png)

> C++draft on mutex: https://eel.is/c++draft/thread.mutex.requirements#mutex.general-22

## Pre- and post- conditions

<img src="image-20210109013817330.png" alt="image-20210109013817330" style="zoom: 33%;" />

## Loop and class invariants

> Typically we have 2 ways to verify our program:
>
> * Formal verification: expensive annotation effort;
> * Testing: incomplete;

### Loop invariants example: 

![image-20210109020333248](image-20210109020333248.png)

### Class invariants

