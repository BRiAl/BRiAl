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

  bool operator() (const Monomial& a, const Monomial& b) const{
    return operator()(strat->generators[a], strat->generators[b]);
  }

private:
  bool operator() (const PolyEntry& entry1, const PolyEntry& entry2) const {
    deg_type d1 = entry1.tailVariables.deg();
    deg_type d2 = entry2.tailVariables.deg();
    if (d1 != d2)
      return (d1 < d2);
    
    return (entry1.weightedLength < entry2.weightedLength);
  }
  
};

END_NAMESPACE_PBORIGB

#endif /* polybori_LessUsedTailVariablesThenLessWeightedLengthInStrat_h_ */
