from polybori.nf import *
import polybori.aes as aes
import polybori.coding as coding
from time import time
from copy import copy
from itertools import chain
from inspect import getargspec
from statistics import used_vars
from heuristics import dense_system,gauss_on_linear
#class PolyBoRiDefaultOptions:
#  def __init__(self):
#      self.opts = {}
#
#  def __call__(self, fname):
#      return self.opts[fname]
#
#  def set(self, fname, opts):
#      self.opts[fname] = opts
               

#default_options = PolyBoRiDefaultOptions()
default_options=dict()
#@Alexander: To much abstraction is not a good choice for scripting
# if you want to override, add functionality, I would inherit from dict
# P.S.: I already made the same error


    
#strategy brainstorming

#count number of unbound vars to see, if ll is a good choice

#-direct computation
#-ll first
#-dp_asc first (don't need walk, as result is usually quite nice)
#-conjunction and factoring first
#-inversion

#LA at few variables and no ll
#LA at "dense" systems


#@TODO: implement this to work with *args, **args for wrapped function


def firstgb_heuristic(d):
    I=d["I"]
    if not "faugere" in d:
        if (have_degree_order() and len(used_vars(I))<200) or (dense_system(I) and len(used_vars(I))<100):
            d["faugere"]=True
            if not "red_tail" in d:
                d["red_tail"]=False
            if not "selection_size" in d:
                d["selection_size"]=10000
            if not ("ll" in d):
                d["ll"]=True

    return d
def trivial_heuristic(d):
    
    return d
class HeuristicalFunction(object):
    def __call__(self,*args,**kwds):
        complete_dict=copy(kwds)
        for (k,v) in zip(self.argnames,args):
            complete_dict[k]=v
        complete_dict=self.heuristicFunction(complete_dict)
        return self.f(**complete_dict)
    def __init__(self,f,heuristic_function):
        (self.argnames,self.varargs,self.varopts,self.defaults)=getargspec(f)
        self.options=dict(zip(self.argnames[-len(self.defaults):],self.defaults))
        self.heuristicFunction=heuristic_function
        self.f=f
        self.__doc__=f.__doc__+"\nOptions are:\n"+"\n".join((k+"  :  "+repr(self.options[k]) for k in self.options))
        
def with_heuristic(heuristic_function):
    def make_wrapper(f):
        wrapped=HeuristicalFunction(f,heuristic_function)
        wrapped.__name__=f.__name__
        return wrapped
    return make_wrapper


@with_heuristic(firstgb_heuristic)
def groebner_basis(I, faugere=False,  coding=False,
       preprocess_only=False, selection_size= 1000,
       full_prot= False, recursion= False, invert= False,
       prot= False, step_factor= 1,
       deg_bound= 1000000000000L, lazy= True, ll= False,
       max_growth= 2.0, exchange= True,
       matrix_prefix= "matrix", red_tail= True,
       implementation="Python", aes= False,
       llfirst= False, noro= False, implications= False,
       draw_matrices= False, llfirstonthefly= False,
       linearAlgebraInLastBlock= True, gauss_on_linear_first=True):
    """Computes a Groebner basis of a given ideal I, w.r.t options."""


    if gauss_on_linear_first:
        I=gauss_on_linear(I)
    import nf
    if full_prot:
        prot=True
    
    nf.print_matrices=draw_matrices
    nf.matrix_prefix=matrix_prefix
    
    if implementation=="Python":
        implementation=buchberger_C_based2
    else:
        implementation=symmGB_F2_C
    
    
    if aes:
        pt=time()
        I=aes.preprocess(I, prot=prot)
        pt2=time()
        if prot:
          print "preprocessing time", pt2-pt
    if coding:
      pt=time()
      I=coding.preprocess(I,prot=prot)
      pt2=time()
      if prot:
        print "preprocessing time", pt2-pt

    if invert:
       I=[p.mapEveryXToXPlusOne() for p in I]
    
            
    
    if preprocess_only:
      for p in I:
        print p
      del p
      del I
      clean_data(mydata)
      import sys
      sys.exit(0)    
    I=implementation(I, optRedTail=red_tail,\
        max_growth=max_growth, step_factor=step_factor,
        implications=implications,prot=prot,
        full_prot=full_prot,deg_bound=deg_bound,
        selection_size=selection_size, optLazy=lazy, 
        optExchange=exchange, optAllowRecursion=recursion,
        use_faugere=faugere,
        use_noro=noro,ll=ll,
        optLinearAlgebraInLastBlock=linearAlgebraInLastBlock)
    return I
