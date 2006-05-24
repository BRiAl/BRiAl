/*
 *  groebner_alg.ccc
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 20.04.06.
 *  Copyright 2006 Mathematisches Forschungsinstitut Oberwolfach. All rights reserved.
 *
 */

#include "groebner_alg.h"
#include <algorithm>
BEGIN_NAMESPACE_PBORIGB
void PairManager::introducePair(const Pair& pair){
  queue.push(pair);
}
bool PairManager::pairSetEmpty() const{
  return queue.empty();
}
Polynomial PairManager::nextSpoly(const PolyEntryVector& gen){
  assert(!(pairSetEmpty()));
  //const IJPairData* ij= dynamic_cast<IJPairData*>(queue.top().data.get());
  if (queue.top().getType()==IJ_PAIR){
    const IJPairData* ij= (IJPairData*)(queue.top().data.get());
    //cout<<"mark1"<<endl;
    this->status.setToHasTRep(ij->i,ij->j);
  } else{
    
    //const VariablePairData *vp=dynamic_cast<VariablePairData*>(queue.top().data.get());
    if (queue.top().getType()==VARIABLE_PAIR){
      //cout<<"mark2"<<endl;
      const VariablePairData *vp=(VariablePairData*)(queue.top().data.get());
      this->strat->generators[vp->i].vPairCalculated.insert(vp->v);
      int i=vp->i;
      Polynomial res=Pair(queue.top()).extract(gen);
      
      queue.pop();
      Polynomial lm=res.lead();
      
      if (lm==this->strat->generators[i].lm)
        res+=this->strat->generators[i].p;
      //best reductor by genesis
      return res;
      
    }
  }
  Polynomial res=Pair(queue.top()).extract(gen);
  
  queue.pop();
  
  return res;
  
}

/// assumes that divisibility condition is fullfilled
class ChainCriterion{
  /// @todo: connect via vars
public:
  const GroebnerStrategy* strat;
  int i,j;
  ChainCriterion(const GroebnerStrategy& strat, const int& i, const int& j){
    this->strat=&strat;
    this->i=i;
    this->j=j;
  }
  bool operator() (const Monomial& lm){
    int index=strat->lm2Index.find(lm)->second;
    //we know such an entry exists
    if ((index!=i)&&(index!=j)){
      if ((strat->pairs.status.hasTRep(i,index)) && (strat->pairs.status.hasTRep(j,index))){
        
        return true;
      }
    }
    return false;
  }
};
class ChainVariableCriterion{
  ///connect via pairs
public:
  const GroebnerStrategy* strat;
  int i;
  idx_type v;
  ChainVariableCriterion(const GroebnerStrategy& strat, int i, idx_type v){
    this->strat=&strat;
    this->i=i;
    this->v=v;
  }
  bool operator() (const Monomial& lm){
    int index=strat->lm2Index.find(lm)->second;
    //we know such an entry exists
    if (index!=i){
      //would be still true for i, but how should that happen
      if ((strat->pairs.status.hasTRep(i,index)) &&(strat->generators[index].vPairCalculated.count(v)==1))
        return true;
    }
   return false;
  }
};

void PairManager::cleanTopByChainCriterion(){
  while(!(this->pairSetEmpty())){
    //cout<<"clean"<<endl;
    
    //const IJPairData* ij= dynamic_cast<IJPairData*>(queue.top().data.get());
    if (queue.top().getType()==IJ_PAIR){
      const IJPairData* ij= (IJPairData*)(queue.top().data.get());

    ///@todo implement this
      const int i=ij->i;
      const int j=ij->j;
      const Monomial lm=queue.top().lm;
      //cout<<"try chain crit"<<endl;
      const BooleSet lms=this->strat->leadingTerms.intersect(lm.divisors());
      //if (lms.length()>2)
        //cout<<"non empty"<<endl;
      if (std::find_if(lms.begin(),lms.end(),ChainCriterion(*(this->strat),i,j))!=lms.end()){
        this->queue.pop();
        strat->pairs.status.setToHasTRep(i,j);
        //cout<<"Chain Criterion"<<endl;
        //cout.flush();
        strat->chainCriterions++;
      } else {
        return;
      }
    }else {
        //const VariablePairData *vp=dynamic_cast<VariablePairData*>(queue.top().data.get());
        if (queue.top().getType()==VARIABLE_PAIR)
        {
          const VariablePairData *vp=(VariablePairData*)(queue.top().data.get());
          if (strat->generators[vp->i].length==1){
            //has been shortened
            queue.pop();
            continue;
          }
          const BooleSet lms=this->strat->leadingTerms.intersect(strat->generators[vp->i].lm.divisors());
          
          Monomial lm=strat->generators[vp->i].lm;
          if (!(strat->leadingTerms.intersect(lm.divisors()).diff(Polynomial(lm)).emptiness())){
            strat->variableChainCriterions++;
           queue.pop();
          } else {
            if (std::find_if(lms.begin(),lms.end(),ChainVariableCriterion(*(this->strat),vp->i,vp->v))!=lms.end()){
              strat->generators[vp->i].vPairCalculated.insert(vp->v);
              this->queue.pop();
              //cout<<"Variable Chain Criterion"<<endl;
              //cout.flush();
              strat->variableChainCriterions++;
            } else return;}
          
          
          
        } else return;
    }
  }
}
PolyEntry::PolyEntry(const Polynomial &p){
  this->p=p;
  this->lm=p.lead();
  this->weightedLength=p.eliminationLength();
  this->length=p.length();
  this->usedVariables=p.usedVariables();
  this->deg=p.deg();
  this->tailVariables=(p-lm).usedVariables();
  this->lmDeg=p.lmDeg();
}
void PolyEntry::recompute_information(){
  assert(this->lm==p.lead());
  this->weightedLength=p.eliminationLength();
  this->length=p.length();
  this->usedVariables=p.usedVariables();
  this->deg=p.deg();
  this->tailVariables=(p-lm).usedVariables();
  assert(this->lmDeg==p.lmDeg());
}
Polynomial reduce_by_monom(const Polynomial& p, const Monomial& m){
  
  if (m.deg()==1){
    cout<<"branch 1\n";
    cout.flush();
    Monomial::const_iterator it=m.begin();
    return Polynomial(BooleSet(p).subset0(*it));
  }
  return p%m;
  Monomial::const_iterator it=m.begin();
  Monomial::const_iterator end=m.end();
  BooleSet dividing_terms
  =BooleSet(p/m);
  //=BooleSet(p);
  
  /*while(it!=end){
    dividing_terms=dividing_terms.subset1(*it);
    it++;
  }*/
  //fast multiply back
  cout<<"branch2\n";
  cout.flush();
  
  dividing_terms.unateProductAssign(m.diagram());
  return Polynomial(BooleSet(p).diff(dividing_terms));
  //return Polynomial(BooleSet(p).diff(BooleSet(m*Polynomial(dividing_terms))));
}
static Polynomial cancel_monomial_in_tail(const Polynomial& p, const Monomial & m){
  Monomial lm=p.lead();
  
  Polynomial res=reduce_by_monom(p,m);
  if (res.lead()==lm){
    return res;
  } else {
    return res+lm;
  }
  /*Polynomial tail=p-lm;
  Monomial used_var=tail.usedVariables();
  
  if (used_var.reducibleBy(m)){
    tail=Polynomial(BooleSet(tail).diff(m.multiples(used_var)));
    
  }
  return tail+lm;*/
}

