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
     return compare(strat->generators[a], strat->generators[b]);
  }
  
  bool operator() (const Exponent& a , const Exponent& b){
    return compare(strat->generators[a], strat->generators[b]);
  }

private:

  bool compare(const PolyEntry& val1, const PolyEntry& val2) const {
    if (val1.ecart() != val2.ecart())
      return (val1.ecart() < val2.ecart());

    return (val1.weightedLength < val2.weightedLength);
  }

};

END_NAMESPACE_PBORIGB

#endif /* polybori_LessEcartThenLessWeightedLengthInStrat_h_ */
