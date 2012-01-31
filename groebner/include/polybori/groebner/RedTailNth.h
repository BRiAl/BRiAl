// -*- c++ -*-
//*****************************************************************************
/** @file RedTailNth.h 
 *
 * @author Michael Brickenstein (original) and Alexander Dreyer (refactored)
 * @date 2012-01-31 
 *
 * This file includes the definition of the class @c RedTailNth.
 *
 * @par Copyright:
 *   (c) 2006-2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_RedTailNth_h_
#define polybori_groebner_RedTailNth_h_

// include basic definitions
#include "groebner_defs.h"

#include "PolyEntry.h"
#include "PolyEntryReference.h"
#include "ReductionStrategy.h"

BEGIN_NAMESPACE_PBORIGB

/** @class RedTailNth
 * 
 * This class applies tail reduction to the n-th element of a given 
 * @c ReductionStrategy.
 *
 **/

class RedTailNth {
public:
  RedTailNth(ReductionStrategy& strat): m_strat(strat) {}

  template <class KeyType>
  const Polynomial& operator()(const KeyType& key) {
    return get(m_strat(key)).p;
  }

private:
  const PolyEntry& get(PolyEntryReference entry) {
    return entry = red_tail(m_strat, entry.get().p);
  }

  ReductionStrategy& m_strat;
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_RedTailNth_h_ */
