from polybori.PyPolyBoRi import *
from polybori.statistics import used_vars_set
from random import Random
import copy
import sys
from exceptions import NotImplementedError
#from dijkstra import mydeg

#for g in G:
#    print hash(g)
#UNSET="ein drittes"
#LAZY="REDTAIL"
#NOREDTAIL="NOREDTAIL"

def myspoly(f,g):
    """only for the profiler to be noticed"""
    return spoly(f,g)
    
    


def lm_tuple(f):
    if isinstance(f,Polynomial):
        f=f.lead()
    #return f.tuple()
    
    res=tuple(f)
    
    return res
def product_criterion(f,g):
    f=set(lm_tuple(f))
    g=set(lm_tuple(g))
    if len(f.intersection(g))==0:
        return True
    else:
        return False
def update_cache2(h,cache):
    (LG,LGM)=cache
    lead=h.lead()
    l=lm_tuple(lead)
    LGM[l]=h
    LG=LG.union(lead.set())
    return (LG,LGM)



    
    
    


matrix_prefix="matrix"
print_matrices=False
#used_polynomials=list()

def pkey(p):
    return (p[0],len(p))
mat_counter=0
def build_and_print_matrices(v,strat):
    
    
    
    treated=BooleSet()
    v=list(v)
    rows=0
    polys_in_mat=[]
    #v_orig=list(v)
    while(len(v)>0):
        rows=rows+1
        p=v[0]
        v=v[1:]
        #used_polynomials.append(p)
        for m in list(p):
            m=Monomial(m)
            if not m in BooleSet(treated):
                i=strat.select(m)
                if i>=0:
                    p2=strat[i]
                    p2*=(m/p2.lead())
                    v.append(p2)
        
                    #print p2
          
            #else:
            #    print "in treated"
        polys_in_mat.append(p)
        treated=treated.union(p.set())
    m2i=dict([(v,k) for (k,v) in enumerate(BooleSet(treated))])
    polys_in_mat=[[m2i[t] for t in p] for p in polys_in_mat]
    polys_in_mat.sort(key=pkey)
    #print polys_in_mat
    global mat_counter
    mat_counter=mat_counter+1
    import Image, ImageDraw
    
    rows=len(polys_in_mat)
    cols=len(m2i)
    im=Image.new("1",(cols,rows),"white")

    #draw = ImageDraw.Draw(im)
    for i in xrange(len(polys_in_mat)):
        p=polys_in_mat[i]
        for j in p:
           assert i<rows
           assert j<cols
           #if mat_counter==1:
            #print (i,j)
           im.putpixel((j,i), 0)
           
    #del draw
    file_name=matrix_prefix+str(mat_counter)+".png"
    import os
    os.system("rm -f file_name")
    im.save(file_name)
    del im
    
    
    print "MATRIX_SIZE:", rows,"x",cols
    
   
def sum_terms(terms):
    l=len(terms)
    if l==0:
        return Polynomial(0)
    if l==1:
        return Polynomial(terms[0])
    if l==2:
        return Polynomial(terms[1]+terms[0])
    s=l/2
    return sum_terms(terms[:s])+sum_terms(terms[s:])
def noro_step(polys,strat):
    def nf(p,strat):
        if p.isZero():
            return p
        else:
            return nf3(strat,p,p.lead())
    llReductor=strat.llReductor
    polys=[nf(ll_red_nf(p,llReductor),strat) for p  in polys]
    polys=[red_tail(strat,p) for p in polys if not p.isZero()]
    terms=BooleSet()
    for p in polys:
        terms=terms.union(p.set())
    terms=list(BooleSet(terms))
    terms.sort(reverse=True)
    i2term=list(enumerate(terms))
    term2i=dict(((i,j) for (j,i) in i2term))
    i2term=dict(i2term)
    #print repr(i2term),repr(term2i)
    rows=len(polys)
    cols=len(terms)
    m=createMatGF2(rows,cols)
    for (i,p) in enumerate(polys):
        for t in BooleSet(p.set()):
            j=term2i[t]
            m[i,j]=1
    
    #for i in xrange(len(polys)):
    #    print "|".join([str(m[i,j]) for j in xrange(len(terms))])
    rank=m.gauss()
    
    polys=[sum_terms([i2term[j] for j in xrange(cols) if m[i,j]]) for i in xrange(rank)]
    #print polys
    return polys
    #for i in xrange(len(polys)):
    #    print "|".join([str(m[i,j]) for j in xrange(len(terms))])
    #print "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    #print "polys:",len(polys),"terms:",len(terms)
    #print "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
