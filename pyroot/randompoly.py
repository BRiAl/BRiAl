from .PyPolyBoRi import Monomial, random_set, Polynomial, \
set_random_seed, Ring, ll_red_nf_redsb, Variable
from .ll import ll_encode
from random import Random
from pprint import pprint, pformat
from .blocks import declare_ring


def gen_random_poly(ring, l, deg, vars_set, seed=123):

    myrange = vars_set
    r = Random(seed)

    def helper(samples):
        if samples == 0:
            return Polynomial(ring.zero())
        if samples == 1:
            d = r.randint(0, deg)
            variables = r.sample(myrange, d)
            m = Monomial(ring)
            for v in sorted(set(variables), reverse=True):
                m = m * Variable(v, ring)
            return Polynomial(m)
        assert samples >= 2
        return helper(samples / 2) + helper(samples - samples / 2)
    p = Polynomial(ring.zero())
    while(len(p) < l):
        p = Polynomial(p.set().union(helper(l - len(p)).set()))
    return p


def sparse_random_system(ring, number_of_polynomials,
    variables_per_polynomial, degree, random_seed=None):
    """
    generates a system, which is sparse in the sense, that each polynomial
    contains only a small subset of variables. In each variable that occurrs in a polynomial it is dense in the terms up to the given degree (every term occurs with probability 1/2).
    The system will be satisfiable by at least one solution.
    >>> from brial import *
    >>> r=Ring(10)
    >>> s=sparse_random_system(r, number_of_polynomials = 20, variables_per_polynomial = 3, degree=2, random_seed=123)
    >>> [p.deg() for p in s]
    [2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2]
    >>> sorted(groebner_basis(s), reverse=True)
    [x(0), x(1), x(2), x(3), x(4) + 1, x(5), x(6) + 1, x(7), x(8) + 1, x(9)]
    """
    if random_seed is not None:
        set_random_seed(random_seed)
    random_generator = Random(random_seed)
    solutions = []
    variables = [ring.variable(i) for i in range(ring.n_variables())]
    for v in variables:
        solutions.append(v + random_generator.randint(0, 1))
    solutions = ll_encode(solutions)
    res = []
    while len(res) < number_of_polynomials:
        variables_as_monomial = Monomial(
            random_generator.sample(
                variables,
                variables_per_polynomial)
        )
        p = Polynomial(random_set(variables_as_monomial, 2 ** (
            variables_per_polynomial - 1)))
        p = sum([p.graded_part(i) for i in range(degree + 1)])
        if p.deg() == degree:
            res.append(p)
    res = [p + ll_red_nf_redsb(p, solutions) for p in res]
    # evaluate it to guarantee a solution
    return res


def sparse_random_system_data_file_content(
    number_of_variables, **kwds):
    r"""
    >>> sparse_random_system_data_file_content(10, number_of_polynomials = 5, variables_per_polynomial = 3, degree=2, random_seed=123) # doctest: +ELLIPSIS
    "declare_ring(['x'+str(i) for in xrange(10)])\nideal=\\\n[...]\n\n"
    """
    dummy_dict = dict()
    r = declare_ring(['x' + str(i) for i in range(number_of_variables)],
        dummy_dict)
    polynomials = sparse_random_system(r, **kwds)
    polynomials = pformat(polynomials)
    res = "declare_ring(['x'+str(i) for in xrange(%s)])\nideal=\\\n%s\n\n" % (
        number_of_variables, polynomials)
    return res


def _test():
    import doctest
    doctest.testmod()

if __name__ == "__main__":
    _test()
