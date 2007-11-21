from polybori.nf import *
import polybori.aes as aesmod
import polybori.coding as coding
from polybori.PyPolyBoRi import *
from polybori.ll import eliminate
from time import time
from copy import copy
from itertools import chain
from inspect import getargspec
from statistics import used_vars, used_vars_set
from heuristics import dense_system,gauss_on_linear

def owns_one_constant(I):
    """Determines whether I contains the constant one polynomial."""
    for p in I:
        if p.isOne():
            return True
    return False


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

def ll_is_good(I):
    lex_lead=set()
    for p in I:
        m=p.lexLead()
        if m.deg()==1:
            lex_lead.add(iter(m).next())
    if len(lex_lead)>=0.8*len(I):
        uv=len(used_vars_set(I))
        if len(lex_lead)>0.9*uv:
            return True
    return False
    
def firstgb_heuristic(d):
    d_orig=d
    d=copy(d)
    def want_la():
        n_used_vars=None
        bound=None
        if have_degree_order():
            new_bound=200
            n_used_vars=len(used_vars_set(I,bound=new_bound))
            if n_used_vars<new_bound:
                return True
            bound=new_bound
        if dense_system(I):
            new_bound=100
            if not (bound and new_bound<bound):
                n_used_vars=len(used_vars_set(I,bound=new_bound))
                bound=new_bound
            if n_used_vars<bound:
                return True
        return False 
    I=d["I"]
    if not "faugere" in d:
        if want_la():

            d["faugere"]=True
            if not "red_tail" in d:
                d["red_tail"]=False
            if not "selection_size" in d:
                d["selection_size"]=10000
            if not ("ll" in d):
                d["ll"]=True
    if not "other_ordering_first" in d:
        #TODO after ll situation might look much different, so heuristic is on wrong place
        if get_order_code()==OrderCode.lp:
            max_non_linear=len(I)/2
            non_linear=0
            for p in I:
                if p.lead().deg()>1:
                    non_linear=non_linear+1
                    if non_linear>max_non_linear:
                        break
            if non_linear>max_non_linear:
                other_ordering_opts=copy(d_orig)
                other_ordering_opts["switch_to"]=OrderCode.dlex
                d["other_ordering_first"]=other_ordering_opts
    if (not "llfirstonthefly" in d) and (not "llfirst" in d) and ll_is_good(I):
        d["llfirst"]=True
    return d
def trivial_heuristic(d):   
    return d
class HeuristicalFunction(object):
    def __call__(self,*args,**kwds):
        complete_dict=copy(kwds)
        heuristic=True
        try:
            heuristic=complete_dict["heuristic"]
            del complete_dict["heuristic"]
        except KeyError:
            pass


        for (k,v) in zip(self.argnames,args):
            complete_dict[k]=v 
        if heuristic:
            complete_dict=self.heuristicFunction(complete_dict)
        return self.f(**complete_dict)
    def __init__(self,f,heuristic_function):
        
        (self.argnames,self.varargs,self.varopts,self.defaults)=getargspec(f)
        if hasattr(f,"options"):
            self.options=f.options
        else:
            self.options=dict(zip(self.argnames[-len(self.defaults):],self.defaults))
        self.heuristicFunction=heuristic_function
        self.f=f
        self.__doc__=f.__doc__+"\nOptions are:\n"+"\n".join((k+"  :  "+repr(self.options[k]) for k in self.options))+"\nTurn off heuristic by setting heuristic=False"
        
def with_heuristic(heuristic_function):
    def make_wrapper(f):
        wrapped=HeuristicalFunction(f,heuristic_function)
        wrapped.__name__=f.__name__
        return wrapped
    return make_wrapper
def clean_polys(I):
    zero=Polynomial(0)
    I=[Polynomial(p) for p in I if p!=zero]
    return I
def clean_polys_pre(I):
    return (clean_polys(I),None) 
