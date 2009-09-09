from polybori.nf import *
import polybori.aes as aesmod

from polybori.PyPolyBoRi import *
from polybori.ll import eliminate, ll_encode
from time import time
from copy import copy
from itertools import chain
from inspect import getargspec
from statistics import used_vars, used_vars_set
from heuristics import dense_system,gauss_on_linear
from itertools import chain
from polybori.interpolate import lex_groebner_basis_for_polynomial_via_variety
from inspect import getargspec
from polybori.fglm import fglm
def owns_one_constant(I):
    """Determines whether I contains the constant one polynomial."""
    for p in I:
        if p.is_one():
            return True
    return False

def want_interpolation_gb(G):
    if get_order_code()!=OrderCode.lp:
        return False
    if len(G)!=1:
        return False
    p=Polynomial(G[0])
    if p.lead_deg()<=1:
        return False
    if p.set().n_nodes()>1000:
        return False
    return True

def ll_is_good(I):
    lex_lead=set()
    for p in I:
        m=p.lex_lead()
        if m.deg()==1:
            lex_lead.add(iter(m.variables()).next().index())
    if len(lex_lead)>=0.8*len(I):
        uv=used_vars_set(I).deg()#don't use len here, which will yield 1
        if len(lex_lead)>0.9*uv:
            if uv- len(lex_lead)>16:
                return "llfirstonthefly"
            else:
                return "llfirst"
    return False
    
def ll_heuristic(d):
    d=copy(d)
    I=d["I"]
    if (not "llfirstonthefly" in d) and (not "llfirst" in d):
        hint=ll_is_good(I)
        if hint:
            d[hint]=True
    return d



def change_order_heuristic(d):
    d_orig=d
    d=copy(d)
    I=d["I"]
    switch_table={OrderCode.lp:OrderCode.dp_asc,OrderCode.dlex:OrderCode.dp_asc}
    if not "other_ordering_first" in d:
        #TODO after ll situation might look much different, so heuristic is on wrong place
        code=get_order_code()
        if code in switch_table:
            max_non_linear=len(I)/2
            non_linear=0
            if code==OrderCode.lp:
                for p in I:
                    if p.lead().deg()>1:
                        non_linear=non_linear+1
                        if non_linear>max_non_linear:
                            break
            if (non_linear>max_non_linear) or (code!=OrderCode.lp):
                other_ordering_opts=copy(d_orig)
                other_ordering_opts["switch_to"]=switch_table[code]
                d["other_ordering_first"]=other_ordering_opts
    return d


def interpolation_gb_heuristic(d):
    d=copy(d)
    I=d["I"]
    if not d.get("other_ordering_opts",False) and want_interpolation_gb(I):
        d["interpolation_gb"]=True
        d["other_ordering_first"]=False
    return d
def linear_algebra_heuristic(d):
    d_orig=d
    d=copy(d)
    I=d["I"]
    def want_la():
        n_used_vars=None
        bound=None
        if have_degree_order():
            new_bound=200
            n_used_vars=used_vars_set(I,bound=new_bound).deg()
            if n_used_vars<new_bound:
                return True
            bound=new_bound
        if dense_system(I):
            new_bound=100
            if not (bound and new_bound<bound):
                n_used_vars=used_vars_set(I,bound=new_bound).deg()
                bound=new_bound
            if n_used_vars<bound:
                return True
        return False
    if not (("faugere" in d and (not d["faugere"])) or ("noro" in d and d["noro"])):
        if ("faugere" in d and d["faugere"]) or want_la():

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
        heuristic=True
        try:
            heuristic=complete_dict["heuristic"]
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
        self.__doc__=f.__doc__
        
def with_heuristic(heuristic_function):
    def make_wrapper(f):
        wrapped=HeuristicalFunction(f,heuristic_function)
        wrapped.__name__=f.__name__
        return wrapped
    return make_wrapper
