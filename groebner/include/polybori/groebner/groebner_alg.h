/*
 *  groebner_alg.h
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 20.04.06.
 *  Copyright 2006 The PolyBoRi Team. See LICENSE file.
 *
 */



#include <polybori.h>
#include "groebner_defs.h"
#include "pairs.h"
#include <boost/dynamic_bitset.hpp>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>
#include <iostream>
#include "cache_manager.h"
#include "polynomial_properties.h"

#ifndef PBORI_GB_ALG_H
#define PBORI_GB_ALG_H


BEGIN_NAMESPACE_PBORIGB

#define LL_RED_FOR_GROEBNER 1
MonomialSet minimal_elements(const MonomialSet& s);
Polynomial map_every_x_to_x_plus_one(Polynomial p);
class PairStatusSet{
public:
  typedef boost::dynamic_bitset<> bitvector_type;
  bool hasTRep(int ia, int ja) const {
    int i,j;
    i=std::min(ia,ja);
    j=std::max(ia,ja);
    return table[j][i]==HAS_T_REP;
  }
  void setToHasTRep(int ia, int ja){
    int i,j;
    i=std::min(ia,ja);
    j=std::max(ia,ja);
    table[j][i]=HAS_T_REP;
  }
  void setToUncalculated(int ia, int ja){
    int i,j;
    i=std::min(ia,ja);
    j=std::max(ia,ja);
    table[j][i]=UNCALCULATED;
  }
  void prolong(bool value=UNCALCULATED){
    int s=table.size();
    table.push_back(bitvector_type(s, value));
  }
  PairStatusSet(int size=0){
    int s=0;
    for(s=0;s<size;s++){
      prolong();
    }
  }
  static const bool HAS_T_REP=true;
  static const bool UNCALCULATED=false;

protected:
std::vector<bitvector_type> table;
};
class GroebnerStrategy;
class PairManager{
public:
  PairStatusSet status;
  GroebnerStrategy* strat;
  PairManager(GroebnerStrategy & strat, 
	      const BoolePolyRing& ring): queue(ring)  {
    this->strat=&strat;
  }

  void appendHiddenGenerators(std::vector<Polynomial>& vec);
  typedef std::priority_queue<Pair,std::vector<PairE>, PairECompare> queue_type;
  queue_type queue;
  void introducePair(const Pair& p);
  Polynomial nextSpoly(const PolyEntryVector& gen);
  bool pairSetEmpty() const;
  void cleanTopByChainCriterion();
protected:
	void replacePair(int& i, int & j);
   };
class MonomialHasher{
public:
  size_t operator() (const Monomial & m) const{
    return m.hash();
  }
};

typedef Monomial::idx_map_type lm2Index_map_type;
typedef Exponent::idx_map_type exp2Index_map_type;
class ReductionStrategy:public PolyEntryVector{
public:
    MonomialSet leadingTerms;
    MonomialSet minimalLeadingTerms;
    MonomialSet leadingTerms11;
    MonomialSet leadingTerms00;
    MonomialSet llReductor;
    MonomialSet monomials;
    MonomialSet monomials_plus_one;
    lm2Index_map_type lm2Index;
    exp2Index_map_type exp2Index;
    bool optBrutalReductions;
    
    bool optLL;

    Polynomial nf(Polynomial p) const;
    bool optRedTailDegGrowth;
    bool optRedTail;
    idx_type reducibleUntil;
    void setupSetsForLastElement();

    // ReductionStrategy(){ set_defaults(); }

    ReductionStrategy(const BoolePolyRing& theRing):
      leadingTerms(theRing.zero()), minimalLeadingTerms(theRing.zero()),
      leadingTerms11(theRing.zero()), leadingTerms00(theRing.zero()), llReductor(theRing.zero()),
      monomials(theRing.zero()), monomials_plus_one(theRing.zero()), lm2Index(),
      exp2Index() {
      set_defaults(); 
    }

    bool canRewrite(const Polynomial& p) const{
        return is_rewriteable(p,minimalLeadingTerms);
    }
    void addGenerator(const Polynomial& p){
        push_back(p);
        setupSetsForLastElement();
    }
    int select1(const Polynomial& p) const;
    int select1(const Monomial& m) const;

    int select_short(const Polynomial& p) const;
    int select_short(const Monomial& m) const;
    Polynomial headNormalForm(Polynomial p) const;
    
    Polynomial reducedNormalForm(Polynomial p) const;

 protected:
    void set_defaults(){
        optLL=false;
        reducibleUntil=-1;
        optBrutalReductions=true;
        optRedTail=true;
        optRedTailDegGrowth=true;
    }
};
class GroebnerStrategy{
public:
  bool containsOne() const{
    return generators.leadingTerms.ownsOne();
  }
  
