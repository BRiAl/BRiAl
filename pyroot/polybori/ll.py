from polybori.PyPolyBoRi import *
def ll_red_nf2(p,reductors):
  
   #print len(reductors)
   if p.isZero() or p.isOne():
     return p
   else:
     index=p.navigation().value()
     if not index in reductors:
       used_vars=p.vars()
       for v in used_vars:
         if v in reductors:
           index=v
           break
       if not (index in reductors):
         #print "irred:",used_vars
         return p
   assert index in reductors
   s0=p.diagram().subset0(index)
   s1=p.diagram().subset1(index)
   res=reductors[index]*ll_red_nf2(Polynomial(s1),reductors)+ll_red_nf2(Polynomial(s0),reductors)
   
   
   #alternative computation
   

   
   return res

def llredsb(polys):
  def lead(p):
    return p.lead()
  reductors=dict()
  
  linear_lead=sorted(polys,key=lead)
  #res=[]
  counter=0
  total=len(linear_lead)
  lastpp=-1
  for p in linear_lead:
        counter=counter+1
        pp=int(counter*100/float(total))
        if pp>lastpp:
          lastpp=pp
          print "llredsb:",pp,"%" 
        #print "lead",p.lead()
        #print "orig_len",len(p)


        #p=red_tail(strat,p)
        lmp=p.lead()
        assert len(list(lmp))==1
        index=iter(lmp).next()
        red_nf_of_tail=ll_red_nf2(p+lmp,reductors)
        assert not (index in reductors)
        reductors[index]=red_nf_of_tail
        #res.append(red_nf_of_tail+lmp)
        #strat.addGenerator(red_nf_of_tail+lmp)
        #print "reduced_len", len(red_nf_of_tail)+1
  return reductors



def ll_red_nf2_Cudd_style(p,reductors):
    #print reductors.nNodes()
    def n2p(n):
        return Polynomial(BooleSet(n))
    def n2d(n):
        return BooleSet(n)
    def p1(p):
        d=p.diagram()
        i=d.navigation().value()
        return Polynomial(d.subset1(i))
    
    def p0(p):
        d=p.diagram()
        i=d.navigation().value()
        return Polynomial(d.subset0(i))
    if isinstance(reductors,Polynomial):
        reductors=reductors.diagram()
    if p.isZero() or p.isOne():
        return p
    if reductors.empty():
        return p
    rnav=reductors.navigation()
    pnav=p.navigation()
    
    while rnav.value()<pnav.value():
       rnav=rnav.thenBranch()
    if rnav.value()==pnav.value():
       return ll_red_nf2_Cudd_style(p1(p),n2d(rnav.thenBranch()))*n2p(rnav.elseBranch())+ll_red_nf2_Cudd_style(p0(p),n2d(rnav.thenBranch()))
    assert rnav.value()>pnav.value()
    return Monomial(Variable(pnav.value()))*ll_red_nf2_Cudd_style(p1(p),n2d(rnav))+ll_red_nf2_Cudd_style(p0(p),n2d(rnav))

def llredsb_Cudd_style(polys):
  def lead(p):
    return p.lead()
  reductors=Polynomial(1)
  
  linear_lead=sorted(polys,key=lead)
  assert len(set([p.lead() for p in linear_lead]))==len(polys)
  assert len([p for p in polys if p.constant()])==0
  assert len([p for p in polys if p.lmDeg()==1])==len(polys)
  assert len(set([p.navigation().value() for p in polys]))==len(polys)
  counter=0
  total=len(linear_lead)
  lastpp=-1
  for p in linear_lead:
        counter=counter+1
        pp=int(counter*100/float(total))
        if pp>lastpp:
          lastpp=pp
          #print "llredsb:",pp,"%" 
        lmp=p.lead()
        assert len(list(lmp))==1
        index=iter(lmp).next()
        red_nf_of_tail=ll_red_nf(p+lmp,BooleSet(reductors.diagram()))
        assert index<reductors.navigation().value()
        reductors=lmp*reductors+red_nf_of_tail
  return reductors



def ll_encode(polys):
  polys=[Polynomial(p) for p in polys]
  def lead(p):
    return p.lead()
  reductors=BooleSet(Polynomial(1).diagram())
  
  linear_lead=sorted(polys,key=lead)
  assert len(set([p.lead() for p in linear_lead]))==len(polys)
  assert len([p for p in polys if p.constant()])==0
  assert len([p for p in polys if p.lmDeg()==1])==len(polys)
  assert len(set([p.navigation().value() for p in polys]))==len(polys)
  counter=0
  total=len(linear_lead)
  lastpp=-1
  for p in linear_lead:
        p_nav=p.navigation()
        reductors=recursively_insert(p_nav.elseBranch(),p_nav.value(),reductors.navigation())
  return reductors

def eliminate(polys, on_the_fly=False):
  rest=[]
  linear_leads=[]
  indices=set()
  for p in polys:
    if p.isZero():
      continue
    lm=p.lead()
    if lm.deg()==1:
      index=iter(lm).next()
      if not (index in indices):
        indices.add(index)
        linear_leads.append(p)
      else:
        rest.append(p)
    else:
      rest.append(p)
  if on_the_fly:
      reductors=ll_encode(linear_leads)
  else:
      reductors=llredsb_Cudd_style(linear_leads)
      reductors=BooleSet(reductors.diagram())
  if on_the_fly:
      red_fun=ll_red_nf_noredsb
  else:
      red_fun=ll_red_nf
  def llnf(p):
      return red_fun(p,reductors)
  return (linear_leads,llnf,[red_fun(p,reductors) for p in rest])
  