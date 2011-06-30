// -*- c++ -*-
//*****************************************************************************
/** @file LMLessCompare.h 
 *
 * @author Michael Brickenstein
 * @date 2011-06-30
 *
 * This file includes the definition of the class @c groebner_LMLessCompare.
 *
 * @par Copyright:
 *   (c) by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_LMLessCompare_h_
#define polybori_groebner_LMLessCompare_h_

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class LMLessCompare
 * @brief This class defines LMLessCompare.
 *
 **/

class LMLessCompare{
public:
  bool operator() (const Polynomial& p1, const Polynomial& p2){
    return p1.lead()<p2.lead();
  }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_LMLessCompare_h_ */
