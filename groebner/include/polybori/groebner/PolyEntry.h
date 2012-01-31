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
#include "PolyEntryBase.h"

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB


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
    vPairCalculated.insert(leadExp.begin(), leadExp.end());
  }

  bool propagatableBy(const PolyEntry& other) const {
    return minimal && (deg <= 2) && (length > 1) && (p != other.p) &&
      tailVariables.reducibleBy(other.leadExp);
  }

  bool isSingleton() const { return length == 1; }
};





inline bool
should_propagate(const PolyEntry& e){
  return ( (e.length == 1) && (e.deg > 0) && (e.deg < 4) ) ||
    ( (e.length == 2) && (e.ecart() == 0) && (e.deg < 3) );

}

END_NAMESPACE_PBORIGB

#endif /* polybori_PolyEntry_h_ */