def build_and_print_matrices_deg_colored(v,strat):
    
    if len(v)==0:
        return
    
    treated=BooleSet()
    v=list(v)
    rows=0
    polys_in_mat=[]
    #v_orig=list(v)
    while(len(v)>0):
        rows=rows+1
        p=v[0]
        v=v[1:]
        #used_polynomials.append(p)
        for m in list(p):
            m=Monomial(m)
            if not m in BooleSet(treated):
                i=strat.select(m)
                if i>=0:
                    p2=strat[i]
                    p2*=(m/p2.lead())
                    v.append(p2)
        
                    #print p2
          
            #else:
            #    print "in treated"
        polys_in_mat.append(p)
        treated=treated.union(p.set())
    m2i=dict([(v,k) for (k,v) in enumerate(BooleSet(treated))])
    max_deg=max([m.deg() for m in BooleSet(treated)])
    if max_deg==0:
        max_deg=1
    i2deg=dict([(m2i[m],m.deg()) for m in BooleSet(treated)])
    polys_in_mat=[[m2i[t] for t in p] for p in polys_in_mat]
    polys_in_mat.sort(key=pkey)
    #print polys_in_mat
    global mat_counter
    mat_counter=mat_counter+1
    import Image, ImageDraw, ImageColor
    
    rows=len(polys_in_mat)
    cols=len(m2i)
    im=Image.new("RGB",(cols,rows),"white")

    #draw = ImageDraw.Draw(im)
    for i in xrange(len(polys_in_mat)):
        p=polys_in_mat[i]
        for j in p:
           assert i<rows
           assert j<cols
           #if mat_counter==1:
            #print (i,j)
           #im.putpixel((j,i), ImageColor.getrgb("hsl(0,"+str((100*i2deg[j])/max_deg)+"%,50%)"))
           im.putpixel((j,i), ImageColor.getrgb("hsl("+str(270-(270*i2deg[j])/max_deg)+",100%,50%)"))
           
    #del draw
    file_name=matrix_prefix+str(mat_counter)+".png"
    import os
    os.system("rm -f file_name")
    im.save(file_name)
    del im
    
    
    print "MATRIX_SIZE:", rows,"x",cols   
    
