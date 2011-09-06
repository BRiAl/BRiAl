/*
 *  groebner_alg.h
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 20.04.06.
 *  Copyright 2006 The PolyBoRi Team. See LICENSE file.
 *
 */

#ifndef PBORI_GB_ALG_H
#define PBORI_GB_ALG_H


#include "PairStatusSet.h"
#include "PairManager.h"
#include "MonomialHasher.h"
#include "ReductionStrategy.h"
#include "GroebnerStrategy.h"
#include "LessWeightedLengthInStrat.h"
#include "LargerDegreeComparer.h"
#include "LessWeightedLengthInStratModified.h"
#include "LessEcartThenLessWeightedLengthInStrat.h"
#include "LessUsedTailVariablesThenLessWeightedLengthInStrat.h"
#include "LessCombinedManySizesInStrat.h"

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


BEGIN_NAMESPACE_PBORIGB

#define LL_RED_FOR_GROEBNER 1
Polynomial map_every_x_to_x_plus_one(Polynomial p);

MonomialSet mod_var_set(const MonomialSet& as, const MonomialSet& vs);
void groebner(GroebnerStrategy& strat);
Polynomial reduce_by_binom(const Polynomial& p, const Polynomial& binom);
Polynomial reduce_by_monom(const Polynomial& p, const Monomial& m);
Polynomial reduce_complete(const Polynomial& p, const Polynomial& reductor);





Polynomial mult_fast_sim(const std::vector<Polynomial>& vec,
			 const BoolePolyRing& ring);
std::vector<Polynomial> full_implication_gb(const Polynomial & p,CacheManager& cache,GroebnerStrategy& strat);
Polynomial reduce_complete(const Polynomial &p, const PolyEntry& reductor, wlen_type &len);
MonomialSet recursively_insert(MonomialSet::navigator p, idx_type idx, MonomialSet mset);




inline Polynomial
cancel_monomial_in_tail(const Polynomial& p, const Monomial & m){
  Monomial lm=p.lead();
  
  Polynomial res=reduce_by_monom(p,m);
  if ((!res.isZero()) && (res.lead()==lm)){
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

inline Polynomial
reduce_by_binom(const Polynomial& p, const Polynomial& binom){
  PBORI_ASSERT(binom.length()==2);
  
  Monomial bin_lead=binom.lead();
  Monomial bin_last=*(++(binom.orderedBegin()));
  
  MonomialSet dividing_terms=((MonomialSet)p).multiplesOf(bin_lead);
  
  Monomial b_p_gcd=bin_last.GCD(bin_lead);
  
  Monomial divide_by=bin_lead/b_p_gcd;
  Monomial multiply_by=bin_last/b_p_gcd;
  
  Polynomial rewritten=((Polynomial) dividing_terms)/divide_by;
  return p-dividing_terms+rewritten*multiply_by;
  
}


inline Polynomial
reduce_by_binom_in_tail (const Polynomial& p, const Polynomial& binom){
  PBORI_ASSERT(binom.length()==2);
  Monomial lm=p.lead();
  return lm+reduce_by_binom(p-lm,binom);
}


END_NAMESPACE_PBORIGB

#endif

