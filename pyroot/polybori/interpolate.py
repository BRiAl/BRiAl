# Copyright (c) 2005-2007 by The PolyBoRi Team
import sys
from polybori.PyPolyBoRi import *
from polybori.randompoly import gen_random_poly
from random import Random
from time import clock


generator=Random()

def add_up_poly_list(l):
    v=BoolePolynomialVector()
    for p in l:
        v.append(p)
    return add_up_polynomials(v)
def bench_interpolate(degree,nvariables,points):
    
    d=degree
    v=nvariables
    c=points
    h=len(points)/2
    part1=generator.sample(c,h)
    part1=add_up_poly_list(part1)
    part2=c+part1
    p=part1
    q=part2
    assert part1.set().intersect(part2).empty()
    c1=clock()
    res2=interpolate_smallest_lex(p,q)
    c2=clock()
    print "finished interpolate_smallest_lex(p,q),len:",len(res2),"time",c2-c1
    c1=clock()
    res1=interpolate(p,q)
    c2=clock()
    print "finished interpolate(p,q)"+len("_smallest_lex")*" "+",len:",res1.set().sizeDouble(),"time:",c2-c1
    return res2



def nf_lex_points(f,p):
    f=Polynomial(f)
    p=BooleSet(p)
    z=f.zerosIn(p)
    return interpolate_smallest_lex(z,p.diff(z))

def gen_random_monomial():
    d=generator.randrange(min(6,v+1))
    variables=generator.sample(xrange(v),d)
    variables=sorted(variables,key=top_index,reverse=True)
    m=Monomial()
    for x in variables:
        m=x*m
    return m
def gen_random_polynomial(max_len=50):
    vec=BoolePolynomialVector()
    for i in xrange(max_len):
        vec.append(gen_random_monomial())
    return add_up_polynomials(vec)


def gen_random_o_z(points,points_p):
    k=generator.randrange(len(points)+1)
    ones=generator.sample(points,k)
    vec=BoolePolynomialVector()
    for p in ones:
        vec.append(p)
    ones=add_up_polynomials(vec)
    return interpolate_smallest_lex(points_p.set().diff(ones),ones)
def variety_lex_leading_terms(points,variables):
    standards=BooleSet()
    points_tuple=tuple(points)
    myvars_div=variables.divisors()
    myvars_iter=iter(myvars_div)
    if points!=myvars_div:
        standards=BooleSet(Polynomial(1))
    len_standards=len(standards)
    standards_old=standards
    while len_standards<len(points):
        standards=standards.union(gen_random_o_z(points_tuple,points))
        
        if standards_old!=standards:
            standards=BooleSet(standards).includeDivisors()
            len_standards=len(standards)
            standards_old=standards

    leading_terms=BooleSet(myvars_div.diff(standards)).minimalElements()
    return leading_terms
def lex_groebner_basis_points(points,variables):
    leads=variety_lex_leading_terms(points,variables)
    return [nf_lex_points(l,points)+l for l in leads]

def lex_groebner_basis_for_polynomial_via_variety(p):
    variables=p.varsAsMonomial()
    return lex_groebner_basis_points(p.zerosIn(variables.divisors()),variables)
if __name__=='__main__':
    nvariables=100
    r=Ring(nvariables)
    for number_of_points in (100,500,1000,2000,3000,4000,5000,10000,20000,50000,100000):
        print "----------"
        print "number_of_points:",number_of_points
        print "generate points"
        points=gen_random_poly(number_of_points,nvariables,[Variable(i) for i in range(nvariables)])
        print "points generated"
        bench_interpolate(nvariables,nvariables,points)
        vars_mon=Monomial()
        for i in reversed(range(nvariables)):
            vars_mon=vars_mon*Variable(i)
        print len(variety_lex_leading_terms(points,vars_mon)), "elements in groebner basis"
