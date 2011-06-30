// -*- c++ -*-
//*****************************************************************************
/** @file PolyMonomialPairComparerLess.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30 
 *
 * This file includes the definition of the class @c PolyMonomialPairComparerLess.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_PolyMonomialPairComparerLess_h_
#define polybori_groebner_PolyMonomialPairComparerLess_h_

// include basic definitions
#include "groebner_defs.h"
#include <utility>
BEGIN_NAMESPACE_PBORIGB

/** @class PolyMonomialPairComparerLess
 * @brief This class defines PolyMonomialPairComparerLess.
 *
 **/
class PolyMonomialPairComparerLess{
public:
  bool operator() (const std::pair<Polynomial,Monomial>& a,
                   const std::pair<Polynomial, Monomial>& b){
        return a.second < b.second;
        
    }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_PolyMonomialPairComparerLess_h_ */