Polynomial reduce_by_binom(const Polynomial& p, const Polynomial& binom){
  assert(binom.length()==2);
  Monomial p_lead=p.lead();
  Monomial bin_lead=binom.lead();
  Polynomial p_reducible=BooleSet(p).intersect(bin_lead.multiples(p.usedVariables()));
  Polynomial p_irreducible=Polynomial(BooleSet(p).diff(p_reducible));
  Monomial bin_last=(binom-bin_lead).lead();
  p_reducible/=bin_lead;
  p_reducible*=bin_last;
  return p_reducible+p_irreducible;
}
static Polynomial reduce_by_binom_in_tail (const Polynomial& p, const Polynomial& binom){
  assert(binom.length()==2);
  Monomial lm=p.lead();
  return lm+reduce_by_binom(p-lm,binom);
}
void GroebnerStrategy::addGenerator(const BoolePolynomial& p){
  PolyEntry e(p);
  if (e.length==1){
    assert(e.p.length()==1);
    Monomial m=e.lm;
    int i;
    for(i=0;i<this->generators.size();i++){
      if ((this->generators[i].length>1) &&(this->generators[i].tailVariables.reducibleBy(e.usedVariables))){
        Polynomial new_p=cancel_monomial_in_tail(this->generators[i].p,m);
        if (new_p!=this->generators[i].p){
          this->generators[i].p=new_p;
          this->generators[i].recompute_information();
        }
      }
    }
  }
/*if ((e.length==2)&&(e.ecart()==0)){
    Monomial m=e.lm;
    int i;
    for(i=0;i<this->generators.size();i++){
      if ((this->generators[i].length>1)&&(this->generators[i].tailVariables.reducibleBy(e.usedVariables))){
        Polynomial new_p=reduce_by_binom_in_tail(this->generators[i].p,e.p);
        if (new_p!=this->generators[i].p){
          this->generators[i].p=new_p;
          this->generators[i].recompute_information();
        }
      }
    }
  }
  */
  //do this before adding leading term
  Monomial::const_iterator it=e.lm.begin();
  Monomial::const_iterator end=e.lm.end();
  BooleSet intersecting_terms=this->leadingTerms;
  while(it!=end){
    //cout<<"intersect"<<endl;
    intersecting_terms=intersecting_terms.subset0(*it);
    it++;
    
  }
  leadingTerms.uniteAssign(Polynomial(e.lm).diagram());
  
  generators.push_back(e);
  pairs.status.prolong(PairStatusSet::HAS_T_REP);
  const int s=generators.size()-1;
  lm2Index[generators[s].lm]=s;

  
  int i;

  
  
  it=generators[s].lm.begin();
  end=generators[s].lm.end();
  while(it!=end){
    if ((generators[s].lm.deg()==1) ||((MonomialSet(p).subset0(*it).emptiness())||(MonomialSet(p).subset0(*it)==(MonomialSet(p).subset1(*it))))){
      generators[s].vPairCalculated.insert(*it);
    } else
      this->pairs.introducePair(Pair(s,*it,generators,VARIABLE_PAIR));
    it++;
    
  }
  intersecting_terms= this->leadingTerms.diff(intersecting_terms);
  BooleSet::const_iterator is_it=intersecting_terms.begin();
  BooleSet::const_iterator is_end=intersecting_terms.end();
  while(is_it!=is_end){
    int index =this->lm2Index[*is_it];
    if (index!=s){
      
      //product criterion doesn't hold
      //try length 1 crit
      if (!((this->generators[index].length==1) &&(this->generators[s].length==1)))
      {        
        this->pairs.introducePair(Pair(index,s,generators));
        this->pairs.status.setToUncalculated(index,s);
      }
    }
    is_it++;
  }
}

void GroebnerStrategy::addGeneratorDelayed(const BoolePolynomial& p){
  this->pairs.introducePair(Pair(p));
}
END_NAMESPACE_PBORIGB
