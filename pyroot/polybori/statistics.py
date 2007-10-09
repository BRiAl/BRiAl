from polybori.PyPolyBoRi import Monomial,Polynomial
def used_vars(l):
    m=Monomial()
    for p in l:
        m=m*Polynomial(p).vars()
    return m