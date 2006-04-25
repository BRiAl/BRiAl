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
  bool calculated(int ia, int ja){
    int i,j;
    i=std::min(ia,ja);
    j=std::max(ia,ja);
    return table[j][i];
  }
  void setToCalculated(int ia, int ja){
    int i,j;
    i=std::min(ia,ja);
    j=std::max(ia,ja);
    table[j][i]=1;
  }
  void prolong(){
    int s=table.size();
    table.push_back(bitvector_type(s));
  }
  PairStatusSet(int size=0){
    int s=0;
    for(s=0;s<size;s++){
      prolong();
    }
  }
protected:
std::vector<bitvector_type> table;
};
class PairManager{
public:
  PairStatusSet status;
  typedef std::priority_queue<Pair,std::vector<Pair>, PairLSCompare> queue_type;
  queue_type queue;
  void introducePair(const Pair& p);
  Polynomial nextSpoly(const PolyEntryVector& gen);
  bool pairSetEmpty();
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
typedef std::hash_map<Monomial,int> lm2Index_map_type;
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
  

  int easyProductCriterions;
  int extendedProductCrit;
  int averageLength;
  GroebnerStrategy():leadingTerms(Polynomial(0).copyDiagram()){}
  lm2Index_map_type lm2Index;
  Polynomial nextSpoly(){
    pairs.nextSpoly(generators);
  }
  
};

END_NAMESPACE_PBORIGB

#endif

