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

using std::cout;
using std::endl;

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

static Polynomial exchange_with_promise(GroebnerStrategy& strat , int i, const Polynomial & p){
  assert(p.lead()==strat.generators[i].lm);
  PolyEntry e(p);
  e.vPairCalculated=strat.generators[i].vPairCalculated;
  Polynomial res=strat.generators[i].p;
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


template <> void SlimgbReduction<SLIMGB_SIMPLEST>::reduce(){
  while (!(to_reduce.empty())){
    //cout<<"looping"<<endl;
    std::vector<Polynomial> curr;
    curr.push_back(to_reduce.top());
    to_reduce.pop();
    //cout<<curr[0];
    Monomial lm=curr[0].lead();
    while ((!(to_reduce.empty())) && (to_reduce.top().lead()==lm)){
      curr.push_back(to_reduce.top());
      to_reduce.pop();
      //cout<<"same"<<endl;
      //cout.flush();
    }
    //cout<<lm;
    //cout.flush();
    int index=select1(*strat,lm);
    if (index>=0){
      Polynomial p_high=(lm/strat->generators[index].lm)*strat->generators[index].p;
      int i,s;
      s=curr.size();
      assert(p_high.lead()==lm);
      for(i=0;i<s;i++){
        curr[i]+=p_high;
        if (!(curr[i].isZero())){
          to_reduce.push(curr[i]);
        }
      }
    } else {
      //simly take the first, not so clever
      Polynomial reductor=curr.back();
      curr.pop_back();
      int i,s;
      s=curr.size();
      if (s>0){
        for(i=0;i<s;i++){
          curr[i]+=reductor;
          if (!(curr[i].isZero())){
            assert(curr[i].lead()<lm);
            to_reduce.push(curr[i]);
          }
          
        }
        result.push_back(reductor);
      } else{
        assert(s==0);
        result.push_back(curr[0]);
      }
    }
  
  }
  
}



class PolynomialSugar{
public:
  PolynomialSugar(const Polynomial& p){
    this->p=p;
    sugar=p.deg();
    this->lm=p.lead();
  }
  const BooleMonomial& lead() const{
    return this->lm;
  }
  deg_type getSugar() const{
    return sugar;
  }
  bool isZero() const{
    return p.isZero();
  }
  void add(const Polynomial p2, deg_type sugar2){
    this->p=p+p2;
    this->sugar=std::max(sugar2,this->sugar);
    this->lm=this->p.lead();
    
  }
  void adjustSugar(){
    sugar=p.deg();
  }
  Polynomial value() const{
    return p;
  }
  wlen_type eliminationLength() const{
    return p.eliminationLengthWithDegBound(sugar);
  }
  void adjustLm(){
    this->lm=this->p.lead();
  }
protected:
  Monomial lm;
  deg_type sugar;
  Polynomial p;
};

class LMLessComparePS{
public:
  bool operator() (const PolynomialSugar& p1, const PolynomialSugar& p2){
    return p1.lead()<p2.lead();
  }
};


static void step_S(std::vector<PolynomialSugar>& curr, std::vector<Polynomial>& result, const BooleMonomial& lm, int index, GroebnerStrategy& strat){
  Polynomial p_high=(lm/strat.generators[index].lm)*strat.generators[index].p;
  deg_type deg_high=strat.generators[index].ecart()+lm.deg();
  int s=curr.size();
  assert(p_high.lead()==lm);
  for(int i=0;i<s;i++){
    curr[i].add(p_high, deg_high);
  }
}

static void step_S_T(std::vector<PolynomialSugar>& curr, std::vector<Polynomial>& result,  const BooleMonomial& lm, int index, GroebnerStrategy& strat){
  int s=curr.size();
  
  int found;
  wlen_type pivot_el;
  found=0;
  pivot_el=curr[0].eliminationLength();
  
  for (int i=1;i<s;i++){
    wlen_type comp=curr[i].eliminationLength();
    if (comp<pivot_el){
      found=i;
      pivot_el=comp;
    }
  }
  if (pivot_el<strat.generators[index].weightedLength){
    for(int i=0;i<s;i++){
      if(i==found) continue;
      curr[i].add(curr[found].value(), curr[found].getSugar());
      ///@todo different prototpye
    }
    
    if (lm.deg()==strat.generators[index].lm){
      assert(lm==strat.generators[index].lm);
      curr[found]=PolynomialSugar(exchange_with_promise(strat, index, curr[found].value()));
      std::cout<<"Exchange"<<endl;
    }
    
    
    deg_type deg_high=strat.generators[index].ecart()+lm.deg();
    curr[found].add((lm/strat.generators[index].lm)*strat.generators[index].p, deg_high);
  } else 
    step_S(curr,result,lm, index,strat);
  
}


static void step_T_simple(std::vector<PolynomialSugar>& curr, std::vector<Polynomial>& result,  const BooleMonomial& lm,GroebnerStrategy& strat){
  int s=curr.size();
  Polynomial reductor;
  int found;
  wlen_type pivot_el;
  found=0;
  pivot_el=curr[0].eliminationLength();
  int i;
  for (i=1;i<s;i++){
    wlen_type comp=curr[i].eliminationLength();
    if (comp<pivot_el){
      found=i;
      pivot_el=comp;
    }
  }
  reductor=curr[found].value();
  curr.erase(curr.begin()+found);
  
  s=s-1;
  
  //will calculate elimLength later so does not increase assymptotic complexity
  deg_type deg_high=reductor.deg();
  for(i=0;i<s;i++){
    //if (i==found) continue;
    curr[i].add(reductor, deg_high);
    if (!(curr[i].isZero())){
      assert(curr[i].lead()<lm);
    }
    
  }
  result.push_back(reductor);
  

}


class PSCompareByEl{
public:
  bool operator() (const PolynomialSugar& p1, const PolynomialSugar& p2){
    return ((p1.getSugar()<p2.getSugar()) ||((p1.getSugar()<=p2.getSugar()) && (p1.eliminationLength()<p2.eliminationLength())));
  }
};

int sum_size(const MonomialSet& s1, const MonomialSet& s2){
  return s1.length()+s2.length()-2*s1.intersect(s2).length();
}


static void step_T_complex(std::vector<PolynomialSugar>& curr, std::vector<Polynomial>& result,  const BooleMonomial& lm,GroebnerStrategy& strat){
  std::sort(curr.begin(), curr.end(), PSCompareByEl());
  const int max_cans=3;
  int s=curr.size();
  Polynomial reductor;
  int found;
  wlen_type pivot_el;
  
  pivot_el=curr[0].eliminationLength();
  
  int i,j;
  for (i=s-1;i>0;i--){
    int found=0;
    MonomialSet as_set(curr[i].value());
    int c_size=sum_size(as_set, MonomialSet(curr[0].value()));
    for (j=1;j<std::min(i,max_cans);j++){ 
      int size2=sum_size(as_set, MonomialSet(curr[j].value()));
      if (size2<c_size){
        found=j;
        c_size=size2;
      }
    }
    curr[i].add(curr[found].value(), curr[found].getSugar());
  }
  reductor=curr[0].value();
  curr.erase(curr.begin());
  result.push_back(reductor);
  
  
}

static void step_T(std::vector<PolynomialSugar>& curr, std::vector<Polynomial>& result,  const BooleMonomial& lm,GroebnerStrategy& strat){
  int s=curr.size();

  if (s>2) return step_T_complex(curr,result, lm, strat);
  else
    step_T_complex(curr,result, lm, strat);


  
  
}


std::vector<Polynomial> parallel_reduce(std::vector<Polynomial> inp, GroebnerStrategy& strat){
  std::vector<Polynomial> result;
  int i,s;
  s=inp.size();
  std::priority_queue<PolynomialSugar, std::vector<PolynomialSugar>, LMLessComparePS> to_reduce;

  for(i=0;i<s;i++){
    to_reduce.push(PolynomialSugar(inp[i]));
  }
  
  while (!(to_reduce.empty())){

    std::vector<PolynomialSugar> curr;
    curr.push_back(to_reduce.top());
    to_reduce.pop();

    Monomial lm=curr[0].lead();
    while ((!(to_reduce.empty())) && (to_reduce.top().lead()==lm)){
      curr.push_back(to_reduce.top());
      to_reduce.pop();

    }
 
    int index=select1(strat,lm);
    if (index>=0){
      if (curr.size()>1){
        step_S_T(curr,result,lm, index,strat);
      } else{
        step_S(curr,result,lm, index,strat);
      }
    } else {
      //simly take the first, not so clever
     
      int i,s;
      s=curr.size();
      if (s>1){
        step_T_simple(curr,result,lm,strat);
      } else{
        assert(s==1);
        result.push_back(curr[0].value());
        curr.clear();
      }
      
    }
    
    //after each loop push back
    s=curr.size();
    for(i=0;i<s;i++){
      if (!(curr[i].isZero()))
        to_reduce.push(curr[i]);
    }
    curr.clear();
  }
  return result;
  
}


static int select1(GroebnerStrategy& strat, const Polynomial& p){
  MonomialSet ms=strat.leadingTerms.intersect(p.lmDivisors());
  Polynomial workaround =Polynomial(ms);
  if (ms.emptiness())
    return -1;
  else {
    Monomial min=*(std::min_element(workaround.begin(),workaround.end(), LessWeightedLengthInStrat(strat)));
    
    return strat.lm2Index[min];
  }
}
static int select1(GroebnerStrategy& strat, const Monomial& m){
  MonomialSet ms=strat.leadingTerms.intersect(m.divisors());
  if (ms.emptiness())
    return -1;
  else {
    Monomial min=*(std::min_element(ms.begin(),ms.end(), LessWeightedLengthInStrat(strat)));
    
    return strat.lm2Index[min];
  }
}
static Polynomial redTail(GroebnerStrategy& strat, Polynomial tail){
  
}
END_NAMESPACE_PBORIGB