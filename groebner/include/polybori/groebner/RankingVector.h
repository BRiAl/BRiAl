// -*- c++ -*-
//*****************************************************************************
/** @file RankingVector.h 
 *
 * @author Michael Brickenstein (original) and Alexander Dreyer (refactored)
 * @date 2012-01-31 
 *
 * This file includes the definition of the class @c RankingVector.
 *
 * @par Copyright:
 *   (c) 2006-2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_RankingVector_h_
#define polybori_groebner_RankingVector_h_

// include basic definitions
#include "groebner_defs.h"

#include <vector>

BEGIN_NAMESPACE_PBORIGB

/** @class RankingVector
 * @brief This class defines RankingVector.
 *
 **/
class RankingVector:
  public std::vector<int> {
  typedef std::vector<value_type> base; 

public:
  RankingVector(size_type len): base(len, 0) {}

  void increment(size_type idx) {  ++operator[](idx); }

  void rerank(const Exponent& exp) {
    if (exp.deg() >= 2)
      for_each(exp.begin(), exp.end(), *this, &RankingVector::increment);
  }

  value_type max_index() const {
    return (empty()? -1: std::max_element(begin(), end()) - begin());
  }

};


END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_RankingVector_h_ */
