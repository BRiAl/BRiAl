from polybori.PyPolyBoRi import Polynomial, Variable, BooleSet
from polybori.partial import PartialFunction
from polybori.specialsets import all_monomials_of_degree_d, power_set
from polybori.ll import ll_encode, ll_red_nf_redsb
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
    """Adds n bit variables, by Lucas theorem
    >>> from polybori import *
    >>> r=Ring(10)
    >>> add_bits([Variable(i) for i in xrange(3)])
    [x(0) + x(1) + x(2), x(0)*x(1) + x(0)*x(2) + x(1)*x(2)]
    >>> add_bits([Variable(i) for i in xrange(4)])
    [x(0) + x(1) + x(2) + x(3), x(0)*x(1) + x(0)*x(2) + x(0)*x(3) + x(1)*x(2) + x(1)*x(3) + x(2)*x(3)]
    >>> add_bits([Variable(0)])
    [x(0)]
    """
    bits=list(bits)
    if len(bits)<2:
        return bits
    n=len(bits)
    
    bits_expr=[]#[sum(bits)]
    step=0
    while n>2**step:
        bits_expr.append(Polynomial(all_monomials_of_degree_d(2**step, bits)))
        step=step+1
    return bits_expr

def add_bit_expressions(bit_expressions):
    """Adds n bits, which can be arbitrary expressions, the first n variables of the ring
    are reversed for usage in this function.
    >>> from polybori import *
    >>> r=Ring(20)
    >>> add_bit_expressions([Variable(i) for i in xrange(10,13)])
    [x(10) + x(11) + x(12), x(10)*x(11) + x(10)*x(12) + x(11)*x(12)]
    >>> add_bit_expressions([Variable(i) for i in xrange(10,13)])
    [x(10) + x(11) + x(12), x(10)*x(11) + x(10)*x(12) + x(11)*x(12)]
    >>> add_bit_expressions([Variable(11), Variable(11)])
    [0]
    >>> add_bit_expressions([Variable(11),Variable(12),Variable(13)])
    [x(11) + x(12) + x(13), x(11)*x(12) + x(11)*x(13) + x(12)*x(13)]
    """
    
    bit_variables=[Variable(i) for i in xrange(len(bit_expressions))]
    for expr in bit_expressions:
        assert BooleSet(expr).navigation().value()>=len(bit_variables)
    mapping=ll_encode([b+expr for (b, expr) in zip(bit_variables, bit_expressions)])
    return [ll_red_nf_redsb(p, mapping) for p in add_bits(bit_variables)]

def add_words(words):
    """def adds n words, this words are supposed to consists of list of their bits.
    >>> from polybori import *
    >>> r=Ring(1000)
    >>> add_words([[Variable(100+i*3+j) for i in xrange(2)] for j in xrange(3)])
    [x(100) + x(101) + x(102), x(100)*x(101) + x(100)*x(102) + x(101)*x(102) + x(103) + x(104) + x(105), x(100)*x(101)*x(103) + x(100)*x(101)*x(104) + x(100)*x(101)*x(105) + x(100)*x(102)*x(103) + x(100)*x(102)*x(104) + x(100)*x(102)*x(105) + x(101)*x(102)*x(103) + x(101)*x(102)*x(104) + x(101)*x(102)*x(105) + x(103)*x(104) + x(103)*x(105) + x(104)*x(105)]
    >>> res=add_words([[Variable(100+i*9+j) for i in xrange(4)] for j in xrange(9)])
    >>> [len(p) for p in res]
    [9, 45, 495, 12870, 735462, 70285482, 1891358892]
    >>> [p.deg() for p in res]
    [1, 2, 4, 8, 12, 18, 25]
    >>> [p.n_nodes() for p in res]
    [9, 25, 54, 100, 153, 211, 249]
    """

    max_word_length=max((len(w) for w in words))
    res=[]
    while len(words)>0:
        words=[w for w in words if len(w)>0]
        bits=add_bit_expressions([w[0] for w in words])
        words=[w[1:] for w in words]
        if len(bits)>0:
            res.append(bits[0])
            words.append(bits[1:])
    return res
    
def _test():
    import doctest
    doctest.testmod()

if __name__ == "__main__":
    _test()