// -*- c++ -*-
//*****************************************************************************
/** @file PSCompareByEl.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30 
 *
 * This file includes the definition of the class @c PSCompareByEl.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_PSCompareByEl_h_
#define polybori_groebner_PSCompareByEl_h_

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class PSCompareByEl
 * @brief This class defines PSCompareByEl.
 *
 **/
class PSCompareByEl{
public:
  bool operator() (const PolynomialSugar& p1, const PolynomialSugar& p2){
    return ((p1.getSugar()<p2.getSugar()) ||((p1.getSugar()<=p2.getSugar()) && (p1.eliminationLength()<p2.eliminationLength())));
  }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_PSCompareByEl_h_ */
