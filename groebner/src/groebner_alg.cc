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
static bool extended_product_criterion(const PolyEntry& m, const PolyEntry& m2){
  //BooleMonomial m;
  ///@todo need GCDdeg
  bool res=(m.lm.GCD(m2.lm).deg()==common_literal_factors_deg(m.literal_factors, m2.literal_factors));
  //if (res)
  //  cout<<"EXTENDED PRODUCT_CRIT";
  return res;
}

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
      if (!(res.isZero())){
        Monomial lm=res.lead();
      
        if (lm==this->strat->generators[i].lm)
          res+=this->strat->generators[i].p;
      }
      /*else{
        if (lm.reducibleBy(this->strat->generators[i].lm))
          res=spoly(res,this->strat->generators[i].p);
      }*/
      //best reductor by genesis
      return res;
      
    }
  }
  Polynomial res=Pair(queue.top()).extract(gen);
  
  queue.pop();
  
  return res;
  
}
GroebnerStrategy::GroebnerStrategy(const GroebnerStrategy& orig)
:pairs(orig.pairs),generators(orig.generators),leadingTerms(orig.leadingTerms), lm2Index(orig.lm2Index)
{
 
  reductionSteps=orig.reductionSteps;
  normalForms=orig.normalForms;
  currentDegree=orig.currentDegree;
  chainCriterions=orig.chainCriterions;
  variableChainCriterions=orig.variableChainCriterions;
  easyProductCriterions=orig.easyProductCriterions;
  extendedProductCriterions=orig.extendedProductCriterions;
  averageLength=orig.averageLength;
  this->pairs.strat=this; 
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
      if ((strat->generators[i].length==1) &&(strat->generators[j].length==1)){
        this->queue.pop();
        strat->pairs.status.setToHasTRep(i,j);
        continue;
        
      }
      if (extended_product_criterion(strat->generators[i], strat->generators[j])){
        //cout<<"Delayed Extended PC"<<endl;
        this->queue.pop();
        strat->pairs.status.setToHasTRep(i,j);
        this->strat->extendedProductCriterions++;
        continue;
      }
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
          if (strat->generators[vp->i].literal_factors.occursAsLeadOfFactor(vp->v)){
            cout<<"delayed variable linear factor criterion"<<endl;
            queue.pop();
            continue;
          }
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
PolyEntry::PolyEntry(const Polynomial &p):literal_factors(p){
  this->p=p;

  this->lm=p.lead();
  this->weightedLength=p.eliminationLength();
  this->length=p.length();
  this->usedVariables=p.usedVariables();
  this->deg=p.deg();
  this->tailVariables=(p-lm).usedVariables();
  this->lmDeg=p.lmDeg();
}
void PolyEntry::recomputeInformation(){
  assert(this->lm==p.lead());
  this->weightedLength=p.eliminationLength();
  this->length=p.length();
  this->usedVariables=p.usedVariables();
  this->deg=p.deg();
  this->tailVariables=(p-lm).usedVariables();
  this->literal_factors=LiteralFactorization(p);
  assert(this->lmDeg==p.lmDeg());
}
Polynomial reduce_by_monom(const Polynomial& p, const Monomial& m){
  
  if (m.deg()==1){
    //cout<<"branch 1\n";
    //cout.flush();
    Monomial::const_iterator it=m.begin();
    return Polynomial(BooleSet(p).subset0(*it));
  }
#if 0
  //buggy variant 1
  return p%m;
#endif
#if 1
  //working variant

  Monomial::const_iterator it=m.begin();
  Monomial::const_iterator end=m.end();
  BooleSet dividing_terms
  //=BooleSet(p/m);
  =BooleSet(p);
  
  while(it!=end){
    dividing_terms=dividing_terms.subset1(*it);
    it++;
  }
#endif
#if 0
  //buggy variant 2
  if (p==Polynomial(m))
    return Polynomial(0);
  BooleSet dividing_terms
    =BooleSet(p/m);
    
#endif
  //fast multiply back
  //cout<<"branch2\n";
  //cout.flush();
  
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
  Monomial bin_last=(binom-bin_lead).lead();
  
  //Polynomial p_reducible_base=p;
  Monomial::const_iterator it=bin_lead.begin();
  Monomial::const_iterator end=bin_lead.end();
  BooleSet dividing_terms
    //=BooleSet(p/m);
    =BooleSet(p);
  
  while(it!=end){
    dividing_terms=dividing_terms.subset1(*it);
    it++;
  }
  
  
  
  Polynomial canceled_lead(BooleSet(p).diff(dividing_terms.unateProduct(bin_lead.diagram())));
  
  Monomial b_p_gcd=bin_last.GCD(bin_lead);
  
  Polynomial first_mult_half=dividing_terms.unateProduct(BooleSet(Polynomial(b_p_gcd)));
  Polynomial multiplied=(bin_last/b_p_gcd)*first_mult_half;
  
  Polynomial res=multiplied+canceled_lead;
  //cout<<"p:"<<p<<endl;
  //cout<<"res:"<<res<<endl;
  return res;
}

Polynomial reduce_complete(const Polynomial& p, const Polynomial& reductor){

  
  Monomial p_lead=p.lead();
  Monomial reductor_lead=reductor.lead();
  Polynomial reductor_tail=reductor-reductor_lead;

  
  
  //Polynomial p_reducible_base=p;
  Monomial::const_iterator it=reductor_lead.begin();
  Monomial::const_iterator end=reductor_lead.end();
  
  BooleSet dividing_terms
    //=BooleSet(p/m);
    =BooleSet(p);
  
  while(it!=end){
    dividing_terms=dividing_terms.subset1(*it);
    it++;
  }
  
  
  
  Polynomial canceled_lead(BooleSet(p).diff(dividing_terms.unateProduct(reductor_lead.diagram())));
  
  Polynomial::const_iterator it_r=reductor_tail.begin();
  Polynomial::const_iterator end_r=reductor_tail.end();
  Polynomial res=canceled_lead;
  while(it_r!=end_r){
    Monomial m=(*it_r);
    Monomial b_p_gcd=m.GCD(reductor_lead);
    Polynomial first_mult_half=dividing_terms.unateProduct(BooleSet(Polynomial(b_p_gcd)));
    Polynomial multiplied=(m/b_p_gcd)*first_mult_half;
    res+=multiplied;
    ++it_r;
    
  }
  return res;
}



static Polynomial reduce_by_binom_in_tail (const Polynomial& p, const Polynomial& binom){
  assert(binom.length()==2);
  Monomial lm=p.lead();
  return lm+reduce_by_binom(p-lm,binom);
}

class HasTRepOrExtendedProductCriterion{
public:
  GroebnerStrategy* strat;
  int j;
  HasTRepOrExtendedProductCriterion(GroebnerStrategy& strat, int j){
    this->strat=&strat;
    this->j=j;
  }
  bool operator() (const Monomial &m){
    int i;
    i=strat->lm2Index[m];
    
    if (strat->pairs.status.hasTRep(i,j))
      return true;
    
    if (extended_product_criterion(strat->generators[i],strat->generators[j])){
      strat->pairs.status.setToHasTRep(i,j);
      strat->extendedProductCriterions++;
      return true;
    }
    return false;
  }

};
void GroebnerStrategy::addGenerator(const BoolePolynomial& p){
  PolyEntry e(p);
  Monomial lm=e.lm;
  if ((e.length==1) && (e.deg>0)){
    assert(e.p.length()==1);
    Monomial m=e.lm;
    int i;
    for(i=0;i<this->generators.size();i++){
      if ((this->generators[i].length>1) &&(this->generators[i].tailVariables.reducibleBy(m))){
        Polynomial new_p=cancel_monomial_in_tail(this->generators[i].p,m);
        if (new_p!=this->generators[i].p){
          this->generators[i].p=new_p;
          this->generators[i].recomputeInformation();
        }
      }
    }
  }
if ((e.length==2)&&(e.ecart()==0)){
    Monomial m=e.lm;
    int i;
    for(i=0;i<this->generators.size();i++){
      if ((this->generators[i].length>1)&&(this->generators[i].tailVariables.reducibleBy(m))){
        Polynomial new_p=reduce_by_binom_in_tail(this->generators[i].p,e.p);
        if (new_p!=this->generators[i].p){
          this->generators[i].p=new_p;
          this->generators[i].recomputeInformation();
        }
      }
    }
  }
  
  //do this before adding leading term
  Monomial::const_iterator it=e.lm.begin();
  Monomial::const_iterator end=e.lm.end();
  BooleSet other_terms=this->leadingTerms;
  

  while(it!=end){
    //cout<<"intersect"<<endl;
    other_terms=other_terms.subset0(*it);
    it++;
    
  }
  BooleSet intersecting_terms= this->leadingTerms.diff(other_terms);
  this->easyProductCriterions+=this->leadingTerms.length()-intersecting_terms.length();
  
 
  
  generators.push_back(e);
  pairs.status.prolong(PairStatusSet::HAS_T_REP);
  const int s=generators.size()-1;
  lm2Index[generators[s].lm]=s;

  
  int i;

  
  
  it=generators[s].lm.begin();
  end=generators[s].lm.end();
  while(it!=end){
    if ((generators[s].lm.deg()==1) || generators[s].literal_factors.occursAsLeadOfFactor(*it)){//((MonomialSet(p).subset0(*it).emptiness())||(MonomialSet(p).subset0(*it)==(MonomialSet(p).subset1(*it))))){
      generators[s].vPairCalculated.insert(*it);
    } else
      this->pairs.introducePair(Pair(s,*it,generators,VARIABLE_PAIR));
    it++;
    
  }
    BooleSet::const_iterator is_it=intersecting_terms.begin();
  BooleSet::const_iterator is_end=intersecting_terms.end();
  
  while(is_it!=is_end){
    int index =this->lm2Index[*is_it];
    if (index!=s){
      
      //product criterion doesn't hold
      //try length 1 crit
      if (!((generators[index].length==1) &&(generators[s].length==1)))
      {        
        this->pairs.status.setToUncalculated(index,s);
      } else this->extendedProductCriterions++;
    }
    is_it++;
  }

  Monomial crit_vars=Polynomial(intersecting_terms).usedVariables();
  
    
  //other_terms.unateProductAssign((crit_vars*lm).divisors());
    {
      other_terms.unateProductAssign(lm.diagram());
      other_terms.unateProductAssign(crit_vars.divisors());

    //crit_vars*=lm; //vars from lm are in any term in crit_terms anyway
    BooleSet crit_terms=intersecting_terms.unateProduct(lm.diagram()).diff(other_terms);
    while(!(crit_terms.emptiness())){
      Monomial min_crit=crit_terms.lastLexicographicalTerm();
      crit_terms=crit_terms.diff(min_crit.multiples(crit_vars));
      BooleSet act_l_terms=leadingTerms.intersect(min_crit.divisors());
      assert(!(act_l_terms.emptiness()));
      
      
      //extended product criterion not to forget for others
      if (std::find_if(act_l_terms.begin(), act_l_terms.end(),HasTRepOrExtendedProductCriterion(*this,s))!=act_l_terms.end()){
        continue;
      }
      Monomial min=*(std::min_element(act_l_terms.begin(),act_l_terms.end(), LessWeightedLengthInStrat(*this)));
      
      int chosen_index=lm2Index[min];
   
      /*if (extended_product_criterion(this->generators[chosen_index], this->generators[s])){
        this->extendedProductCriterions++;
        this->pairs.status.hasTRep(chosen_index,s);
      }
      else*/
        this->pairs.introducePair(Pair(chosen_index,s,generators));
        //cout<<"pair with:"<<chosen_index<<endl;
        //cout<<generators[chosen_index].p;
        //cout<<generators[s].p;

    }
  }
    
    
    //!!!!! here we add the lm !!!!
    leadingTerms.uniteAssign(Polynomial(e.lm).diagram());
 /* is_it=intersecting_terms.begin();

  while(is_it!=is_end){
    int index =this->lm2Index[*is_it];
    if (index!=s){
      
      //product criterion doesn't hold
      //try length 1 crit
      if (!(this->pairs.status.hasTRep(index,s)))
      { 
        
        if (extended_product_criterion(this->generators[index], this->generators[s])){
          this->extendedProductCriterions++;
          this->pairs.status.hasTRep(index,s);
        }
        else
          this->pairs.introducePair(Pair(index,s,generators));
        //this->pairs.status.setToUncalculated(index,s);
      } 
    }
    is_it++;
  }
  */
  
  
  /*
  while(is_it!=is_end){
    int index =this->lm2Index[*is_it];
    if (index!=s){
      
      //product criterion doesn't hold
      //try length 1 crit
      if (!(((generators[index].length==1) &&(generators[s].length==1))||extended_product_criterion(generators[s], generators[index])))
      {        
        this->pairs.introducePair(Pair(index,s,generators));
        this->pairs.status.setToUncalculated(index,s);
      } else this->extendedProductCriterions++;
    }
    is_it++;
  }
   
  */
  
  //if (e.literal_factors.trivial()){
  /*  Polynomial negation=Polynomial(true)-e.p;
    LiteralFactorization fac_neg(negation);
    if (!(fac_neg.trivial())){
      cout<<"!!!!!!!!!!!!!!!!!"<<endl;
      cout<<"found new implications"<<endl;
      LiteralFactorization::map_type::const_iterator nb= fac_neg.factors.begin();
      LiteralFactorization::map_type::const_iterator ne= fac_neg.factors.end();
      while(nb!=ne){
        
        idx_type var=nb->first;
        bool val=(!(nb->second));
        this->addGeneratorDelayed(Monomial(Variable(var))+Polynomial(val));
        nb++;
      }
      this->addGeneratorDelayed(Polynomial(true)-fac_neg.rest);
    }
  //}*/
}
void GroebnerStrategy::addNonTrivialImplicationsDelayed(const Polynomial& p){
  Polynomial negation=Polynomial(true)-p;
  LiteralFactorization fac_neg(negation);
  if (!(fac_neg.trivial())){
    cout<<"!!!!!!!!!!!!!!!!!"<<endl;
    cout<<"found new implications"<<endl;
    LiteralFactorization::map_type::const_iterator nb= fac_neg.factors.begin();
    LiteralFactorization::map_type::const_iterator ne= fac_neg.factors.end();
    while(nb!=ne){
      
      idx_type var=nb->first;
      bool val=(!(nb->second));
      this->addGeneratorDelayed(Monomial(Variable(var))+Polynomial(val));
      nb++;
    }
    this->addGeneratorDelayed(Polynomial(true)-fac_neg.rest);
  } else {
    
    LiteralFactorization fac_p(p);
    if (!(fac_p.trivial()) &&(!(fac_p.rest.isOne()))){
      LiteralFactorization fac_res_neg(fac_p.rest+Polynomial(true));
      if(!(fac_res_neg.trivial())){
        cout<<"!!!!!!!!!!!!!!!!!"<<endl;
        cout<<"found nested new implications"<<endl;

      }
    }
  }
  
}
void GroebnerStrategy::addGeneratorDelayed(const BoolePolynomial& p){
  this->pairs.introducePair(Pair(p));
}




bool GroebnerStrategy::variableHasValue(idx_type v){
  int i;
  int s=this->generators.size();
  Monomial m=Variable(v);
  for(i=0;i<s;i++){
    if (this->generators[i].deg==1){
      if (this->generators[i].usedVariables==m){
        return true;
      }
    }
  }
  return false;
}
END_NAMESPACE_PBORIGB
