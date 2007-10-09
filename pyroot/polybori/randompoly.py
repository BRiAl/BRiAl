from polybori.PyPolyBoRi import *
from random import Random


def gen_random_poly(l,deg=10,vars_set=list(xrange(100)),seed=123):
    myrange=vars_set
    r=Random(seed)
    def helper(samples):
        if samples==0:
            return Polynomial(0)
        if samples==1:
            d=r.randint(0,deg)
            variables=r.sample(myrange,d)
            m=Monomial()
            for v in sorted(variables,reverse=True):
                m=m*Variable(v)
            return Polynomial(m)
        assert samples>=2
        return helper(samples/2)+helper(samples-samples/2)
    p=Polynomial(0)
    while(len(p)<l):
        p=Polynomial(p.diagram().union(helper(l-len(p)).diagram()))
    return p

