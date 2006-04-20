/*
 *  pairs.h
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 19.04.06.
 *  Copyright 2006 Mathematisches Forschungsinstitut Oberwolfach. All rights reserved.
 *
 */
#include <functional>
#include "groebner_defs.h"
#include <boost/shared_ptr.hpp>
#include <queue>
#include <algorithm>
#include <utility>
#ifndef PB_PAIR_H
#define PB_PAIR_H

 BEGIN_NAMESPACE_PBORIGB

class PolyEntry{
public:
  PolyEntry(const Polynomial &p);
  Polynomial p;
  Monomial lm;
  wlen_type weightedLength;
  len_type length;
  deg_type deg;
  deg_type lmDeg;
  deg_type ecart() const{
    return deg-lmDeg;
  }
};
//using std::less;
typedef std::vector<PolyEntry> PolyEntryVector;

class PairData{
public:
  //gives back encoded
  virtual ~PairData(){}
  //we don't demand that the pair is in a consistent state later
  virtual Polynomial extract(const PolyEntryVector& v);
};
class IJPairData: public PairData{
public:
  int i;
  int j;
  Polynomial extract(const PolyEntryVector& v){
    return spoly(v[i].p,v[j].p);
  }
  IJPairData(int i, int j){
    this->i=i;
    this->j=j;
  }
};
class PolyPairData: public PairData{
public:
  Polynomial p;
  Polynomial extract(const PolyEntryVector& v){
    return p;
  }
  PolyPairData(const BoolePolynomial& p){
    this->p=p;
  }
};

class VariablePairData: public PairData{
public:
  int i;
  idx_type v;
  Polynomial extract(const PolyEntryVector& gen){
    return Monomial(Variable(v))*gen[i].p;
  }
  VariablePairData(int i, idx_type v){
    this->v=v;
    this->i=i;
  }
};
typedef boost::shared_ptr<PairData> pair_data_ptr;
const int VARIABLE_PAIR=0;

class PairLS{
public:
  wlen_type wlen;
  deg_type sugar;
  //three sorts of pairs
  //x*poly, poly, i,j
  pair_data_ptr data;
  Monomial lm; //must not be the real lm, can be lm of syzygy or something else
  Polynomial extract(const PolyEntryVector& v){
    data->extract(v);
  }
  PairLS(int i, int j, const PolyEntryVector &v):
    data(new IJPairData(i,j)),
    lm(v[i].lm*v[j].lm),
    wlen(v[i].weightedLength+v[j].weightedLength-2)
  {
    sugar=lm.deg()+std::max(v[i].ecart(),v[j].ecart());
  }
  PairLS(int i, idx_type v, const PolyEntryVector &gen,int type):
    data(new VariablePairData(i,v)),sugar(gen[i].deg+1),wlen(gen[i].weightedLength),
  lm(gen[i].lm)
  
  {
    assert(type==VARIABLE_PAIR);
  }
  
  PairLS(const Polynomial& delayed):
    data(new PolyPairData(delayed)),
    lm(delayed.lead()), 
    sugar(delayed.deg()), wlen(delayed.eliminationLength()){
  }
  
};

class PairLSCompare{
public:
  ///replaces less template
  bool operator() (const PairLS& l, const PairLS& r){
    if (l.sugar!=r.sugar) return l.sugar>r.sugar; //greater sugar, less importance
    if (l.wlen!=r.wlen) return l.wlen>r.wlen;
    ///@todo lm comparison
    return false;
  }
};

typedef PairLS Pair;

 END_NAMESPACE_PBORIGB

#endif
