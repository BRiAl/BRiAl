// -*- c++ -*-
//*****************************************************************************
/** @file PolyPairData.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30 
 *
 * This file includes the definition of the class @c PolyPairData.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_PolyPairData_h_
#define polybori_groebner_PolyPairData_h_

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class PolyPairData
 * @brief This class defines PolyPairData.
 *
 **/
class PolyPairData: public PairData{
public:
  Polynomial p;
  Polynomial extract(const PolyEntryVector& v) const {
    return p;
  }
  PolyPairData(const BoolePolynomial& poly): p(poly) { }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_PolyPairData_h_ */
