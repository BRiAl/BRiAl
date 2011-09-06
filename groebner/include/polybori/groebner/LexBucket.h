/*
 *  pairs.h
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 19.04.06.
 *  Copyright 2006 The PolyBoRi Team. See LICENSE file.
 *
 */
#include <functional>
#include "groebner_defs.h"

#include "LiteralFactorization.h"
#include <boost/shared_ptr.hpp>
#include <queue>
#include <algorithm>
#include <utility>
#include <set>
#include <vector>

#ifndef PB_LEXBUCKETS_H
#define PB_LEXBUCKETS_H

BEGIN_NAMESPACE_PBORIGB
Polynomial without_prior_part(Polynomial p, idx_type tail_start);

class LexBucket{
 //typedef CTypes::idx_type idx_type;
public:
  static const int var_group_size=1;
  BoolePolyRing ring;
  LexBucket(const BoolePolyRing& input_ring): ring(input_ring), front(input_ring) {
    ones=false;
    updateTailStart();
  }
  LexBucket& operator+=(const Polynomial& p);
  LexBucket(const Polynomial& p): ring(p.ring()), front(p) {
    ones=false;
    if (!(p.isConstant())){
    updateTailStart();
    Polynomial back=without_prior_part(p, tail_start);
    if (!(back.isZero())){
      if (back.isOne()){
        ones=(!(ones));
      } else{
        buckets.push_back(back);
      }
    }
    front-=back;
   } else {
     updateTailStart();
     front=0;
     if (p.isOne()) ones=true;
   }
  }
  void clearFront(){
    front=0;
    while((front.isZero())&& (buckets.size()>0)){
      increaseTailStart(tail_start+var_group_size);
    }
  }
  Exponent leadExp();
  bool isZero();
  //we assume that p has smaller/equal terms than the bucket, or the bucket is zero
  void updateTailStart();
  idx_type getTailStart();
  void increaseTailStart(idx_type new_start);
  Polynomial value();
  Polynomial getFront(){
    return front;
  }
  
  bool isOne(){
    usualAssertions();
    if ((front.isZero()) && (ones) && (buckets.size()==0)) return true;
    else return false;
  }
private:
  void usualAssertions(){
      PBORI_ASSERT((buckets.size()==0)||(!(front.isZero())));
  }
  std::vector<Polynomial> buckets;
  Polynomial front;
  idx_type tail_start;
  bool ones;
  
};

 END_NAMESPACE_PBORIGB

#endif