  GroebnerStrategy(const GroebnerStrategy& orig);
  std::vector<Polynomial>  minimalizeAndTailReduce();
  std::vector<Polynomial>  minimalize();
  int addGenerator(const BoolePolynomial& p, bool is_impl=false, std::vector<int>* impl_v=NULL);
  void addGeneratorDelayed(const BoolePolynomial & p);
  void addAsYouWish(const Polynomial& p);  
  void addGeneratorTrySplit(const Polynomial& p, bool is_minimal);
  bool variableHasValue(idx_type i);
  void llReduceAll();
  void treat_m_p_1_case(const PolyEntry& e);
  PairManager pairs;
  bool reduceByTailReduced;
  ReductionStrategy generators;
  bool optDrawMatrices;
  std::string matrixPrefix;
  //const char* matrixPrefix;
  boost::shared_ptr<CacheManager> cache;
  BoolePolyRing r;
  bool enabledLog;
   unsigned int reductionSteps;
  int normalForms;
  int currentDegree;
  int chainCriterions;
  int variableChainCriterions;
  int easyProductCriterions;
  int extendedProductCriterions;
  int averageLength;
  
  bool optHFE;
  bool optLazy;
  bool optModifiedLinearAlgebra;
  bool optDelayNonMinimals;
  
  bool optExchange;
  bool optAllowRecursion;
  
  bool optStepBounded;
  bool optLinearAlgebraInLastBlock;
  bool optRedTailInLastBlock;


	GroebnerStrategy(const BoolePolyRing& input_ring):
  generators(input_ring),
        r(input_ring), pairs(*this, input_ring), cache(new CacheManager()) {

        optDrawMatrices=false;
        optModifiedLinearAlgebra=false;
        matrixPrefix="mat";
	  optDelayNonMinimals=true;
		
		chainCriterions=0;
		enabledLog=false;
        
        //if (this->r.ordering().isDegreeOrder())
		//    optBrutalReductions=false;
		//else
        
		variableChainCriterions=0;
		extendedProductCriterions=0;
		easyProductCriterions=0;
		
		optExchange=true;
        optHFE=false;
		optStepBounded=false;
		optAllowRecursion=true;
        optLinearAlgebraInLastBlock=true;
        if (this->r.ordering().isBlockOrder())
            optRedTailInLastBlock=true;
        else 
            optRedTailInLastBlock=false;

		if (this->r.ordering().isDegreeOrder())
			optLazy=false;
		else
			optLazy=true;
		reduceByTailReduced=false;
		generators.llReductor=BooleSet(input_ring.one());
	}

    Polynomial nextSpoly(){
    return pairs.nextSpoly(generators);
  }
  void addNonTrivialImplicationsDelayed(const PolyEntry& p);
  void propagate(const PolyEntry& e); 
  void propagate_step(const PolyEntry& e, std::set<int> others);
  void log(const char* c){
      if (this->enabledLog)
        std::cout<<c<<std::endl;
  }

  Polynomial redTail(const Polynomial& p);
  std::vector<Polynomial> noroStep(const std::vector<Polynomial>&);
  std::vector<Polynomial> faugereStepDense(const std::vector<Polynomial>&);
  //std::vector<Polynomial> faugereStepDenseModified(const std::vector<Polynomial>&);
  Polynomial nf(Polynomial p) const;
  void symmGB_F2();
  int suggestPluginVariable();
  std::vector<Polynomial> allGenerators();
  protected:
      std::vector<Polynomial> treatVariablePairs(int s);
      void treatNormalPairs(int s,MonomialSet intersecting_terms,MonomialSet other_terms, MonomialSet ext_prod_terms);
      void addVariablePairs(int s);
      std::vector<Polynomial> add4ImplDelayed(const Polynomial& p, const Exponent& lm_exp, const Exponent& used_variables,int s, bool include_orig);
      std::vector<Polynomial> addHigherImplDelayedUsing4(int s, const LiteralFactorization& literal_factors, bool include_orig);
      
      
};


MonomialSet mod_var_set(const MonomialSet& as, const MonomialSet& vs);
void groebner(GroebnerStrategy& strat);
Polynomial reduce_by_binom(const Polynomial& p, const Polynomial& binom);
Polynomial reduce_by_monom(const Polynomial& p, const Monomial& m);
Polynomial reduce_complete(const Polynomial& p, const Polynomial& reductor);
class LessWeightedLengthInStrat{
public:
  const ReductionStrategy* strat;
  LessWeightedLengthInStrat(const ReductionStrategy& strat){
    this->strat=&strat;
  }
  bool operator() (const Monomial& a , const Monomial& b){
    return (*strat)[strat->lm2Index.find(a)->second].weightedLength<(*strat)[strat->lm2Index.find(b)->second].weightedLength;
    
  }
  bool operator() (const Exponent& a , const Exponent& b){
    return (*strat)[strat->exp2Index.find(a)->second].weightedLength<(*strat)[strat->exp2Index.find(b)->second].weightedLength;
    
  }
};


