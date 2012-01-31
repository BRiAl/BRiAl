// -*- c++ -*-
//*****************************************************************************
/** @file TimesVariableSubstitution.h 
 *
 * @author Michael Brickenstein (original) and Alexander Dreyer (refactored)
 * @date 2012-01-31 
 *
 * This file includes the definition of the class @c TimesVariableSubstitution.
 *
 * @par Copyright:
 *   (c) 2006-2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_TimesVariableSubstitution_h_
#define polybori_groebner_TimesVariableSubstitution_h_

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class TimesVariableSubstitution
 * 
 * This class computes the product of a polynomial and the sum of a pair
 * of variables (namely a variable substitution).
 *
 **/

class TimesVariableSubstitution {
public:
  template <class PairType>
  Polynomial operator()(const Polynomial& lhs, const PairType& rhs) const {
    return lhs * (lhs.ring().variable(rhs.first) +
                  lhs.ring().variable(rhs.second));
  }
};


END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_TimesVariableSubstitution_h_ */
