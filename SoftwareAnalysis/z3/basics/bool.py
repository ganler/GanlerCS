import z3

if __name__ == '__main__':
    '''
    And, Or, Not, Implies, If, Bi-Implies: ==
    0: p -> q     :=> ~p or q
    1: r <-> ~q   :=> (~r and ~q) and (q and r)
    2: ~p or r
    -------------------------------------------
    Theoretically, in 3-way logics, we got:
    0: true; 1: false; 2: not-sure;
    For p -> q statements:
      if p is true  |= q is deterministic.
      if p is false |= we have no idea about q.
        i.e., q is undeterministic.
        "undeterministic" is 1 in binary logic.
    '''
    p, q, r = [z3.Bool(x) for x in 'pqr']
    exps = [
            z3.Implies(p, q),
            r == z3.Not(q),
            z3.Or(z3.Not(p), r)
            ]
    z3.solve(*exps) # unsat!