def clean_polys(I):
    zero=Polynomial(0)
    I=list(set((Polynomial(p) for p in I if p!=zero)))
    return I
def clean_polys_pre(I):
    return (clean_polys(I),None) 
def gb_with_pre_post_option(
    option,pre=None,
    post=None,if_not_option=tuple(),
    default=False):
    def make_wrapper(f):
        def wrapper(I, **kwds):
            prot=kwds.get("prot", False)
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
                   pre_args=getargspec(pre)[0]
                   if prot:
                       print "preprocessing for option:", option
                   
                   (I,state)=pre(**dict([(k,v) for (k,v) in locals().iteritems() if k in pre_args]))
            I=f(I,**kwds)
            if option_set:
                if post:
                    post_args=getargspec(post)[0]
                    if prot:
                        print "postprocessing for option:", option
                    I=post(**dict([(k,v) for (k,v) in locals().iteritems() if k in post_args]))
            return I
        wrapper.__name__=f.__name__
        wrapper.__doc__=f.__doc__
        if hasattr(f,"options"):
            wrapper.options=copy(f.options)
        else:
            (argnames,varargs,varopts,defaults)=getargspec(f)
            wrapper.options=dict(zip(argnames[-len(defaults):],defaults))

        wrapper.options[option]=default
        return wrapper
    return make_wrapper
def redsb_post(I,state):
    if I==[]:
        return []
    else:
        return I.minimalize_and_tail_reduce()
def minsb_post(I,state):
    if I==[]:
        return []
    else:
        return I.minimalize()
def invert_all(I):
    return [p.map_every_x_to_x_plus_one() for p in I]
def invert_all_pre(I):
    return (invert_all(I),None)
def invert_all_post(I,state):
    return invert_all(I)
    
def llfirst_pre(I,prot):
    (eliminated,llnf, I)=eliminate(I,on_the_fly=False,prot=prot)
    return (I,eliminated)

def ll_constants_pre(I):
    I_new=[]
    ll=[]
    leads=set()
    for p in I:
        if p.lex_lead_deg()==1:
            l=p.lead()
            if not (l in leads) and len(p)<=2:
                tail=p+l
                if tail.deg()<=0:
                    ll.append(p)
                    leads.add(l)
                    continue
        I_new.append(p)
    encoded=ll_encode(ll)
    reduced=[]
    for p in I_new:
        p=ll_red_nf_redsb(p,encoded)
        if not p.is_zero():
            reduced.append(p)
    return (reduced,ll)


def variety_size_from_gb(I):
    sm=Monomial(used_vars_set(I)).divisors()
    for p in I:
        m=p.lead()
        sm=sm.diff(sm.multiples_of(m))
    return sm.size_double()
def other_ordering_pre(I,option_set,kwds):
    main_kwds=kwds
    options=option_set
    ocode=get_order_code()
    old_ring=global_ring()
    try:
        change_ordering(options["switch_to"])
        new_ring=global_ring()
        kwds=dict((k,options[k]) for k in options if not (k in ("other_ordering_first","switch_to","I")))
        kwds["redsb"]=True
        I_orig=I
        I=groebner_basis(I,**kwds)
        variety_size=variety_size_from_gb(I)
        if variety_size<50000:
            main_kwds["convert_with_fglm_from_ring"]=new_ring
    finally:
        old_ring.set()
    return (I,None)
def llfirstonthefly_pre(I,prot):
    (eliminated,llnf, I)=eliminate(I,on_the_fly=True)
    return (I,eliminated)

def gauss_on_linear_pre(I, prot):
    return (gauss_on_linear(I), None)

def easy_linear_polynomials_pre(I):
    res=[]
    for p in I:
        res.append(p)
        if p.deg()>=2:
            opp=p+1
            for q in easy_linear_factors(opp):
                res.append(q+1)
    return (list(set(res)), None)

