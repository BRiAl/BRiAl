// -*- c++ -*-
//*****************************************************************************
/** @file PairECompare.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30 
 *
 * This file includes the definition of the class @c PairECompare.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_PairECompare_h_
#define polybori_groebner_PairECompare_h_

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class PairECompare
 * @brief This class defines PairECompare.
 *
 **/
class PairECompare: 
  public CFactoryBase {
public:
  PairECompare(const BoolePolyRing& ring):
    CFactoryBase(ring) {}

  ///replaces less template
  bool operator() (const PairE& l, const PairE& r){
    if (l.sugar!=r.sugar) return l.sugar>r.sugar; //greater sugar, less importance
    if (l.wlen!=r.wlen) return l.wlen>r.wlen;
    if (l.lm!=r.lm) return parent().ordering().compare(l.lm, r.lm) 
		      == CTypes::greater_than;
    
    ///@todo lm comparison
    return false;
  }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_PairECompare_h_ */
