from polybori.PyPolyBoRi import Monomial,Polynomial,Variable

def used_vars(l, bound=None):
    m=Monomial()
    for p in l:
        m=m*Polynomial(p).vars()
        if bound and len(m) > bound:
            return m
    return m

def key(v):
    return v.index()
def used_vars_set(l,bound=None):
    m=Monomial()
    s=set()
    for p in l:
        s.update(Polynomial(p).vars())
        if bound and len(s)>bound:
            break
    for v in sorted(list(s),key=key,reverse=True):
        m=v*m
    return m
