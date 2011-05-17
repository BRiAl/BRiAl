from sys import modules
from itertools import chain
import re
import warnings
compatibility_mode=False
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
    """
    Constructor of the class Monomial
    Constructs a Monomial from
      - Monomial
      - Variable
      - sequence of variables
    """
    if not arg:
        monomial_old_init(self,global_ring())
    else:
        try:
            monomial_old_init(self,arg)
        except:
            items=sorted((x for x in arg),reverse=True, key=top_index)
            prototype=Monomial(items[0])
            for x in items[1:]:
                prototype*=x
            monomial_old_init(self,prototype)

Monomial.__init__=monomial_new_init

booleset_old_init=BooleSet.__init__
def booleset_new_init(self,arg=None, second=None):
    """
    Constructor of the class BooleSet (constructs a BooleSet from a CCuddNavigator
       arg    : of type polybori.dynamic.PyPolyBoRi.CCuddNavigator
       second : of type polybori.dynamic.PyPolyBoRi.BooleRing 
    """
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
            for i in list(arg):
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

_gauss_on_polys=gauss_on_polys
def gauss_on_polys(l):
    vec=BoolePolynomialVector(l)
    return list(_gauss_on_polys(vec))

snake_pattern=re.compile('(((?<=[a-z])[A-Z])|([A-Z](?![A-Z]|$)))')
def to_snake_case(s):
    return snake_pattern.sub('_\\1', s).lower().strip('_')
    
def warn_snake_case(a, snaked):
    warnings.warn("access to %s is deprecated, use %s instead" % (a, snaked), stacklevel=3)
def _strategy_getattr(self, a):
    if hasattr(self.reduction_strategy, a):
        return getattr(self.reduction_strategy,a)
    snaked=to_snake_case(a)

    if snaked in self.__dict__:
        warn_snake_case(a, snaked)
        return self.__dict__[snaked]
    if hasattr(self.reduction_strategy, snaked):
        warn_snake_case(a, snaked)
        return getattr(self.reduction_strategy, snaked)
    raise AttributeError
def _strategy_setattr(self, a, v):
    if a in self.__dict__:
        self.__dict__[a]=v
        return
    if hasattr(self.reduction_strategy, a):
        return setattr(self.reduction_strategy, a, v)
    snaked=to_snake_case(a)
    if snaked in self.__dict__:
        warn_snake_case(a, snaked)
        return setattr(self, snaked, v)
    
    if hasattr(self.reduction_strategy, snaked):
        warn_snake_case(a, snaked)
        return setattr(self.reduction_strategy, snaked, v)
    self.__dict__[a]=v
if compatibility_mode:
    GroebnerStrategy.__getattr__=_strategy_getattr
    GroebnerStrategy.__setattr__=_strategy_setattr

_add_up_polynomials=add_up_polynomials
def add_up_polynomials(polys):
    """
    Adds up the polynomials in polys (which should be a BoolePolynomialVector or a sequence of ???
    """
    if not isinstance(polys, BoolePolynomialVector):
        vec = BoolePolynomialVector
        for p in polys:
            vec.append(p)
        polys=vec
    return _add_up_polynomials(polys)
