from polybori.PyPolyBoRi import *
from polybori.specialsets import all_monomials_of_degree_d
def simple_graded(f, d):
    return sum((t for t in f if t.deg()==d))
def friendly_graded(f, d):
    vec=BoolePolynomialVector()
    for t in f:
        if t.deg()!=d:
            continue
        else:
            vec.append(t)
    return add_up_polynomials(vec)
def highlevel_graded(f,d):
    return Polynomial(f.set().intersect(all_monomials_of_degree_d(d,f.vars())))
def recursive_graded(f,d):
    def do_recursive_graded(f,d):
        if f.empty():
            return f
        if d==0:
            if Monomial() in f:
                return Polynomial(1).set()
            else:
                return BooleSet()
        else:
            nav=f.navigation()
            if nav.constant():
                return BooleSet()
            return if_then_else(
                nav.value(),
                do_recursive_graded(BooleSet(nav.thenBranch()),d-1),
                do_recursive_graded(BooleSet(nav.elseBranch()),d))
    return Polynomial(do_recursive_graded(f.set(),d))
        