def llfirst_post(I,state,prot):
    eliminated=state
    for p in I:
        if p.is_one():
            return [p]
    else:
        if len(eliminated)>0:
            I=list(chain(I,eliminated))
            #redsb just for safety, as don't know how option is set
            I=groebner_basis(I,llfirst=False,llfirstonthefly=False,ll_constants=False,other_ordering_first=False,eliminate_identical_variables=False, redsb=True,prot=prot)
    return I


def ll_constants_post(I,state):
    eliminated=state
    for p in I:
        if p.is_one():
            return [p]
    else:
        if len(eliminated)>0:
            I=list(chain(I,eliminated))
            #redsb just for safety, as don't know how option is set
        return I

def result_to_list_post(I,state):
    return list(I)
def fix_deg_bound_post(I,state):
    if isinstance(I,GroebnerStrategy):
        return I.all_generators()
    else:
        return I

def incremental_pre(I,prot, kwds):
    def sort_key(p):
        p=Polynomial(p)
        return (p.navigation().value(), -p.deg())
    I=sorted(I, key=sort_key)
    inc_sys=[]
    kwds=copy(kwds)
    kwds['incremental']=False

    for p in I[:-1]:
        inc_sys.append(p)
        inc_sys=groebner_basis(inc_sys, **kwds)
        if prot:
            print "incrementally calculating GB, adding generator:", p
    inc_sys.append(I[:-1])
    return (inc_sys,None)

def eliminate_identical_variables_pre(I, prot):
    changed=True
    ll_system=[]
    treated_linears=set()
    while changed:
        changed=False
        rules=dict()
        for p in I:
            t=p+p.lead()
            if p.lead_deg()==1:
                l=p.lead()
                if l in treated_linears:
                    continue
                else:
                    treated_linears.add(l)
                if t.deg()>0:
                    rules.setdefault(t, [])
                    leads=rules[t]
                    leads.append(l)
        def my_sort_key(l):
            return l.navigation().value()
        for (t, leads) in rules.iteritems():
            if len(leads)>1:
                changed=True
                leads=sorted(leads, key=my_sort_key, reverse=True)
                chosen=leads[0]
                for v in leads[1:]:
                    ll_system.append(chosen+v)
    if len(ll_system)>0:
        ll_encoded=ll_encode(ll_system, reduce=True)
        I=set([ll_red_nf_redsb(p, ll_encoded) for p in I])
    return (I, ll_system)

