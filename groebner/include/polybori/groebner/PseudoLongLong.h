// -*- c++ -*-
//*****************************************************************************
/** @file PseudoLongLong.h 
 *
 * @author Alexander Dreyer
 * @date 2012-02-23
 *
 * This file includes the definition of the class @c PseudoLongLong.
 *
 * @par Copyright:
 *   (c) 2012 
by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_PseudoLongLong_h_
#define polybori_groebner_PseudoLongLong_h_

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class PseudoLongLong
 * @brief This class defines PseudoLongLong.
 *
 **/

template <unsigned long High, unsigned long Low>
class PseudoLongLong {
public:
  typedef unsigned long long_type;
  enum { first = High, second = Low};
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_PseudoLongLong_h_ */
