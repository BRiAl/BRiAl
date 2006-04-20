/*
 *  groebner_alg.h
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 20.04.06.
 *  Copyright 2006 Mathematisches Forschungsinstitut Oberwolfach. All rights reserved.
 *
 */



#include <polybori.h>
#include "groebner_defs.h"
#include "pairs.h"
#include <boost/dynamic_bitset.hpp>
#include <vector>
#ifndef PBORI_GB_ALG_H
#define PBORI_GB_ALG_H


BEGIN_NAMESPACE_PBORIGB



class PairStatusSet: protected std::vector<boost::dynamic_bitset<> >{
  bool calculated(int i, int j){
    if (i>j){
      int h;
      h=i;
      i=j;
      j=h;
    }
    assert(i<j);
  }
  void setToCalculated(int i, int j){
  }
};
class PairManager{
public:
  PairStatusSet status;
  std::vector<Pair> queue;
  
};
class PolyEntry{
  Polynomial p;
  Monomial lm;
  wlen_type weighted_length;
  len_type length;
  
  
};
class GroebnerStrategy{
public:
  PairManager pairs;
  std::vector<PolyEntry> generators;
  





   unsigned int reductionSteps;
  int normalForms;
  int currentDegree;
  

  int easyProductCriterions;
  int extendedProductCrit;
  int averageLength;

};

END_NAMESPACE_PBORIGB
#endif
