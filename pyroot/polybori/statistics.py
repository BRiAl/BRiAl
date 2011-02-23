from polybori.PyPolyBoRi import Monomial,Polynomial,Variable,top_index

def used_vars(l, bound=None):
    m=Monomial()
    for p in l:
        m=m*Polynomial(p).vars_as_monomial()
        if bound and len(m) > bound:
            return m
    return m

def used_vars_set(l,bound=None):
    m = Monomial()
    s=set()
    for p in l:
        s.update(Polynomial(p).vars_as_monomial().variables())
        if bound and len(s)>bound:
            break
    sorted_s = sorted(list(s),key=top_index,reverse=True)
    if sorted_s:
        m = sorted_s[0]
        for v in sorted_s[1:]:
            m=v*m
    return m
