import z3

def print_expression(exp):
    print("num args: ", exp.num_args())
    print("children: ", exp.children())
    print("1st child:", exp.arg(0))
    print("2nd child:", exp.arg(1))
    print("operator: ", exp.decl())
    print("op name:  ", exp.decl().name())

if __name__ == '__main__':
    '''
    What to prove:
    x > 2, y < 10, x + 2*y == 7
    '''
    x = z3.Int('x')
    y = z3.Int('y')

    exps = [x > 2, y < 10, x + 2 * y == 7]

    for exp in exps:
        print_expression(exp)

    z3.solve(*exps)

