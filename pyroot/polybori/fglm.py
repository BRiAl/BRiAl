from polybori.PyPolyBoRi import BooleSet, Polynomial, BoolePolynomialVector, FGLMStrategy,\
    dp_asc, Monomial, Ring
from polybori.blocks import declare_ring
def fglm(I, from_ring, to_ring):
    """
    converts *reduced* Groebner Basis in from_ring to a GroebnerBasis in to_ring.
    It acts independend of the global ring, which is restored at the end of the
    computation,
    >>> r=declare_ring(['x','y','z'],dict())
    >>> old_ring = r
    >>> new_ring = old_ring.clone()
    >>> new_ring.change_ordering(dp_asc)
    >>> (x,y,z) = [new_ring.variable(i) for i in xrange(3)]
    >>> ideal=[x+z, y+z]# lp Groebner basis
    >>> list(fglm(ideal, old_ring, new_ring))
    [y + x, z + x]
    """
    vec=BoolePolynomialVector(I)
    res= FGLMStrategy(from_ring,to_ring,vec).main()
    res = [to_ring.coerce(p) for p in res]
    return res
    
def vars_real_divisors(monomial, monomial_set):
    """
    returns all elements of of monomial_set, which result multiplied by a variable in monomial.
    >>> from polybori.PyPolyBoRi import Ring
    >>> r=Ring(1000)
    >>> x = Variable = VariableFactory(r)
    >>> b=BooleSet([x(1)*x(2),x(2)])
    >>> vars_real_divisors(x(1)*x(2)*x(3),b)
    {{x(1),x(2)}}
    """
    return BooleSet(Polynomial(monomial_set.divisors_of(monomial)).graded_part(monomial.deg()-1))

def m_k_plus_one(completed_elements, variables):
    """ calculates $m_{k+1}$ from the FGLM algorithm as described in Wichmanns diploma thesis
    It would be nice to be able to efficiently extract the smallest term of a polynomial
    >>> r=Ring(1000)
    >>> x = Variable = VariableFactory(r)
    >>> s=BooleSet([x(1)*x(2),x(1),x(2),Monomial(r),x(3)])
    >>> variables=BooleSet([x(1),x(2),x(3)])
    >>> m_k_plus_one(s,variables)
    x(2)*x(3)
    >>> r.change_ordering(dp_asc)
    >>> m_k_plus_one(s,variables)
    x(1)*x(3)
    """
    return sorted(completed_elements.cartesian_product(variables).diff(completed_elements))[0]
def _test():
    import doctest
    doctest.testmod()

if __name__ == "__main__":
    _test()
