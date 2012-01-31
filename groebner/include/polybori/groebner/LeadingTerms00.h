// -*- c++ -*-
//*****************************************************************************
/** @file LeadingTerms00.h 
 *
 * @author Michael Brickenstein (original) and Alexander Dreyer (refactored)
 * @date 2012-01-31 
 *
 * This file includes the definition of the class @c LeadingTerms00.
 *
 * @par Copyright:
 *   (c) 2006-2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_LeadingTerms00_h_
#define polybori_groebner_LeadingTerms00_h_

#include "PolyEntry.h"
#include "TermsFacade.h"

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class LeadingTerms00
 * @brief This class defines LeadingTerms00.
 *
 **/

class LeadingTerms00:
  public TermsFacade<LeadingTerms00> {

public:
  template <class Type>
  LeadingTerms00(const Type& rhs): TermsFacade<LeadingTerms00>(rhs) {}

  bool isCompatible(const PolyEntry& entry) const {
    return entry.literal_factors.is00Factorization();
  }
};


END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_LeadingTerms00_h_ */
