// -*- c++ -*-
//*****************************************************************************
/** @file LMLessComparePS.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30 
 *
 * This file includes the definition of the class @c LMLessComparePS.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_LMLessComparePS_h_
#define polybori_groebner_LMLessComparePS_h_

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class LMLessComparePS
 * @brief This class defines LMLessComparePS.
 *
 **/
class LMLessComparePS{
public:
  bool operator() (const PolynomialSugar& p1, const PolynomialSugar& p2){
    return p1.lead()<p2.lead();
  }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_LMLessComparePS_h_ */
