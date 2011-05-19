from polybori.PyPolyBoRi import Monomial,Polynomial,top_index

def used_vars(l, bound=None):
    if not l:
        return []
    m = Monomial(Polynomial(iter(l).next()).vars_as_monomial())
    for p in l[1:]:
        m=m*Polynomial(p).vars_as_monomial()
        if bound and len(m) > bound:
            return m
    return m

def used_vars_set(l,bound=None):
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
