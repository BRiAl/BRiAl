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
    uv=used_vars_set(i)*used_vars_set(j)
    t=Variable(0)
    if uv.reducibleBy(t):
        raise ValueError, "First ring variable has to be reserved as helper variable t"
    if not t>uv:
        raise ValueError, "need elimination ordering for first ring variable"
    gb=groebner_basis(list(chain((t*p for p in i),((1+t)*p for p in j))),**gb_opts)
    return [p for p in gb if p.navigation().value()>t.index()]