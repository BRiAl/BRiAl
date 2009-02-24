from polybori.PyPolyBoRi import Polynomial
from polybori.partial import PartialFunction
from polybori.specialsets import all_monomials_of_degree_d, power_set
def add_bits_old(bits):
    """Adds n bits
    >>> from polybori import *
    >>> r=Ring(10)
    >>> add_bits_old([Variable(i) for i in xrange(3)])
    [x(0) + x(1) + x(2), x(0)*x(1) + x(0)*x(2) + x(1)*x(2)]
    >>> add_bits_old([Variable(i) for i in xrange(4)])
    [x(0) + x(1) + x(2) + x(3), x(0)*x(1) + x(0)*x(2) + x(0)*x(3) + x(1)*x(2) + x(1)*x(3) + x(2)*x(3)]
    """
    bits=list(bits)
    n=len(bits)
    deg_d_monomials=[Polynomial(all_monomials_of_degree_d(i, bits)) for i in xrange(n+1)]
    full=power_set(bits)
    bits_expr=[]#[sum(bits)]
    step=0
    while n>2**step:
        to_one=sum([deg_d_monomials[i] for i in xrange(n+1) if i & 2**step])
        to_one=Polynomial(to_one)
        fun=PartialFunction(ones=to_one, zeros=full.diff(to_one))
        poly=fun.interpolate_smallest_lex()
        bits_expr.append(poly)
        step=step+1
    return bits_expr

def add_bits(bits):
    """Adds n bits, by Lucas theorem
    >>> from polybori import *
    >>> r=Ring(10)
    >>> add_bits([Variable(i) for i in xrange(3)])
    [x(0) + x(1) + x(2), x(0)*x(1) + x(0)*x(2) + x(1)*x(2)]
    >>> add_bits([Variable(i) for i in xrange(4)])
    [x(0) + x(1) + x(2) + x(3), x(0)*x(1) + x(0)*x(2) + x(0)*x(3) + x(1)*x(2) + x(1)*x(3) + x(2)*x(3)]
    """
    bits=list(bits)
    n=len(bits)
    
    bits_expr=[]#[sum(bits)]
    step=0
    while n>2**step:
        bits_expr.append(Polynomial(all_monomials_of_degree_d(2**step, bits)))
        step=step+1
    return bits_expr


def _test():
    import doctest
    doctest.testmod()

if __name__ == "__main__":
    _test()