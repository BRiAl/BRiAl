// -*- c++ -*-
//*****************************************************************************
/** @file LessUsedTailVariablesThenLessWeightedLengthInStrat.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-29 
 *
 * This file includes the definition of the class @c LessUsedTailVariablesThenLessWeightedLengthInStrat.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_LessUsedTailVariablesThenLessWeightedLengthInStrat_h_
#define polybori_groebner_LessUsedTailVariablesThenLessWeightedLengthInStrat_h_

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class LessUsedTailVariablesThenLessWeightedLengthInStrat
 * @brief This class defines LessUsedTailVariablesThenLessWeightedLengthInStrat.
 *
 **/
class LessUsedTailVariablesThenLessWeightedLengthInStrat{
public:
  const GroebnerStrategy* strat;
  LessUsedTailVariablesThenLessWeightedLengthInStrat(const GroebnerStrategy& strat){
    this->strat=&strat;
  }
  bool operator() (const Monomial& a , const Monomial& b) const{
    int i=strat->generators.lm2Index.find(a)->second;
    int j=strat->generators.lm2Index.find(b)->second;
    deg_type d1=strat->generators[i].tailVariables.deg();
    deg_type d2=strat->generators[j].tailVariables.deg();;
    if (d1!=d2){
      return (d1<d2);
          }
    return (strat->generators[i].weightedLength<strat->generators[j].weightedLength);
    
  }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_LessUsedTailVariablesThenLessWeightedLengthInStrat_h_ */
