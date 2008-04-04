from polybori.PyPolyBoRi import GroebnerStrategy, Polynomial

def interred(l):
    """computes a new generating system (g1, ...,gn), spanning the same ideal modulo field equations.
    The system is interreduced: For i!=j: gi.lead() does not divide any term of gj
    """
    l=[Polynomial(p) for p in l if not p==0]
    l_old=None
    l=tuple(l)
    while l_old!=l:
        l_old=l
        l=sorted(l,key=Polynomial.lead)
        g=GroebnerStrategy()
        g.optRedTail=True
        for p in l:
            p=g.nf(p)
            if not p.isZero():
                g.addGenerator(p)
        l=tuple(g)
    return l