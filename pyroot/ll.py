from __future__ import print_function

from .PyPolyBoRi import *
from .statistics import used_vars_set

from .rank import rank

from functools import reduce

lead_index = top_index

#(p):
#  return iter(p.lex_lead()).next().index()#first index

def combine(reductors, p, reduce=None):
    p_nav = p.navigation()
    assert p_nav.value() < reductors.navigation().value()
    p_else = BooleSet(p_nav.else_branch(), p.ring())
    if reduce:
        p_else = reduce(p_else, reductors)
    return if_then_else(p_nav.value(), reductors, p_else)


def llredsb_Cudd_style(polys):

    if polys:
        reductors = Polynomial(polys[0].ring().one()).set()
    else:
        reductors = None

    linear_lead = sorted(polys, key=lead_index, reverse=True)
    assert len(set([p.lex_lead() for p in linear_lead])) == len(polys)
    assert len([p for p in polys if p.constant()]) == 0
    assert len([p for p in polys if p.lex_lead_deg() == 1]) == len(polys)
    assert len(set([p.navigation().value() for p in polys])) == len(polys)
    for p in linear_lead:
        reductors = combine(reductors, p, reduce=ll_red_nf_redsb)
    return reductors


def ll_encode(polys, reduce=False, prot=False, reduce_by_linear=True):
    polys = [Polynomial(p) for p in polys]
    linear_lead = sorted(polys, key=lead_index, reverse=True)
    assert len(set([p.lex_lead() for p in linear_lead])) == len(polys)
    assert len([p for p in polys if p.constant()]) == 0
    assert len([p for p in polys if p.lex_lead_deg() == 1]) == len(polys)
    assert len(set([p.navigation().value() for p in polys])) == len(polys)
    if (not reduce) and reduce_by_linear:
        linear_polys = [p for p in polys if p.deg() == 1]
        if linear_polys:
            linear_ll = ll_encode(linear_polys, reduce=True,
                reduce_by_linear=False)
            polys = [p.lex_lead() + ll_red_nf_redsb(p + p.lex_lead(),
                linear_ll) for p in polys]
    if reduce:
        reduce = ll_red_nf_redsb
    else:
        reduce = None


    if polys:
        reductors = Polynomial(polys[0].ring().one()).set()
    else:
        reductors = None

    last = None
    counter = 0
    for p in linear_lead:

        if prot:
            counter = counter + 1
            progress = (counter * 100) / len(linear_lead)
            if last != progress:
                print(str(progress) + "%")
            last = progress
        reductors = combine(reductors, p, reduce=reduce)
    return reductors


def eliminate(polys, on_the_fly=False, prot=False, reduction_function=None,
    optimized=True):
    """There exists an optimized variant, which reorders the variable in a different
  ring.
  """
    polys = [Polynomial(p) for p in polys]
    rest = []
    linear_leads = []
    linear_leading_monomials = set()
    for p in polys:
        if p.is_zero():
            continue
        lm = p.lex_lead()
        if lm.deg() == 1:

            if not (lm in linear_leading_monomials):
                linear_leading_monomials.add(lm)
                linear_leads.append(p)
            else:
                rest.append(p)
        else:
            rest.append(p)
    if len(linear_leads) == 0:
        def identity(p):
            return p
        return (linear_leads, identity, rest)
    if reduction_function is None:
        if on_the_fly:
            if optimized:
                reduction_function = ll_red_nf_noredsb_single_recursive_call
            else:
                reduction_function = ll_red_nf_noredsb
        else:
            reduction_function = ll_red_nf_redsb

    def llnf(p):
        return reduction_function(p, reductors)
    reduced_list = []
    if optimized:
        (llnf, reduced_list) = eliminate_ll_ranked(
        linear_leads,
        rest,
        reduction_function=reduction_function,
        reduce_ll_system=(not on_the_fly),
        prot=prot)
    else:
        reductors = ll_encode(linear_leads, reduce=(not on_the_fly), prot=prot)
        for p in rest:
            p = reduction_function(p, reductors)
            if p.is_one():
                reduced_list = [p]
                break
            else:
                reduced_list.append(p)

    return (linear_leads, llnf, reduced_list)


def construct_map_by_indices(to_ring, idx_mapping):
    v = BoolePolynomialVector((max(idx_mapping.keys()) + 1) * [to_ring.zero()])
    for (from_idx, to_idx) in idx_mapping.items():
        val = to_ring.variable(to_idx)
        v[from_idx] = val
    return v


