// -*- c++ -*-
//*****************************************************************************
/** @file LessEcartThenLessWeightedLengthInStrat.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-29 
 *
 * This file includes the definition of the class @c LessEcartThenLessWeightedLengthInStrat.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_LessEcartThenLessWeightedLengthInStrat_h_
#define polybori_groebner_LessEcartThenLessWeightedLengthInStrat_h_

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class LessEcartThenLessWeightedLengthInStrat
 * @brief This class defines LessEcartThenLessWeightedLengthInStrat.
 *
 **/
class LessEcartThenLessWeightedLengthInStrat{
public:
  const GroebnerStrategy* strat;
  LessEcartThenLessWeightedLengthInStrat(const GroebnerStrategy& strat){
    this->strat=&strat;
  }
  bool operator() (const Monomial& a , const Monomial& b){
    int i=strat->generators.lm2Index.find(a)->second;
    int j=strat->generators.lm2Index.find(b)->second;
    if (strat->generators[i].ecart()!=strat->generators[j].ecart()){
      if (strat->generators[i].ecart()<strat->generators[j].ecart())
        return true;
      else
        return false;
    }
    return (strat->generators[i].weightedLength<strat->generators[j].weightedLength);
    
  }
  
  bool operator() (const Exponent& a , const Exponent& b){
    int i=strat->generators.exp2Index.find(a)->second;
    int j=strat->generators.exp2Index.find(b)->second;
    if (strat->generators[i].ecart()!=strat->generators[j].ecart()){
      if (strat->generators[i].ecart()<strat->generators[j].ecart())
        return true;
      else
        return false;
    }
    return (strat->generators[i].weightedLength<strat->generators[j].weightedLength);
    
  }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_LessEcartThenLessWeightedLengthInStrat_h_ */
