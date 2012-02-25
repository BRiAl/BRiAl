// -*- c++ -*-
//*****************************************************************************
/** @file DelayedLongProduct.h 
 *
 * @author Alexander Dreyer
 * @date 2012-02-23
 *
 * This file includes the definition of the class @c DelayedLongProduct.
 *
 * @par Copyright:
 *   (c) by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_DelayedLongProduct_h_
#define polybori_groebner_DelayedLongProduct_h_

// include basic definitions
#include "groebner_defs.h"
#include "DelayedLongLong.h"
#include "LongLongConstant.h"
#include "LongProductLess.h"

BEGIN_NAMESPACE_PBORIGB

/** @class DelayedLongProduct
 * @brief This class defines a delayed product of longs and comparison with
 * @c LongLong Constant.
 *
 **/

class DelayedLongProduct:
  private DelayedLongLong {

  typedef DelayedLongLong base;
public:
  DelayedLongProduct(const long_type& high, const long_type & low):
    base(high, low) {}

  bool greater(long_type rhs) const {
    if (high(first) && high(second))
      return (first > rhs/second);

    return (first * second) > rhs;
  }

  template <long_type MaxHigh, long_type MaxLow>
  bool greater(const LongLongConstant<MaxHigh, MaxLow>&) const {
    return LongProductLess<MaxHigh, MaxLow>()(first, second);
  }
};

template <class RhsType>
inline bool
operator> (DelayedLongProduct lhs, const RhsType& rhs) {
   return lhs.greater(rhs);
}



END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_DelayedLongProduct_h_ */
