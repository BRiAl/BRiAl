// -*- c++ -*-
//*****************************************************************************
/** @file LeadingTerms.h 
 *
 * @author Michael Brickenstein (original) and Alexander Dreyer (refactored)
 * @date 2012-01-31 
 *
 * This file includes the definition of the class @c LeadingTerms.
 *
 * @par Copyright:
 *   (c) 2006-2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_LeadingTerms_h_
#define polybori_groebner_LeadingTerms_h_

#include "TermsFacade.h"
#include "PolyEntry.h"

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class LeadingTerms
 * @brief This class defines LeadingTerms.
 *
 **/
class LeadingTerms:
  public TermsFacade<LeadingTerms> {

public:
  template <class Type>
  LeadingTerms(const Type& rhs): TermsFacade<LeadingTerms>(rhs) {}

  bool isCompatible(const PolyEntry&) const { return true; }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_LeadingTerms_h_ */
