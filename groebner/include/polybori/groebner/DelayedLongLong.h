// -*- c++ -*-
//*****************************************************************************
/** @file DelayedLongLong.h 
 *
 * @author Alexander Dreyer
 * @date 2012-02-23
 *
 * This file includes the definition of the class @c DelayedLongLong.
 *
 * @par Copyright:
 *   (c) 2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_DelayedLongLong_h_
#define polybori_groebner_DelayedLongLong_h_

// include basic definitions
#include "groebner_defs.h"
#include "BitMask.h"

BEGIN_NAMESPACE_PBORIGB

/** @class DelayedLongLong
 * @brief This class unses two (unsigned) longs to represent @c long long.
 *
 **/

class DelayedLongLong:
  protected std::pair<unsigned long, unsigned long>,
  protected BitMask<sizeof(unsigned long)*4> {

public:
  typedef unsigned long long_type;

protected:
  typedef std::pair<long_type, long_type> base;

public:
  DelayedLongLong(const long_type& high, const long_type& low):
    base(high, low) {}

#ifdef PBORI_HAVE_LONG_LONG
  operator unsigned long long() const {
    return (unsigned long long(first) << (sizeof(long_type)*8)) + second;
  }
#endif
};


END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_DelayedLongLong_h_ */
