// -*- c++ -*-
//*****************************************************************************
/** @file LongProductLess.h 
 *
 * @author Alexander Dreyer
 * @date 2012-02-23
 *
 * This file includes the definition of the class @c LongProductLess.
 *
 * @par Copyright:
 *   (c) 2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_LongProductLess_h_
#define polybori_groebner_LongProductLess_h_

// include basic definitions
#include "groebner_defs.h"
#include "DelayedLongLong.h"

BEGIN_NAMESPACE_PBORIGB

/** @class LongProductLess
 * @brief This class compared a delayed product with a delayed (constant) sum
 * of (unsigned) long. It avoids to use long long instances.
 *
 **/

template <DelayedLongLong::long_type MaxHigh,
          DelayedLongLong::long_type MaxLow>
class LongProductLess:
  private DelayedLongLong {
  typedef DelayedLongLong base;

public:
  LongProductLess():
    base(0, 0) {}

  bool operator()(const long_type& higher, const long_type & lower) {

    return most(high(higher) * high(lower)) || 
      mid(high(higher)*low(lower)) || mid(low(higher)*high(lower)) ||
      least(low(higher)*low(lower));
  }

protected:
  bool most(const long_type& number) {
    first = number;
    second = 0;
    return (first > MaxHigh);
  }

  bool mid(const long_type& number) {
    first += high(number);

    if (first > MaxHigh)
      return true;
    second += low(number);

    first += high(second);

    if (first > MaxHigh)
      return true;

    second = low(second);
    return false;
  }

  bool least(const long_type& number) {
    return mid(high(number)) ||
      ((first == MaxHigh) && ( (shift(second) + low(number)) > MaxLow));
  }


};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_LongProductLess_h_ */
