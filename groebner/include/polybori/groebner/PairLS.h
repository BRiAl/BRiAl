// -*- c++ -*-
//*****************************************************************************
/** @file PairLS.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30 
 *
 * This file includes the definition of the class @c PairLS.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_PairLS_h_
#define polybori_groebner_PairLS_h_

// include basic definitions
#include "groebner_defs.h"
#include <boost/shared_ptr.hpp>

BEGIN_NAMESPACE_PBORIGB

typedef boost::shared_ptr<PairData> pair_data_ptr;

enum {
  VARIABLE_PAIR,
  IJ_PAIR,
  DELAYED_PAIR
};



/** @class PairLS
 * @brief This class defines PairLS.
 *
 **/
class PairLS{
private:
  int type;
public:
  int getType() const{
    return type;
  }
  wlen_type wlen;
  deg_type sugar;
  //three sorts of pairs
  //x*poly, poly, i,j
  pair_data_ptr data;
  Monomial lm; //must not be the real lm, can be lm of syzygy or something else
  Polynomial extract(const PolyEntryVector& v){
    return data->extract(v);
  }
  PairLS(int i, int j, const PolyEntryVector &v):
    wlen(v[i].weightedLength+v[j].weightedLength-2),
    data(new IJPairData(i,j)),
    lm(v[i].lead*v[j].lead)
  {
    type=IJ_PAIR;
    sugar=lm.deg()+std::max(v[i].ecart(),v[j].ecart());
  }
  PairLS(int i, idx_type v, const PolyEntryVector &gen,int type):
     // sugar(gen[i].lmDeg+1),///@only do that because of bad criteria impl
    wlen(gen[i].weightedLength+gen[i].length),
    sugar(gen[i].deg+1),
    data(new VariablePairData(i,v)), lm(gen[i].lead) {
    PBORI_ASSERT(type==VARIABLE_PAIR);
    this->type=type;
  }
  
  PairLS(const Polynomial& delayed):
    type(DELAYED_PAIR), wlen(delayed.eliminationLength()), 
    sugar(delayed.deg()),
    data(new PolyPairData(delayed)), 
    lm(delayed.lead()) { }
  
};

END_NAMESPACE_PBORIGB

#endif /* polybori_PairLS_h_ */
