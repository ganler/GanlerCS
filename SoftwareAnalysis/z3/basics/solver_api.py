import z3

if __name__ == '__main__':
    x, y = z3.Reals('x y')
    s = z3.Solver()

    s.add(x > 1)
    s.add(y > 1)
    s.add(z3.Or(x + y > 3, x - y < 2))

    print('Printing constrains...')

    for c in s.assertions():
        print(c)

    # print(s.statistics())
    print(s.check())
    
    m = s.model()
    
    for d in m.decls():
        print(f'{d.name()} := {m[d]}')