@gb_with_pre_post_option("clean_arguments",pre=clean_polys_pre,default=True)
@gb_with_pre_post_option("easy_linear_polynomials", pre=easy_linear_polynomials_pre, default=True)
@with_heuristic(ll_heuristic)
@gb_with_pre_post_option("result_to_list",post=result_to_list_post,default=True)
@with_heuristic(interpolation_gb_heuristic)
@gb_with_pre_post_option("invert",pre=invert_all_pre,post=invert_all_post,default=False)
@gb_with_pre_post_option("gauss_on_linear", pre=gauss_on_linear_pre, default=True)
@gb_with_pre_post_option("ll_constants", pre=ll_constants_pre,post=ll_constants_post,default=True)
@gb_with_pre_post_option("eliminate_identical_variables", pre=eliminate_identical_variables_pre, post=llfirst_post, default=True)
@gb_with_pre_post_option("llfirst",if_not_option=["llfirstonthefly"],pre=llfirst_pre,post=llfirst_post,default=False)
@gb_with_pre_post_option("llfirstonthefly",pre=llfirstonthefly_pre,post=llfirst_post,default=False)
@gb_with_pre_post_option("incremental",pre=incremental_pre)
@with_heuristic(change_order_heuristic)
@gb_with_pre_post_option("other_ordering_first",if_not_option=["interpolation_gb"],pre=other_ordering_pre,default=False)
@with_heuristic(linear_algebra_heuristic)
@gb_with_pre_post_option("fix_deg_bound",if_not_option=["interpolation_gb"], post=fix_deg_bound_post,default=True)
@gb_with_pre_post_option("minsb",post=minsb_post,if_not_option=["redsb","deg_bound","interpolation_gb","convert_with_fglm_from_ring"],default=True)
@gb_with_pre_post_option("redsb",post=redsb_post,if_not_option=["deg_bound","interpolation_gb","convert_with_fglm_from_ring"],default=True)
def groebner_basis(I, faugere=False,
       preprocess_only=False, selection_size= 1000,
       full_prot= False, recursion= False,
       prot= False, step_factor= 1,
       deg_bound=False, lazy= True, ll= False,
       max_growth= 2.0, exchange= True,
       matrix_prefix= "matrix", red_tail= True,
       implementation="Python", aes= False,
       llfirst= False, noro= False, implications= False,
       draw_matrices= False, llfirstonthefly= False,
       linear_algebra_in_last_block=True, heuristic=True,unique_ideal_generator=False, interpolation_gb=False, clean_and_restart_algorithm=False, convert_with_fglm_from_ring=None,
       red_tail_deg_growth=True, modified_linear_algebra=True):
    """Computes a Groebner basis of a given ideal I, w.r.t options."""
    if full_prot:
        prot=True
    if prot:
        print "number of passed generators:",len(I)
    if not convert_with_fglm_from_ring is None:
        from_ring=convert_with_fglm_from_ring
        to_ring=global_ring()
        return fglm(I, from_ring, to_ring)
    if interpolation_gb:
        if len(I)!=1 or get_order_code()!=OrderCode.lp:
            raise ValueError
        return lex_groebner_basis_for_polynomial_via_variety(I[0])
    if deg_bound is False:
        deg_bound=100000000L
    zero=Polynomial(0)
    I=[Polynomial(p) for p in I if p!=zero]
    if unique_ideal_generator:
        prod=1
        for p in I:
            prod=(p+1)*prod
        I=[prod+Polynomial(1)]

    import nf
    nf.print_matrices=draw_matrices
    nf.matrix_prefix=matrix_prefix
    
    if implementation=="Python":
        implementation=symmGB_F2_python
    else:
        implementation=symmGB_F2_C
    
    if aes:
        pt=time()
        I=aesmod.preprocess(I, prot=prot)
        pt2=time()
        if prot:
          print "preprocessing time", pt2-pt

    if preprocess_only:
      for p in I:
        print p
      import sys
      sys.exit(0)
    def call_algorithm(I,max_generators=None):
        return implementation(I, opt_red_tail=red_tail,\
            max_growth=max_growth, step_factor=step_factor,
            implications=implications,prot=prot,
            full_prot=full_prot,deg_bound=deg_bound,
            selection_size=selection_size, opt_lazy=lazy, 
            opt_exchange=exchange, opt_allow_recursion=recursion,
            use_faugere=faugere,
            use_noro=noro,ll=ll,
            draw_matrices=draw_matrices,
            matrix_prefix=matrix_prefix,
            modified_linear_algebra=modified_linear_algebra,
            opt_linear_algebra_in_last_block=linear_algebra_in_last_block,
            max_generators=max_generators, red_tail_deg_growth=red_tail_deg_growth)
    if clean_and_restart_algorithm:
        for max_generators in [1000,10000,50000,100000,200000,300000,400000,None]:
            try:
                return call_algorithm(I,max_generators=max_generators)
            except GeneratorLimitExceeded, e:
                I=list(e.strat.all_generators())
                del e.strat
                if prot:
                    print "generator limit exceeded:", max_generators, "restarting algorithm"
    else:
        return call_algorithm(I)

groebner_basis.__doc__=groebner_basis.__doc__+"\nOptions are:\n"+"\n".join((k+"  :  "+repr(groebner_basis.options[k]) for k in groebner_basis.options))+"\nTurn off heuristic by setting heuristic=False"
