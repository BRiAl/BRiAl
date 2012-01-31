// -*- c++ -*-
//*****************************************************************************
/** @file PolyFromPolyEntry.h 
 *
 * @author Michael Brickenstein (original) and Alexander Dreyer (refactored)
 * @date 2012-01-31 
 *
 * This file includes the definition of the class @c PolyFromPolyEntry.
 *
 * @par Copyright:
 *   (c) 2006-2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_PolyFromPolyEntry_h_
#define polybori_groebner_PolyFromPolyEntry_h_

// include basic definitions
#include "groebner_defs.h"

#include "PolyEntry.h"
#include <functional>

BEGIN_NAMESPACE_PBORIGB

/** @class PolyFromPolyEntry
 * @brief This class defines an operator which extracts the Polynomial from a PolyEntry
 *
 **/

class PolyFromPolyEntry:
  public std::unary_function<PolyEntry, Polynomial> {
public:
  const Polynomial& operator()(const PolyEntry& entry) const {
    return entry.p;
  }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_PolyFromPolyEntry_h_ */
