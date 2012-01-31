// -*- c++ -*-
//*****************************************************************************
/** @file GetNthPoly.h 
 *
 * @author Michael Brickenstein (original) and Alexander Dreyer (refactored)
 * @date 2012-01-31 
 *
 * This file includes the definition of the class @c GetNthPoly.
 *
 * @par Copyright:
 *   (c) 2006-2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_GetNthPoly_h_
#define polybori_groebner_GetNthPoly_h_

// include basic definitions
#include "groebner_defs.h"

#include "ReductionStrategy.h"

BEGIN_NAMESPACE_PBORIGB

/** @class GetNthPoly
 * 
 * This class extractos the polynomial of the n-th element of a given 
 * @c ReductionStrategy.
 *
 **/
class GetNthPoly {
public:
  GetNthPoly(ReductionStrategy& strat): m_strat(strat) {}

  template <class KeyType>
  const Polynomial& operator()(const KeyType& key) {
    return m_strat[key].p;
  }

  ReductionStrategy& m_strat;
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_GetNthPoly_h_ */
