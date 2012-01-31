// -*- c++ -*-
//*****************************************************************************
/** @file MinimalLeadingTerms.h 
 *
 * @author Michael Brickenstein (original) and Alexander Dreyer (refactored)
 * @date 2012-01-31 
 *
 * This file includes the definition of the class @c MinimalLeadingTerms.
 *
 * @par Copyright:
 *   (c) 2006-2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_MinimalLeadingTerms_h_
#define polybori_groebner_MinimalLeadingTerms_h_

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class MinimalLeadingTerms
 * @brief This class defines MinimalLeadingTerms.
 *
 **/

class MinimalLeadingTerms:
  public MonomialSet {

  typedef MonomialSet base;
  typedef MinimalLeadingTerms self;

public:
  template <class Type>
  MinimalLeadingTerms(const Type& value): base(value) { }

  /// Insert leading term and return monomials, that are not minimal (any more)
  MonomialSet update(const Monomial& lm) {

    MonomialSet divisors(divisorsOf(lm));
    if(divisors.isZero())
      return cleanup(lm);

    return  (divisors == lm.set()? lm.ring().zero(): lm.set());
  }
  
private:
  self& operator=(const self& rhs) {
    return static_cast<self&>(static_cast<base&>(*this) = rhs);
  }

  MonomialSet cleanup(const Monomial& lm) {
    MonomialSet removed(multiplesOf(lm).diff(lm.set()));

    PBORI_ASSERT(removed.intersect(*this).intersect(lm.set()).isZero());
    PBORI_ASSERT(assertion(lm, removed.expBegin(),removed.expEnd()));

    *this = diff(removed).unite(lm.set());
    return removed;
  }
  
  bool assertion(const Monomial& lm,
                 MonomialSet::exp_iterator start,
                 MonomialSet::exp_iterator finish) const {
    while (start != finish) {
      if ( (*start) == lm.exp() || !start->reducibleBy(lm.exp()) )
        return false;
      ++start;
    }
    return true;
  }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_MinimalLeadingTerms_h_ */
