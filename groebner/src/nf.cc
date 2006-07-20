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
    assert(index<strat.generators.size());
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
      
      if (strat.generators[index].length==1){
        assert(strat.generators[index].p.length()==1);
        assert(strat.generators[index].lm==strat.generators[index].p.lead());
        //if (p!=strat.generators[index].lm)
          p=reduce_by_monom(p,strat.generators[index].lm);
        //else
        //  p=Polynomial(0);
      } else{
        assert(!(p.isZero()));
        assert(p.reducibleBy(*g));
        assert(!(g->isZero()));
        if (strat.generators[index].length==2)
          p=reduce_by_binom(p,strat.generators[index].p);
        else{
          if (strat.generators[index].deg==1){
            //implies lmDeg==1, ecart=0
            //cout<<"REDUCE_COMPLETE\n";
            assert(strat.generators[index].ecart()==0);
            assert(strat.generators[index].lmDeg==1);
            p=reduce_complete(p,strat.generators[index].p);
          }
          else{
            p=spoly(p,*g);
          }
          
        }
      }
    }
  }
  return p;
}



Polynomial nf2_short(GroebnerStrategy& strat, Polynomial p){
  //parameter by value, so I can modify it
  int index;
  while((index=select_short(strat,p))>=0){
    assert(index<strat.generators.size());
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
      
      if (strat.generators[index].length==1){
        assert(strat.generators[index].p.length()==1);
        assert(strat.generators[index].lm==strat.generators[index].p.lead());
        //if (p!=strat.generators[index].lm)
        p=reduce_by_monom(p,strat.generators[index].lm);
        //else
        //  p=Polynomial(0);
      } else{
        assert(!(p.isZero()));
        assert(p.reducibleBy(*g));
        assert(!(g->isZero()));
        if (strat.generators[index].length==2)
          p=reduce_by_binom(p,strat.generators[index].p);
        else{
          if (strat.generators[index].deg==1){
            //implies lmDeg==1, ecart=0
            //cout<<"REDUCE_COMPLETE\n";
            assert(strat.generators[index].ecart()==0);
            assert(strat.generators[index].lmDeg==1);
            p=reduce_complete(p,strat.generators[index].p);
          }
          else{
            p=spoly(p,*g);
          }
          
        }
      }
    }
  }
  return p;
}



