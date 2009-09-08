from polybori.PyPolyBoRi import Monomial, Variable, global_ring, random_set, Polynomial, set_random_seed
from random import Random

def gen_random_poly(l,deg,vars_set,seed=123):
    myrange=vars_set
    r=Random(seed)
    def helper(samples):
        if samples==0:
            return Polynomial(0)
        if samples==1:
            d=r.randint(0,deg)
            variables=r.sample(myrange,d)
            m=Monomial()
            for v in sorted(set(variables),key=top_index,reverse=True):
                m=m*v
            return Polynomial(m)
        assert samples>=2
        return helper(samples/2)+helper(samples-samples/2)
    p=Polynomial(0)
    while(len(p)<l):
        p=Polynomial(p.set().union(helper(l-len(p)).set()))
    return p

def sparse_random_system(npolys, variables_per_polynomial, degree, random_seed=None):
    """
    generates a system, which is sparse in the sense, that each polynomial
    contains only a small subset of variables. In each variable that occurrs in a polynomial it is dense in the terms up to the given degree (every term occurs with probability 1/2).
    The system will be satisfiable by at least one solution.
    >>> from polybori import *
    >>> r=Ring(10)
    >>> sparse_random_system(npolys = 5, variables_per_polynomial = 3, degree=2, random_seed=123)
    """
    ring = global_ring()
    if random_seed is not None:
        set_random_seed(random_seed)
    random_generator = Random(random_seed)

    variables = [Variable(i) for i in ring.n_variables()]
    res = []
    for i in xrange(npolys):
        variables_as_monomial=Monomial(
        [
            random_generator.sample(
                variables, 
                variables_as_monomial)
        ])
        p=Polynomial(random_set(degree, variables_as_monomial))
        p=sum([p.graded_part(i) for i in xrange(degree+1)])
        if p.deg()==degree:
            res.append(p)
    return res
    
def _test():
    import doctest
    doctest.testmod()

if __name__ == "__main__":
    _test()
