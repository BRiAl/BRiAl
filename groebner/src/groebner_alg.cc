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
:pairs(orig.pairs),
generators(orig.generators),
leadingTerms(orig.leadingTerms),
minimalLeadingTerms(orig.minimalLeadingTerms),
  leadingTerms11(orig.leadingTerms11),
  leadingTerms00(orig.leadingTerms00),
  lm2Index(orig.lm2Index)
{
  optRedTail=orig.optRedTail;
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
            strat->log("delayed variable linear factor criterion");
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
  this->lmExp=lm.exp();
  this->weightedLength=p.eliminationLength();
  this->length=p.length();
  this->usedVariables=p.usedVariables();
  this->deg=p.deg();
  this->tailVariables=(p-lm).usedVariables();
  this->lmDeg=p.lmDeg();
  this->minimal=true;
}


void PolyEntry::recomputeInformation(){
  assert(this->lm==p.lead());
  //so also lmExp keeps constant
  this->weightedLength=p.eliminationLength();
  this->length=p.length();
  this->usedVariables=p.usedVariables();
  this->deg=p.deg();
  this->tailVariables=(p-lm).usedVariables();
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
    int i;
    int s=generators.size();
    for(i=0;i<s;i++){      
      if ((this->generators[i].minimal) &&(this->generators[i].length>1) &&(&this->generators[i]!=&e)&&(this->generators[i].tailVariables.reducibleBy(lm))){
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
        s1=s1.diff(s0.unateProduct(Polynomial(s1).usedVariables().divisors()));
        
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
        s1=s1.diff(s0.unateProduct(Polynomial(s1).usedVariables().divisors()));
        
    }
    return s0.unite(s1.change(i)).unite(result);

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




//static Monomial oper(int i){
//    return Monomial(Variable(i));
//}
static MonomialSet divide_monomial_divisors_out_old(const MonomialSet& s, const Monomial& lm){
    MonomialSet m=s;
    Monomial::const_iterator it_lm=lm.begin();
        Monomial::const_iterator end_lm=lm.end();
        while(it_lm!=end_lm){
            idx_type i=*it_lm;
            m=m.subset0(i).unite(m.subset1(i));
            it_lm++;
        }
   return m;
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
static std::vector<Exponent> minimal_elements_divided(MonomialSet m, Monomial lm){
    std::vector<Exponent> result;
    Exponent exp=lm.exp();
    if (!(m.divisorsOf(lm).emptiness())){
        result.push_back(exp);
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
        Polynomial p=Polynomial(m);
        if (!(m.emptiness())){
            //m=m.unateProduct(lm.diagram());
            result.insert(result.end(), p.expBegin(), p.expEnd());
           /* int i;
            for(i=0;i<result.size();i++){
                result[i]=result[i].multiply(exp);
            }*/
        }
        
        
    }
    return result;
}
#endif
void GroebnerStrategy::addGenerator(const BoolePolynomial& p){
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
  //cout<<"is11"<<is11<<endl;
  //cout<<"is00"<<is00<<endl;
  if (!((is00 && (leadingTerms==leadingTerms00))||(is11 && (leadingTerms==leadingTerms11)))){
    //cout<<"have pairs"<<endl;
    if (is11)
        other_terms=other_terms.diff(leadingTerms11);
    if (is00)
        other_terms=other_terms.diff(leadingTerms00);
    while(it!=end){
    //cout<<"intersect"<<endl;
    other_terms=other_terms.subset0(*it);
    it++;
    
    }
    
    
    if ((is11)||(is00)){
        MonomialSet ot2;
        if (is00)
            ot2=leadingTerms00;
        else
            ot2=leadingTerms11;
        Monomial::const_iterator lm_start=lm.begin();
        Monomial::const_iterator lm_end=lm.end();
        while(lm_start!=lm_end){
            ot2=(ot2.subset0(*lm_start).unite(ot2.subset1(*lm_start)));
            lm_start++;
        }
        other_terms=other_terms.unite(ot2);
    }
    intersecting_terms=this->leadingTerms.diff(other_terms);

    assert (!((!(p.isOne())) && is00 && is11));
  }
          
  //cout<<"intersect:"<<intersecting_terms.length()<<endl;
     

  
  
  
  ///@todo: correct counting of easy/extended product crit
  /*if (is00){

          intersecting_terms=intersecting_terms.diff(leadingTerms00);
      }
  else {
    if (is11){
      intersecting_terms=intersecting_terms.diff(leadingTerms11);
        //cout<<"is11:"<<leadingTerms11.length()<<":"<<intersecting_terms.length();
      }
  }*/
  this->easyProductCriterions+=this->minimalLeadingTerms.length()-intersecting_terms.length();
  
 
  
  generators.push_back(e);
  pairs.status.prolong(PairStatusSet::HAS_T_REP);
  const int s=generators.size()-1;
  lm2Index[generators[s].lm]=s;
  exp2Index[generators[s].lmExp]=s;
  
  int i;

  
  
  it=generators[s].lm.begin();
  end=generators[s].lm.end();
  while(it!=end){
    if ((generators[s].lm.deg()==1) || generators[s].literal_factors.occursAsLeadOfFactor(*it)){//((MonomialSet(p).subset0(*it).emptiness())||(MonomialSet(p).subset0(*it)==(MonomialSet(p).subset1(*it))))){
      //cout<<"factorcrit"<<endl;
      generators[s].vPairCalculated.insert(*it);
    } else
      this->pairs.introducePair(Pair(s,*it,generators,VARIABLE_PAIR));
    it++;
    
  }
  
  //workaround
  Polynomial inter_as_poly=intersecting_terms;
  //BooleSet::const_iterator is_it=intersecting_terms.begin();
  //BooleSet::const_iterator is_end=intersecting_terms.end();
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

  Monomial crit_vars=Polynomial(intersecting_terms).usedVariables();
  
  
  if(!(Polynomial(other_terms).hasConstantPart()))//.divisorsOf(lm).emptiness()))
   {

#ifndef EXP_FOR_PAIRS
        //MonomialSet already_used;
        std::vector<Monomial> mt_vec=minimal_elements_multiplied(intersecting_terms.intersect(minimalLeadingTerms), lm);
#else
        std::vector<Exponent> mt_vec=minimal_elements_divided(intersecting_terms.intersect(minimalLeadingTerms), lm);
#endif
        //(multiplied_terms.length());
        
        //assert(mt_i==multiplied_terms.length());
        //MonomialSet chosen;
        //mt_i=mt_vec.size()-1; //alternativ mt_i--, but that's very bad
        //we assume, that iteration through polynomial corresponds to a global ordering
        //assert(mt_vec.size()==multiplied_terms.length());
        int mt_i;
        for(mt_i=mt_vec.size()-1;mt_i>=0;mt_i--){
#ifndef EXP_FOR_PAIRS
            Monomial t=mt_vec[mt_i];
            Monomial t_divided=t/lm;
            assert(t.reducibleBy(lm));
#else
            Exponent t_divided=mt_vec[mt_i];
            Exponent t=t_divided*e.lmExp;
#endif
            MonomialSet lm_d=t_divided.divisors();
            if ((other_terms.intersect(lm_d).emptiness())){
           // #ifndef EXP_FOR_PAIRS
           //     MonomialSet act_l_terms=leadingTerms.intersect(t.divisors());
            //#else
                Polynomial act_l_terms=leadingTerms.intersect(t.divisors());
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
                int chosen_index=lm2Index[min];
                this->pairs.introducePair(Pair(chosen_index,s,generators));
            }
            //if (t.intersect())
        }
        
   }
    //!!!!! here we add the lm !!!!
    //we assume that lm is minimal in leadingTerms
    
    
    MonomialSet lm_multiples_min=lm.multiples(Polynomial(minimalLeadingTerms).usedVariables());
    assert(lm_multiples_min.intersect(minimalLeadingTerms).intersect(lm.diagram()).emptiness());
    lm_multiples_min=lm_multiples_min.intersect(minimalLeadingTerms).diff(lm.diagram());
    {
        //assert(multiples_from_minimal.intersect(lm.diagram()).emptiness());
        
        MonomialSet::const_iterator mfm_start=lm_multiples_min.begin();
        MonomialSet::const_iterator mfm_end=lm_multiples_min.end();
        while(mfm_start!=mfm_end){
            assert((*mfm_start)!=lm);
            assert((*mfm_start).reducibleBy(lm));
            generators[lm2Index[*mfm_start]].minimal=false;
            mfm_start++;
        }
    }
    minimalLeadingTerms=leadingTerms.diff(lm_multiples_min);
    leadingTerms.uniteAssign(Polynomial(lm).diagram());
    MonomialSet divisors_from_minimal=minimalLeadingTerms.intersect(lm.divisors());
    if(divisors_from_minimal.emptiness()){
       
        
        
        minimalLeadingTerms.uniteAssign(Polynomial(lm).diagram());
    } else 
    {
        if (!(divisors_from_minimal.diff(lm.diagram()).emptiness()))
            this->generators[s].minimal=false;
    }

    if (generators[s].literal_factors.is11Factorization())
        leadingTerms11.uniteAssign(Polynomial(lm).diagram());
    //doesn't need to be undone on simplification
    if (generators[s].literal_factors.is00Factorization())
        leadingTerms00.uniteAssign(Polynomial(lm).diagram());

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
std::vector<Polynomial> GroebnerStrategy::minimalizeAndTailReduce(){
    MonomialSet m=minimal_elements(this->minimalLeadingTerms);
    std::vector<Polynomial> result;
    MonomialSet::const_iterator it=m.begin();
    MonomialSet::const_iterator end=m.end();
    while(it!=end){
        //redTail
        result.push_back(red_tail(*this,generators[lm2Index[*it]].p));
        it++;
    }
    return result;
    
}
END_NAMESPACE_PBORIGB
