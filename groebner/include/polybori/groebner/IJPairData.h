// -*- c++ -*-
//*****************************************************************************
/** @file IJPairData.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30 
 *
 * This file includes the definition of the class @c IJPairData.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_IJPairData_h_
#define polybori_groebner_IJPairData_h_

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class IJPairData
 * @brief This class defines IJPairData.
 *
 **/
class IJPairData: public PairData{
public:
  int i;
  int j;
  Polynomial extract(const PolyEntryVector& v) const {
    return spoly(v[i].p,v[j].p);
  }
  IJPairData(int i, int j){
    this->i=i;
    this->j=j;
  }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_IJPairData_h_ */
