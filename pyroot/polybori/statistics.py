from polybori.PyPolyBoRi import Monomial,Polynomial,Variable
def used_vars(l):
    m=Monomial()
    for p in l:
        m=m*Polynomial(p).vars()
    return m
def used_vars_set(l):
    m=Monomial()
    s=set()
    for p in l:
        s.update(Polynomial(p).vars())
    for v in reversed(list(s)):
        m=Variable(v)*m
    return m