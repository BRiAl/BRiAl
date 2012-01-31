// -*- c++ -*-
//*****************************************************************************
/** @file MonomialTerms.h 
 *
 * @author Michael Brickenstein (original) and Alexander Dreyer (refactored)
 * @date 2012-01-31 
 *
 * This file includes the definition of the class @c MonomialTerms.
 *
 * @par Copyright:
 *   (c) 2006-2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_MonomialTerms_h_
#define polybori_groebner_MonomialTerms_h_

#include "TermsFacade.h"

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class MonomialTerms
 * @brief This class defines MonomialTerms.
 *
 **/

class MonomialTerms:
  public TermsFacade<MonomialTerms> {

public:
  template <class Type>
  MonomialTerms(const Type& rhs): TermsFacade<MonomialTerms>(rhs) {}

  bool isCompatible(const PolyEntry& entry) const {
    PBORI_ASSERT( (entry.length != 1) || (entry.p == entry.lead) );
    return (entry.length == 1);
  }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_MonomialTerms_h_ */
