// -*- c++ -*-
//*****************************************************************************
/** @file LessWeightedLengthInStratModified.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-29 
 *
 * This file includes the definition of the class @c LessWeightedLengthInStratModified.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_LessWeightedLengthInStratModified_h_
#define polybori_groebner_LessWeightedLengthInStratModified_h_

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

inline wlen_type wlen_literal_exceptioned(const PolyEntry& e){
    wlen_type res=e.weightedLength;
    if ((e.deg==1) && (e.length<=4)){
        //if (e.length==1) return -1;
        //if (e.p.hasConstantPart()) return 0;
        return res-1;
    }
    return res;
}

/** @class LessWeightedLengthInStratModified
 * @brief This class defines LessWeightedLengthInStratModified.
 *
 **/

///@todo: in many cases, indices should be precalculated
class LessWeightedLengthInStratModified{
public:
  const ReductionStrategy* strat;
  LessWeightedLengthInStratModified(const ReductionStrategy& strat){
    this->strat=&strat;
  }
  bool operator() (const Monomial& a , const Monomial& b){
    wlen_type wa=wlen_literal_exceptioned((*strat)[a]);
    wlen_type wb=wlen_literal_exceptioned((*strat)[b]);
    
    return wa<wb;
    
  }
  bool operator() (const Exponent& a , const Exponent& b){
    wlen_type wa=wlen_literal_exceptioned((*strat)[a]);
    wlen_type wb=wlen_literal_exceptioned((*strat)[b]);
    
    return wa<wb;
    
  }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_LessWeightedLengthInStratModified_h_ */
