from sys import modules
from itertools import chain
# First try, whether PyPolyBoRi is already in modules (e.g. static profiling)
try:
    pb = modules["PyPolyBoRi"]
    for k in dir(pb):
        globals()[k]=getattr(pb,k)   
except:
    from polybori.dynamic.PyPolyBoRi import *
def replace_init_proc(cls):
    old_init=cls.__init__
    def init(self,iteratable=None):
        old_init(self)
        if iteratable:
            for item in iteratable:
                self.append(item)
    cls.__init__=init
    
replace_init_proc(BoolePolynomialVector)
replace_init_proc(IntVector)

monomial_old_init=Monomial.__init__
def monomial_new_init(self,arg=None):
    if arg==None:
        monomial_old_init(self)
    else:
        try:
            monomial_old_init(self,arg)
        except:
            monomial_old_init(self)
            items=sorted((x.index() for x in arg),reverse=True)
            for i in items:
                self*=Variable(i)
Monomial.__init__=monomial_new_init
booleset_old_init=BooleSet.__init__
def booleset_new_init(self,arg=None):
    if arg==None:
        booleset_old_init(self)
    else:
        try:
            booleset_old_init(self,arg)
        except:
            s=set()
            v=BoolePolynomialVector()
            for i in arg:
                s.add(Monomial(i))
            for i in s:
                v.append(i)
            p=add_up_polynomials(v)
            booleset_old_init(self,p.set())
BooleSet.__init__=booleset_new_init

for k in OrderCode.values:
    globals()[str(OrderCode.values[k])]=OrderCode.values[k]
