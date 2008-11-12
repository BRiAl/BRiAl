from polybori.PyPolyBoRi import *

def vars_real_divisors(monomial, monomial_set):
    """
    returns all elements of of monomial_set, which result multiplied by a variable in monomial.
    >>> from polybori.PyPolyBoRi import Ring
    >>> r=Ring(1000)
    >>> x=Variable
    >>> b=BooleSet([x(1)*x(2),x(2)])
    >>> vars_real_divisors(x(1)*x(2)*x(3),b)
    {{x(1),x(2)}}
    """
    return BooleSet(Polynomial(monomial_set.divisors_of(monomial)).graded_part(monomial.deg()-1))

def m_k_plus_one(completed_elements, variables):
    """ calculates $m_{k+1}$ from the FGLM algorithm as described in Wichmanns diploma thesis
    It would be nice to be able to efficiently extract the smallest term of a polynomial
    >>> r=Ring(1000)
    >>> x=Variable
    >>> s=BooleSet([x(1)*x(2),x(1),x(2),Monomial(),x(3)])
    >>> variables=BooleSet([x(1),x(2),x(3)])
    >>> m_k_plus_one(s,variables)
    x(2)*x(3)
    >>> change_ordering(dp_asc)
    >>> m_k_plus_one(s,variables)
    x(1)*x(3)
    """
    return sorted(completed_elements.cartesianProduct(variables).diff(completed_elements))[0]
def _test():
    import doctest
    doctest.testmod()

if __name__ == "__main__":
    _test()
