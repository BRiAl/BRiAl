// -*- c++ -*-
//*****************************************************************************
/** @file ReductionTerms.h 
 *
 * @author Michael Brickenstein (original) and Alexander Dreyer (refactored)
 * @date 2012-01-05
 *
 * This file includes the definition of the class @c ReductionTerms.
 *
 * @par Copyright:
 *   (c) 2006-2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_ReductionTerms_h_
#define polybori_groebner_ReductionTerms_h_

#include "BoundedDivisorsOf.h"
#include "RelatedTerms.h"
#include "LLReductor.h"
#include "MinimalLeadingTerms.h"
#include "MonomialTerms.h"
#include "LeadingTerms.h"
#include "LeadingTerms00.h"
#include "LeadingTerms11.h"
#include "MonomialPlusOneTerms.h"

#include <polybori/routines/pbori_algo.h> // which


// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB


// nf
MonomialSet mod_mon_set(const MonomialSet& as, const MonomialSet &vs);

/** @class ReductionTerms
 * @brief This class defines term for @c ReductionStrategy
 *
 **/

class ReductionTerms {
  /// Type of *this
  typedef ReductionTerms self;

public:
  LeadingTerms leadingTerms;
  MinimalLeadingTerms minimalLeadingTerms;
  LeadingTerms11 leadingTerms11;
  LeadingTerms00 leadingTerms00;
  LLReductor llReductor;
  MonomialTerms monomials;
  MonomialPlusOneTerms monomials_plus_one;
  
  /// Initialize term data for given ring
  ReductionTerms(const BoolePolyRing& ring):
    leadingTerms(ring), minimalLeadingTerms(ring),
    leadingTerms11(ring), leadingTerms00(ring),
    llReductor(ring), monomials(ring), monomials_plus_one(ring)  { }

  /// Compute terms owning variables of current entry's leading term
  /// @note Side effect: adds additional data to @c treat_pairs
  RelatedTerms related(const PolyEntry& entry) const {
    MonomialSet empty(entry.p.ring());
    bool is00 = entry.literal_factors.is00Factorization();
    bool is11 = entry.literal_factors.is11Factorization();

    if (!( (is00 && (leadingTerms == leadingTerms00)) ||
           (is11 && (leadingTerms == leadingTerms11))) ){
      PBORI_ASSERT (entry.p.isOne() || !is00 || !is11);

      return RelatedTerms(entry.lead, leadingTerms,
                          which(is11, leadingTerms11,
                                is00, leadingTerms00, empty));
    }
    return RelatedTerms(entry.p.ring());
  }

};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_ReductionTerms_h_ */
