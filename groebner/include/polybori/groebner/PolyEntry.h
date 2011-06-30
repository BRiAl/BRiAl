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

// include basic definitions
#include "groebner_defs.h"

#include "literal_factorization.h"

BEGIN_NAMESPACE_PBORIGB

/** @class PolyEntry
 * @brief This class defines PolyEntry.
 *
 **/
class PolyEntry{
   PolyEntry();                 /* never use this one! */
public:
  PolyEntry(const Polynomial &p);

  bool operator==(const PolyEntry& other) const{
      return p==other.p;
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
  ///set of variables with which pair was calculated
  std::set<idx_type> vPairCalculated; 
  deg_type ecart() const{
    return deg-leadDeg;
  }
  bool minimal;
  void recomputeInformation();
};

END_NAMESPACE_PBORIGB

#endif /* polybori_PolyEntry_h_ */
