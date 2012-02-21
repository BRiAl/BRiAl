// -*- c++ -*-
//*****************************************************************************
/** @file PolyEntryBase.h 
 *
 * @author Michael Brickenstein (original) and Alexander Dreyer (refactored)
 * @date 2012-01-31 
 *
 * This file includes the definition of the class @c PolyEntryBase.
 *
 * @par Copyright:
 *   (c) 2006-2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_PolyEntryBase_h_
#define polybori_groebner_PolyEntryBase_h_

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class PolyEntryBase
 * @brief This class defines the base of @c PolyEntry.
 *
 **/

class PolyEntryBase {
public:
  PolyEntryBase(const Polynomial &poly):
    literal_factors(poly),
    p(poly), lead(poly.ring()), weightedLength(), 
    length(poly.length()), deg(poly.deg()), 
    // empty/zero default values to be filled below (TODO: use inheritance here)
    leadDeg(), leadExp(), gcdOfTerms(poly.ring()), 
    usedVariables(poly.usedVariablesExp()),
    tailVariables(), tail(poly.ring()),  minimal(true), vPairCalculated() {
    
    lead = p.boundedLead(deg);
    leadExp = lead.exp();
    leadDeg = leadExp.deg();
    
    if (leadDeg == deg)
      weightedLength = length;
    else
      weightedLength = poly.eliminationLengthWithDegBound(deg);
    
    tail = poly-lead;
    tailVariables = tail.usedVariablesExp();
  }

  LiteralFactorization literal_factors;
  Polynomial p;
  Monomial lead;
  wlen_type weightedLength;
  len_type length;
  deg_type deg;
  deg_type leadDeg;
  Exponent leadExp;
  Monomial gcdOfTerms;
  Exponent usedVariables;
  Exponent tailVariables;
  Polynomial tail;
  bool minimal;
  ///set of variables with which pair was calculated
  std::set<idx_type> vPairCalculated; 
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_PolyEntryBase_h_ */
