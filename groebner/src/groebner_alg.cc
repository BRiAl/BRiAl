/*
 *  groebner_alg.ccc
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 20.04.06.
 *  Copyright 2006 Mathematisches Forschungsinstitut Oberwolfach. All rights reserved.
 *
 */

#include "groebner_alg.h"
#include "nf.h"
#include <algorithm>
#include <set>

#define HAVE_DLEX4_DATA 1

#ifdef HAVE_DLEX4_DATA
#include "dlex4data.h"
#endif

#define HAVE_LP4_DATA 1

#ifdef HAVE_LP4_DATA
#include "lp4data.h"
#endif

#define HAVE_DP_ASC4_DATA 1

#ifdef HAVE_DP_ASC4_DATA
#include "dp_asc4data.h"
#endif

#include "CCacheManagement.h"


BEGIN_NAMESPACE_PBORIGB
static MonomialSet divide_monomial_divisors_out(const BooleSet& s, const Monomial& lm);
static MonomialSet minimal_elements_cudd_style(MonomialSet m);
static bool have_ordering_for_tables(){
        
        
      
    const int order_code=BoolePolyRing::getOrderCode();
    #ifdef HAVE_DLEX4_DATA
        if (order_code==COrderEnums::dlex)
           return true;
    #endif



    #ifdef HAVE_LP4_DATA
        if (order_code==COrderEnums::lp)
           return true;
    #endif

    

    #ifdef HAVE_DP_ASC4_DATA
        if (order_code==COrderEnums::dp_asc)
           return true;
    #endif
    return false;
}
static bool extended_product_criterion(const PolyEntry& m, const PolyEntry& m2){
  //BooleMonomial m;
  ///@todo need GCDdeg
  
  bool res=(m.lm.GCD(m2.lm).deg()==common_literal_factors_deg(m.literal_factors, m2.literal_factors));
  //if (res)
  //  cout<<"EXTENDED PRODUCT_CRIT";
  return res;
}

void PairManager::replacePair(int& i, int& j){
  MonomialSet m=strat->leadingTerms.divisorsOf(strat->generators[i].lmExp.LCM(strat->generators[j].lmExp));
  MonomialSet::exp_iterator it=m.expBegin();
  MonomialSet::exp_iterator end=m.expEnd();
  int i_n=i;
  int j_n=j;
  while(it!=end){
    Exponent curr=*it;
    int index=strat->exp2Index[curr];
    wlen_type curr_wl=strat->generators[index].weightedLength;
    if ((index!=i) && (index!=j)){
    if ((status.hasTRep(index,i)) &&
      (strat->generators[i_n].weightedLength>curr_wl) &&
      (strat->generators[index].ecart()<=strat->generators[i].ecart())){
      i_n=index;
    }
    if ((status.hasTRep(index,j)) &&
      (strat->generators[j_n].weightedLength>curr_wl) &&
      (strat->generators[index].ecart()<=strat->generators[j].ecart())
      ){
      j_n=index;
    }
  }
    it++;
  }
  assert(i_n!=j_n);
  if (i_n!=j_n){
    i=i_n;
    j=j_n;
  }
}
void PairManager::introducePair(const Pair& pair){
  queue.push(pair);
}
bool PairManager::pairSetEmpty() const{
  return queue.empty();
}
Polynomial PairManager::nextSpoly(const PolyEntryVector& gen){
  assert(!(pairSetEmpty()));
  bool replaced_used=false;
  
  Polynomial replaced;
  Pair act_pair(queue.top());
  queue.pop();
  //const IJPairData* ij= dynamic_cast<IJPairData*>(queue.top().data.get());
  if (act_pair.getType()==IJ_PAIR){
    IJPairData* ij= (IJPairData*)(act_pair.data.get());
    int i=ij->i;
    int j=ij->j;
    
    replacePair(i,j);
    if ((i!=ij->i)||(ij->j!=j)){
      replaced=spoly(strat->generators[i].p,strat->generators[j].p);
      replaced_used=true;
      this->status.setToHasTRep(i,j);
      //this->status.setToHasTRep(ij->i,j);
      //this->status.setToHasTRep(i,ij->j);
    }
    
    //ij->i=i;
    //ac->j;
    //cout<<"mark1"<<endl;
    this->status.setToHasTRep(ij->i,ij->j);
  } else{
    
    
    if (act_pair.getType()==VARIABLE_PAIR){
      
      const VariablePairData *vp=(VariablePairData*)(act_pair.data.get());
      this->strat->generators[vp->i].vPairCalculated.insert(vp->v);
      int i=vp->i;
      Polynomial res=act_pair.extract(gen);
      
      
      if (!(res.isZero())){
        Monomial lm=res.lead();
      
        if (lm==this->strat->generators[i].lm)
          res+=this->strat->generators[i].p;
      }
      
      return res;
      
    }
  }
  if (replaced_used) return replaced;
  Polynomial res=act_pair.extract(gen);
  
  
  
  return res;
  
}
GroebnerStrategy::GroebnerStrategy(const GroebnerStrategy& orig)
:pairs(orig.pairs),
generators(orig.generators),
leadingTerms(orig.leadingTerms),
minimalLeadingTerms(orig.minimalLeadingTerms),
  leadingTerms11(orig.leadingTerms11),
  leadingTerms00(orig.leadingTerms00),
  lm2Index(orig.lm2Index), exp2Index(orig.exp2Index)
{
  optDelayNonMinimals=orig.optDelayNonMinimals;
  optBrutalReductions=orig.optBrutalReductions;
  cache=orig.cache;
  optStepBounded=orig.optStepBounded;

  optAllowRecursion=orig.optAllowRecursion;
  
  optRedTailDegGrowth=orig.optRedTailDegGrowth;
  optLazy=orig.optLazy;
  optRedTail=orig.optRedTail;
  optExchange=orig.optExchange;
  reductionSteps=orig.reductionSteps;
  normalForms=orig.normalForms;
  currentDegree=orig.currentDegree;
  chainCriterions=orig.chainCriterions;
  variableChainCriterions=orig.variableChainCriterions;
  easyProductCriterions=orig.easyProductCriterions;
  extendedProductCriterions=orig.extendedProductCriterions;
  averageLength=orig.averageLength;
  enabledLog=orig.enabledLog;
  reduceByTailReduced=orig.reduceByTailReduced;
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
  bool operator() (const Exponent& lmExp){
    int index=strat->exp2Index.find(lmExp)->second;
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
  bool operator() (const Exponent& lmExp){
    int index=strat->exp2Index.find(lmExp)->second;
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
      if (strat->pairs.status.hasTRep(i,j)) {queue.pop();continue;}
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
      const Exponent lm=queue.top().lm;
      //cout<<"try chain crit"<<endl;
      const MonomialSet lms=this->strat->leadingTerms.intersect(lm.divisors());
      assert(lm==strat->generators[i].lmExp.LCM(strat->generators[j].lmExp));
      //assert(strat->leadingTerms.divisorsOf(strat->generators[i].lmExp.LCM(strat->generators[j].lmExp))==lms);
      //should be set
      //if (lms.length()>2)
        //cout<<"non empty"<<endl;
      if (std::find_if(lms.expBegin(),lms.expEnd(),ChainCriterion(*(this->strat),i,j))!=lms.expEnd()){
        this->queue.pop();
        strat->pairs.status.setToHasTRep(i,j);
        //cout<<"Chain Criterion"<<endl;
        //cout.flush();
        strat->chainCriterions++;
        continue;
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
          //const MonomialSet lms=this->strat->leadingTerms.intersect(strat->generators[vp->i].lm.divisors());
          
          //Monomial lm=strat->generators[vp->i].lm;
          //Exponent lmExp=strat->generators[vp->i].lmExp;
          if (strat->generators[vp->i].literal_factors.occursAsLeadOfFactor(vp->v)){
            strat->log("delayed variable linear factor criterion");
            queue.pop();
            continue;
          }
          /*if (!(strat->leadingTerms.intersect(lmExp.divisors()).diff(Polynomial(lm)).emptiness())){
            strat->variableChainCriterions++;
           queue.pop();
          } else {
            if (std::find_if(lms.expBegin(),lms.expEnd(),ChainVariableCriterion(*(this->strat),vp->i,vp->v))!=lms.expEnd()){
              strat->generators[vp->i].vPairCalculated.insert(vp->v);
              this->queue.pop();
              //cout<<"Variable Chain Criterion"<<endl;
              //cout.flush();
              strat->variableChainCriterions++;
            } else return;}
          
          */
          if (!(strat->generators[vp->i].minimal)){
             this->queue.pop();
             strat->variableChainCriterions++;
             continue;
          }
          return;
        } else return;
    }
  }
}
PolyEntry::PolyEntry(const Polynomial &p):literal_factors(p){
  this->p=p;
  this->deg=p.deg();
  this->lm=p.boundedLead(deg);
  this->lmExp=lm.exp();
  this->lmDeg=lmExp.deg();
  this->length=p.length();
  if(lmDeg==deg)
    this->weightedLength=this->length;
  else
    this->weightedLength=p.eliminationLengthWithDegBound(deg);
  
  this->usedVariables=p.usedVariablesExp();
  tail=p-lm;
  this->tailVariables=tail.usedVariablesExp();
  
  this->minimal=true;
}


