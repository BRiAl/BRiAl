from polybori.PyPolyBoRi import *

def llredsb_Cudd_style(polys):
  def lead(p):
    return p.lexLead()
  reductors=Polynomial(1)
  
  linear_lead=sorted(polys,key=lead)
  assert len(set([p.lexLead() for p in linear_lead]))==len(polys)
  assert len([p for p in polys if p.constant()])==0
  assert len([p for p in polys if p.lexLmDeg()==1])==len(polys)
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
        lmp=p.lexLead()
        assert len(list(lmp))==1
        index=iter(lmp).next()
        red_nf_of_tail=ll_red_nf(p+lmp,BooleSet(reductors.set()))
        assert index<reductors.navigation().value()
        reductors=lmp*reductors+red_nf_of_tail
  return reductors



def ll_encode(polys):
  polys=[Polynomial(p) for p in polys]
  def lead(p):
    return p.lexLead()
  reductors=BooleSet(Polynomial(1).set())
  
  linear_lead=sorted(polys,key=lead)
  assert len(set([p.lexLead() for p in linear_lead]))==len(polys)
  assert len([p for p in polys if p.constant()])==0
  assert len([p for p in polys if p.lexLmDeg()==1])==len(polys)
  assert len(set([p.navigation().value() for p in polys]))==len(polys)
  counter=0
  total=len(linear_lead)
  lastpp=-1
  for p in linear_lead:
        p_nav=p.navigation()
        reductors=recursively_insert(p_nav.elseBranch(),p_nav.value(),reductors)
  return reductors

def eliminate(polys, on_the_fly=False):
  rest=[]
  linear_leads=[]
  indices=set()
  for p in polys:
    if p.isZero():
      continue
    lm=p.lexLead()
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
      reductors=BooleSet(reductors.set())
  if on_the_fly:
      red_fun=ll_red_nf_noredsb
  else:
      red_fun=ll_red_nf
  def llnf(p):
      return red_fun(p,reductors)
  return (linear_leads,llnf,[red_fun(p,reductors) for p in rest])
  
