# 
#  intersect.py
#  PolyBoRi
#  
#  Created by Michael Brickenstein on 2008-09-24.
#  Copyright 2008 The PolyBoRi Team
# 

from polybori.PyPolyBoRi import Variable
from polybori.gbcore import groebner_basis
from polybori.statistics import used_vars_set
from itertools import chain

def intersect(i,j,**gb_opts):
    """
    This functions intersects two ideals. The first ring variable is used as helper variable for this
    intersection. It is assumed, that it doesn't occur in the ideals, and that we have an elimination ordering
    for this variables. Both assumptions are checked.
    >>> from polybori.PyPolyBoRi import Ring
    >>> r=Ring(1000)
    >>> x=Variable
    >>> intersect([x(1),x(2)+1],[x(1),x(2)])
    [x(1)]
    """
    uv=used_vars_set(i)*used_vars_set(j)
    t=Variable(0)
    if uv.reducible_by(t):
        raise ValueError, "First ring variable has to be reserved as helper variable t"
    if not t>uv:
        raise ValueError, "need elimination ordering for first ring variable"
    gb=groebner_basis(list(chain((t*p for p in i),((1+t)*p for p in j))),**gb_opts)
    return [p for p in gb if p.navigation().value()>t.index()]

def _test():
    import doctest
    doctest.testmod()

if __name__ == "__main__":
    _test()
