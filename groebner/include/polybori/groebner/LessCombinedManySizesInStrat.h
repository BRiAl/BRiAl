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
  const GroebnerStrategy* strat;
  LessCombinedManySizesInStrat(GroebnerStrategy& strat){
    this->strat=&strat;
  }

  bool operator() (const Monomial& a , const Monomial& b) const {
    return operator()(strat->generators[a], strat->generators[b]);
  }

private:
  bool operator() (const PolyEntry& entry1, const PolyEntry& entry2) const {
    deg_type d1 = entry1.tailVariables.deg();
    deg_type d2 = entry2.tailVariables.deg();
    wlen_type w1 = d1;
    wlen_type w2 = d2;
    w1 *= entry1.length;
    w1 *= entry1.ecart();
    w2 *= entry2.length;
    w2 *= entry2.ecart();
    return w1 < w2;       
  }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_LessCombinedManySizesInStrat_h_ */
