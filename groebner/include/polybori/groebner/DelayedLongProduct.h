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
#include "NBitsUsed.h"

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

  /// compare carry-over savely
  // b != 0,  a <= c/b:  a*b <= (c/b)*b  <= (c/b)*b + (c%b) = c
  // b != 0,  a >  c/b:  Assume a*b  <= c = (c/b)*b + (c%b) ->
  //   (c/b)*b < a*b  <= (c/b)*b + (c%b)  ->  c/b < a < c/b+1 (contradicts int)
  bool greater(long_type rhs) const {
    return (second != 0) && (first > rhs/second);
  }

  /// compare carry-over savely with represented by two unsigned longs
  template <long_type MaxHigh, long_type MaxLow>
  bool greater(const LongLongConstant<MaxHigh, MaxLow>&) const {

   if (second == 0)
      return false;

    BitMask<sizeof(long_type)*8 - NBitsUsed<MaxHigh>::value > front;
    const long_type front_part = (front.shift(MaxHigh) + front.high(MaxLow));
    long_type divided  = front_part / second;

    return  (front.high(divided) == 0) && 
      (first > front.back(divided)) &&
      (first - front.back(divided) > 
       (front.back(front_part%second)+ front.low(MaxLow) ) / second);
  }
};

template <class RhsType>
inline bool
operator> (DelayedLongProduct lhs, const RhsType& rhs) {
   return lhs.greater(rhs);
}



END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_DelayedLongProduct_h_ */
