// -*- c++ -*-
//*****************************************************************************
/** @file PairLSCompare.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30 
 *
 * This file includes the definition of the class @c PairLSCompare.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_PairLSCompare_h_
#define polybori_groebner_PairLSCompare_h_

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class PairLSCompare
 * @brief This class defines PairLSCompare.
 *
 **/
class PairLSCompare{
public:
  ///replaces less template
  bool operator() (const PairLS& l, const PairLS& r){
    if (l.sugar!=r.sugar) return l.sugar>r.sugar; //greater sugar, less importance
    if (l.wlen!=r.wlen) return l.wlen>r.wlen;
    if (l.lm!=r.lm) return l.lm>r.lm;
    
    ///@todo lm comparison
    return false;
  }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_PairLSCompare_h_ */
