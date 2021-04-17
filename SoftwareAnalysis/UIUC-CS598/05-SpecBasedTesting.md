# Spec-based Testing

* @requires: precondition;
* @ensures: postcondition;

Specification <-> Contracts;

## Korat: Automated Testing Based on Java Predicates (ISSTA'02)

Given a formal specification **for a method**:
* Korat uses the method **precondition** to automatically generate all (nonisomorphic) test cases **up to a given small size**.
* then uses the method **postcondition** as the test oracle to check the correctness of each output;

### Finitization

```python
# For a binary tree;
def GetFinitization(int NUM_node):
    Finitization f(BinaryTree); # class.
    nodes = createNodes(NUM_node)
    
    # Initialize with a null;
    nodes.add(null)

    # Assign all related objects;
    f.set("BinaryTree.root", nodes)
    f.set("Node.left", nodes)
    f.set("Node.right", nodes)

    return f
```

* Finitization: a set of bounds limiting the valid inputs (size);
    * specify the # objects;
    * specify set of related object fields (including the objects of objects)

### State space

Say that we have `NUM_node = 3`:
* we have 3+1=4 nodes in the **value space** now `Node0~2, NULL`;
* All possible tests:
    * For each node, its left & right node can be # value space = 4 :=> (LEFT * RIGHT)^NONE\_ROOT\_NODES = (4 * 4)^3;
    * And we want to select one node as the root :=> N \*= 3

For N nodes:

$$
|\textrm{Space}| = (N+1)^{2n+1}
$$

Sooooo many inputs but not all of them are valid.

#### Vector presentation

Field domain: all possible value-class pairs;

Create a vector `Vec(size=elements.size())`:
* Each elements can be assigned to an integer regarding a value in its value space;

In our last example:

The vector [ root, N0.left, N0.right, N1.left, ..., N2.right ]

#### How to search

* Naive: Starting from a full 0 (NULL) vector, iterate and invoke testing!
* Field ordering -> pruning;
    * Given a vector, we execute and when returing false/true, we found that some nodes are not reached yet! Then all vectors who follows such pattern are false/true! (because it failed before knowing it!)

#### Isomorphism

Two data structures are isomorphic if a permutation exists between the two that preserves structure.

**TODO**: understand this part in the future.

