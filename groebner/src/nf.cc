/*
 *  nf.cc
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 25.04.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "nf.h"
#include <iostream>
BEGIN_NAMESPACE_PBORIGB
Polynomial nf1(GroebnerStrategy& strat, Polynomial p){
  //parameter by value, so I can modify it
  int index;
  while((index=select1(strat,p))>=0){
    p=spoly(p,strat.generators[index].p);
    
  }
  return p;
}
Polynomial nf2(GroebnerStrategy& strat, Polynomial p){
  //parameter by value, so I can modify it
  int index;
  while((index=select1(strat,p))>=0){
    Polynomial* g=&strat.generators[index].p;
    if (g->nNodes()==1){
      idx_type v=*(g->navigation());
      if (g->length()==1)
      {
        p=Polynomial(p.diagram().subset0(v));
      } else {
        Polynomial p2=Polynomial(p.diagram().subset1(v));
        p=Polynomial(p.diagram().subset0(v))+p2;
      }
    } else {
      p=spoly(p,*g);
    }}
  return p;
}
const int FARE_WORSE=10;
Polynomial nf_delaying(GroebnerStrategy& strat, Polynomial p){
  //parameter by value, so I can modify it
  wlen_type initial=p.eliminationLength();
  int index;
  bool first=true;
  while((index=select1(strat,p))>=0){
    Polynomial* g=&strat.generators[index].p;
    if (g->nNodes()==1){
      idx_type v=*(g->navigation());
      if (g->length()==1)
      {
        p=Polynomial(p.diagram().subset0(v));
      } else {
        Polynomial p2=Polynomial(p.diagram().subset1(v));
        p=Polynomial(p.diagram().subset0(v))+p2;
      }
    } else {
      if ((first==true) ||(strat.generators[index].weightedLength<= FARE_WORSE*initial))
        p=spoly(p,*g);
      else {
        strat.addGeneratorDelayed(p);
        std::cout<<"Delay"<<endl;
        return Polynomial(false);
      }
    }
    first=false;
  }
  return p;
}

static Polynomial exchange(GroebnerStrategy& strat , int i, const Polynomial & p){
  assert(p.lead()==strat.generators[i].lm);
  PolyEntry e(p);
  e.vPairCalculated=strat.generators[i].vPairCalculated;
  Polynomial res=spoly(strat.generators[i].p,p);
  strat.generators[i]=e;
  return res;
}
Polynomial nf_delaying_exchanging(GroebnerStrategy& strat, Polynomial p){
  //parameter by value, so I can modify it
  wlen_type initial=p.eliminationLength();
  int index;
  bool first=true;
  while((index=select1(strat,p))>=0){
    Polynomial* g=&strat.generators[index].p;
    if (g->nNodes()==1){
      idx_type v=*(g->navigation());
      if (g->length()==1)
      {
        p=Polynomial(p.diagram().subset0(v));
      } else {
        Polynomial p2=Polynomial(p.diagram().subset1(v));
        p=Polynomial(p.diagram().subset0(v))+p2;
      }
    } else {
      if ((p.lead()==strat.generators[index].lm) && (p.eliminationLength()<strat.generators[index].weightedLength)){
        p=exchange(strat,index,p);
        std::cout<<"Exchange"<<endl;
      } else{
        if ((first==true) ||(strat.generators[index].weightedLength<= FARE_WORSE*initial))
          p=spoly(p,*g);
        else {
          strat.addGeneratorDelayed(p);
          std::cout<<"Delay"<<endl;
          return Polynomial(false);
        }
      }
    }
    first=false;
  }
  return p;
}
END_NAMESPACE_PBORIGB