// -*- c++ -*-
//*****************************************************************************
/** @file PseudoLongProduct.h 
 *
 * @author Alexander Dreyer
 * @date 2012-02-23
 *
 * This file includes the definition of the class @c PseudoLongProduct.
 *
 * @par Copyright:
 *   (c) by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_PseudoLongProduct_h_
#define polybori_groebner_PseudoLongProduct_h_

// include basic definitions
#include "groebner_defs.h"
#include "PseudoLongLong.h"
#include "BitMask.h"

BEGIN_NAMESPACE_PBORIGB

/** @class PseudoLongProduct
 * @brief This class defines a delayed product of longs and comparison with
 * @c LongLong Constant.
 *
 **/

class PseudoLongProduct:
  protected BitMask<sizeof(unsigned long)*4> {

public:
  typedef unsigned long long_type;

  PseudoLongProduct(const long_type& first, const long_type& second):
    most(high(first)*high(second)), least(low(first)*low(second)) {

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
operator> (PseudoLongProduct lhs, const RhsType& rhs) {
  return lhs.greater(rhs);
}



END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_PseudoLongProduct_h_ */
