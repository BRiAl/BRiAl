/*
 *  pairs.cc
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 19.04.06.
 *  Copyright 2006 Mathematisches Forschungsinstitut Oberwolfach. All rights reserved.
 *
 */

#include "lexbuckets.h"
BEGIN_NAMESPACE_PBORIGB

static Polynomial sum_up_buckets(std::vector<Polynomial>::iterator it, const int s){
  
   if (s==0) return 0;
   if (s==1) return *it;
   if (s==2) return (*it)+(*(it++));
   return sum_up_buckets(it,s/2)+sum_up_buckets(it+s/2,s-s/2);
   
}
Polynomial without_prior_part(Polynomial p,idx_type tail_start){
  Polynomial::navigator nav=p.navigation();
  while((*nav)<tail_start){
    nav.incrementElse();
  }
  return Polynomial(nav);
}
Exponent LexBucket::leadExp(){
  usualAssertions();
  if (front.isZero()) {
    Exponent one;
    return one;
    //might be one ore zero
  } else{
    return front.leadExp();
  }
  
}
Polynomial LexBucket::value(){
  usualAssertions();
  Polynomial sum;
  Polynomial bucket_value=sum_up_buckets(buckets.begin(), buckets.size());
  sum=front+bucket_value+ones;
  
  //don't change order
  if (bucket_value.isConstant()){
    if (bucket_value.isOne()) ones=(!(ones));
    buckets.resize(0);
  } else { 
    buckets.resize(1);
    buckets[0]=bucket_value;
  }
  return sum;
}
bool LexBucket::isZero(){
  assert(!((front.isZero())&&(buckets.size()!=0)));
  return (front.isZero() &&(!(ones)));
}
//we assume that p has smaller/equal terms than the bucket, or the bucket is zero
LexBucket& LexBucket::operator+=(const Polynomial& p){
  usualAssertions();
  Polynomial pback=without_prior_part(p,tail_start);
  Polynomial pfront=p-pback;
  front+=pfront;
  if (front.isOne()){
    ones=(!(ones));
    front=0;
  }
  if (!(pback.isZero())){
    if (pback.isOne()){
      ones=(!(ones));
    } else buckets.push_back(pback);
  }
  
  while((front.isZero())&& (buckets.size()>0)){
    increaseTailStart(tail_start+var_group_size);
  }

  
  
}
void LexBucket::increaseTailStart(idx_type new_start){
  assert(new_start>=tail_start);
  tail_start=new_start;
  int i;
  std::vector<Polynomial> front_vec;
  for(i=buckets.size()-1;i>=0;i--){
    Polynomial old=buckets[i];
    buckets[i]=without_prior_part(buckets[i],new_start);
    Polynomial front_part=old-buckets[i];
    if (!(front_part.isZero())) front_vec.push_back(front_part);
    if ((buckets[i]).isConstant()){
      if (buckets[i].isOne()){
        ones=(!(ones));
      }
      buckets.erase(buckets.begin()+i);
    }
  }
  front+=sum_up_buckets(front_vec.begin(), front_vec.size());
  
}
void LexBucket::updateTailStart(){
  if (front.isConstant()) {tail_start=BoolePolyRing::nRingVariables()+1; return;}
  idx_type front_idx=(*(front.navigation()));
  tail_start=front_idx - (front_idx % var_group_size) + var_group_size;
}
idx_type LexBucket::getTailStart(){
  return tail_start;
}
END_NAMESPACE_PBORIGB
