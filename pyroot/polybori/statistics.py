from polybori.PyPolyBoRi import Monomial,Polynomial,Variable,top_index

def used_vars(l, bound=None):
    m=Monomial()
    for p in l:
        m=m*Polynomial(p).varsAsMonomial()
        if bound and len(m) > bound:
            return m
    return m

def used_vars_set(l,bound=None):
    m=Monomial()
    s=set()
    for p in l:
        s.update(Polynomial(p).varsAsMonomial().variables())
        if bound and len(s)>bound:
            break
    for v in sorted(list(s),key=top_index,reverse=True):
        m=v*m
    return m
