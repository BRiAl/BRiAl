// -*- c++ -*-
//*****************************************************************************
/** @file ChainVariableCriterion.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30 
 *
 * This file includes the definition of the class @c ChainVariableCriterion.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_ChainVariableCriterion_h_
#define polybori_groebner_ChainVariableCriterion_h_

// include basic definitions
#include "groebner_defs.h"
#include "GroebnerStrategy.h"

BEGIN_NAMESPACE_PBORIGB

/** @class ChainVariableCriterion
 * @brief This class defines ChainVariableCriterion.
 *
 **/
class ChainVariableCriterion{
  ///connect via pairs
public:
  const GroebnerStrategy* strat;
  int i;
  idx_type v;
  ChainVariableCriterion(const GroebnerStrategy& strat, int i, idx_type v){
    this->strat=&strat;
    this->i=i;
    this->v=v;
  }
  bool operator() (const Exponent& lmExp){
    int index=strat->generators.index(lmExp);
    //we know such an entry exists
    if (index!=i){
      //would be still true for i, but how should that happen
      if ((strat->pairs.status.hasTRep(i,index)) &&(strat->generators[index].vPairCalculated.count(v)==1))
        return true;
    }
   return false;
  }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_ChainVariableCriterion_h_ */
