// -*- c++ -*-
//*****************************************************************************
/** @file PolyEntryPtrLmLess.h 
 *
 * @author Michael Brickenstein (original) and Alexander Dreyer (refactored)
 * @date 2012-01-31 
 *
 * This file includes the definition of the class @c PolyEntryPtrLmLess.
 *
 * @par Copyright:
 *   (c) 2006-2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_PolyEntryPtrLmLess_h_
#define polybori_groebner_PolyEntryPtrLmLess_h_

// include basic definitions
#include "groebner_defs.h"
#include "PolyEntry.h"

BEGIN_NAMESPACE_PBORIGB

/** @class PolyEntryPtrLmLess
 * @brief This class compares pointers to PolyEntry by their leading monomials.
 *
 **/
class PolyEntryPtrLmLess {
public:
  bool operator()(const PolyEntry* lhs, const PolyEntry* rhs) const {
    return lhs->lead < rhs->lead;
  }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_PolyEntryPtrLmLess_h_ */
