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
#include "NBitsUsed.h"


BEGIN_NAMESPACE_PBORIGB

/** @class DelayedLongProduct
 * @brief This class defines a delayed product of longs and comparison with
 * @c LongLong Constant.
 *
 **/

class DelayedLongProduct:
  public std::pair<unsigned long, unsigned long>,
  protected BitMask<sizeof(unsigned long)*4> {

  typedef std::pair<unsigned long, unsigned long> base;

public:
  typedef unsigned long long_type;

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
  template <long_type MaxLow>
  bool greater(const PseudoLongLong<0, MaxLow>&) const {
    return greater(MaxLow);
  }

  /// compare carry-over savely with represented by two unsigned longs
  template <long_type MaxHigh, long_type MaxLow>
  bool greater(const PseudoLongLong<MaxHigh, MaxLow>&) const {

    long_type least = low(first)*low(second);
    long_type mixed = high(least) + high(first)*low(second);
    long_type most = high(mixed) + high(first)*high(second);
    if (most > MaxHigh)
      return true;

    mixed = low(mixed) + low(first)*high(second);
    most += high(mixed);
    least = shift(mixed) + low(least);
    return (most > MaxHigh) || ( (most == MaxHigh) && (least > MaxLow) );
  }
};

template <class RhsType>
inline bool
operator> (DelayedLongProduct lhs, const RhsType& rhs) {
  return lhs.greater(rhs);
}



END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_DelayedLongProduct_h_ */
