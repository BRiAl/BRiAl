// -*- c++ -*-
//*****************************************************************************
/** @file ShorterEliminationLengthModified.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30 
 *
 * This file includes the definition of the class @c ShorterEliminationLengthModified.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_ShorterEliminationLengthModified_h_
#define polybori_groebner_ShorterEliminationLengthModified_h_

// include basic definitions
#include "groebner_defs.h"
#include "GroebnerStrategy.h"

BEGIN_NAMESPACE_PBORIGB

/** @class ShorterEliminationLengthModified
 * @brief This class defines ShorterEliminationLengthModified.
 *
 **/
class ShorterEliminationLengthModified{
  public:
    const GroebnerStrategy* strat;
  wlen_type el;
  deg_type lm_deg;
  ShorterEliminationLengthModified(const GroebnerStrategy& strat,wlen_type el, deg_type  lm_deg){
    this->el=el;
    this->strat=&strat;
    this->lm_deg=lm_deg;
    
  }
  bool operator() (const Exponent& e){

    PBORI_ASSERT(e.deg()<=lm_deg);

    const PolyEntry* p=&strat->generators[e];
    return p->weightedLength<=el+(lm_deg-p->leadDeg)*p->length;
  }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_ShorterEliminationLengthModified_h_ */