void PolyEntry::recomputeInformation(){
  assert(this->lm==p.lead());
  if (!(BoolePolyRing::isDegreeOrder()))
      this->deg=p.deg();
  //so also lmExp keeps constant
  this->length=p.length();
  if (lmDeg==deg)
    this->weightedLength=this->length;
  else
    this->weightedLength=p.eliminationLengthWithDegBound(deg);
  
  this->usedVariables=p.usedVariablesExp();
  tail=p-lm;
  this->tailVariables=tail.usedVariablesExp();
  this->literal_factors=LiteralFactorization(p);
  //minimal keeps constant
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

static Polynomial multiply_recursively(Polynomial a,Polynomial b){
  if (a.isZero()) return Polynomial(0);
  if (a.isOne()) return b;
  if (b.isZero()) return Polynomial(0);
  if (b.isOne()) return a;
  int index=*(a.navigation());
  Polynomial as0=a.diagram().subset0(index);
  Polynomial as1=a.diagram().subset1(index);
  if (as0==as1){
    b=b.diagram().subset0(index);
  }
  if (b.isZero()) return Polynomial(0);
  if (b.isOne()) return a;
  as0=multiply_recursively(as0,b);
  as1=multiply_recursively(as1,((Monomial) Variable(index))*b);
  return as0+as1;
}
static Polynomial multiply_recursively2(Polynomial a,Polynomial b){

  if (a.isZero()) return Polynomial(0);
  if (a.isOne()) return b;
  if (b.isZero()) return Polynomial(0);
  if (b.isOne()) return a;
  if (a==b) return a;

  typedef PBORI::CCommutativeCacheManagement<CCacheTypes::multiply_recursive>
    cache_mgr_type;

  cache_mgr_type cache_mgr;

  PBORI::BoolePolynomial::navigator cached =
    cache_mgr.find(a.navigation(), b.navigation());

  Polynomial result;

  if (cached.isValid() ){
    result = (CDDInterface<ZDD>)
      PBORI::CTypes::dd_base(&PBORI::BoolePolyRing::activeManager().manager(),
                               cached);
  }
  else {
    int indexa=*(a.navigation());
    int indexb=*(b.navigation());
    int index=std::min(indexa,indexb);
    Polynomial as0=a.diagram().subset0(index);
    Polynomial as1=a.diagram().subset1(index);
    Polynomial bs0=b.diagram().subset0(index);
    Polynomial bs1=b.diagram().subset1(index);
    if (as0==as1){
      //Polynomial zero(0);
      //bs1=zero.diagram();
      result = (Polynomial)multiply_recursively2(as1,bs0).diagram().change(index)
        +multiply_recursively2(as0,bs0);
    } else {
      if (bs0==bs1){
        //Polynomial zero(0);
        //as1=zero.diagram();
        result = (Polynomial)(multiply_recursively2(as0,bs1).diagram().change(index))
            +multiply_recursively2(as0,bs0);
      } else {
        result = (Polynomial)( (multiply_recursively2(as0,bs1) 
                                + multiply_recursively2(as1,bs1)
                                + multiply_recursively2(as1,bs0)).diagram().change(index) )
          +multiply_recursively2(as0,bs0);
      }
    }

   
 
    cache_mgr.insert(a.navigation(), b.navigation(), result.navigation());
  }


  return result;
}

static Polynomial multiply_recursively3(Polynomial a,Polynomial b){
  if (a.isZero()) return Polynomial(0);
  if (a.isOne()) return b;
  if (b.isZero()) return Polynomial(0);
  if (b.isOne()) return a;
  if (a==b) return a;

  typedef PBORI::CCommutativeCacheManagement<CCacheTypes::multiply_recursive>
    cache_mgr_type;

  cache_mgr_type cache_mgr(PBORI::BoolePolyRing::activeManager());

  PBORI::BoolePolynomial::navigator cached =
    cache_mgr.find(a.navigation(), b.navigation());

  Polynomial result;

  if (cached.isValid() ){
    result = (CDDInterface<ZDD>)
      PBORI::CTypes::dd_base(&PBORI::BoolePolyRing::activeManager().manager(),
                               cached);
  }
  else {

    int indexa=*(a.navigation());
    int indexb=*(b.navigation());
    int index=std::min(indexa,indexb);
    Polynomial as0=a.diagram().subset0(index);
    Polynomial as1=a.diagram().subset1(index);
    Polynomial bs0=b.diagram().subset0(index);
    Polynomial bs1=b.diagram().subset1(index);
    if (indexa!=indexb){
      
      if (as0==as1){
        Polynomial zero(0);
        bs1=zero.diagram();
      } else {
        if (bs0==bs1){
          Polynomial zero(0);
          as1=zero.diagram();
        }
      }
      result = ((Polynomial) (multiply_recursively3(as0,bs1)
                     + multiply_recursively3(as1,bs1)
                     + multiply_recursively3(as1,bs0)).diagram().change(index))
        + multiply_recursively3(as0,bs0);
    } else {
      
      Polynomial res00=multiply_recursively3(as0,bs0);
      Polynomial res11=multiply_recursively3(as1+as0,bs0+bs1)+res00;
      result = res11.diagram().change(index).unite(res00.diagram());
    }
  }

  return result;
}
static Polynomial multiply(Polynomial a, size_t len_a, Polynomial b, size_t len_b){
  if (len_a>len_b) std::swap(a,b);
  return multiply_recursively(a,b);
}


Polynomial reduce_complete(const Polynomial &p, const PolyEntry& reductor, wlen_type & len){
  //cout<<"red complete"<<endl;
  MonomialSet rewriteable_terms_divided=p.diagram();
  Exponent::const_iterator it=reductor.lmExp.begin();
  Exponent::const_iterator end=reductor.lmExp.end();
  while (it!=end){
    rewriteable_terms_divided=rewriteable_terms_divided.subset1(*it);
    it++;
  }
  //divide_monomial_divisors_out(p.diagram(),reductor.lm);
  //Polynomial res=p-reductor.lm*(Polynomial)rewriteable_terms_divided;
  Polynomial factor_reductor=reductor.p;//tail;
  size_t factor_reductor_len=reductor.length;//factor_reductor.length();
  size_t rewriteable_terms_len=rewriteable_terms_divided.length();
  len=len+(factor_reductor_len-2)*rewriteable_terms_len;
  Polynomial product=
#if 1
    multiply_recursively2(factor_reductor,rewriteable_terms_divided);
#else
    multiply(factor_reductor,factor_reductor_len,rewriteable_terms_divided,rewriteable_terms_len);
#endif

  /*if (factor_reductor_len<rewriteable_terms_len){
    product=factor_reductor*((Polynomial)rewriteable_terms_divided);
  
  } else {
    product=((Polynomial)rewriteable_terms_divided)*((Polynomial) factor_reductor);
  }*/
  //res=res+product;
  return p+product;
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
  bool operator() (const Exponent &m){
    int i;
    i=strat->exp2Index[m];
    
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


bool should_propagate(const PolyEntry& e){
 return ((((e.length==1) && (e.deg>0) && (e.deg<4)))||((e.length==2)&&(e.ecart()==0) &&(e.deg<3)));

}

void GroebnerStrategy::propagate_step(const PolyEntry& e, std::set<int> others){
  
  if (should_propagate(e)){
    Monomial lm=e.lm;
    Exponent exp=e.lmExp;
    int i;
    int s=generators.size();
    for(i=0;i<s;i++){      
      if ((this->generators[i].minimal) &&(this->generators[i].length>1) &&(&this->generators[i]!=&e)&&(this->generators[i].tailVariables.reducibleBy(exp))){
        Polynomial new_p;
        if (e.length==1){
          new_p=cancel_monomial_in_tail(this->generators[i].p,e.lm);
        } else {
          assert(e.length==2);
          new_p=reduce_by_binom_in_tail(this->generators[i].p,e.p);
        }
        if (generators[i].p!=new_p){
          generators[i].p=new_p;
          generators[i].recomputeInformation();
          if (generators[i].length==2)
            addNonTrivialImplicationsDelayed(generators[i]);
          others.insert(i);
          
        }
      }
    }
  }
  if (!(others.empty()))
  { ///@todo: should take the one with smallest lm
    std::set<int>::iterator ob=others.begin();
    int next=*ob;
    others.erase(ob);
    this->propagate_step(generators[next],others);
  }
}
void GroebnerStrategy::propagate(const PolyEntry& e){
  if (should_propagate(e)){
    propagate_step(e, std::set<int>());
  }
}

static std::vector<idx_type> contained_variables(const MonomialSet& m){
    std::vector<idx_type> result;
    MonomialSet::navigator nav=m.navigation();
    while (!(nav.isConstant())){
        idx_type v=*nav;
        MonomialSet::navigator check_nav=nav.thenBranch();
        while(!(check_nav.isConstant())){
            check_nav.incrementElse();
        }
        if (check_nav.terminalValue()){
            result.push_back(v);
        }
        nav.incrementElse();
        
    }
    return result;
}

MonomialSet minimal_elements_internal(const MonomialSet& s){
    if (s.emptiness()) return s;
    if (Polynomial(s).isOne()) return s;
    MonomialSet::navigator nav=s.navigation();
    int i=*nav;
    
    
    if (Polynomial(s).hasConstantPart()) return MonomialSet(Polynomial(true));
    int l=s.length();
    if (l<=1) {
        return s;
    }
    
    if(l==2){
        MonomialSet::const_iterator it=s.begin();
        Monomial a=*it;
        Monomial b=*(++it);
        if (a.reducibleBy(b)) return MonomialSet(b.diagram());
        else return s;
    }
    
    MonomialSet s0_raw=s.subset0(i);
    MonomialSet s0=minimal_elements_internal(s0_raw);
    MonomialSet s1=minimal_elements_internal(s.subset1(i).diff(s0_raw));
    if (!(s0.emptiness())){
        s1=s1.diff(s0.unateProduct(Polynomial(s1).usedVariablesExp().divisors()));
        
    }
    return s0.unite(s1.change(i));

}

MonomialSet minimal_elements_internal2(MonomialSet s){
    if (s.emptiness()) return s;
    if (Polynomial(s).isOne()) return s;
    
    
    
    
    if (Polynomial(s).hasConstantPart()) return MonomialSet(Polynomial(true));
    MonomialSet result;
    std::vector<idx_type> cv=contained_variables(s);
    if ((cv.size()>0) && (s.length()==cv.size())){
        return s;
    } else {
    
        int z;
        MonomialSet cv_set;
        for(z=cv.size()-1;z>=0;z--){
            Monomial mv=Variable(cv[z]);
            cv_set=cv_set.unite(mv.diagram());
        }
        for(z=0;z<cv.size();z++){
            s=s.subset0(cv[z]);
        }
        result=cv_set;
    }
    
    if (s.emptiness()) return result;
    assert(!(Polynomial(s).hasConstantPart()));
    
    
    
    MonomialSet::navigator nav=s.navigation();
    idx_type i;
    #if 1
    
    //first index of ZDD
    i=*nav;
    #else
    
    //first index of least lex. term
    while(!(nav.isConstant())){
        i=*nav;
        nav.incrementElse();
    }
    #endif
    
    
    
    /*MonomialSet s0=minimal_elements_internal2(s.subset0(i));
    MonomialSet s1=s.subset1(i);
    if ((s0!=s1)&&(!(s1.diff(s0).emptiness()))){
        s1=minimal_elements_internal2(s1.unite(s0)).diff(s0);
    } else return s0;
    return s0.unite(s1.change(i));*/
    
    MonomialSet s0_raw=s.subset0(i);
    MonomialSet s0=minimal_elements_internal2(s0_raw);
    MonomialSet s1=minimal_elements_internal2(s.subset1(i).diff(s0_raw));
    if (!(s0.emptiness())){
        s1=s1.diff(s0.unateProduct(Polynomial(s1).usedVariablesExp().divisors()));
        
    }
    return s0.unite(s1.change(i)).unite(result);

}
std::vector<Exponent> minimal_elements_internal3(MonomialSet s){
    std::vector<Exponent> result;
    if (s.emptiness()) return result;
    if ((Polynomial(s).isOne()) || (Polynomial(s).hasConstantPart())){
        result.push_back(Exponent());
        return result;
    }
    std::vector<idx_type> cv=contained_variables(s);
    int i;
    for(i=0;i<cv.size();i++){
            s=s.subset0(cv[i]);
            Exponent t;
            t.insert(cv[i]);
            result.push_back(t);
    }
    
    if (s.emptiness()){
        return result;
    } else {
        std::vector<Exponent> exponents;
        //Pol sp=s;
        exponents.insert(exponents.end(), s.expBegin(),s.expEnd());
        int nvars=BoolePolyRing::nRingVariables();
        std::vector<std::vector<int> > occ_vecs(nvars);
        for(i=0;i<exponents.size()-1;i++){
            Exponent::const_iterator it=((const Exponent&) exponents[i]).begin();
            Exponent::const_iterator end=((const Exponent&) exponents[i]).end();
            while(it!=end){
                occ_vecs[*it].push_back(i);
                it++;
            }
        }
        //now exponents are ordered
        /*std::vector<std::set<int> > occ_sets(nvars);
        for(i=occ_sets.size()-1;i>=0;i--){
            occ_sets[i].insert(occ_vecs[i].begin(),occ_vecs[i].end());
        }*/
        std::vector<bool> still_minimal(exponents.size());
        for(i=exponents.size()-1;i>=0;i--){
            still_minimal[i]=true;
        }
        int result_orig=result.size();
        //cout<<"orig:"<<exponents.size()<<endl;
        //lex smalles is last so backwards
        for(i=exponents.size()-1;i>=0;i--){
            if (still_minimal[i]){
                //we assume, that each exponents has deg>0
                Exponent::const_iterator it=((const Exponent&) exponents[i]).begin();
                Exponent::const_iterator end=((const Exponent&) exponents[i]).end();
                int first_index=*it;
                std::vector<int> occ_set=occ_vecs[first_index];
                it++;
                while(it!=end){
                    
                    std::vector<int> occ_set_next;
                    set_intersection(
                        occ_set.begin(),
                        occ_set.end(),
                        occ_vecs[*it].begin(), 
                        occ_vecs[*it].end(),
                        std::back_insert_iterator<std::vector<int> >(occ_set_next));
                    occ_set=occ_set_next;
                    it++;
                }
                
                std::vector<int>::const_iterator oc_it= occ_set.begin();
                std::vector<int>::const_iterator  oc_end= occ_set.end();
                while(oc_it!=oc_end){
                    still_minimal[*oc_it]=false;
                    oc_it++;
                }
                
                it=((const Exponent&) exponents[i]).begin();
                while(it!=end){
                    std::vector<int> occ_set_difference;
                    set_difference(
                        occ_vecs[*it].begin(),
                        occ_vecs[*it].end(),
                        occ_set.begin(),
                        occ_set.end(),
                        std::back_insert_iterator<std::vector<int> >(occ_set_difference));
                    occ_vecs[*it]=occ_set_difference;
                    it++;
                }
                result.push_back(exponents[i]);
            }
        }
        //cout<<"after:"<<result.size()-result_orig<<endl;
        
    }
    return result;
}
MonomialSet minimal_elements(const MonomialSet& s){
#if 1
    return minimal_elements_internal2(s);
#else
#if 1
  return s.minimalElements();
#else
  MonomialSet minElts = minimal_elements_internal(s);

  if (minElts!=s.minimalElements()){

    std::cout <<"ERROR minimalElements"<<std::endl;
    std::cout <<"orig "<<s<< std::endl;
    std::cout <<"correct "<<minElts<< std::endl;
    std::cout <<"wrong "<<s.minimalElements()<< std::endl;
  }


  return minElts;
#endif
#endif
}

class SetBitUInt{
    public:
        void operator() (unsigned int& vec, int index){
            vec|=(1<<index);
        }
};
class ZeroFunction{
    public:
        unsigned int operator() (unsigned int val){
            return 0;
        }
};
template<class value_type, class initializer, class set_bit> value_type p2code(Polynomial p, const std::vector<char> & ring_2_0123, int max_vars){
    Polynomial::exp_iterator it_p=p.expBegin();
    Polynomial::exp_iterator end_p=p.expEnd();
    initializer init;
    value_type p_code=init(max_vars);
    assert(max_vars<sizeof(unsigned int)*8);
    set_bit bit_setter;
    while(it_p!=end_p){
        Exponent curr_exp=*it_p;
        Exponent::const_iterator it_v=curr_exp.begin();
        Exponent::const_iterator end_v=curr_exp.end();
        unsigned int exp_code=0;
        //exp code is int between 0 and 15
        while(it_v!=end_v){
            //cout<<"table value:"<<(int)ring_2_0123[(*it_v)]<<endl;
            exp_code|=(1<<ring_2_0123[(*it_v)]);
            //cout<<"exp_code:"<<exp_code<<endl;
            it_v++;
        }
        //cout<<"exp_code final:"<<exp_code<<endl;
        //p_code|=(1<<exp_code);
        bit_setter(p_code,exp_code);
        //so p code is 16-bit unsigned int
        //int is fastest
        it_p++;
    }
    return p_code;
}
Polynomial translate_indices(const Polynomial& p, const std::vector<idx_type>& table){
    if (p.isConstant()) return p;
    int index=*(p.navigation());
    int index_mapped=table[index];
    
#if 0
return
      dd_backward_transform(p.navigation(), Polynomial(0), 
                            mapped_new_node<std::vector<idx_type>,
                            Variable, Monomial, Polynomial>(table), 
                            integral_constant<bool, true, Polynomial>());


#else

    MonomialSet s1=p.diagram().subset1(index);
    MonomialSet s0=p.diagram().subset0(index);
    if (s1!=s0){
      s1=translate_indices(s1,table);
      s1=s1.change(index_mapped);
      s0=translate_indices(s0,table);
    } else {
      s0=translate_indices(s0,table);
      s1=s0.change(index_mapped);
      
    }
    return s1.unite(s0);
    //we can assume that index_mapped does not occur in the translated subpolynomial, in which index index does not occurr
   // return translate_indices(p.diagram().subset1(index),table).diagram().change(index_mapped).unite(
  //    translate_indices(p.diagram().subset0(index), table));
#endif


}
static unsigned int p2code_4(Polynomial p, const std::vector<char> & ring_2_0123){
    return p2code<unsigned int, ZeroFunction, SetBitUInt>(p,ring_2_0123, 4);
    /*Polynomial::exp_iterator it_p=p.expBegin();
    Polynomial::exp_iterator end_p=p.expEnd();
    unsigned int p_code=0;
    while(it_p!=end_p){
        Exponent curr_exp=*it_p;
        Exponent::const_iterator it_v=curr_exp.begin();
        Exponent::const_iterator end_v=curr_exp.end();
        unsigned int exp_code=0;
        //exp code is int between 0 and 15
        while(it_v!=end_v){
            //cout<<"table value:"<<(int)ring_2_0123[(*it_v)]<<endl;
            exp_code|=(1<<ring_2_0123[(*it_v)]);
            //cout<<"exp_code:"<<exp_code<<endl;
            it_v++;
        }
        //cout<<"exp_code final:"<<exp_code<<endl;
        p_code|=(1<<exp_code);
        //so p code is 16-bit unsigned int
        //int is fastest
        it_p++;
    }
    return p_code;
    */
}


static Monomial code_2_m_4(unsigned int code, std::vector<idx_type> back_2_ring){
    int i;
    Monomial res;
    //cout<<"m_code:"<<code<<endl;
    for(i=3;i>=0;i--){
        if ((code & (1<<i))!=0){
            res*=Variable(back_2_ring[i]);
            //res=res.diagram().change(back_2_ring[i]);
        }
    }
    //cout<<"m:"<<res<<endl;
    return res;
}
static Polynomial code_2_poly_4(unsigned int code, std::vector<idx_type> back_2_ring){
    int i;
    Polynomial p;
    //unsigned int m_code;
    for(i=15;i>=0;i--){
        if ((code & (1<<i))!=0){
            Monomial m=code_2_m_4(i,back_2_ring);
            p+=m;
        }
    }
    //cout<<"p input code"<<code<<"p out:"<<p<<endl;
    return p;
}


static void mark_all_variable_pairs_as_calculated(GroebnerStrategy& strat, int s){
    BooleExponent::const_iterator it=strat.generators[s].lmExp.begin();
    BooleExponent::const_iterator end=strat.generators[s].lmExp.end();
     while(it!=end){
          strat.generators[s].vPairCalculated.insert(*it);
          it++;
    } 
}

static Polynomial multiply_with_literal_factors(const LiteralFactorization& lf, Polynomial p){
    LiteralFactorization::map_type::const_iterator it=lf.factors.begin();
    LiteralFactorization::map_type::const_iterator end=lf.factors.end();
    LiteralFactorization::var2var_map_type::const_iterator it_v=lf.var2var_map.begin();
    LiteralFactorization::var2var_map_type::const_iterator end_v=lf.var2var_map.end();
    while(it!=end){
        idx_type var=it->first;
        int val=it->second;
        if (val==0){
            p=p.diagram().change(var);
        } else {
            p=p.diagram().change(var).unite(p.diagram());
        }
        it++;
    }
    while(it_v!=end_v){
        idx_type var=it_v->first;
        idx_type var2=it_v->second;
        p=p.diagram().change(var).unite(p.diagram().change(var2));
        it_v++;
    }
    return p;
}
static int get_table_entry4(int p_code, int pos){
    switch(BoolePolyRing::getOrderCode()){
        #ifdef HAVE_LP4_DATA
        case COrderEnums::lp:
            return lp4var_data[p_code][pos];
        #endif
        #ifdef  HAVE_DLEX4_DATA
        case COrderEnums::dlex:
            return dlex4var_data[p_code][pos];
        #endif
        #ifdef HAVE_DP_ASC4_DATA
        case COrderEnums::dp_asc:
            return dp_asc4var_data[p_code][pos];
        #endif
        default:
            cerr<<"using tables with forbidden order"<<endl;
    }
}
std::vector<Polynomial> GroebnerStrategy::addHigherImplDelayedUsing4(int s, const LiteralFactorization& literal_factors, bool include_orig){
    if (literal_factors.rest.isOne()){
        if(s>=0)
            mark_all_variable_pairs_as_calculated(*this, s);
        return std::vector<Polynomial>();
    }
    Polynomial p=literal_factors.rest;
    
   
    //Monomial used_variables_m=p.usedVariables();
    Exponent used_variables=p.usedVariablesExp();
    Exponent e=p.leadExp();
    if (e.size()>4) cerr<<"too many variables for table"<<endl;
    
    std::vector<char> ring_2_0123(BoolePolyRing::nRingVariables());
    std::vector<idx_type> back_2_ring(4);
    set_up_translation_vectors(ring_2_0123, back_2_ring, used_variables);
    unsigned int p_code=p2code_4(p, ring_2_0123);
    int i;
    if ((get_table_entry4(p_code,0)==p_code) && (get_table_entry4(p_code,1)==0)){
        if (s>=0)
            mark_all_variable_pairs_as_calculated(*this, s);
        return std::vector<Polynomial>();
    }
    
    //Exponent e_i_high=multiply_with_literal_factor(literal_factors,Polynomial(Monomial(e_i)).lead());
    bool can_add_directly=true;
    std::vector<Polynomial> impl;
    for(i=0;get_table_entry4(p_code,i)!=0;i++){
        unsigned int impl_code=get_table_entry4(p_code,i);
        if ((include_orig) ||(p_code!=impl_code)){
            Polynomial p_i=code_2_poly_4(impl_code, back_2_ring);
            Exponent e_i=p_i.leadExp();
            if ((include_orig)||(e_i!=e)){
                p_i=
                    multiply_with_literal_factors(
                        literal_factors,
                        p_i);
               impl.push_back(p_i);
                if ((can_add_directly) &&(!(this->minimalLeadingTerms.divisorsOf(p_i.leadExp()).emptiness())))
                //e_i is wrong here, have to multiply
                    can_add_directly=false;
            }
        }
    }
    
    
     if (s>=0)
         mark_all_variable_pairs_as_calculated(*this, s);
     
      if (can_add_directly){
        return impl;
    } else {
        if (!(include_orig)){
            std::vector<Polynomial>::iterator it=impl.begin();
            std::vector<Polynomial>::iterator end=impl.end();
        
            while(it!=end){
                addGeneratorDelayed(*it);
                it++;

            }
        }
        return std::vector<Polynomial>();
    }
}

std::vector<Polynomial> GroebnerStrategy::add4ImplDelayed(const Polynomial& p, const Exponent& lm_exp, const Exponent& used_variables,int s, bool include_orig){
    //cout<<"I am here";
    //Polynomial p=generators[s].p;
    //Exponent used_variables=generatusedVariables;

    Exponent e=lm_exp;//generators[s].lmExp;

    std::vector<char> ring_2_0123(BoolePolyRing::nRingVariables());
    std::vector<idx_type> back_2_ring(4);
    set_up_translation_vectors(ring_2_0123, back_2_ring, used_variables);
    
    unsigned int p_code=p2code_4(p, ring_2_0123);

    if ((get_table_entry4(p_code,0)==p_code) && (get_table_entry4(p_code,1)==0)){
        if(s>=0)
            mark_all_variable_pairs_as_calculated(*this, s);

        return std::vector<Polynomial>();
    }
    
    int i;
    bool can_add_directly=true;
    std::vector<Polynomial> impl;
    for(i=0;get_table_entry4(p_code,i)!=0;i++){
        unsigned int impl_code=get_table_entry4(p_code,i);
        if ((include_orig) ||(p_code!=impl_code)){
            Polynomial p_i=code_2_poly_4(impl_code, back_2_ring);
            Exponent e_i=p_i.leadExp();

            if ((include_orig) ||(e_i!=e)){
                impl.push_back(p_i);
                if ((can_add_directly)&&(!(this->minimalLeadingTerms.divisorsOf(e_i).emptiness())))
                    can_add_directly=false;
                //impl.push_back(p_i);
                //addGeneratorDelayed(p_i);
            }
        }
    }
    
    
     if (s>=0)
         mark_all_variable_pairs_as_calculated(*this, s);
    
    if (can_add_directly){
        return impl;
    } else {
        std::vector<Polynomial>::iterator it=impl.begin();
        std::vector<Polynomial>::iterator end=impl.end();
        if (!(include_orig)){
            while(it!=end){
             addGeneratorDelayed(*it);
             it++;
            
            }
        }
        return std::vector<Polynomial>();
    }
    
    
}
void GroebnerStrategy::addVariablePairs(int s){
     Exponent::const_iterator it=generators[s].lmExp.begin();
     Exponent::const_iterator end=generators[s].lmExp.end();
     while(it!=end){
         if ((generators[s].lm.deg()==1) ||
            generators[s].literal_factors.occursAsLeadOfFactor(*it))
         {
              //((MonomialSet(p).subset0(*it).emptiness())||(MonomialSet(p).subset0(*it)==(MonomialSet(p).subset1(*it))))){
      //cout<<"factorcrit"<<endl;
             generators[s].vPairCalculated.insert(*it);
          } else
            this->pairs.introducePair(Pair(s,*it,generators,VARIABLE_PAIR));
          it++;
    
    } 
}

//static Monomial oper(int i){
//    return Monomial(Variable(i));
//}
static MonomialSet divide_monomial_divisors_out_old(const MonomialSet& s, const Monomial& lm){
  /// deactivated, because segfaults on SatTestCase, AD
  ///   return s.existAbstract(lm.diagram());
  /**/
    MonomialSet m=s;
    Monomial::const_iterator it_lm=lm.begin();
        Monomial::const_iterator end_lm=lm.end();
        while(it_lm!=end_lm){
            idx_type i=*it_lm;
            m=m.subset0(i).unite(m.subset1(i));
            it_lm++;
        }
   return m;/**/
}
static MonomialSet do_divide_monomial_divisors_out(const MonomialSet& s, Monomial::const_iterator it, Monomial::const_iterator end){
    if(it==end) return s;
    if (s.emptiness()) return s;
    
    Monomial::const_iterator itpp=it;
    itpp++;
    return (do_divide_monomial_divisors_out(s.subset0(*it),itpp,end).unite(do_divide_monomial_divisors_out(s.subset1(*it),itpp,end)));
    
}
static MonomialSet divide_monomial_divisors_out(const BooleSet& s, const Monomial& lm){
    return do_divide_monomial_divisors_out(s,lm.begin(),lm.end());
}
#define EXP_FOR_PAIRS
#ifndef EXP_FOR_PAIRS
static std::vector<Monomial> minimal_elements_multiplied(MonomialSet m, Monomial lm){
  
  
    std::vector<Monomial> result;
    if (!(m.divisorsOf(lm).emptiness())){
        result.push_back(lm);
    } else {
        Monomial v;
        //lm austeilen
        m=divide_monomial_divisors_out(m,lm);
        
        //std::vector<idx_type> cv=contained_variables(m);
        //int i;
        /*for(i=0;i<cv.size();i++){
            result.push_back(((Monomial)Variable(cv[i]))*lm);
            m=m.subset0(cv[i]);
        }*/
        m=minimal_elements(m);
        if (!(m.emptiness())){
            m=m.unateProduct(lm.diagram());
            result.insert(result.end(), m.begin(), m.end());
        }
        
        
    }
    return result;

}
#else
#if 0
static std::vector<Exponent> minimal_elements_divided(MonomialSet m, Monomial lm){
    std::vector<Exponent> result;
    Exponent exp;//=lm.exp();
    if (!(m.divisorsOf(lm).emptiness())){
        result.push_back(exp);
    } else {
        Monomial v;
        
        m=divide_monomial_divisors_out(m,lm);
        
        
        return minimal_elements_internal3(m);
        
    }
    return result;
}
#else
static std::vector<Exponent> minimal_elements_divided(MonomialSet m, Monomial lm){
    std::vector<Exponent> result;
    Exponent exp;//=lm.exp();
    if (!(m.divisorsOf(lm).emptiness())){
        result.push_back(exp);
    } else {
        Monomial v;
        m=divide_monomial_divisors_out(m,lm);
        m=minimal_elements_cudd_style(m);
        result.resize(m.length());
        std::copy(m.expBegin(),m.expEnd(),result.begin());
        //return minimal_elements_internal3(m);
        
    }
    return result;
}
#endif
#endif

std::vector<Polynomial> GroebnerStrategy::treatVariablePairs(int s){
  std::vector<Polynomial> impl;
  PolyEntry& e=generators[s];
  if (have_ordering_for_tables()){
    int uv=e.usedVariables.deg();
    if (uv<=4){
      impl=add4ImplDelayed(e.p,e.lmExp,e.usedVariables,s,false);
    } else {

      int uv_opt=uv-e.literal_factors.factors.size()-2*e.literal_factors.var2var_map.size();
      ////should also be proofable for var2var factors
      assert(uv_opt==e.literal_factors.rest.nUsedVariables());//+2*var2var_map.size());
      if (uv_opt<=4){
        impl=addHigherImplDelayedUsing4(s, e.literal_factors,false);
      } else {
        addVariablePairs(s);
      }
    }
  }

  else {

    addVariablePairs(s);
  }

  return impl;
}
static MonomialSet do_minimal_elements_cudd_style(MonomialSet m, MonomialSet mod){
  if (m.emptiness()) return MonomialSet();
  Polynomial p=m;
  Polynomial p_mod=mod;
  if (p_mod.hasConstantPart())
    return MonomialSet();
  if (p.hasConstantPart())
    return ((Polynomial)1).diagram();
  MonomialSet result;
  int index=*m.navigation();
  
  
  
  
  if (!mod.emptiness())
  {
    MonomialSet::navigator nav_mod=mod.navigation();
    while((!(nav_mod.isConstant())) && (index>*nav_mod)){
      nav_mod.incrementElse();
     
    }
    mod=(CDDInterface<ZDD>)
      PBORI::CTypes::dd_base(&PBORI::BoolePolyRing::activeManager().manager(),
                                 nav_mod);
  }
  
  typedef PBORI::CCacheManagement<CCacheTypes::minimal_mod>
    cache_mgr_type;

  cache_mgr_type cache_mgr;
  PBORI::BoolePolynomial::navigator cached =
    cache_mgr.find(m.navigation(), mod.navigation());


      
  if (cached.isValid() ){
    return (CDDInterface<ZDD>)
      PBORI::CTypes::dd_base(&PBORI::BoolePolyRing::activeManager().manager(),
                               cached);
  }
  
  
  if (mod.emptiness()){
    
    MonomialSet result0=do_minimal_elements_cudd_style(m.subset0(index) , MonomialSet());
    MonomialSet result1= do_minimal_elements_cudd_style(
      m.subset1(index),result0);
    result= result0.unite(result1.change(index));
     
  } else {
      MonomialSet mod0=mod.subset0(index);
      MonomialSet result0=do_minimal_elements_cudd_style(m.subset0(index) , mod0);
      MonomialSet mod1=mod.subset1(index);
      MonomialSet result1= do_minimal_elements_cudd_style(
        m.subset1(index),result0.unite(mod0.unite(mod1)));
      result= result0.unite(result1.change(index));
  }
  cache_mgr.insert(m.navigation(), mod.navigation(), result.navigation());
  return result;
}
static MonomialSet minimal_elements_cudd_style(MonomialSet m){
  return do_minimal_elements_cudd_style(m, MonomialSet());
}
void GroebnerStrategy::treatNormalPairs(int s,MonomialSet intersecting_terms,MonomialSet other_terms){
    PolyEntry e=generators[s];
    if(!(Polynomial(other_terms).hasConstantPart()))//.divisorsOf(lm).emptiness()))
     {
       BooleMonomial lm=e.lm;

  #ifndef EXP_FOR_PAIRS
          //MonomialSet already_used;
          std::vector<Monomial> mt_vec=minimal_elements_multiplied(intersecting_terms.intersect(minimalLeadingTerms), lm);
  #else
          std::vector<Exponent> mt_vec=minimal_elements_divided(intersecting_terms.intersect(minimalLeadingTerms), lm);
  #endif

          int mt_i;
          for(mt_i=mt_vec.size()-1;mt_i>=0;mt_i--){
  #ifndef EXP_FOR_PAIRS
              Monomial t=mt_vec[mt_i];
              Monomial t_divided=t/lm;
              assert(t.reducibleBy(lm));
  #else
              Exponent t_divided=mt_vec[mt_i];
              Exponent t=t_divided+e.lmExp;
  #endif
              MonomialSet lm_d=t_divided.divisors();
              if ((other_terms.intersect(lm_d).emptiness())){
             // #ifndef EXP_FOR_PAIRS
             //     MonomialSet act_l_terms=leadingTerms.intersect(t.divisors());
              //#else
                  MonomialSet act_l_terms=leadingTerms.divisorsOf(t);//intersect(t.divisors());//divisorsOf((Monomial)t);//.intersect(t.divisors());
              //#endif
                  if (std::find_if(act_l_terms.expBegin(), act_l_terms.expEnd(),HasTRepOrExtendedProductCriterion(*this,s))!=act_l_terms.expEnd()){
                      //at this point we assume minimality of t_divided w.r.t. natural partial order
                      //chosen.unite(act_l_terms.weakDivide(lm.diagram()));
                      continue;
                  }
                  //chosen=chosen.unite(t_divided.diagram());
                  #ifndef EXP_FOR_PAIRS

                  Monomial min=*(std::min_element(act_l_terms.begin(),act_l_terms.end(), LessWeightedLengthInStrat(*this)));
                  #else

                  Exponent min=*(std::min_element(act_l_terms.expBegin(),act_l_terms.expEnd(), LessWeightedLengthInStrat(*this)));
                  #endif
                  int chosen_index=exp2Index[min];
                  this->pairs.introducePair(Pair(chosen_index,s,generators));
              }
              //if (t.intersect())
          }

     }
}
int GroebnerStrategy::addGenerator(const BoolePolynomial& p, bool is_impl,std::vector<int>* impl_v){
  
  
  PolyEntry e(p);
  Monomial lm=e.lm;
  this->propagate(e);
  //do this before adding leading term
  Monomial::const_iterator it=e.lm.begin();
  Monomial::const_iterator end=e.lm.end();
  BooleSet other_terms=this->leadingTerms;
  MonomialSet intersecting_terms;
  bool is00=e.literal_factors.is00Factorization();
  bool is11=e.literal_factors.is11Factorization();
  
  if (!((is00 && (leadingTerms==leadingTerms00))||(is11 && (leadingTerms==leadingTerms11)))){
    
    if (is11)
        other_terms=other_terms.diff(leadingTerms11);
    if (is00)
        other_terms=other_terms.diff(leadingTerms00);
    while(it!=end){
    
    other_terms=other_terms.subset0(*it);
    it++;
    
    }
    
    MonomialSet ot2;
    if ((is11)||(is00)){
        
        if (is00)
            ot2=leadingTerms00;
        else
            ot2=leadingTerms11;
          /// deactivated existAbstract, because sigfaults on SatTestCase, AD
          
        if (!(ot2.emptiness())){
            other_terms=other_terms.unite(divide_monomial_divisors_out(ot2,lm));
        }
       
        //other_terms=other_terms.unite(ot2);
    }
    intersecting_terms=this->leadingTerms.diff(other_terms);
    intersecting_terms=intersecting_terms.diff(ot2);
    assert (!((!(p.isOne())) && is00 && is11));
  }
          
  
  this->easyProductCriterions+=this->minimalLeadingTerms.length()-intersecting_terms.length();
  
 
  
  generators.push_back(e);
  pairs.status.prolong(PairStatusSet::HAS_T_REP);
  const int s=generators.size()-1;
  
  
  int i;

  
  Polynomial inter_as_poly=intersecting_terms;
  
  Polynomial::exp_iterator is_it=inter_as_poly.expBegin();
  Polynomial::exp_iterator is_end=inter_as_poly.expEnd();
  while(is_it!=is_end){
    int index =this->exp2Index[*is_it];
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
  if (impl_v!=NULL){
    assert(is_impl);
    int impl_v_size=impl_v->size();
    int j;
    for (j=0;j<impl_v_size;j++){
        this->pairs.status.setToHasTRep((*impl_v)[j],s);
    }
  }
  //Monomial crit_vars=Polynomial(intersecting_terms).usedVariables();
  
  
   treatNormalPairs(s,intersecting_terms,other_terms);
    //!!!!! here we add the lm !!!!
    //we assume that lm is minimal in leadingTerms
    
    
    
    
    std::vector<Polynomial> impl;
    MonomialSet divisors_from_minimal=minimalLeadingTerms.divisorsOf(lm);//intersect(lm.divisors());
    if(divisors_from_minimal.emptiness()){
       
        
        
        MonomialSet lm_multiples_min=minimalLeadingTerms.multiplesOf(lm);
        //minimalLeadingTerms.intersect(lm.multiples(minimalLeadingTerms.usedVariables()));
        lm_multiples_min=lm_multiples_min.diff(lm.diagram());
        //(lm.diagram()).diff(lm.diagram());
    
        assert(lm_multiples_min.intersect(minimalLeadingTerms).intersect(lm.diagram()).emptiness());

        {
        
        
            MonomialSet::exp_iterator mfm_start=lm_multiples_min.expBegin();
            MonomialSet::exp_iterator mfm_end=lm_multiples_min.expEnd();
            while(mfm_start!=mfm_end){
                assert((*mfm_start)!=e.lmExp);
                assert((*mfm_start).reducibleBy(e.lmExp));
                generators[exp2Index[*mfm_start]].minimal=false;
                mfm_start++;
            }
        }
        minimalLeadingTerms=minimalLeadingTerms.diff(lm_multiples_min);
        
        minimalLeadingTerms.uniteAssign(Polynomial(lm).diagram());   
        
        if (!(is_impl)){

        impl=treatVariablePairs(s);
      }
        else {
          mark_all_variable_pairs_as_calculated(*this, s);
        }
        
    } else 
    {
        //cerr<<"Warning:adding non minimal element to strategy"<<std::endl;
        //assert(false);
        if (!(divisors_from_minimal.diff(lm.diagram()).emptiness()))
            this->generators[s].minimal=false;
    }
    leadingTerms.uniteAssign(Polynomial(lm).diagram());
    if (generators[s].literal_factors.is11Factorization())
        leadingTerms11.uniteAssign(Polynomial(lm).diagram());
    //doesn't need to be undone on simplification
    if (generators[s].literal_factors.is00Factorization())
        leadingTerms00.uniteAssign(Polynomial(lm).diagram());
    lm2Index[generators[s].lm]=s;
    exp2Index[generators[s].lmExp]=s;
    int impl_s=impl.size();
    std::vector<int> implication_indices;
    implication_indices.push_back(s);
    for(i=0;i<impl_s;i++){
        Polynomial implication=impl[i];
        if (optRedTail)
            implication=red_tail(*this,implication);
            
        implication_indices.push_back(
            addGenerator(implication,true,&implication_indices));
        //addGeneratorDelayed(implication);
    }
    return s;
}
void GroebnerStrategy::addNonTrivialImplicationsDelayed(const PolyEntry& e){
  
  const Polynomial &p=e.p;
  Monomial factor;
  const LiteralFactorization& fac_orig=e.literal_factors;
  if (fac_orig.factors.size()>0){
    LiteralFactorization::map_type::const_iterator it=fac_orig.factors.begin();
    LiteralFactorization::map_type::const_iterator end=fac_orig.factors.end();
    while(it!=end){
        if (it->second==0)
            factor*=Variable(it->first);
        it++;
    }
    
  }
  /*Polynomial p_divided=p;
  Monomial::const_iterator m_b=factor.begin();
  Monomial::const_iterator m_e=factor.end();
  while(m_b!=m_e){
    p_divided=((BooleSet)p_divided).subset1(*m_b);
    m_b++;
  }*/
  Polynomial negation=Polynomial(true)-p/factor;
  if (!(negation.isZero())){
  LiteralFactorization fac_neg(negation);
  if (!(fac_neg.trivial())){
    this->log("!!!!!!!!!!!!!!!!!");
    this->log("found new implications");

    LiteralFactorization::map_type::const_iterator nb= fac_neg.factors.begin();
    LiteralFactorization::map_type::const_iterator ne= fac_neg.factors.end();
    while(nb!=ne){
      
      idx_type var=nb->first;
      bool val=1-nb->second;
      this->addGeneratorDelayed(factor*(Variable(var)+Polynomial(val)));
      nb++;
    }
    LiteralFactorization::var2var_map_type::const_iterator vnb= fac_neg.var2var_map.begin();
    LiteralFactorization::var2var_map_type::const_iterator vne= fac_neg.var2var_map.end();
    while(vnb!=vne){
      
      idx_type var=vnb->first;
      idx_type val=vnb->second;
      this->addGeneratorDelayed(factor*(Variable(var)+Variable(val)+Polynomial(true)));
      vnb++;
    }
    if (!(fac_neg.rest.isOne()))
        this->addGeneratorDelayed(factor*(Polynomial(true)-fac_neg.rest));
  } else {
    /*
    LiteralFactorization fac_p(p);
    if (!(fac_p.trivial()) &&(!(fac_p.rest.isOne()))){
      LiteralFactorization fac_res_neg(fac_p.rest+Polynomial(true));
      if(!(fac_res_neg.trivial())){
        cout<<"!!!!!!!!!!!!!!!!!"<<endl;
        cout<<"found nested new implications"<<endl;

      }
    }
    */
  }
  }
  
}
void GroebnerStrategy::addGeneratorDelayed(const BoolePolynomial& p){
  this->pairs.introducePair(Pair(p));
}




bool GroebnerStrategy::variableHasValue(idx_type v){
  int i;
  int s=this->generators.size();
  //Monomial m=Variable(v);
  for(i=0;i<s;i++){
    if (this->generators[i].usedVariables.deg()==1){
      if ((*(this->generators[i].usedVariables.begin()))==v){
        return true;
      }
    }
  }
  return false;
}
std::vector<Polynomial> GroebnerStrategy::minimalizeAndTailReduce(){
    MonomialSet m=minimal_elements(this->minimalLeadingTerms);
    std::vector<Polynomial> result;
    bool tail_growth_bak=optRedTailDegGrowth;
    optRedTailDegGrowth=true;
    MonomialSet::const_iterator it=m.begin();
    MonomialSet::const_iterator end=m.end();
    while(it!=end){
        //redTail
        result.push_back(red_tail(*this,generators[lm2Index[*it]].p));
        it++;
    }
    optRedTailDegGrowth=tail_growth_bak;
    return result;
    
}

std::vector<Polynomial> GroebnerStrategy::minimalize(){
    MonomialSet m=minimal_elements(this->minimalLeadingTerms);
    std::vector<Polynomial> result;
    MonomialSet::const_iterator it=m.begin();
    MonomialSet::const_iterator end=m.end();
    while(it!=end){
        //redTail
        result.push_back(generators[lm2Index[*it]].p);
        it++;
    }
    return result;
    
}

class ShorterEliminationLength{
  public:
    const GroebnerStrategy* strat;
    wlen_type el;
    ShorterEliminationLength(const GroebnerStrategy& strat,wlen_type el){
    this->el=el;
    this->strat=&strat;
    }
    bool operator() (const Exponent& e){
        assert(strat->exp2Index.find(e)!=strat->exp2Index.end());
        return (strat->generators[strat->exp2Index.find(e)->second].weightedLength<=el);
    }
};

class ShorterEliminationLengthModified{
  public:
    const GroebnerStrategy* strat;
  wlen_type el;
  deg_type lm_deg;
  ShorterEliminationLengthModified(const GroebnerStrategy& strat,wlen_type el, deg_type  lm_deg){
    this->el=el;
    this->strat=&strat;
    this->lm_deg=lm_deg;
    
  }
  bool operator() (const Exponent& e){
    assert(strat->exp2Index.find(e)!=strat->exp2Index.end());
    assert(e.deg()<=lm_deg);

    const PolyEntry* p=&strat->generators[strat->exp2Index.find(e)->second];
    return p->weightedLength<=el+(lm_deg-p->lmDeg)*p->length;
  }
};
void GroebnerStrategy::addGeneratorTrySplit(const Polynomial & p, bool is_minimal){
  std::vector<Polynomial> impl;
  int way=0;
  if (have_ordering_for_tables()){

    int u_v=p.usedVariablesExp().deg();
    if  (u_v<=4) {
      way=1;
      impl=add4ImplDelayed(p,p.leadExp(),p.usedVariablesExp(),-1,true);
    } else {
      way=2;
      if (((optAllowRecursion) && (u_v<=15))||(u_v<=10)){
        way=3;
        LiteralFactorization f(p);
        if (f.rest.usedVariablesExp().deg()<=4){
          way=4;
          impl=addHigherImplDelayedUsing4(-1,f,true);
        } else {
          deg_type rest_lead_exp_deg=f.rest.leadExp().deg();
          deg_type rest_used_variables_deg=f.rest.usedVariablesExp().deg();
          if ((optAllowRecursion) &&(is_minimal) && 

            (
            (rest_used_variables_deg<=rest_lead_exp_deg+2)||
            ((rest_lead_exp_deg<=6)
            &&(rest_used_variables_deg<=rest_lead_exp_deg+3))||
            ((rest_lead_exp_deg<=4)
            &&(rest_used_variables_deg<=rest_lead_exp_deg+4))||
            ((rest_lead_exp_deg<=3)
            &&(rest_used_variables_deg<=rest_lead_exp_deg+5))||
            ((rest_lead_exp_deg<=2)
            &&(rest_used_variables_deg<=rest_lead_exp_deg+7))))
          {
                        //orig +2
            log("Recursive call");
            impl=full_implication_gb(f.rest,*cache,*this);
            int i;
            int s=impl.size();
            for(i=0;i<s;i++){
              impl[i]=multiply_with_literal_factors(f,impl[i]);
            }
          }
        }

      } else {

      }

    }


  } 
  if (impl.size()==0)
    addGenerator(p);
  else{
    int s=impl.size();
    int i;
    std::vector<int> implication_indices;
    for(i=0;i<s;i++){
      assert(!(impl[i].isZero()));
      if (minimalLeadingTerms.divisorsOf(impl[i].leadExp()).emptiness()){

        
        Polynomial p_impl=impl[i];
        if(optRedTail) p_impl=red_tail(*this,p_impl);
        assert(!(p_impl.isZero()));
        implication_indices.push_back(
          addGenerator(p_impl,true,&implication_indices));
      }
      else {
        addGeneratorDelayed(impl[i]);
                //assert(impl[i]!=)
      }

    }
    assert(!(leadingTerms.divisorsOf(p.leadExp()).emptiness()));
  }
}
void GroebnerStrategy::addAsYouWish(const Polynomial& p){
    //if (p.isZero()) return;
    Exponent lm_exp=p.leadExp();
    MonomialSet divisors=this->leadingTerms.divisorsOf(lm_exp);
    #if 0
    if (divisors.emptiness())
        addGenerator(p);
    else addGeneratorDelayed(p);
    #else
    if ((optDelayNonMinimals) && (!(divisors.emptiness()))){
      addGeneratorDelayed(p);
      return;
    }else {
      if (divisors.owns(Monomial(lm_exp))){
          addGeneratorDelayed(p);
          return;
    }}
    wlen_type el=p.eliminationLength();
    if (std::find_if(
            divisors.expBegin(),
            divisors.expEnd(),
            ShorterEliminationLengthModified(*this,el,lm_exp.deg()))!=divisors.expEnd()){
        this->addGeneratorDelayed(p);
    } else {
        Polynomial pr;
        if (optRedTail)
            pr=red_tail(*this,p);
        else 
            pr=p;
        if (pr!=p){
            el=pr.eliminationLength();
            if (std::find_if(
                    divisors.expBegin(),
                    divisors.expEnd(),
                    ShorterEliminationLengthModified(*this,el,lm_exp.deg()))!=divisors.expEnd()){
                this->addGeneratorDelayed(pr);
            } else {
                if (divisors.emptiness())
                    this->addGeneratorTrySplit(pr, true);
                else
                    addGenerator(pr);
            }
        } else
            if (divisors.emptiness())
                this->addGeneratorTrySplit(p, true);
            else
                addGenerator(p);
    }
    #endif
}
static std::vector<Polynomial> small_next_degree_spolys(GroebnerStrategy& strat, double f, int n){
  std::vector<Polynomial> res;
  assert(!(strat.pairs.pairSetEmpty()));
  strat.pairs.cleanTopByChainCriterion();
  deg_type deg=strat.pairs.queue.top().sugar;
  wlen_type wlen=strat.pairs.queue.top().wlen;
  while((!(strat.pairs.pairSetEmpty())) &&(strat.pairs.queue.top().sugar<=deg) && (strat.pairs.queue.top().wlen<=wlen*f+2)&& (res.size()<n)){
    
    assert(strat.pairs.queue.top().sugar==deg);
    res.push_back(strat.nextSpoly());
    strat.pairs.cleanTopByChainCriterion();
  }
  return res;
  
}
void GroebnerStrategy::symmGB_F2(){
    const double max_growth=2.0;
    const int selection_size=1000;
    const double pair_size_factor=2.0;
    while(pairs.queue.size()>0){
        std::vector<Polynomial> next=small_next_degree_spolys(*this,pair_size_factor,selection_size);
        std::vector<Polynomial> res;
        if (next.size()>100)
           res=parallel_reduce(next,*this,10,max_growth);
        else{
           if (next.size()>10)
              res=parallel_reduce(next,*this,30,max_growth);
           else
              res=parallel_reduce(next,*this,100, max_growth);}
        int s=res.size();
        int i;
        for(i=s-1;i>=0;i--){
            Polynomial p=res[i];
            addAsYouWish(res[i]);
            if (p.isOne())
                return;
        }
        
    }
}
static int get_first_variable_index(const Polynomial& p){
    return *(p.navigation());
}
Polynomial mult_fast_sim(const std::vector<Polynomial>& vec){
    std::vector<Polynomial> new_vec;
    
    int i;
    int s=vec.size();
    int index=-1;
    for(i=0;i<s;i++){
        if (vec[i].isZero()) return Polynomial(0);
        if (!(vec[i].isOne())){
    
            new_vec.push_back(vec[i]);
    
            if (index<0){
                index=*(vec[i].navigation());
    
                }
            else {
    
                //int a_i=*p.navigation();
                index=std::min(index,*(vec[i].navigation()));
            }
        }
    }
    
    
    
    if (new_vec.size()==0) return Polynomial(1);
    if (new_vec.size()==1) return new_vec[0];
    s=new_vec.size();
    std::vector<Polynomial> s0_vec(s);
    std::vector<Polynomial> s1_vec(s);
    for(i=0;i<s;i++){
        s0_vec[i]=new_vec[i].diagram().subset0(index);
        s1_vec[i]=new_vec[i].diagram().subset1(index).Xor(new_vec[i].diagram().subset0(index));
    }
    
    Polynomial s0=mult_fast_sim(s0_vec);
    Polynomial s1=mult_fast_sim(s1_vec);
    
    return ((Monomial) Variable(index))*(s1+s0)+s0;
}
std::vector<Polynomial> full_implication_gb(const Polynomial & p, CacheManager& cache, GroebnerStrategy& strat_param){
    bool succ;
    CacheManager::res_type cache_res=cache.lookup(p,succ);
    if (!(succ)){
    GroebnerStrategy strat;
    strat.cache=strat_param.cache;
    //strat.log("Recursive call");
    strat.optAllowRecursion=true;
    strat.addGenerator(p);
    strat.symmGB_F2();
    std::vector<Polynomial> res=strat.minimalizeAndTailReduce();
    cache.insert(p,res);
    return res;
    } else
    {   
        strat_param.log("used dynamic cache");
        std::vector<Polynomial> res(*cache_res);
        return res;
    }
}
int GroebnerStrategy::suggestPluginVariable(){
    std::vector<int> ranking(BoolePolyRing::nRingVariables());
    int s=ranking.size();
    int i;
    for(i=0;i<s;i++){ ranking[i]=0;}
    int result=-1;
    int most_occ=0;
    MonomialSet::exp_iterator it=minimalLeadingTerms.expBegin();
    MonomialSet::exp_iterator end=minimalLeadingTerms.expEnd();
    while(it!=end){
        Exponent curr=*it;
        if (curr.deg()>=2){
        Exponent::const_iterator curr_it=curr.begin();
        Exponent::const_iterator curr_end=curr.end();
        while(curr_it!=curr_end){
            ranking[*curr_it]++;
            curr_it++;
        }}
        it++;
    }
    
    for(i=0;i<ranking.size();i++){
        if (most_occ<ranking[i])
        {
            most_occ=ranking[i];
            result=i;
        }
    }
    return result;
}
END_NAMESPACE_PBORIGB
