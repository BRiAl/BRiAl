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
#include "PseudoLongLong.h"
#include "BitMask.h"

BEGIN_NAMESPACE_PBORIGB

/** @class DelayedLongProduct
 * @brief This class defines a delayed product of longs and comparison with
 * @c LongLong Constant.
 *
 **/

class DelayedLongProduct:
  protected BitMask<sizeof(unsigned long)*4> {

public:
  typedef unsigned long long_type;

  DelayedLongProduct(const long_type& first, const long_type & second):
    least(low(first)*low(second)), most(high(first)*high(second)) {

    long_type mixed = high(least) + high(first)*low(second);
    most += high(mixed);
    
    mixed = low(mixed) + low(first)*high(second);
    most += high(mixed);

    least = shift(mixed) + low(least);
  }

  /// compare carry-over savely
  bool greater(long_type rhs) const {
    return (most > 0) || (least > rhs);
  }

  /// compare carry-over savely with represented by two unsigned longs
  template <long_type MaxLow>
  bool greater(const PseudoLongLong<0, MaxLow>&) const {
    return greater(MaxLow);
  }

  /// compare carry-over savely with represented by two unsigned longs
  template <long_type MaxHigh, long_type MaxLow>
  bool greater(const PseudoLongLong<MaxHigh, MaxLow>&) const {
    return (most > MaxHigh) || ( (most == MaxHigh) && (least > MaxLow) );
  }

private:
  long_type most, least;
};

template <class RhsType>
inline bool
operator> (DelayedLongProduct lhs, const RhsType& rhs) {
  return lhs.greater(rhs);
}



END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_DelayedLongProduct_h_ */
