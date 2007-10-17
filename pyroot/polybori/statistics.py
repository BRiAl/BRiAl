from polybori.PyPolyBoRi import Monomial,Polynomial,Variable
def used_vars(l):
    m=Monomial()
    for p in l:
        m=m*Polynomial(p).vars()
    return m

def used_vars(l, bound):
    m=Monomial()
    for p in l:
        m=m*Polynomial(p).vars()
        if len(m) > bound:
            return m
    return m

def used_vars_set(l):
    m=Monomial()
    s=set()
    for p in l:
        s.update(Polynomial(p).vars())
    for v in reversed(list(s)):
        m=Monomial(Variable(v))*m
    return m
    

def used_vars_set(l,bound=None):
    m=Monomial()
    s=set()
    for p in l:
        s.update(Polynomial(p).vars())
        if bound and len(s)>bound:
            break
    for v in reversed(list(s)):
        m=Monomial(Variable(v))*m
    return m
