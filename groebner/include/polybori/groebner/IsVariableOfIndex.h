// -*- c++ -*-
//*****************************************************************************
/** @file IsVariableOfIndex.h 
 *
 * @author Michael Brickenstein (original) and Alexander Dreyer (refactored)
 * @date 2012-01-31 
 *
 * This file includes the definition of the class @c IsVariableOfIndex.
 *
 * @par Copyright:
 *   (c) 2006-2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_IsVariableOfIndex_h_
#define polybori_groebner_IsVariableOfIndex_h_

// include basic definitions
#include "groebner_defs.h"

#include "PolyEntry.h"

BEGIN_NAMESPACE_PBORIGB

/** @class IsVariableOfIndex
 * 
 * Check whether a @c PolyEntry is actually a variable of a given index.
 **/

class IsVariableOfIndex {
public:
  IsVariableOfIndex(idx_type value): m_value(value) {}

  bool operator()(const PolyEntry& rhs) const {
    return (rhs.isSingleton() && rhs.usedVariables.deg() == 1) && 
      (*(rhs.usedVariables.begin()) == m_value);
  }

private:
  idx_type m_value;
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_IsVariableOfIndex_h_ */
