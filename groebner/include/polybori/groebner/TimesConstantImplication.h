// -*- c++ -*-
//*****************************************************************************
/** @file TimesConstantImplication.h 
 *
 * @author Michael Brickenstein (original) and Alexander Dreyer (refactored)
 * @date 2012-01-31 
 *
 * This file includes the definition of the class @c TimesConstantImplication.
 *
 * @par Copyright:
 *   (c) 2006-2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_TimesConstantImplication_h_
#define polybori_groebner_TimesConstantImplication_h_

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class TimesConstantImplication
 *
 * This class the product of a polynomial with a simple polynomials given
 * as a pair of a variable and a Boolean constant value.
 *
 **/

class TimesConstantImplication {
public:
  template <class PairType>
  Polynomial operator()(const Polynomial& lhs, const PairType& rhs) const {
    return lhs * (lhs.ring().variable(rhs.first) + bool(rhs.second) );
  }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_TimesConstantImplication_h_ */
