// -*- c++ -*-
//*****************************************************************************
/** @file PairData.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30 
 *
 * This file includes the definition of the class @c PairData.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_PairData_h_
#define polybori_groebner_PairData_h_

// include basic definitions
#include "groebner_defs.h"
#include "PolyEntry.h"

BEGIN_NAMESPACE_PBORIGB


template <class CompareType>
inline Exponent
min_exponent(const MonomialSet& ms, const CompareType& comp) {
  
  return *(std::min_element(ms.expBegin(), ms.expEnd(), comp));
}

template <>
inline Exponent
min_exponent(const MonomialSet& ms, const lex_tag&) {

  return *ms.expBegin();
}
template <class CompareType>
inline Monomial
min_monomial(const MonomialSet& ms, const CompareType& comp) {
  
  return *(std::min_element(ms.begin(), ms.end(), comp));
}

template <>
inline Monomial
min_monomial(const MonomialSet& ms, const lex_tag&) {

  return *ms.begin();
}

typedef Monomial::idx_map_type lm2Index_map_type;
typedef Exponent::idx_map_type exp2Index_map_type;

class PolyEntryVector:
  public std::vector<PolyEntry> {

public:
  PolyEntryVector():
    lm2Index(), exp2Index() {}

  template <class CompareType>
  inline int min_lm_index(const MonomialSet& ms,
			 const CompareType& comp) const {
    if (ms.isZero())
      return -1;
    
    return lm2Index.find(min_monomial(ms, comp))->second;
  }

  template <class CompareType>
  inline int min_exp_index(const MonomialSet& ms,
			   const CompareType& comp) const {
    if (ms.isZero())
      return -1;
    
    return exp2Index.find(min_exponent(ms, comp))->second;
  }

  lm2Index_map_type lm2Index;
  exp2Index_map_type exp2Index;
};

/** @class PairData
 * @brief This class defines PairData.
 *
 **/
class PairData{
public:
  //gives back encoded
  virtual ~PairData() {}
  //we don't demand that the pair is in a consistent state later
  virtual Polynomial extract(const PolyEntryVector& v)=0;
};

END_NAMESPACE_PBORIGB

#endif /* polybori_PairData_h_ */