def symmGB_F2_python(G,deg_bound=1000000000000,over_deg_bound=0, use_faugere=False,use_noro=False,optLazy=True,optRedTail=True, max_growth=2.0, step_factor=1.0, implications=False, prot=False, full_prot=False,selection_size=1000, optExchange=True, optAllowRecursion=False,ll=False,optLinearAlgebraInLastBlock=True):
    #print implications
    if use_noro:
        raise NotImplementedError
    if use_noro and use_faugere:
        raise Exception
    def add_to_basis(strat,p):
        if p.isZero():
            if prot:
                print "-"
        else:
            if prot:
                #print "s"
                if full_prot:
                    print p
                print "Result: ", "deg:", p.deg(), "lm: ", p.lead(), "el: ", p.elength()
            strat.addAsYouWish(p)
            
            if prot:
                print "#Generators:", len(strat)
    
    if (isinstance(G,list)):
        if len(G)==0:
            return []
            
            
        #print "gr"
        G=[Polynomial(g) for g in G]
        #print "gra"    
        strat=GroebnerStrategy()
        strat.optRedTail=optRedTail
        strat.optLazy=optLazy
        strat.optExchange=optExchange
        strat.optAllowRecursion=optAllowRecursion
        strat.enabledLog=prot
        strat.optLL=ll
        strat.optLinearAlgebraInLastBlock=optLinearAlgebraInLastBlock
        #strat.addGenerator(G[0])
        
        
        strat.redByReduced=False#True
        

        for g in  G:
            #print G[i], G[i].__class__,i,len(G)
            if not g.isZero():
                strat.addGeneratorDelayed(g)
    else:
        strat=G
        
    if prot:
        print "added delayed"
    i=0
    while strat.npairs()>0:
        i=i+1
        if prot:
            #pass 
            #print_ring_info()
            #ps=strat.someSpolysInNextDegree(1000)
            print "Current Degree:", strat.topSugar()
        if (strat.topSugar()>deg_bound) and (over_deg_bound<=0):
            return strat
        if (strat.topSugar()>deg_bound):
            ps=strat.someSpolysInNextDegree(over_deg_bound)
            over_deg_bound-=len(ps)
        else:
            #ps=strat.smallSpolysInNextDegree(100.0,selection_size)
            ps=strat.someSpolysInNextDegree(selection_size)
            
        if prot:
            print "(", strat.npairs(), ")"
        if prot:
            print "start reducing"
            print "Chain Crit. : ",strat.chainCriterions, "VC:",strat.variableChainCriterions,\
            "EASYP",strat.easyProductCriterions,"EXTP",strat.extendedProductCriterions
            print len(ps), "spolys added"
            
        #red=SlimgbReduction(strat)
        #ps=set(ps)
        #print len(ps), "spolys added"
        
        
        
        
        
        
        if use_noro or use_faugere:
             #res=noro_step(ps,strat)
             v=BoolePolynomialVector()
             for p in ps:
                    #print p
                    #p=ll_red_nf(p,strat.llReductor)
                    if not p.isZero():
                        v.append(p)
             if use_noro:
                 res=strat.noroStep(v)
             else:
                 res=strat.faugereStepDense(v) 
        else:
            v=BoolePolynomialVector()
            for p in ps:
                #print p
                p=Polynomial(mod_mon_set(BooleSet(p.set()),strat.monomials))
                #p=ll_red_nf(p,strat.llReductor)
                if not p.isZero():
                    v.append(p)
            if print_matrices:
                build_and_print_matrices_deg_colored(v,strat)
            if len(v)>100:
               res=parallel_reduce(v,strat,int(step_factor*10),max_growth)
            else:
               if len(v)>10:
                  res=parallel_reduce(v,strat,int(step_factor*30),max_growth)
               else:
                  res=parallel_reduce(v,strat,int(step_factor*100), max_growth)
        
        #red.reduce()
        if prot:
            print "end reducing"
        #res=red.result
        def sort_key(p):
            return p.lead()
        res_cp=sorted(res, key=sort_key)
        #res_cp=list(res)
        #res_cp.reverse()
        old_ll=strat.llReductor
        for p in  res_cp:
            #if prot:
            #    print "start redTail"
            #    print len(p)
            
            #p2=nf3_short(strat,p)
            #if p2.isZero():
            #    continue
            #if strat.optRedTail:
            #   p=red_tail(strat,p)
               #p=red_tail_short(strat,p)
           
            #p=red_tail_self_tuning(strat,p)
            
            #p=red_tail_short(strat,p)
            #if prot:
            #    print len(p)
            #    print "end redTail"
            old_len=len(strat)
            add_to_basis(strat,p)
            if implications and old_len==len(strat)-1:
                strat.implications(len(strat)-1)
            if p.isOne():
                #strat.toStdOut()
                if prot:
                    print "GB is 1"
                return strat
            if prot:
                print "(", strat.npairs(), ")"
        new_ll=strat.llReductor
        #if old_ll!=new_ll:
        #    strat.llReduceAll()
        
        strat.cleanTopByChainCriterion()
    #strat.toStdOut()
    return strat

def GPS(G,vars_start, vars_end):
    def step(strat,trace,var,val):
        print "plugin: ",var,val
        print "npairs", strat.npairs()
        strat=GroebnerStrategy(strat)
        print "npairs", strat.npairs()
        strat.addGeneratorDelayed(Polynomial(Monomial(Variable(var))+val))
        strat=symmGB_F2_python(strat,prot=True,deg_bound=2, over_deg_bound=10)
        if var<=vars_start:
            strat=symmGB_F2_python(strat, prot=True, optLazy=False, redTail=False)
        if strat.containsOne():
            pass
        else:
            if var<=vars_start:
                #bug: may contain Delayed polynomials
                print "!!!!!!! SOLUTION",trace
                raise Exception
                #yield trace
            else:
                branch(strat,trace+[(var,val)],var-1)
                #workaround because of stack depth
                #step(strat,trace+[(var,val)],var-1, 0)
                #step(strat,trace+[(var,val)],var-1, 1)
                
    def branch(strat,trace, var):
        while(strat.variableHasValue(var)):
            #remember to add value to trace
            var=var-1
        step(strat, trace, var, 0)
        step(strat, trace, var, 1)
    strat=GroebnerStrategy()
    #strat.addGenerator(G[0])
    for g in G[:]:
        strat.addGeneratorDelayed(g)
    branch(strat,[],vars_end-1)



