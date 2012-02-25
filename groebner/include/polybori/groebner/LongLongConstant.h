// -*- c++ -*-
//*****************************************************************************
/** @file LongLongConstant.h 
 *
 * @author Alexander Dreyer
 * @date 2012-02-23
 *
 * This file includes the definition of the class @c LongLongConstant.
 *
 * @par Copyright:
 *   (c) 2012 
by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_LongLongConstant_h_
#define polybori_groebner_LongLongConstant_h_

// include basic definitions
#include "groebner_defs.h"
#include "DelayedLongLong.h"

BEGIN_NAMESPACE_PBORIGB

/** @class LongLongConstant
 * @brief This class defines LongLongConstant.
 *
 **/

template <DelayedLongLong::long_type High,
          DelayedLongLong::long_type Low>
class LongLongConstant {
public:
  typedef typename DelayedLongLong::long_type long_type;
  static const long_type first = High;
  static const long_type second = Low;

  operator DelayedLongLong() const {
    return DelayedLongLong(first, second);
  }
#ifdef PBORI_HAVE_LONG_LONG
  operator unsigned long long() const {
    return operator DelayedLongLong();
  }
#endif
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_LongLongConstant_h_ */
