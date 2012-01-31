// -*- c++ -*-
//*****************************************************************************
/** @file MonomialPlusOneTerms.h 
 *
 * @author Michael Brickenstein (original) and Alexander Dreyer (refactored)
 * @date 2012-01-31 
 *
 * This file includes the definition of the class @c MonomialPlusOneTerms.
 *
 * @par Copyright:
 *   (c) 2006-2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_MonomialPlusOneTerms_h_
#define polybori_groebner_MonomialPlusOneTerms_h_

#include "TermsFacade.h"
#include "PolyEntry.h"

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class MonomialPlusOneTerms
 * @brief This class defines MonomialPlusOneTerms.
 *
 **/
class MonomialPlusOneTerms:
  public TermsFacade<MonomialPlusOneTerms> {

public:
  template <class Type>
  MonomialPlusOneTerms(const Type& rhs): TermsFacade<MonomialPlusOneTerms>(rhs) {}

  bool isCompatible(const PolyEntry& entry) const { 
    PBORI_ASSERT( (entry.length != 2) || entry.p.isPair());
    return (entry.length == 2) && (entry.p.hasConstantPart());
  }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_MonomialPlusOneTerms_h_ */