class LargerDegreeComparer{
public:
  bool operator() (const Monomial& a , const Monomial& b){
      return a.deg() > b.deg(); 
    
  }
  bool operator() (const Exponent& a , const Exponent& b){
      return a.deg()>b.deg();
    
  }
};

inline wlen_type wlen_literal_exceptioned(const PolyEntry& e){
    wlen_type res=e.weightedLength;
    if ((e.deg==1) && (e.length<=4)){
        //if (e.length==1) return -1;
        //if (e.p.hasConstantPart()) return 0;
        return res-1;
    }
    return res;
}
///@todo: in many cases, indices should be precalculated
class LessWeightedLengthInStratModified{
public:
  const ReductionStrategy* strat;
  LessWeightedLengthInStratModified(const ReductionStrategy& strat){
    this->strat=&strat;
  }
  bool operator() (const Monomial& a , const Monomial& b){
    wlen_type wa=wlen_literal_exceptioned((*strat)[strat->lm2Index.find(a)->second]);
    wlen_type wb=wlen_literal_exceptioned((*strat)[strat->lm2Index.find(b)->second]);
    
    return wa<wb;
    
  }
  bool operator() (const Exponent& a , const Exponent& b){
    wlen_type wa=wlen_literal_exceptioned((*strat)[strat->exp2Index.find(a)->second]);
    wlen_type wb=wlen_literal_exceptioned((*strat)[strat->exp2Index.find(b)->second]);
    
    return wa<wb;
    
  }
};
class LessEcartThenLessWeightedLengthInStrat{
public:
  const GroebnerStrategy* strat;
  LessEcartThenLessWeightedLengthInStrat(const GroebnerStrategy& strat){
    this->strat=&strat;
  }
  bool operator() (const Monomial& a , const Monomial& b){
    int i=strat->generators.lm2Index.find(a)->second;
    int j=strat->generators.lm2Index.find(b)->second;
    if (strat->generators[i].ecart()!=strat->generators[j].ecart()){
      if (strat->generators[i].ecart()<strat->generators[j].ecart())
        return true;
      else
        return false;
    }
    return (strat->generators[i].weightedLength<strat->generators[j].weightedLength);
    
  }
  
  bool operator() (const Exponent& a , const Exponent& b){
    int i=strat->generators.exp2Index.find(a)->second;
    int j=strat->generators.exp2Index.find(b)->second;
    if (strat->generators[i].ecart()!=strat->generators[j].ecart()){
      if (strat->generators[i].ecart()<strat->generators[j].ecart())
        return true;
      else
        return false;
    }
    return (strat->generators[i].weightedLength<strat->generators[j].weightedLength);
    
  }
};
class LessUsedTailVariablesThenLessWeightedLengthInStrat{
public:
  const GroebnerStrategy* strat;
  LessUsedTailVariablesThenLessWeightedLengthInStrat(const GroebnerStrategy& strat){
    this->strat=&strat;
  }
  bool operator() (const Monomial& a , const Monomial& b) const{
    int i=strat->generators.lm2Index.find(a)->second;
    int j=strat->generators.lm2Index.find(b)->second;
    deg_type d1=strat->generators[i].tailVariables.deg();
    deg_type d2=strat->generators[j].tailVariables.deg();;
    if (d1!=d2){
      return (d1<d2);
          }
    return (strat->generators[i].weightedLength<strat->generators[j].weightedLength);
    
  }
};

class LessCombinedManySizesInStrat{
public:
  GroebnerStrategy* strat;
  LessCombinedManySizesInStrat(GroebnerStrategy& strat){
    this->strat=&strat;
  }
  bool operator() (const Monomial& a , const Monomial& b){
    int i=strat->generators.lm2Index[a];
    int j=strat->generators.lm2Index[b];
        deg_type d1=strat->generators[i].tailVariables.deg();
    deg_type d2=strat->generators[j].tailVariables.deg();
    wlen_type w1=d1;
    wlen_type w2=d2;
    w1*=strat->generators[i].length;
    w1*=strat->generators[i].ecart();
    w2*=strat->generators[j].length;
    w2*=strat->generators[j].ecart();
    return w1<w2;
    
        
  }
};


Polynomial mult_fast_sim(const std::vector<Polynomial>& vec,
			 const BoolePolyRing& ring);
std::vector<Polynomial> full_implication_gb(const Polynomial & p,CacheManager& cache,GroebnerStrategy& strat);
Polynomial reduce_complete(const Polynomial &p, const PolyEntry& reductor, wlen_type &len);
MonomialSet contained_variables_cudd_style(const MonomialSet& m);
MonomialSet minimal_elements_cudd_style(MonomialSet m);
MonomialSet recursively_insert(MonomialSet::navigator p, idx_type idx, MonomialSet mset);
MonomialSet minimal_elements_cudd_style_unary(MonomialSet m);
END_NAMESPACE_PBORIGB

#endif

