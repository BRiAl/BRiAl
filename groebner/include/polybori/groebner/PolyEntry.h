// -*- c++ -*-
//*****************************************************************************
/** @file PolyEntry.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30 
 *
 * This file includes the definition of the class @c PolyEntry.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_PolyEntry_h_
#define polybori_groebner_PolyEntry_h_

#include "LiteralFactorization.h"

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

class PolyEntryBase {
public:
  explicit PolyEntryBase(const Polynomial &poly):
    literal_factors(poly),
    p(poly), usedVariables(poly.usedVariablesExp()),
    deg(poly.deg()), length(poly.length()), minimal(true),
    // empty/zero default values to be filled below (TODO: use inheritance here)
    lead(poly.ring()), leadExp(), leadDeg(), tail(poly.ring()), 
    weightedLength(), tailVariables(), gcdOfTerms(poly.ring()) {
    
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

/** @class PolyEntry
 * @brief This class defines PolyEntry.
 *
 **/
class PolyEntry:
  public PolyEntryBase {
  PolyEntry();                 /* never use this one! */

  typedef PolyEntry self;
  typedef PolyEntryBase base;

public:
  PolyEntry(const Polynomial &p): base(p) {}

  bool operator==(const self& other) const { return p == other.p; }

  self& operator=(const self& rhs) {
    return static_cast<self&>(base::operator=(rhs));
  }

  self& operator=(const Polynomial& rhs) {
    p = rhs;
    recomputeInformation();
    return *this;
  }

  deg_type ecart() const{ return deg-leadDeg; }

  void recomputeInformation();

  void markVariablePairsCalculated() {
    Exponent::const_iterator it(leadExp.begin()), end(leadExp.end());
    while(it != end){
      vPairCalculated.insert(*it);
      it++;
    } 
  }

  bool propagatableBy(const PolyEntry& other) const {
    return minimal && (deg <= 2) && (length > 1) && (this != &other) &&
      tailVariables.reducibleBy(other.leadExp);
  }
};





inline bool
should_propagate(const PolyEntry& e){
  return ( (e.length == 1) && (e.deg > 0) && (e.deg < 4) ) ||
    ( (e.length == 2) && (e.ecart() == 0) && (e.deg < 3) );

}

END_NAMESPACE_PBORIGB

#endif /* polybori_PolyEntry_h_ */
