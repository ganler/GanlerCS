import z3

if __name__ == '__main__':
    x = z3.Real('x')
    const = z3.RealVal(1) / 3
    z3.set_option(rational_to_decimal=True)
    z3.solve(x + const == 0)
