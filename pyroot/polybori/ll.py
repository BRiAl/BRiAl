from polybori.PyPolyBoRi import *

lead_index=top_index
#(p):
#  return iter(p.lexLead()).next().index()#first index
 
def combine(reductors,p, reduce=None):
    p_nav=p.navigation()
    assert p_nav.value()<reductors.navigation().value()
    p_else=BooleSet(p_nav.elseBranch(), p.ring())
    if reduce:
        p_else=reduce(p_else,reductors)
    return if_then_else(p_nav.value(),reductors,p_else)
    

def llredsb_Cudd_style(polys):

  reductors=Polynomial(1).set()
  
  linear_lead=sorted(polys,key=lead_index,reverse=True)
  assert len(set([p.lexLead() for p in linear_lead]))==len(polys)
  assert len([p for p in polys if p.constant()])==0
  assert len([p for p in polys if p.lexLmDeg()==1])==len(polys)
  assert len(set([p.navigation().value() for p in polys]))==len(polys)
  for p in linear_lead:
        reductors=combine(reductors,p,reduce=ll_red_nf)
  return reductors



def ll_encode(polys):
  polys=[Polynomial(p) for p in polys]
  reductors=Polynomial(1).set()
  
  linear_lead=sorted(polys,key=lead_index, reverse=True)
  assert len(set([p.lexLead() for p in linear_lead]))==len(polys)
  assert len([p for p in polys if p.constant()])==0
  assert len([p for p in polys if p.lexLmDeg()==1])==len(polys)
  assert len(set([p.navigation().value() for p in polys]))==len(polys)

  for p in linear_lead:
      reductors=combine(reductors,p,reduce=None)
  return reductors

def eliminate(polys, on_the_fly=False):
  rest=[]
  linear_leads=[]
  linear_leading_monomials=set()
  for p in polys:
    if p.isZero():
      continue
    lm=p.lexLead()
    if lm.deg()==1:
      
      if not (lm in linear_leading_monomials):
        linear_leading_monomials.add(lm)
        linear_leads.append(p)
      else:
        rest.append(p)
    else:
      rest.append(p)
  if on_the_fly:
      reductors=ll_encode(linear_leads)
  else:
      reductors=llredsb_Cudd_style(linear_leads)
  if on_the_fly:
      red_fun=ll_red_nf_noredsb
  else:
      red_fun=ll_red_nf
  def llnf(p):
      return red_fun(p,reductors)
  return (linear_leads,llnf,[red_fun(p,reductors) for p in rest])
  
