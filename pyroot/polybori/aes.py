from polybori.nf import *
from polybori.blocks import Block
from itertools import chain
from ll import ll_encode
cache={}
def m2code(m,table):
    res=0
    for v in m.variables():
        res=res+(1<<table[v.index()])
    #print "m2code",m,res
    return res
def p2code(p,table):
    res=0
    for m in p.terms():
        res=res+(1<<m2code(m,table))
    return res
def translate_m(m,table):
    res=Monomial()
    for v in m.variables():
        res=Monomial(Variable(table[v.index()]))*res
    return res
def translate(p,table):
    res=0
    #would be optimal to work on tree level
    for m in Polynomial(p).terms():
        res=res+translate_m(m,table)
    #print "tranlated_p", p,"new",res
    return res
def cached_GB(I,prot=False):
    
    #return list(symmGB_F2_C(I,prot=True, opt_lazy=False,opt_exchange=False).minimalize_and_tail_reduce())
    
    
    m=Monomial()
    for p in I:
        m=m*p.vars_as_monomial()
    table_to_ring=dict(enumerate([v.index() for v in m.variables()]))
    table_to_123=dict([(v.index(),k) for (k,v) in enumerate(m.variables())])
    codes=tuple(sorted([p2code(p,table_to_123) for p in I]))
    if codes in cache:
        #print "codes found",codes
        #print "ideal",I
        return [translate(p,table_to_ring) for p in cache[codes]]
    
    I_t=[translate(p,table_to_123) for p in I]
    #print "orig",I_t
    I=I_t
    I_t=list(symmGB_F2_C(I_t, opt_lazy=False,opt_exchange=False,prot=prot).minimalize_and_tail_reduce())
    #print "GB", I_t
    #print "code:",codes
    #print "ideal:", I
    cache[codes]=I_t
    return [translate(p,table_to_ring) for p in I_t]
def preprocess(I, prot=False):
    global cache
    if get_order_code()==OrderCode.lp:
      import cache as cache_module
      cache=cache_module.cache
      del cache_module
      sys.modules.pop("cache")
    #TODO clean up in dp for unittests
    if prot:
      print "preprocess AES" 
    I=set([Polynomial(p) for p in I])
    linear=[]
    square=[]
    for p in I:
        if p.is_zero():
            continue
        if p.deg()==1:
            linear.append(p)
        else:
            assert p.deg()==2
            square.append(p)
    subs={}        
    for p in square:
        m=p.vars_as_monomial()
        if m in subs:
            subs[m].append(p)
        else:
            subs[m]=[p]
    
    res=list(symmGB_F2_python(linear,prot=prot).minimalize_and_tail_reduce())
    for m in subs:
    
        sub_system=subs[m]
        
    
        sub_system=cached_GB(sub_system,prot=prot)
    
        res.extend(list(sub_system))
    
    cache={}
    return res
    
def var_list(n,r,c,e):
    m=r*c*e
    def with_index(s,i):
        if len(s)==1:
            return s+str(i)
        else:
            return s+"_"+str(i)
    symbols=chain(("c","x"+str(n)),
        (s+str(i+shift) for i in xrange(n-1,0,-1) for (s,shift) in (("w",0),("x",0))),\
        ("w0","p"),
        (s+str(i) for i in xrange(n,0,-1) for s in ("k","s")),
        ("k0",))
    
    #symbols=chain(("c"),
    #    (s+str(i+shift) for i in xrange(n,0,-1) for (s,shift) in (("x",0),("w",0))),\
    #    ("p",),
    #    ("k"+str(n),),
    #    (s+str(i) for i in xrange(n-1,0-1,-1) for s in ("s","k")),
    #    )
    block_list=list(chain((s+str(i) for s in ("reserved_front","reserved_back") for i in xrange(e)),(with_index(s,i) for s in symbols for i in xrange(m))))
    block_start=2*e+     2*(n+1)*m# first summand for reserved
    return (block_start,block_list)
def sbox_generator4(code):
    change_ordering(code)
    from nf import symmGB_F2_python
    a3=Variable(0)
    a2=Variable(1)
    a1=Variable(2)
    a0=Variable(3)
    b3=Variable(4)
    b2=Variable(5)
    b1=Variable(6)
    b0=Variable(7)
    equations=[a0+b3*b2*b1+b2*b1*b0+b2*b1+b2*b0+b3+b2+b1+b0,
      a1+b3*b1*b0+b3*b1+b2*b1+b2*b0+b1*b0+b3,
      a2+b3*b2*b0+b3*b0+b2*b0+b1*b0+b3+b2,
      a3+b3*b2*b1+b3*b2+b3*b1+b3*b0+b3+b2+b1]
    equations=symmGB_F2_python(equations)
    equations=list(equations.minimalize_and_tail_reduce())
    change_ordering(OrderCode.lp)
    def sbox(a3,a2,a1,a0,b3,b2,b1,b0):
        mymap=[Variable(i)+locals()["a"+str(4-i-1)] for i in xrange(4)]+[Variable(4+i)+locals()["b"+str(4-i-1)] for i in xrange(4)]
        mymap=ll_encode(mymap)
        #print Polynomial(mymap)
        return (ll_red_nf_redsb(e,mymap) for e in equations)
    return sbox
    #dirty
