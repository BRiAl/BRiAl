// -*- c++ -*-
//*****************************************************************************
/** @file ExpLexLess.h 
 *
 * @author Michael Brickenstein
 * @date 2011-07-01
 *
 * This file includes the definition of the class @c groebner_ExpLexLess.
 *
 * @par Copyright:
 *   (c) 2006-2011 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_ExpLexLess_h_
#define polybori_groebner_ExpLexLess_h_

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class ExpLexLess
 * @brief This class defines ExpLexLess.
 *
 **/

class ExpLexLess {
public:
  bool operator()(const Exponent&lhs, const Exponent& rhs) const {
    return LexOrder().compare(lhs, rhs)==CTypes::less_than;
  }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_ExpLexLess_h_ */
