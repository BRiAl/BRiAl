#!/sw/bin/python
# encoding: utf-8
"""
untitled.py

Created by Michael Brickenstein on 2007-03-05.
Copyright (c) 2007 Mathematisches Forschungsinstitut Oberwolfach gGmbH. All rights reserved.
"""

import sys
from optparse import OptionParser
if __name__ == "__main__":
    import pathadjuster
from polybori.PyPolyBoRi import *
from polybori.gbrefs import my_import, load_data, clean_data
from polybori.blocks import IfThen
from copy import copy
from polybori.ll import ll_red_nf2, llredsb,llredsb_Cudd_style, ll_red_nf2_Cudd_style,ll_encode

def find_one(p,res=None):
    def zero_nav(n):
        return n.constant() and (not n.terminalOne())
    try:
        p=p.navigation()
    except AttributeError:
        pass
    if res is None:
        res=dict()
    if zero_nav(p):
        raise ValueError
    if p.terminalOne():
        return res
    else_branch=p.elseBranch()
    if zero_nav(else_branch):
        res[Monomial(Variable(p.value()))]=1
        find_one(p.thenBranch(),res)
    else:
        res[Monomial(Variable(p.value()))]=0
        find_one(else_branch,res)
    return res

parser = OptionParser()
NF3="nf3"
LINEAR_LEAD_NOREDSB="ll"
parser.add_option("--method",
                  action="store", dest="method", type="choice",
                  choices=["nf3","linear-lead-redsb",LINEAR_LEAD_NOREDSB],
                  default="linear-lead-redsb",
                  help="select method")

def my_red_nf(p,strat):
  if p.isZero():
    return Polynomial(0)
  hr=nf3(strat,p,p.lead())
  if hr.isZero():
    return Polynomial(0)
  return red_tail(strat,hr)
def gen_strat(polys):
  polys=[Polynomial(p) for p in polys]
  polys=[p for p in polys if not p.isZero()]
  assert len(set([p.lead() for p in polys]))==len(polys)
  strat=GroebnerStrategy()
  for p in polys:
    print "Adding"
    strat.addGenerator(p)
  print "finished"
  return strat
def logicaland(l):
  res=Polynomial(0)
  for p in l:
    res=1+(res+1)*(p+1)
  return res
def logicalor(l):
  res=Polynomial(1)
  for p in l:
    res=res*p
  return res



        
def proof(ifthen,strat):
  ip=ifthen.ifpart
  it=ifthen.thenpart
  print "proofing:", ifthen
  c=logicalor([1+logicaland(ip),logicaland(it)])
  if c.isZero():
    print "TRUE (trivial)"
    return
  else:
    c=nf3(strat,c,c.lead())
    if c.isZero():
      print "TRUE"
      return
    else:
      print "FALSE"


def proofll(ifthen,reductors,redsb=True,prot=True):
  
  if prot and (not ifthen.supposedToBeValid):
      print "THIS THEOREM IS NOT SUPPOSED TO BE VALID"
  ip_pre=ifthen.ifpart
  ip=[]

  for p in ip_pre:
    p=Polynomial(p)
    if p.isZero(): continue
    li=list(p.lead())
    if len(li)==1 and (not (li[0] in list(Polynomial(reductors).lead()))):
      assert not Polynomial(reductors).isZero()
      lead_index=li[0]
      if redsb:
          p=ll_red_nf(p,reductors)
          reductors=ll_red_nf(Polynomial(reductors),BooleSet(p.diagram()))
      red_lead=[i for i in Polynomial(reductors).lead() if i<lead_index]

      
      p_nav=p.navigation()
      reductors=recursively_insert(p_nav.elseBranch(),p_nav.value(),reductors.navigation())
    else:
      ip.append(p)
  it=ifthen.thenpart
  if prot:
      print "proofing:", ifthen
  ip=logicaland(ip)
  for c in it:
    if prot:
        print "proofing part:",c
    c=logicalor([1+ip,c])

    if c.isZero():
      if prot:
         print "TRUE (trivial)"
      return True
    else:
      c_orig=c
      if redsb:
          c=ll_red_nf(c,reductors)
      else:
          c=ll_red_nf_noredsb(c,reductors)
      if c.isZero():
        if prot:
            print "TRUE"
        return True
      else:
        if prot:
            print "FAILED"
            print "can construct COUNTER EXAMPLE with:", find_one(c)
        return False
        

def proofll_old(ifthen,reductors):
  ip_pre=ifthen.ifpart
  ip=[]
  reductors=copy(reductors)
  for p in ip_pre:
    p=Polynomial(p)
    if p.isZero(): continue
    li=list(p.lead())
    if len(li)==1 and (not (li[0] in reductors)):
      index=li[0]
      tail=p.lead()+p
      reductors[index]=ll_red_nf2(tail,reductors)
      for v in reductors:
        if v<index and index in reductors[v].vars():
          to_red=reductors[v]
          s0=Polynomial(to_red.diagram().subset0(index))
          s1=Polynomial(to_red.diagram().subset1(index))
          to_red=reductors[index]*s1+s0
          reductors[v]=to_red
    else:
      ip.append(p)
  it=ifthen.thenpart
  print "proofing:", ifthen
  ip=logicaland(ip)
  for c in it:
    print "proofing part:",c
    c=logicalor([1+ip,c])

    if c.isZero():
      print "TRUE (trivial)"
    else:
      c_orig=c
      c=ll_red_nf2(c,reductors)
      encoded_redsb=llredsb_Cudd_style([Monomial(Variable(i)) + reductors[i] for i in reductors])
      print ll_red_nf2_Cudd_style(c_orig,encoded_redsb)
      assert c==ll_red_nf2_Cudd_style(c_orig,encoded_redsb)
      assert c==ll_red_nf(c_orig,BooleSet(encoded_redsb.diagram()))
      
      if c.isZero():
        print "TRUE"
      else:
        print "FAILED"

def to_if_then(p):
  if isinstance(p,IfThen):
    return p
  else:
    return IfThen([],[p])
def main(argv=None):
     (opts,args)= parser.parse_args()
     mydata=load_data(args[0])
     claims=mydata.claims
     if opts.method==NF3:
       strat=gen_strat(mydata.ideal)
       for c in claims:
         proof(to_if_then(c),strat)
       del strat
       try:
         del c
       except NameError:
         pass
     else:
       if opts.method==LINEAR_LEAD_NOREDSB:
           reductors=ll_encode(mydata.ideal)
           for c in claims:
                proofll(to_if_then(c),reductors,redsb=False)
           del reductors
           try:
                del c
           except NameError:
                pass
       else:
           reductors=BooleSet(llredsb_Cudd_style(mydata.ideal).diagram())
           for c in claims:
             proofll(to_if_then(c),reductors)
           del reductors
           try:
              del c
           except NameError:
              pass
     return 0

if __name__ == "__main__":
    sys.exit(main())
