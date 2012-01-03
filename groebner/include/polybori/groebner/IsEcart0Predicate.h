// -*- c++ -*-
//*****************************************************************************
/** @file IsEcart0Predicate.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30 
 *
 * This file includes the definition of the class @c IsEcart0Predicate.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_IsEcart0Predicate_h_
#define polybori_groebner_IsEcart0Predicate_h_

// include basic definitions
#include "groebner_defs.h"
#include "ReductionStrategy.h"

BEGIN_NAMESPACE_PBORIGB

/** @class IsEcart0Predicate
 * @brief This class defines IsEcart0Predicate.
 *
 **/
class IsEcart0Predicate{
public:
IsEcart0Predicate(const ReductionStrategy& strat){
    this->strat=&strat;
}
bool operator() (const Exponent& e){
    return (*strat)[e].ecart()==0;
}


private:
    const ReductionStrategy* strat;
};

END_NAMESPACE_PBORIGB

#endif /* polybori_IsEcart0Predicate_h_ */
