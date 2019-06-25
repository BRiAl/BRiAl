# Copyright (c) 2005-2007 by The PolyBoRi Team

from __future__ import print_function

import sys
from .PyPolyBoRi import *
from .randompoly import gen_random_poly
from random import Random
try:
    from time import process_time as clock
except ImportError:
    from time import clock

generator = Random()


def add_up_poly_list(l, init):
    v = BoolePolynomialVector()
    for p in l:
        v.append(p)
    return add_up_polynomials(v, init)


def bench_interpolate(degree, nvariables, points):

    d = degree
    v = nvariables
    c = points
    h = len(points) / 2
    terms = set(c.terms())
    part1 = generator.sample(terms, h)
    part1 = add_up_poly_list(part1, Polynomial(c.ring().zero()))
    part2 = c + part1
    p = part1
    q = part2
    assert part1.set().intersect(part2).empty()
    c1 = clock()
    res2 = interpolate_smallest_lex(p, q)
    c2 = clock()
    print("finished interpolate_smallest_lex(p,q),len:", len(res2), "time", c2
        - c1)
    c1 = clock()
    res1 = interpolate(p, q)
    c2 = clock()
    print("finished interpolate(p,q)" + len("_smallest_lex") * " " + ",len:",
        res1.set().size_double(), "time:", c2 - c1)
    return res2


def nf_lex_points(f, p):
    f = Polynomial(f)
    p = BooleSet(p)
    z = f.zeros_in(p)
    return interpolate_smallest_lex(z, p.diff(z))


def gen_random_monomial():
    d = generator.randrange(min(6, v + 1))
    variables = generator.sample(range(v), d)
    variables = sorted(variables, key=top_index, reverse=True)
    m = variables[0]
    for x in variables[1:]:
        m = x * m
    return m


def gen_random_polynomial(ring, max_len=50):
    vec = BoolePolynomialVector()
    for i in range(max_len):
        vec.append(gen_random_monomial(ring))
    return add_up_polynomials(vec, Polynomial(ring.zero()))


def gen_random_o_z(points, points_p):
    k = generator.randrange(len(points) + 1)
    ones = generator.sample(points, k)
    vec = BoolePolynomialVector()
    for p in ones:
        vec.append(p)
    ones = add_up_polynomials(vec, Polynomial(points_p.ring().zero()))
    return interpolate_smallest_lex(points_p.set().diff(ones), ones)


def variety_lex_leading_terms(points, variables):
    ring = variables.ring()
    standards = BooleSet(ring.zero())
    assert type(points) == BooleSet, "Points needs to be a BooleSet"
    points_tuple = tuple(points)
    myvars_div = variables.divisors()
    myvars_iter = iter(myvars_div)
    if points != myvars_div:
        standards = BooleSet(ring.one())
    len_standards = len(standards)
    standards_old = standards
    while len_standards < len(points):
        standards = standards.union(gen_random_o_z(points_tuple, points))

        if standards_old != standards:
            standards = BooleSet(standards).include_divisors()
            len_standards = len(standards)
            standards_old = standards

    leading_terms = BooleSet(myvars_div.diff(standards)).minimal_elements()
    return leading_terms


def lex_groebner_basis_points(points, variables):
    leads = variety_lex_leading_terms(points, variables)
    return [nf_lex_points(l, points) + l for l in leads]


def lex_groebner_basis_for_polynomial_via_variety(p):
    variables = p.vars_as_monomial()
    return lex_groebner_basis_points(p.zeros_in(variables.divisors()),
        variables)
if __name__ == '__main__':
    nvariables = 100
    r = declare_ring([Block("x", nvariables)])
    for number_of_points in (100, 500, 1000, 2000, 3000, 4000, 5000, 10000,
        20000, 50000, 100000):
        print("----------")
        print("number_of_points:", number_of_points)
        print("generate points")
        points = gen_random_poly(number_of_points, nvariables, [Variable(i)
            for i in range(nvariables)])
        print("points generated")
        bench_interpolate(nvariables, nvariables, points)
        vars_mon = Monomial(r)
        for i in reversed(range(nvariables)):
            vars_mon = vars_mon * Variable(i, r)
        print(len(variety_lex_leading_terms(points, vars_mon)),
            "elements in groebner basis")