def eliminate_ll_ranked(ll_system, to_reduce,
                        reduction_function=ll_red_nf_noredsb,
                        reduce_ll_system=False, prot=False):

    assert(ll_system)
    from_ring = ll_system[0].ring()

    ll_ranks = rank(ll_system)
    add_vars = set(used_vars_set(to_reduce).variables()).difference(ll_ranks.
        keys())
    for v in add_vars:
        ll_ranks[v] = -1

      #pushing variables ignored by ll to the front means,
      #that the routines will quickly eliminate them
      #and they won't give any overhead
    def sort_key(v):
        return (ll_ranks[v], v.index())
    sorted_vars = sorted(ll_ranks.keys(), key=sort_key)

    def var_index(v):
        return iter(Monomial(v).variables()).next().index()
  #sorted_var_indices=[var_index(v) for v in sorted_vars]
    to_ring = Ring(len(sorted_vars))
    map_back_indices = dict([(i, var_index(v)) for (i, v) in enumerate(
        sorted_vars)])
    map_from_indices = dict([(var_index(v), i) for (i, v) in enumerate(
        sorted_vars)])
  #dict([(v,k) for (k,v) in enumerate(sorted_var_indices)])
    var_names = [str(v) for v in sorted_vars]
    try:
        for (i, v) in enumerate(sorted_vars):
            assert var_names[i] == str(v), (var_names[i], v, var_index(v), i)
      #        _set_variable_name(to_ring, i, var_names[i] + "TO")
    finally:
        pass
    try:
        map_from_vec = construct_map_by_indices(to_ring, map_from_indices)
    finally:
        pass
    map_back_vec = construct_map_by_indices(from_ring, map_back_indices)

    def map_from(p):
        res = substitute_variables(to_ring, map_from_vec, p)
      # assert str(p)==str(res), (str(p), str(res), list(map_from_vec),
      # list(map_back_vec))
        return res

    def map_back(p):
        return substitute_variables(from_ring, map_back_vec, p)
    try:
        ll_opt_encoded = ll_encode([map_from(p) for p in ll_system],
            prot=False,
            reduce=reduce_ll_system)

        def llnf(p):
            return map_back(reduction_function(map_from(p), ll_opt_encoded))
        opt_eliminated = [llnf(p) for p in to_reduce]
    finally:
        pass
    return (llnf, opt_eliminated)


class RingMap(object):
    """Define a mapping between two rings by common variable names.

    >>> from brial.frontend import *
    >>> to_ring = declare_ring([Block("x", 10)], globals())
    >>> from_ring = declare_ring([Block("y", 5), Block("x", 10)], globals())
    >>> mapping = RingMap(to_ring, from_ring)
    >>> (x(1)+1).navigation().value()
    6
    >>> mapping(x(1)+1)
    x(1) + 1
    >>> mapping(x(1)+1).navigation().value()
    1
    >>> mapping.invert(mapping(x(1)+1))
    x(1) + 1
    >>> mapping.invert(mapping(x(1)+1)).navigation().value()
    6
    >>> mapping(y(1)+1)
    Traceback (most recent call last):
    ...
    RuntimeError: Operands come from different manager.
    """

    def __init__(self, to_ring, from_ring):
        """Initialize map by two given rings.

        >>> from brial.frontend import *
        >>> to_ring = declare_ring([Block("x", 10)], globals())
        >>> from_ring = declare_ring([Block("y", 5), Block("x", 10)], globals())
        >>> mapping = RingMap(to_ring, from_ring)
        >>> mapping(x(1)+1)
        x(1) + 1
        """

        def vars(ring):
            return [ring.variable(i) for i in range(ring.n_variables())]

        def indices(vars):
            return dict([(str(v), idx) for (idx, v) in enumerate(vars)])

        self.to_ring = to_ring
        self.from_ring = from_ring
        to_vars = vars(to_ring)
        from_vars = vars(from_ring)
        to_indices = indices(to_vars)
        from_indices = indices(from_vars)
        common = list(set(to_indices.keys()) & set(from_indices.keys()))

        to_map = list(from_vars)
        for elt in common:
            to_map[from_indices[elt]] = to_vars[to_indices[elt]]

        from_map = list(to_vars)
        for elt in common:
            from_map[to_indices[elt]] = from_vars[from_indices[elt]]

        self.to_map = BoolePolynomialVector(to_map)
        self.from_map = BoolePolynomialVector(from_map)

    def __call__(self, poly):
        """Execute the map to change rings.

        >>> from brial.frontend import *
        >>> to_ring = declare_ring([Block("x", 10)], globals())
        >>> from_ring = declare_ring([Block("y", 5), Block("x", 10)], globals())
        >>> mapping = RingMap(to_ring, from_ring)
        >>> mapping(x(1)+1)
        x(1) + 1
        """
        return substitute_variables(self.to_ring, self.to_map, poly)

    def invert(self, poly):
        """Inverted map to initial ring.

        >>> from brial.frontend import *
        >>> to_ring = declare_ring([Block("x", 10)], globals())
        >>> from_ring = declare_ring([Block("y", 5), Block("x", 10)], globals())
        >>> mapping = RingMap(to_ring, from_ring)
        >>> mapping.invert(mapping(x(1)+1))
        x(1) + 1
        """
        return substitute_variables(self.from_ring, self.from_map, poly)