def GPS_with_proof_path(G,proof_path, deg_bound,over_deg_bound):
    def step(strat,trace,  proof_path, pos, val):
        print proof_path
        print "plugin: ",pos,val, proof_path[pos]
        print "npairs", strat.npairs()
        strat=GroebnerStrategy(strat)
        print "npairs", strat.npairs()
        print "npairs", strat.npairs()
        plug_p=Polynomial(proof_path[pos])+val
        plug_p_lead=plug_p.lead()
        if len(plug_p)==2 and (plug_p+plug_p_lead).deg()==0:
            for v in plug_p_lead:
                strat.addGeneratorDelayed(Variable(v)+1)
        else:
            strat.addGeneratorDelayed(plug_p)
        print "npairs", strat.npairs()
        print "pos:", pos
        strat=symmGB_F2_python(strat,deg_bound=deg_bound, optLazy=False,over_deg_bound=over_deg_bound,prot=True)
        print "npairs", strat.npairs()
        pos=pos+1
        if pos>=len(proof_path):
            print "OVER"
            strat=symmGB_F2_python(strat,prot=True)
        if strat.containsOne():
            pass
        else:
            if pos>=len(proof_path):
                print "npairs", strat.npairs()
                #strat.toStdOut()
                #l=[p for p in strat]
                #strat2=symmGB_F2_python(l,prot=True)
                #strat2.toStdOut()
                print "minimized:"
                for p in strat.minimalizeAndTailReduce():
                    print p
                #bug: may contain Delayed polynomials
                print "!!!!!!! SOLUTION",trace
                raise Exception
                #yield trace
            else:
                branch(strat,trace+[(pos,val)],proof_path,pos)
                #workaround because of stack depth
                #step(strat,trace+[(var,val)],var-1, 0)
                #step(strat,trace+[(var,val)],var-1, 1)
                
    def branch(strat,trace, proof_path,pos):
        
        step(strat, trace,  proof_path, pos, 0)
        step(strat, trace,  proof_path, pos, 1)
    strat=GroebnerStrategy()
    strat.addGenerator(Polynomial(G[0]))
    for g in G[1:]:
        strat.addGeneratorDelayed(Polynomial(g))
    branch(strat,[], proof_path, 0)



def GPS_with_suggestions(G,deg_bound,over_deg_bound, optLazy=True,optRedTail=True,initial_bb=True):
    def step(strat,trace, var,val):
        print trace
        plug_p=val+var
        print "plugin: ",len(trace),plug_p
        trace=trace+[plug_p]
        strat=GroebnerStrategy(strat)
 

        
        strat.addGeneratorDelayed(plug_p)
        print "npairs", strat.npairs()
        
        strat=symmGB_F2_python(strat,deg_bound=deg_bound,optLazy=optLazy,over_deg_bound=over_deg_bound,prot=True)
        
        #pos=pos+1
        if not strat.containsOne():
            branch(strat,trace)
                        
    def branch(strat,trace):
        print "branching"
        index=strat.suggestPluginVariable();
        if index<0:
            uv=set(used_vars_set(strat))
            lv=set([iter(p.lead()).next() for p in strat if p.lmDeg()==1])
            candidates=uv.difference(lv)
            if len(candidates)>0:
                index=iter(candidates).next()
        if index>=0:
            print "chosen index:", index
            step(strat, trace,  Polynomial(Monomial(Variable(index))),0)
            step(strat, trace,  Polynomial(Monomial(Variable(index))),1)
        else:
            print "FINAL!!!", index
            strat=symmGB_F2_python(strat, prot=True)
            if not strat.containsOne():
                print "TRACE", trace
                print "SOLUTION"
                for p in strat.minimalizeAndTailReduce():
                    print p
                raise Exception
    def sort_crit(p):
        #return (p.deg(),p.lead(),p.elength())
        return (p.lead(),p.deg(),p.elength())
    strat=GroebnerStrategy()
    strat.optRedTail=optRedTail#True
    strat.optExchange=False
    strat.optAllowRecursion=False
    #strat.optRedTailDegGrowth=False
    strat.optLazy=optLazy
    #strat.optLazy=True
    first_deg_bound=1
    G=[Polynomial(p) for p in G]
    G.sort(key=sort_crit)
    higher_deg={}
    if initial_bb:
      for g in G:
          print g
          index=strat.select(g.lead())
          if p.deg()==1:#(index<0):
              strat.addAsYouWish(g)
          else:
              first_deg_bound=max(first_deg_bound,g.deg())
              strat.addGeneratorDelayed(g)
              #d=p.deg()
              #if d in higher_deg:
              #    higher_deg[d].append(g)
              #else:
              #    higher_deg[d]=[g]
          print g,len(strat)
    else:
      for g in G:
        strat.addAsYouWish(g)
    #strat.optRedTailDegGrowth=True
    #for d in sorted(higher_deg.keys()):
    #
   # 
   #     while (strat.npairs()>0) and (strat.topSugar()<d):
    #        ps=strat.smallSpolysInNextDegree(2.0,100)
    ##        vec=parallel_reduce(ps,strat,int(1*30),2.0)
     #       for p in vec:
     #           strat.addAsYouWish(p)
     #   vec=BoolePolynomialVector()
     #   for p in higher_deg[d]:
     #       vec.append(p)
     ##   vec=parallel_reduce(vec,strat,int(1*30),2.0)
      #  for p in vec:
      #      print p
      #      strat.addAsYouWish(p)
    if initial_bb:
      strat=symmGB_F2_python(strat,deg_bound=max(deg_bound,first_deg_bound), optLazy=optLazy,over_deg_bound=0,prot=True)
    strat.optLazy=optLazy
    print "INITIALIZED"
    branch(strat,[])



