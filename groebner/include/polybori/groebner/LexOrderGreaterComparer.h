// -*- c++ -*-
//*****************************************************************************
/** @file LexOrderGreaterComparer.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30 
 *
 * This file includes the definition of the class @c LexOrderGreaterComparer.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_LexOrderGreaterComparer_h_
#define polybori_groebner_LexOrderGreaterComparer_h_

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class LexOrderGreaterComparer
 * @brief This class defines LexOrderGreaterComparer.
 *
 **/
class LexOrderGreaterComparer{
    LexOrder o;
public:
    bool operator() (const Monomial& m1, const Monomial& m2){
        return o.compare(m1,m2)==BoolePolyRing::greater_than;
    }
    bool operator() (const Exponent& m1, const Exponent& m2){
        return o.compare(m1,m2)==BoolePolyRing::greater_than;
    }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_LexOrderGreaterComparer_h_ */
