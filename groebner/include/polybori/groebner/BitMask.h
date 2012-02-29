// -*- c++ -*-
//*****************************************************************************
/** @file BitMask.h 
 *
 * @author Alexander Dreyer
 * @date 2012-02-23
 *
 * This file includes the definition of the class @c BitMask.
 *
 * @par Copyright:
 *   (c) 2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_BitMask_h_
#define polybori_groebner_BitMask_h_

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class BitMask
 * @brief This class defines a bit mask and related operations.
 *
 **/

template <unsigned NBits>
class BitMask;

template <>
class BitMask<0> {
public:
  enum { nbits = 0, mask = (unsigned long)0 };

  unsigned long low(const unsigned long& value) const { return 0; }
  const unsigned long& high(const unsigned long& value) const { return value; }
  const unsigned long& shift(const unsigned long& value) const { return value; }
  unsigned long back(const unsigned long& value) const { return 0; }
};


template <unsigned NBits>
class BitMask {
public:
  enum { nbits = NBits, mask = ((unsigned long)(BitMask<nbits-1>::mask) << 1) | 0x1};

  unsigned long low(const unsigned long& value) const {
    return value & mask;
  }
  unsigned long high(const unsigned long& value) const {
    return value >> NBits;
  }
  unsigned long shift(const unsigned long& value) const {
    return value << NBits;
  }
  unsigned long back(const unsigned long& value) const {
    return value << (sizeof(unsigned long)*8 - NBits);
  }
};

template <>
class BitMask<sizeof(unsigned long)*8> {
public:
  enum { nbits = sizeof(unsigned long)*8,
	 mask =  ((unsigned long)(BitMask<nbits-1>::mask) << 1) | 0x1};

  const unsigned long& low(const unsigned long& value) const { return value; }
  unsigned long high(const unsigned long& value) const { return 0; }
  unsigned long shift(const unsigned long& value) const { return 0; }
  const unsigned long& back(const unsigned long& value) const { return value; }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_BitMask_h_ */
