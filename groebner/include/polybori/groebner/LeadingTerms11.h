// -*- c++ -*-
//*****************************************************************************
/** @file LeadingTerms11.h 
 *
 * @author Michael Brickenstein (original) and Alexander Dreyer (refactored)
 * @date 2012-01-31 
 *
 * This file includes the definition of the class @c LeadingTerms11.
 *
 * @par Copyright:
 *   (c) 2006-2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_LeadingTerms11_h_
#define polybori_groebner_LeadingTerms11_h_

#include "TermsFacade.h"
#include "PolyEntry.h"

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class LeadingTerms11
 * @brief This class defines LeadingTerms11.
 *
 **/
class LeadingTerms11:
  public TermsFacade<LeadingTerms11> {

public:
  template <class Type>
  LeadingTerms11(const Type& rhs): TermsFacade<LeadingTerms11>(rhs) {}

  bool isCompatible(const PolyEntry& entry) const {
    return entry.literal_factors.is11Factorization();
  }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_LeadingTerms11_h_ */
