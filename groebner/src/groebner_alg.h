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
  void addGenerator(const BoolePolynomial& p);
  void addGeneratorDelayed(const BoolePolynomial & p);
    PairManager pairs;
  PolyEntryVector generators;
  MonomialSet leadingTerms;
   unsigned int reductionSteps;
  int normalForms;
  int currentDegree;
  int chainCriterions;
  int variableChainCriterions;
  GroebnerStrategy():pairs(*this){
    chainCriterions=0;
    variableChainCriterions=0;
    extendedProductCriterions=0;
  }

  int easyProductCriterions;
  int extendedProductCriterions;
  int averageLength;
  
  lm2Index_map_type lm2Index;
  Polynomial nextSpoly(){
    return pairs.nextSpoly(generators);
  }
  void addNonTrivialImplicationsDelayed(const Polynomial& p);  
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

END_NAMESPACE_PBORIGB

#endif

