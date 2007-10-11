from polybori.nf import *
import polybori.aes as aes
import polybori.coding as coding
from time import time

class PolyBoRiDefaultOptions:
  def __init__(self):
      self.opts = {}

  def __call__(self, fname):
      return self.opts[fname]

  def set(self, fname, opts):
      self.opts[fname] = opts
               

default_options = PolyBoRiDefaultOptions()

default_options.set( 'groebner_basis',
                     { 'faugere': False,  'coding': False,
                       'preprocess_only': False, 'selection_size': 1000,
                       'full_prot': False, 'recursion': False, 'invert': False,
                       'prot': False, 'step_factor': 1,
                       'deg_bound': 1000000000000L, 'lazy': True, 'll': False,
                       'max_growth': 2.0, 'exchange': True,
                       'matrix_prefix': 'matrix', 'red_tail': True,
                       'implementation': 'Python', 'aes': False,
                       'llfirst': False, 'noro': False, 'implications': False,
                       'draw_matrices': False, 'llfirstonthefly': False } )

def groebner_basis(I, **kwargs):
    """Computes a Groebner basis of a given ideal I, w.r.t options."""

    class toAttr:
        def __init__(self,indict):
            self.stuff = indict
        def __getattr__(self,which):
            return self.stuff.get(which,None)

    options = default_options('groebner_basis')
    options.update(kwargs)
    options = toAttr(options)

    import nf
    if options.full_prot:
        options.prot=True
    
    nf.print_matrices=options.draw_matrices
    nf.matrix_prefix=options.matrix_prefix
    
    if options.implementation=="Python":
        implementation=buchberger_C_based2
    else:
        implementation=symmGB_F2_C
    
    
    if options.aes:
        pt=time()
        I=aes.preprocess(I, prot=options.prot)
        pt2=time()
        if options.prot:
          print "preprocessing time", pt2-pt
    if options.coding:
      pt=time()
      I=coding.preprocess(I,prot=options.prot)
      pt2=time()
      if options.prot:
        print "preprocessing time", pt2-pt

    if options.invert:
       I=[p.mapEveryXToXPlusOne() for p in I]
    
            
    
    if options.preprocess_only:
      for p in I:
        print p
      del p
      del I
      clean_data(mydata)
      import sys
      sys.exit(0)    
    I=implementation(I, optRedTail=options.red_tail,\
        max_growth=options.max_growth, step_factor=options.step_factor,
        implications=options.implications,prot=options.prot,
        full_prot=options.full_prot,deg_bound=options.deg_bound,
        selection_size=options.selection_size, optLazy=options.lazy, 
        optExchange=options.exchange, optAllowRecursion=options.recursion,
        use_faugere=options.faugere,
        use_noro=options.noro,ll=options.ll)
    return I
