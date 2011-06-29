// -*- c++ -*-
//*****************************************************************************
/** @file LessCombinedManySizesInStrat.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-29 
 *
 * This file includes the definition of the class @c LessCombinedManySizesInStrat.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_LessCombinedManySizesInStrat_h_
#define polybori_groebner_LessCombinedManySizesInStrat_h_

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class LessCombinedManySizesInStrat
 * @brief This class defines LessCombinedManySizesInStrat.
 *
 **/
class LessCombinedManySizesInStrat{
public:
  GroebnerStrategy* strat;
  LessCombinedManySizesInStrat(GroebnerStrategy& strat){
    this->strat=&strat;
  }
  bool operator() (const Monomial& a , const Monomial& b){
    int i=strat->generators.lm2Index[a];
    int j=strat->generators.lm2Index[b];
        deg_type d1=strat->generators[i].tailVariables.deg();
    deg_type d2=strat->generators[j].tailVariables.deg();
    wlen_type w1=d1;
    wlen_type w2=d2;
    w1*=strat->generators[i].length;
    w1*=strat->generators[i].ecart();
    w2*=strat->generators[j].length;
    w2*=strat->generators[j].ecart();
    return w1<w2;
    
        
  }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_LessCombinedManySizesInStrat_h_ */