def gb_with_pre_post_option(option,pre=None,post=None,if_not_option=tuple(),default=False, pass_option_set=False):
    def make_wrapper(f):
        def wrapper(I,**kwds):
            for o in if_not_option:
                if (o in kwds and kwds[o]) or (o not in kwds and groebner_basis.options[o]):
                    option_set=False
            if not "option_set" in locals():
                if option in kwds:
                    
                    option_set=kwds[option]
                else:
                    option_set=default
            kwds=dict(((o,kwds[o]) for o in kwds if o!=option))
            state=None
            if option_set:
               if pre:
                   if not pass_option_set:
                       (I,state)=pre(I)
                   else:
                       (I,state)=pre(I,option_set)
               
            res=f(I,**kwds)
            if option_set:
                if post:
                    res=post(res,state)
                

            return res
        wrapper.__name__=f.__name__
        wrapper.__doc__=f.__doc__+"\n Setting invert=True input and output get a transformation x+1 for each variable x, which shouldn't effect the calculated GB, but the algorithm"
        if hasattr(f,"options"):
            wrapper.options=copy(f.options)
        else:
            (argnames,varargs,varopts,defaults)=getargspec(f)
            wrapper.options=dict(zip(argnames[-len(defaults):],defaults))

        wrapper.options[option]=default
        return wrapper
    return make_wrapper
def redsb_post(I,state):
    return I.minimalizeAndTailReduce()
def minsb_post(I,state):
    return I.minimalize()
def invert_all(I):
    return [p.mapEveryXToXPlusOne() for p in I]
def invert_all_pre(I):
    return (invert_all(I),None)
def invert_all_post(I,state):
    return invert_all(I)
    
def llfirst_pre(I):
    (eliminated,llnf, I)=eliminate(I,on_the_fly=False)
    return (I,eliminated)

def other_ordering_pre(I,options):
    ocode=get_order_code()
    assert ocode==OrderCode.lp
    #in parcticular it does not work for block orderings, because of the block sizes
    change_ordering(options["switch_to"])
    kwds=dict((k,options[k]) for k in options if not (k in ("other_ordering_first","switch_to","I")))
    I=groebner_basis(I,other_ordering_first=False,**kwds)
    change_ordering(ocode)
    return (I,None)
def llfirstonthefly_pre(I):
    (eliminated,llnf, I)=eliminate(I,on_the_fly=True)
    return (I,eliminated)
def llfirst_post(I,eliminated):
    for p in I:
        if p.isOne():
            return [p]
    else:
        if len(eliminated)>0:
            I=list(I)+list(eliminated)
            #redsb just for safety, as don't know how option is set
            I=groebner_basis(I,llfirst=False,llfirstonthefly=False,other_ordering_first=False,redsb=True)
    return I
    if not I.containsOne():
        for p in eliminated:
            I.addGenerator(p)
    I.symmGB_F2()
    return I
def result_to_list_post(I,state):
    return list(I)

@with_heuristic(firstgb_heuristic)
@gb_with_pre_post_option("result_to_list",post=result_to_list_post,default=True)
@gb_with_pre_post_option("clean_arguments",pre=clean_polys_pre,default=True)
@gb_with_pre_post_option("invert",pre=invert_all_pre,post=invert_all_post,default=False)
@gb_with_pre_post_option("llfirst",if_not_option=["llfirstonthefly"],pre=llfirst_pre,post=llfirst_post,default=False)
@gb_with_pre_post_option("llfirstonthefly",pre=llfirstonthefly_pre,post=llfirst_post,default=False)
@gb_with_pre_post_option("other_ordering_first",pre=other_ordering_pre,default=False,pass_option_set=True)
@gb_with_pre_post_option("minsb",post=minsb_post,if_not_option=["redsb"],default=True)
@gb_with_pre_post_option("redsb",post=redsb_post,default=True)

def groebner_basis(I, faugere=False,  coding=False,
       preprocess_only=False, selection_size= 1000,
       full_prot= False, recursion= False,
       prot= False, step_factor= 1,
       deg_bound= 1000000000000L, lazy= True, ll= False,
       max_growth= 2.0, exchange= True,
       matrix_prefix= "matrix", red_tail= True,
       implementation="Python", aes= False,
       llfirst= False, noro= False, implications= False,
       draw_matrices= False, llfirstonthefly= False,
       linearAlgebraInLastBlock=True, gauss_on_linear_first=True):
    """Computes a Groebner basis of a given ideal I, w.r.t options."""
    
    zero=Polynomial(0)
    I=[Polynomial(p) for p in I if p!=zero]

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
        I=aesmod.preprocess(I, prot=prot)
        pt2=time()
        if prot:
          print "preprocessing time", pt2-pt
    if coding:
      pt=time()
      I=coding.preprocess(I,prot=prot)
      pt2=time()
      if prot:
        print "preprocessing time", pt2-pt


    
            

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
