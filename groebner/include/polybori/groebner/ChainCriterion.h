// -*- c++ -*-
//*****************************************************************************
/** @file ChainCriterion.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30 
 *
 * This file includes the definition of the class @c ChainCriterion.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_ChainCriterion_h_
#define polybori_groebner_ChainCriterion_h_

// include basic definitions
#include "groebner_defs.h"
#include "GroebnerStrategy.h"

BEGIN_NAMESPACE_PBORIGB

/** @class ChainCriterion
 * @brief This class defines ChainCriterion.
 *
 **/
class ChainCriterion{
  /// @todo: connect via vars
public:
  const GroebnerStrategy* strat;
  int i,j;
  ChainCriterion(const GroebnerStrategy& strat, int i, int j){
    this->strat=&strat;
    this->i=i;
    this->j=j;
  }
  bool operator() (const Exponent& lmExp){
    int index=strat->generators.index(lmExp);
    //we know such an entry exists
    if ((index!=i)&&(index!=j)){
      if ((strat->pairs.status.hasTRep(i,index)) && (strat->pairs.status.hasTRep(j,index))){
        
        return true;
      }
    }
    return false;
  }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_ChainCriterion_h_ */
