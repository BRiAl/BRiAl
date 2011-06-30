// -*- c++ -*-
//*****************************************************************************
/** @file PolyMonomialPairComparerLexLess.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30 
 *
 * This file includes the definition of the class @c PolyMonomialPairComparerLexLess.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_PolyMonomialPairComparerLexLess_h_
#define polybori_groebner_PolyMonomialPairComparerLexLess_h_

// include basic definitions
#include "groebner_defs.h"
#include <utility> 

BEGIN_NAMESPACE_PBORIGB

/** @class PolyMonomialPairComparerLexLess
 * @brief This class defines PolyMonomialPairComparerLexLess.
 *
 **/
class PolyMonomialPairComparerLexLess{
public:
    LexOrder o;
    bool operator() (const std::pair<Polynomial,Monomial>& a,
                     const std::pair<Polynomial, Monomial>& b){
        return o.compare(a.second,b.second)==BoolePolyRing::less_than;
        
    }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_PolyMonomialPairComparerLexLess_h_ */
