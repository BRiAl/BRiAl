// -*- c++ -*-
//*****************************************************************************
/** @file SetAssociatedMinimal.h 
 *
 * @author Michael Brickenstein (original) and Alexander Dreyer (refactored)
 * @date 2012-01-31 
 *
 * This file includes the definition of the class @c SetAssociatedMinimal.
 *
 * @par Copyright:
 *   (c) 2006-2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_SetAssociatedMinimal_h_
#define polybori_groebner_SetAssociatedMinimal_h_

// include basic definitions
#include "groebner_defs.h"

#include "PolyEntryVector.h"

BEGIN_NAMESPACE_PBORIGB

/** @class SetAssociatedMinimal
 * @brief This class set the attribute @c minimal of a given index to a
 * valued selected by the template parameter @c Value.
 *
 **/
template <class KeyType, bool Value>
class SetAssociatedMinimal {
public:
  SetAssociatedMinimal(PolyEntryVector& strat): m_strat(strat) {}

  void operator()(const KeyType& key) const { 
    m_strat(key).minimal = Value;
  }

private:
  PolyEntryVector& m_strat;
};
END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_SetAssociatedMinimal_h_ */
