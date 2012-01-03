// -*- c++ -*-
//*****************************************************************************
/** @file ShorterEliminationLength.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30 
 *
 * This file includes the definition of the class @c ShorterEliminationLength.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_ShorterEliminationLength_h_
#define polybori_groebner_ShorterEliminationLength_h_

// include basic definitions
#include "groebner_defs.h"
#include "GroebnerStrategy.h"

BEGIN_NAMESPACE_PBORIGB

/** @class ShorterEliminationLength
 * @brief This class defines ShorterEliminationLength.
 *
 **/
class ShorterEliminationLength{
  public:
    const GroebnerStrategy* strat;
    wlen_type el;
    ShorterEliminationLength(const GroebnerStrategy& strat,wlen_type el){
    this->el=el;
    this->strat=&strat;
    }
    bool operator() (const Exponent& e){
        return (strat->generators[e].weightedLength<=el);
    }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_ShorterEliminationLength_h_ */
