// -*- c++ -*-
//*****************************************************************************
/** @file RelatedTerms.h 
 *
 * @author Michael Brickenstein (original) and Alexander Dreyer (refactored)
 * @date 2012-01-27
 *
 * This file includes the definition of the class @c RelatedTerms.
 *
 * @par Copyright:
 *   (c) 2006-2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_RelatedTerms_h_
#define polybori_groebner_RelatedTerms_h_

#include "RelatedTermsBase.h"
#include <polybori/common/TransformedSequence.h>

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class RelatedTerms
 * @brief This class finds those terms in a given set which are related to
 * given terms
 *
 *  Get interesting terms (active ones owning some of @c lead's variables)
 **/
class RelatedTerms:
  protected RelatedTermsBase {
  typedef RelatedTermsBase base;

public:
  typedef TransformedSequence<MonomialSet, BoundedDivisorsOf>
  divisors_sequence_type;

  /// Empty set of terms
  RelatedTerms(const BoolePolyRing& ring): base(ring) { }

  /// Constructor   
  RelatedTerms(const Monomial& lead, const MonomialSet& monomials,
               const MonomialSet& ignorable):
    base(lead, monomials, ignorable, monomials.diff(ignorable))  { }
  
  /// Access actual monomial set
  const MonomialSet& terms() const { return related_terms(); }

  /// Get sequence of corresponding divisors (in the given monomial set)
  divisors_sequence_type divisors(const MonomialSet& monomials) const { 
    return divisors_sequence_type(factors(monomials), divisors_of());
  };
};


END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_RelatedTerms_h_ */
