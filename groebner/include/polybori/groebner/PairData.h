// -*- c++ -*-
//*****************************************************************************
/** @file PairData.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30 
 *
 * This file includes the definition of the class @c PairData.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_PairData_h_
#define polybori_groebner_PairData_h_

// include basic definitions

#include "groebner_defs.h"
#include "PolyEntryVector.h"

BEGIN_NAMESPACE_PBORIGB

/** @class PairData
 * @brief This class defines PairData.
 *
 **/
class PairData{
public:
  //gives back encoded
  virtual ~PairData() {}
  //we don't demand that the pair is in a consistent state later
  virtual Polynomial extract(const PolyEntryVector& v) const =0;
};

END_NAMESPACE_PBORIGB

#endif /* polybori_PairData_h_ */
