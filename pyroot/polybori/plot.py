#!/usr/bin/env python
# encoding: utf-8
"""
untitled.py

Created by Michael Brickenstein on 2008-10-17.
Copyright (c) 2008 Mathematisches Forschungsinstitut Oberwolfach. All rights reserved.
"""

import sys
import os
from polybori.PyPolyBoRi import Ring, Polynomial, Variable, BooleSet

graph_template="""
digraph polynomial{
graph [ ordering="out" ];

#for n in nodes
${identifier(n)}[label="${label(n)}", shape="${shape(n)}"];
#end

#for n in non_constant_nodes
${identifier(n)} -> ${identifier(n.else_branch())} [style="dotted"];
${identifier(n)} -> ${identifier(n.then_branch())};
#end
}
"""



def plot(p):
    """docstring for plot"""
    def find_navs(nav):
        if not nav in nodes:
            nodes.add(nav)
            if not nav.constant():
                find_navs(nav.then_branch())
                find_navs(nav.else_branch())
    p=Polynomial(p)
    nodes=set()
    nav=p.navigation()
    find_navs(nav)
    non_constant_nodes=[n for n in nodes if not n.constant()]
    node_to_int=dict([(n,i) for (i,n) in enumerate(nodes)])
    
    
    r=p.ring()
    def identifier(n):
        return "n"+str(node_to_int[n])
    def label(n):
        if n.constant():
            if n.terminal_one():
                return "1"
            else:
                return "0"
        else:
            return str(r.var(n.value()))
    def shape(n):
        if n.constant():
            return "box"
        else:
            return "ellipse"
    from genshi.template import TextTemplate
    tmpl = TextTemplate(graph_template)
    stream = tmpl.generate(**locals())
    return str(stream)
    
        
def main():
    r=Ring(1000)
    x=Variable
    from os import system
    from polybori.specialsets import all_monomials_of_degree_d, power_set
    full_set=list(power_set([Variable(i) for i in xrange(15)]))
    from random import Random
    generator=Random(123)
    random_set=sum(generator.sample(full_set,30))
    full_polynomial=list(all_monomials_of_degree_d(3, [Variable(i) for i in xrange(30)]))
    random_poly=sum(generator.sample(full_polynomial,30))
    polynomials=[
    x(1)*x(2)+x(3),
    (x(1)+1)*(x(2)+x(3)),
    (x(1)+1)*(x(2)+1)*(x(3)+1),
    x(1)*x(2)+x(2)*x(3)+x(1)*x(3)+x(1),
    x(0)+x(1)+x(2)+x(3)+x(4)+x(5),
    all_monomials_of_degree_d(3,[x(i) for i in xrange(10)]),
    power_set([x(i) for i in xrange(10)]),
    random_poly,
    random_set,
    Polynomial(all_monomials_of_degree_d(3,[x(i) for i in xrange(10)])) +
        Polynomial(power_set([x(i) for i in xrange(10)])),
    Polynomial(power_set([x(i) for i in xrange(10)]))+1
    ]
    for (i,p) in enumerate(polynomials):
        dot=plot(p)
        dot_file=str(i) +".dot"
        f=open(dot_file, "w")
        f.write(dot)
        f.close()
        png_file=str(i)+".png"
        system("dot -Tpng -o "+png_file+" " + dot_file)
        
if __name__ == '__main__':
    main()