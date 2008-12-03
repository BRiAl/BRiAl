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
            prototype=Monomial()
            items=sorted((x for x in arg),reverse=True, key=top_index)
            for x in items:
                prototype*=x
            monomial_old_init(self,prototype)
Monomial.__init__=monomial_new_init
booleset_old_init=BooleSet.__init__
def booleset_new_init(self,arg=None, second=None):
    if arg==None:
        booleset_old_init(self)
    elif second != None:
        booleset_old_init(self, arg, second)
    else:
        try:
            booleset_old_init(self,arg)
        except:
            s=set()
            v=BoolePolynomialVector()
            for i in set(arg):
                s.add(Monomial(i))
            for i in s:
                v.append(i)
            p=add_up_polynomials(v)
            booleset_old_init(self,p.set())
BooleSet.__init__=booleset_new_init

for k in OrderCode.values:
    globals()[str(OrderCode.values[k])]=OrderCode.values[k]

monomial_cplusplus_div = Monomial.__div__

def monomial_python_div(self, arg):
    try:
        return monomial_cplusplus_div(self, arg)
    except ValueError:
        return Polynomial(0, self.ring())

Monomial.__div__ = monomial_python_div

variable_cplusplus_div = Variable.__div__

def variable_python_div(self, arg):
    try:
        return variable_cplusplus_div(self,arg)
    except ValueError:
        return Polynomial(0, self.ring())

Variable.__div__ = variable_python_div
