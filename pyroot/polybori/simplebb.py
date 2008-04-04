from polybori.PyPolyBoRi import *
from polybori.interred import interred
def buchberger(l):
    "calculates a (non minimal) Groebner basis"
    l=interred(l)
    #for making sure, that every polynomial has a different leading term
    #needed for addGenerator
    g=GroebnerStrategy()
    for p in l:
        g.addGenerator(p)
    while g.npairs()>0:
        g.cleanTopByChainCriterion()
        p=g.nextSpoly()
        p=g.nf(p)
        if not p.isZero():
            g.addGenerator(p)
    return list(g)
        