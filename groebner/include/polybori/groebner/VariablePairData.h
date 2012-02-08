// -*- c++ -*-
//*****************************************************************************
/** @file VariablePairData.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30 
 *
 * This file includes the definition of the class @c VariablePairData.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_VariablePairData_h_
#define polybori_groebner_VariablePairData_h_

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class VariablePairData
 * @brief This class defines VariablePairData.
 *
 **/
class VariablePairData: public PairData{
public:
  int i;
  idx_type v;
  Polynomial extract(const PolyEntryVector& gen) const {
    return Monomial(Variable(v, gen[i].p.ring()))*gen[i].p;
  }
  VariablePairData(int i, idx_type v){
    this->v=v;
    this->i=i;
  }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_VariablePairData_h_ */
