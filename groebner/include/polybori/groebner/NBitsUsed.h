// -*- c++ -*-
//*****************************************************************************
/** @file NBitsUsed.h 
 *
 * @author Alexander Dreyer 
 * @date 2012-02-28 
 *
 * This file includes the definition of the class @c NBitsUsed.
 *
 * @par Copyright:
 *   (c) 2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_NBitsUsed_h_
#define polybori_groebner_NBitsUsed_h_

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class NBitsUsed
 * @brief This class defines @c NBitsUsed whose value attribute computes the
 * actual number of used bits at compile time.
 *
 **/

template<unsigned long NValue>
class NBitsUsed {
public:
  enum { value = NBitsUsed<(NValue >> 1)>::value + 1};
};

template<>
class NBitsUsed<0> {
public:
  enum { value = 0};
};


END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_NBitsUsed_h_ */
