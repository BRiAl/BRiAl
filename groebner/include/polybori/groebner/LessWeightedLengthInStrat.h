// -*- c++ -*-
//*****************************************************************************
/** @file LessWeightedLengthInStrat.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-29 
 *
 * This file includes the definition of the class @c LessWeightedLengthInStrat.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_LessWeightedLengthInStrat_h_
#define polybori_groebner_LessWeightedLengthInStrat_h_

// include basic definitions
#include "ReductionStrategy.h"
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class LessWeightedLengthInStrat
 * @brief This class defines LessWeightedLengthInStrat.
 *
 **/
class LessWeightedLengthInStrat{
public:
  const ReductionStrategy* strat;
  LessWeightedLengthInStrat(const ReductionStrategy& strat){
    this->strat=&strat;
  }
  bool operator() (const Monomial& a , const Monomial& b){
    return (*strat)[strat->lm2Index.find(a)->second].weightedLength<(*strat)[strat->lm2Index.find(b)->second].weightedLength;
    
  }
  bool operator() (const Exponent& a , const Exponent& b){
    return (*strat)[strat->exp2Index.find(a)->second].weightedLength<(*strat)[strat->exp2Index.find(b)->second].weightedLength;
    
  }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_LessWeightedLengthInStrat_h_ */
