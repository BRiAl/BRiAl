/*
 *  groebner_alg.h
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 20.04.06.
 *  Copyright 2006 Mathematisches Forschungsinstitut Oberwolfach. All rights reserved.
 *
 */

#define HAVE_HASH_MAP 1

#include <polybori.h>
#include "groebner_defs.h"
#include "pairs.h"
#include <boost/dynamic_bitset.hpp>
#include <vector>
#include <algorithm>
#include <utility>
#include <iostream>
#ifdef HAVE_HASH_MAP
#include <ext/hash_map>
#else
#include <map>
#endif
#ifndef PBORI_GB_ALG_H
#define PBORI_GB_ALG_H


BEGIN_NAMESPACE_PBORIGB

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
  PairManager(GroebnerStrategy & strat){
    this->strat=&strat;
  }
  typedef std::priority_queue<Pair,std::vector<Pair>, PairLSCompare> queue_type;
  queue_type queue;
  void introducePair(const Pair& p);
  Polynomial nextSpoly(const PolyEntryVector& gen);
  bool pairSetEmpty() const;
  void cleanTopByChainCriterion();
   };
class MonomialHasher{
public:
  size_t operator() (const Monomial & m) const{
    return m.hash();
  }
};
#ifdef HAVE_HASH_MAP
typedef __gnu_cxx::hash_map<Monomial,int, MonomialHasher> lm2Index_map_type;
#else
typedef std::map<Monomial,int> lm2Index_map_type;
#endif
class GroebnerStrategy{
public:
  GroebnerStrategy(const GroebnerStrategy& orig);
  void addGenerator(const BoolePolynomial& p);
  void addGeneratorDelayed(const BoolePolynomial & p);
  
  bool variableHasValue(idx_type i);
  PairManager pairs;
  PolyEntryVector generators;
  MonomialSet leadingTerms;
  MonomialSet minimalLeadingTerms;
  MonomialSet leadingTerms11;
  MonomialSet leadingTerms00;
  bool enabledLog;
   unsigned int reductionSteps;
  int normalForms;
  int currentDegree;
  int chainCriterions;
  int variableChainCriterions;
  int easyProductCriterions;
  int extendedProductCriterions;
  int averageLength;
  
  lm2Index_map_type lm2Index;
  
  GroebnerStrategy():pairs(*this){
    chainCriterions=0;
    enabledLog=false;
    variableChainCriterions=0;
    extendedProductCriterions=0;
    easyProductCriterions=0;
  }

    Polynomial nextSpoly(){
    return pairs.nextSpoly(generators);
  }
  void addNonTrivialImplicationsDelayed(const PolyEntry& p);
  void propagate(const PolyEntry& e); 
  void propagate_step(const PolyEntry& e, std::set<int> others);
  void log(const char* c){
      if (this->enabledLog)
          std::cout<<c<<endl;
  }
};
void groebner(GroebnerStrategy& strat);
Polynomial reduce_by_binom(const Polynomial& p, const Polynomial& binom);
Polynomial reduce_by_monom(const Polynomial& p, const Monomial& m);
Polynomial reduce_complete(const Polynomial& p, const Polynomial& reductor);
class LessWeightedLengthInStrat{
public:
  GroebnerStrategy* strat;
  LessWeightedLengthInStrat(GroebnerStrategy& strat){
    this->strat=&strat;
  }
  bool operator() (const Monomial& a , const Monomial& b){
    return strat->generators[strat->lm2Index[a]].weightedLength<strat->generators[strat->lm2Index[b]].weightedLength;
    
  }
};
class LessEcartThenLessWeightedLengthInStrat{
public:
  GroebnerStrategy* strat;
  LessEcartThenLessWeightedLengthInStrat(GroebnerStrategy& strat){
    this->strat=&strat;
  }
  bool operator() (const Monomial& a , const Monomial& b){
    int i=strat->lm2Index[a];
    int j=strat->lm2Index[b];
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
  GroebnerStrategy* strat;
  LessUsedTailVariablesThenLessWeightedLengthInStrat(GroebnerStrategy& strat){
    this->strat=&strat;
  }
  bool operator() (const Monomial& a , const Monomial& b){
    int i=strat->lm2Index[a];
    int j=strat->lm2Index[b];
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
    int i=strat->lm2Index[a];
    int j=strat->lm2Index[b];
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



END_NAMESPACE_PBORIGB

#endif

