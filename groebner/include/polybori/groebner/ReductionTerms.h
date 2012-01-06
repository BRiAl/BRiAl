// -*- c++ -*-
//*****************************************************************************
/** @file ReductionTerms.h 
 *
 * @author Alexander Dreyer
 * @date 2012-01-05
 *
 * This file includes the definition of the class @c ReductionTerms.
 *
 * @par Copyright:
 *   (c) 2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_ReductionTerms_h_
#define polybori_groebner_ReductionTerms_h_

#include "ll_red_nf.h"

// include basic definitions
#include "groebner_defs.h"


BEGIN_NAMESPACE_PBORIGB

// groebner_alg.h
MonomialSet recursively_insert(MonomialSet::navigator p,
			       idx_type idx, MonomialSet mset);

/** @class ReductionTerms
 * @brief This class defines term for @c ReductionStrategy
 *
 **/

class ReductionTerms {
  /// Type of *this
  typedef ReductionTerms self;

public:
  MonomialSet leadingTerms;
  MonomialSet minimalLeadingTerms;
  MonomialSet leadingTerms11;
  MonomialSet leadingTerms00;
  MonomialSet llReductor;
  MonomialSet monomials;
  MonomialSet monomials_plus_one;
  
  ReductionTerms(const BoolePolyRing& ring):
    leadingTerms(ring), minimalLeadingTerms(ring),
    leadingTerms11(ring), leadingTerms00(ring),
    llReductor(ring.one()), monomials(ring), monomials_plus_one(ring)  { }

protected:
  void updateLeadingTerms(const PolyEntry& entry) {
    
    const MonomialSet& terms = entry.lead.set();
    leadingTerms = leadingTerms.unite(terms);
    
    //doesn't need to be undone on simplification
    if (entry.literal_factors.is11Factorization())
      leadingTerms11 = leadingTerms11.unite(terms);
    
    if (entry.literal_factors.is00Factorization())
      leadingTerms00 = leadingTerms00.unite(terms);
  }


  void updateMonomials(const PolyEntry& entry) {
    
    if (entry.length == 1){
      PBORI_ASSERT(entry.p.length() == 1);
      monomials = monomials.unite(entry.p);
    }
  }

  MonomialSet updateMinimalLeadingTerms(const Monomial& lm) {

    MonomialSet divisors = minimalLeadingTerms.divisorsOf(lm);
    MonomialSet removed(lm.ring());
    if(divisors.isZero()) {
      removed = minimalLeadingTerms.multiplesOf(lm).diff(lm.set());
      
      PBORI_ASSERT(removed.intersect(minimalLeadingTerms).
		   intersect(lm.set()).isZero());
    
      minimalLeadingTerms = minimalLeadingTerms.diff(removed).unite(lm.set());
    }
    else if (divisors.diff(lm.set()).isZero())
      removed = lm.set();

    return removed;
  }

  Polynomial insertIntoLLReductor(const PolyEntry& entry) {

    Polynomial poly = ll_red_nf(entry.p, llReductor);
    PBORI_ASSERT(poly.lead() == entry.lead);
    
    llReductor = recursively_insert(poly.navigation().elseBranch(),
				    entry.lead.firstIndex(),
				    ll_red_nf(llReductor, poly.set()));
    return poly;
  }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_ReductionTerms_h_ */