def GPS_with_non_binary_proof_path(G,proof_path, deg_bound,over_deg_bound):
    def step(strat,trace,  proof_path, pos, choice):
        print proof_path
        print "plugin: ",pos
        print "npairs", strat.npairs()
        strat=GroebnerStrategy(strat)
        print "npairs", strat.npairs()
        print "npairs", strat.npairs()
        for p in proof_path[pos][choice]:
            print p
            strat.addGeneratorDelayed(Polynomial(p))
 
        print "npairs", strat.npairs()
        print "pos:", pos
        strat=symmGB_F2_python(strat,deg_bound=deg_bound, over_deg_bound=over_deg_bound,prot=True)
        print "npairs", strat.npairs()
        pos=pos+1
        if pos>=len(proof_path):
            print "OVER"
            strat=symmGB_F2_python(strat)
        if strat.containsOne():
            pass
        else:
            if pos>=len(proof_path):
                print "npairs", strat.npairs()
                strat.toStdOut()
                l=[p for p in strat]
                strat2=symmGB_F2_python(l)
                strat2.toStdOut()
                #bug: may contain Delayed polynomials
                print "!!!!!!! SOLUTION",trace
                raise Exception
                #yield trace
            else:
                branch(strat,trace+[(pos,choice)],proof_path,pos)
                #workaround because of stack depth
                #step(strat,trace+[(var,val)],var-1, 0)
                #step(strat,trace+[(var,val)],var-1, 1)
                
    def branch(strat,trace, proof_path,pos):
        for i in xrange(len(proof_path[pos])):
            step(strat, trace,  proof_path, pos, i)
 
    strat=GroebnerStrategy()
    strat.addGenerator(G[0])
    for g in G[1:]:
        strat.addGeneratorDelayed(g)
    branch(strat,[], proof_path, 0)

def symmGB_F2_C(G,optExchange=True,deg_bound=1000000000000,optLazy=False,over_deg_bound=0, optRedTail=True, max_growth=2.0, step_factor=1.0, implications=False, prot=False, full_prot=False,selection_size=1000, optAllowRecursion=False, use_noro=False,use_faugere=False,ll=False,optLinearAlgebraInLastBlock=True):
    #print implications
    if use_noro:
        raise NotImplementedError, "noro not implemented for symmgb"    
    if (isinstance(G,list)):
        if len(G)==0:
            return []
            
            
        
        G=[Polynomial(g) for g in G]    
        strat=GroebnerStrategy()
        strat.optRedTail=optRedTail
        strat.enabledLog=prot
        strat.optLazy=optLazy
        strat.optExchange=optExchange
        strat.optLL=ll
        strat.optAllowRecursion=optAllowRecursion
        strat.optLinearAlgebraInLastBlock=optLinearAlgebraInLastBlock
        strat.enabledLog=prot
        #strat.addGenerator(G[0])
        
        
        strat.redByReduced=False#True
        
        #if PROT:
        #    print "added first"
        for g in G:#[1:]:
            if not g.isZero():
                strat.addGeneratorDelayed(g)
    strat.symmGB_F2()
    return strat
