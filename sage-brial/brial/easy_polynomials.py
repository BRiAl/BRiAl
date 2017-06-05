from .interpolate import variety_lex_leading_terms, nf_lex_points
from .PyPolyBoRi import easy_linear_factors


def easy_linear_polynomials(p):
    """ Get linear polynomials implied by given polynomial.

    >>> from brial.frontend import *
    >>> easy_linear_polynomials(x(1)*x(2) + 1)
    [x(1) + 1, x(2) + 1]
    >>> easy_linear_polynomials(x(1)*x(2) + 0)
    []
    >>> easy_linear_polynomials(x(0)*x(1) + x(0)*x(2) + 1)
    [x(0) + 1, x(1) + x(2) + 1]
    """
    res = []
    if p.deg() >= 2:
        if p.vars_as_monomial().deg() > 8:
            opp = p + 1
            for q in easy_linear_factors(opp):
                res.append(q + 1)
        else:
            res = easy_linear_polynomials_via_interpolation(p)
    return res


def easy_linear_polynomials_via_interpolation(p):
    """ Get linear polynomials implied by given polynomial using interpolation
    of the variety.

    >>> from brial.frontend import *
    >>> easy_linear_polynomials_via_interpolation(x(1)*x(2) + 1)
    [x(1) + 1, x(2) + 1]
    >>> easy_linear_polynomials_via_interpolation(x(1)*x(2) + 0)
    []
    >>> easy_linear_polynomials_via_interpolation(x(0)*x(1) + x(0)*x(2) + 1)
    [x(0) + 1, x(1) + x(2) + 1]
    """
    res = []
    p_vars = p.vars_as_monomial()
    space = p_vars.divisors()
    zeros = p.zeros_in(space)
    lex_leads = variety_lex_leading_terms(zeros, p_vars)
    for m in lex_leads:
        if m.deg() == 1:
            red = m + nf_lex_points(m, zeros)
            if red.lead_deg() == 1:  # normal ordering
                res.append(red)
    return res
