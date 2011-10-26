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

def monomial_new_init(self, arg=None):
    """
    Constructor of the class Monomial
    Constructs a Monomial from
      - Monomial
      - Variable
      - sequence of variables
    """
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
    if second != None:
        booleset_old_init(self, arg, second)
    else:
        try:
            booleset_old_init(self,arg)
        except:
            s=set()
            v=BoolePolynomialVector()
            arglist = list(arg)
            for i in arglist:
		s.add(Monomial(i))
            for i in s:
                v.append(i)
            p=add_up_polynomials(v, Polynomial(arglist[0].ring().zero()))
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
def add_up_polynomials(polys, init):
    """
    Adds up the polynomials in polys (which should be a BoolePolynomialVector or a sequence of ???
    """
    if not isinstance(polys, BoolePolynomialVector):
        vec = BoolePolynomialVector
        for p in polys:
            vec.append(p)
        polys=vec

    return _add_up_polynomials(polys, init)

old_ring_var=Ring.var
def ring_var(self, i):
    """Deprecated; use Ring.variable(...). """ 
    warnings.warn('Ring.var is deprectated; use Ring.variable instead')
    return old_ring_var(self, i)

ring_var.__doc__ += old_ring_var.__doc__
Ring.var=ring_var

def weakringref_call(self):
    if self.is_valid():
        return self.deref()
    return None

WeakRingRef.__call__ = weakringref_call

_cpp_ring_init = Ring.__init__
_cpp_change_ordering = Ring.change_ordering
_cpp_set_variable_name = Ring.set_variable_name
_cpp_append_block = Ring.append_block
_cpp_ring_clone = Ring.clone

def _ring_settings(ring, names, blocks):
    for (idx, elt) in enumerate(names):
        _cpp_set_variable_name(ring, idx, elt)

    for elt in blocks:
        _cpp_append_block(ring, elt)


def _ring_init(self, first, ordering=None, names=[], blocks=[]):
    """Ring(n, ordering, names, block) generates a new Boolean 
    polynomial ring with n variables, given  monomial ordering, variable
    names and block ordering blocks, if given.
    Further information/call patterns: """""" """
    if ordering is None:
        _cpp_ring_init(self, first)
    else:
        _cpp_ring_init(self, first, ordering)
    _ring_settings(self, names, blocks)

_ring_init.__doc__ += _cpp_ring_init.__doc__
Ring.__init__ = _ring_init

def _ring_clone(self, ordering=None, names=[], blocks=[]):
    """ring.clone(ordering=..., names=..., block=...) generates a shallow copy
    of ring, but with different ordering, names or blocks if given.
    Further information/call patterns: """
    ring = _cpp_ring_clone(self)
    if ordering is not None:
        _cpp_change_ordering(ring, ordering)

    _ring_settings(ring, names, blocks)

    return ring

_ring_clone.__doc__ += _cpp_ring_clone.__doc__
Ring.clone = _ring_clone

def _change_ordering(self, ordercode):
    """ Deprecated; use Ring.clone(ordering=...). """
    warnings.warn('Ring.change_ordering is deprectated:\
    use Ring.clone(ordering=...) instead')
    return _cpp_change_ordering(self, ordercode)

_change_ordering.__doc__ += _cpp_change_ordering.__doc__
Ring.change_ordering = _change_ordering

def _set_variable_name(self, idx, name):
    """ Deprecated; use Ring.clone(names=...). """
    warnings.warn('Ring.set_variable_name is deprectated:\
    use Ring.clone(names=...) instead')
    return _cpp_set_variable_name(self, idx, name)    

_set_variable_name.__doc__ += _cpp_set_variable_name.__doc__
Ring.set_variable_name = _set_variable_name

def _append_block(self, next_block_start):
    """Deprectated; use Ring.clone(blocks=...). """
    warnings.warn('Ring.append_block is deprectated:\
    use Ring.clone(blocks=...) instead')
    return _cpp_append_block(self, next_block_start)    

_append_block.__doc__ += _cpp_append_block.__doc__
Ring.append_block = _append_block



