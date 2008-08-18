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
        reductors=combine(reductors,p,reduce=ll_red_nf_redsb)
  return reductors



def ll_encode(polys, reduce=False, prot=False):
  if reduce:
      reduce=ll_red_nf_redsb
  else:
      reduce=None
  polys=[Polynomial(p) for p in polys]
  reductors=Polynomial(1).set()
  
  linear_lead=sorted(polys,key=lead_index, reverse=True)
  assert len(set([p.lexLead() for p in linear_lead]))==len(polys)
  assert len([p for p in polys if p.constant()])==0
  assert len([p for p in polys if p.lexLmDeg()==1])==len(polys)
  assert len(set([p.navigation().value() for p in polys]))==len(polys)
  last=None
  counter=0
  for p in linear_lead:

      if prot:
          counter=counter+1
          progress=(counter*100)/len(linear_lead)
          if last!=progress:
              print str(progress)+"%"
          last=progress
      reductors=combine(reductors,p,reduce=reduce)
  return reductors

def eliminate(polys, on_the_fly=False,prot=False):
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
  reductors=ll_encode(linear_leads,reduce=(not on_the_fly),prot=prot)

  if on_the_fly:
      red_fun=ll_red_nf_noredsb
  else:
      red_fun=ll_red_nf_redsb
  def llnf(p):
      return red_fun(p,reductors)
  reduced_list=[]
  for p in rest:
      p=red_fun(p,reductors)
      if p.isOne():
          reduced_list=[p]
          break
      else:
          reduced_list.append(p)

  return (linear_leads,llnf,reduced_list)
  