Polynomial nf3(GroebnerStrategy& strat, Polynomial p){
  int index;
  while((index=select1(strat,p))>=0){
    assert(index<strat.generators.size());
  
    Polynomial* g=&strat.generators[index].p;
    
    if //((strat.generators[index].deg==1)&&(lm!=strat.generators[index].lm)){
    ((strat.generators[index].length<4) &&(strat.generators[index].ecart()==0) && (p.lead()!=strat.generators[index].lm)){
      p=reduce_complete(p,strat.generators[index].p);

    } else{
      p=spoly(p,*g);
    }
  }
  return p;
}
Polynomial nf3_db(GroebnerStrategy& strat, Polynomial p, int deg_bound){
  int index;
  while((index=select1(strat,p))>=0){
    assert(index<strat.generators.size());
    if((strat.generators[index].ecart()>0) && strat.generators[index].ecart()+p.lmDeg()-strat.generators[index].lm.deg()>deg_bound)
        return p;
    Polynomial* g=&strat.generators[index].p;
    
    if //((strat.generators[index].deg==1)&&(lm!=strat.generators[index].lm)){
    ((strat.generators[index].length<4) &&(strat.generators[index].ecart()==0) && (p.lead()!=strat.generators[index].lm)){
      p=reduce_complete(p,strat.generators[index].p);

    } else{
      p=spoly(p,*g);
    }
  }
  return p;
}
Polynomial nf3_short(GroebnerStrategy& strat, Polynomial p){
  int index;
  while((index=select_short(strat,p))>=0){
    assert(index<strat.generators.size());
    Polynomial* g=&strat.generators[index].p;
    
    if //((strat.generators[index].deg==1)&&(lm!=strat.generators[index].lm)){
    ((strat.generators[index].length<4) &&(strat.generators[index].ecart()==0) && (p.lead()!=strat.generators[index].lm)){
      p=reduce_complete(p,strat.generators[index].p);
      
    } else{
      p=spoly(p,*g);
    }
  }
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
  //PolyEntry e(p);
  //e.vPairCalculated=strat.generators[i].vPairCalculated;
  Polynomial res=strat.generators[i].p;
  strat.generators[i].p=p;
  strat.generators[i].recomputeInformation();
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
  bool isOne(){
    return p.isOne();
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

  int s=curr.size();
  
  if ((strat.generators[index].length>2)||(lm==strat.generators[index].lm)){
    
    
    
    if //((strat.generators[index].deg==1)&&(lm!=strat.generators[index].lm)){
    ((strat.generators[index].length<4) &&(strat.generators[index].ecart()==0) && (lm!=strat.generators[index].lm)){
      //implies lmDeg==1, ecart=0
      //cout<<"REDUCE_COMPLETE\n";
      //assert(strat.generators[index].ecart()==0);
      //assert(strat.generators[index].lmDeg==1);
      //p=reduce_complete(p,strat.generators[index].p);
      
      for(int i=0;i<s;i++){
        
        //curr[i].add(p_high, deg_high);
        Polynomial to_red=curr[i].value();
        to_red=reduce_complete(to_red,strat.generators[index].p);
        curr[i]=PolynomialSugar(to_red);
      }
      
      
    }
    else{
      Polynomial p_high=(lm/strat.generators[index].lm)*strat.generators[index].p;
  deg_type deg_high=strat.generators[index].ecart()+lm.deg();
  assert(p_high.lead()==lm);
      for(int i=0;i<s;i++){
        curr[i].add(p_high, deg_high);
      }
    }
  } else {
    assert(strat.generators[index].length<=2);
    if (strat.generators[index].length==2){
      assert(strat.generators[index].p.length()==2);
      for(int i=0;i<s;i++){
        
        //curr[i].add(p_high, deg_high);
        Polynomial to_red=curr[i].value();
        to_red=reduce_by_binom(to_red,strat.generators[index].p);
        curr[i]=PolynomialSugar(to_red);
      }
    } else {
      ///@todo: check for sugar garanties
      assert(strat.generators[index].length==1);
      assert(strat.generators[index].p.length()==1);
    
      for(int i=0;i<s;i++){
        Polynomial to_red=curr[i].value();
        to_red=reduce_by_monom(to_red,strat.generators[index].lm);//BooleSet(to_red).diff(strat.generators[index].lm.multiples(to_red.usedVariables()));
        curr[i]=PolynomialSugar(to_red);
      }
      
    }
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
  
  Polynomial pivot;
  /*if (pivot_el<strat.generators[index].weightedLength){
      pivot=redTail(strat,curr[found].value());
      pivot_el=pivot.eliminationLength();
      curr[found]=pivot;
      }
    */  
  if (pivot_el<strat.generators[index].weightedLength){
    
    for(int i=0;i<s;i++){
      if(i==found) continue;
      curr[i].add(curr[found].value(), curr[found].getSugar());
      ///@todo different prototpye
    }

    if ((pivot.deg()<=strat.generators[index].deg) &&(lm.deg()==strat.generators[index].lmDeg)){
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
  MonomialSet m1=s1;
  MonomialSet m2=s2;
  Monomial lm=Polynomial(m1).lead();
  int d=lm.deg()/2;
  int i;
  Monomial::const_iterator iter=lm.begin();
  for(i=0;i<d;i++){
    assert(iter!=lm.end());
    m1=m1.subset1(*iter);
    m2=m2.subset1(*iter);
    iter++;
    
  }
  return m1.length()+m2.length()-2*m1.intersect(m2).length();
}


static void step_T_complex(std::vector<PolynomialSugar>& curr, std::vector<Polynomial>& result,  const BooleMonomial& lm,GroebnerStrategy& strat){
  std::sort(curr.begin(), curr.end(), PSCompareByEl());
  const int max_cans=5;
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


std::vector<Polynomial> parallel_reduce(std::vector<Polynomial> inp, GroebnerStrategy& strat, int average_steps, double delay_f){

  
  std::vector<Polynomial> result;
  int i,s;
  s=inp.size();
  int max_steps=average_steps*s;
  int steps=0;
  std::priority_queue<PolynomialSugar, std::vector<PolynomialSugar>, LMLessComparePS> to_reduce;
  deg_type max_sugar=0;
  unsigned int max_length=0;
  for(i=0;i<s;i++){
    if (inp[i].isOne()){
      result.push_back(inp[i]);
      return result;
    }
    PolynomialSugar to_push=PolynomialSugar(inp[i]);
    max_length=std::max(max_length,inp[i].length());
    max_sugar=std::max(max_sugar,to_push.getSugar());
    
    to_reduce.push(to_push);
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
      steps=steps+curr.size();
      if (curr.size()>1){
        step_S_T(curr,result,lm, index,strat);
        //step_S(curr,result,lm, index,strat);
      } else{
        step_S(curr,result,lm, index,strat);
      }
    } else {
      //simly take the first, not so clever
     
      int i,s;
      s=curr.size();
      if (s>1){
        steps+=curr.size()-1;
        step_T_complex(curr,result,lm,strat);
      } else{
        assert(s==1);
        result.push_back(curr[0].value());
        curr.clear();
      }
      
    }
    
    //after each loop push back
    s=curr.size();
    for(i=0;i<s;i++){
      if (!(curr[i].isZero())){
        if (((curr[i].getSugar()<=max_sugar)&&(curr[i].value().length()<=delay_f*max_length))||(curr[i].isOne())){
          if (curr[i].isOne()){
            result.clear();
            result.push_back(curr[i].value());
            return result;
          }
          to_reduce.push(curr[i]);
        } else {
          cout<<"Delaying"<<endl;
          cout.flush();
          strat.addGeneratorDelayed(curr[i].value());
        }
      }
    }
    curr.clear();
    if (steps>max_steps){
        cout<<"Too many steps\n"<<endl;
        while (!(to_reduce.empty())){
            
            Monomial lm=to_reduce.top().lead();
            if (select1(strat,lm)>=0){
                while((!(to_reduce.empty()))&&(to_reduce.top().lead()==lm)){
                    strat.addGeneratorDelayed(to_reduce.top().value());
                    to_reduce.pop();
                } 
            }else {
                std::vector<PolynomialSugar> this_lm;
                while((!(to_reduce.empty()))&&(to_reduce.top().lead()==lm)){
                    this_lm.push_back(to_reduce.top());
                    to_reduce.pop();
                }
                std::vector<PolynomialSugar>::iterator for_basis=std::min_element(this_lm.begin(),this_lm.end(),PSCompareByEl());
                int i;
                for(i=0;i<this_lm.size();i++){
                    if (this_lm[i].value()!=for_basis->value()){
                        strat.addGeneratorDelayed(this_lm[i].value());
                    }
                }
                result.push_back((*for_basis).value());
            }
            
            
            
 
        }
        return result;
    }
  }
  return result;
  
}
typedef LessWeightedLengthInStrat StratComparerForSelect;
static int select_short(GroebnerStrategy& strat, const Polynomial& p){
  MonomialSet ms=strat.leadingTerms.intersect(p.lmDivisors());
  //Polynomial workaround =Polynomial(ms);
  
  if (ms.emptiness())
    return -1;
  else {
    
    //Monomial min=*(std::min_element(ms.begin(),ms.end(), LessWeightedLengthInStrat(strat)));
    Monomial min=*(std::min_element(ms.begin(),ms.end(), LessWeightedLengthInStrat(strat)));
    
    int res=strat.lm2Index[min];
    if ((strat.generators[res].weightedLength<=2)/*||(strat.generators[res].ecart()==0)*/) return res;
    else return -1;
  }
  
}
static int select_short(GroebnerStrategy& strat, const Monomial& m){
  MonomialSet ms=strat.leadingTerms.intersect(m.divisors());
  if (ms.emptiness())
    return -1;
  else {
    //Monomial min=*(std::min_element(ms.begin(),ms.end(), LessWeightedLengthInStrat(strat)));
    Monomial min=*(std::min_element(ms.begin(),ms.end(), LessWeightedLengthInStrat(strat)));
    int res=strat.lm2Index[min];
    if ((strat.generators[res].weightedLength<=2)/*||(strat.generators[res].ecart()==0)*/) return res;
    else return -1;

  }
}



static int select1(GroebnerStrategy& strat, const Polynomial& p){
  MonomialSet ms=strat.leadingTerms.intersect(p.lmDivisors());
  //Polynomial workaround =Polynomial(ms);
  
  if (ms.emptiness())
    return -1;
  else {
    
    //Monomial min=*(std::min_element(ms.begin(),ms.end(), LessWeightedLengthInStrat(strat)));
    Monomial min=*(std::min_element(ms.begin(),ms.end(), StratComparerForSelect(strat)));

    return strat.lm2Index[min];
     
  }
  
}
static int select1(GroebnerStrategy& strat, const Monomial& m){
  MonomialSet ms=strat.leadingTerms.intersect(m.divisors());
  if (ms.emptiness())
    return -1;
  else {
    //Monomial min=*(std::min_element(ms.begin(),ms.end(), LessWeightedLengthInStrat(strat)));
    Monomial min=*(std::min_element(ms.begin(),ms.end(), StratComparerForSelect(strat)));
    return strat.lm2Index[min];
  }
}

static Polynomial nf4(GroebnerStrategy& strat, Polynomial p){
  int index;
  while((index=select1(strat,p))>=0){
    assert(index<strat.generators.size());
    Polynomial* g=&strat.generators[index].p;
    
    if((strat.generators[index].ecart()==0) && (strat.generators[index].length<=4) &&(strat.generators[index].lm!=p.lead())){
      p=reduce_complete(p,strat.generators[index].p);
      
    } else{
      p=spoly(p,*g);
    }
  }
  return p;
  
}

static Polynomial add_up_monomials(std::vector<Monomial>& res_vec, int start, int end){
    int s=end-start;
    if (s==0) return Polynomial();
    if (s==1) return Polynomial(res_vec[start]);
    int h=s/2;
    return add_up_monomials(res_vec,start,start+h)+add_up_monomials(res_vec,start+h,end);
}
static Polynomial add_up_monomials(std::vector<Monomial>& res_vec){
    int s=res_vec.size();
    if (s==0) return Polynomial();
    if (s==1) return Polynomial(res_vec[0]);
    int h=s/2;
    return add_up_monomials(res_vec,0,h)+add_up_monomials(res_vec,h,s);
}
Polynomial redTail(GroebnerStrategy& strat, Polynomial p){
  Polynomial res;
  int deg_bound=p.deg();
  std::vector<Monomial> res_vec;
  while(!(p.isZero())){
    Monomial lm=p.lead();
    //res+=lm;
    res_vec.push_back(lm);
    
    //p-=lm;
    p=Polynomial(p.diagram().diff(lm.diagram()));
    p=nf3(strat,p);
  }
  res=add_up_monomials(res_vec);
  return res;
}
Polynomial red_tail_short(GroebnerStrategy& strat, Polynomial p){
  Polynomial res;
  while(!(p.isZero())){
    Polynomial lm=p.lead();
    res+=lm;
    p-=lm;
    p=nf3_short(strat,p);
  }
  return res;
}
Polynomial red_tail_self_tuning(GroebnerStrategy& strat, Polynomial p){
  Polynomial res;
  int orig_length=p.length();
  bool short_mode=false;
  while(!(p.isZero())){
    Polynomial lm=p.lead();
    res+=lm;
    p-=lm;
    if (short_mode)
      p=nf3_short(strat,p);
    else
      p=nf3(strat,p);
    if ((!short_mode)&&(p.length()+res.length()>2*orig_length+5))
      short_mode=true;
  }
  return res;
}


END_NAMESPACE_PBORIGB
