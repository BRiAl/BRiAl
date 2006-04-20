/*
 *  pairs.h
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 19.04.06.
 *  Copyright 2006 Mathematisches Forschungsinstitut Oberwolfach. All rights reserved.
 *
 */
#include "groebner_defs.h"
#include <boost/shared_ptr.hpp>
 BEGIN_NAMESPACE_PBORIGB

typedef long wlen_type;

typedef int deg_type;
class PairData{
  //gives back encoded
  virtual ~PairData();
};
typedef boost::shared_ptr<PairData> pair_data_ptr;
class PairLS{
  wlen_type wlen;
  deg_type sugar;
  //three sorts of pairs
  //x*poly, poly, i,j
  pair_data_ptr data;
  Monomial lm; //must not be the real lm, can be lm of syzygy or something else
};

typedef PairLS Pair;

 END_NAMESPACE_PBORIGB