/*
 *  nf.h
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 25.04.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */
#include <algorithm>
#include "groebner_alg.h"
#ifndef PBORI_GB_NF_H
#define PBORI_GB_NF_H
BEGIN_NAMESPACE_PBORIGB
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
static int select1(GroebnerStrategy& strat, const Polynomial& p){
  MonomialSet ms=strat.leadingTerms.intersect(p.lmDivisors());
  Polynomial workaround =Polynomial(ms);
  if (ms.emptiness())
    return -1;
  else {
    Monomial min=*(std::min_element(workaround.begin(),workaround.end(), LessWeightedLengthInStrat(strat)));
    
    return strat.lm2Index[min];
  }
}
Polynomial nf1(GroebnerStrategy& strat, Polynomial p);

Polynomial nf2(GroebnerStrategy& strat, Polynomial p);

END_NAMESPACE_PBORIGB
#endif
