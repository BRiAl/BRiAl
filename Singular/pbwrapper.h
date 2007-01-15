#ifndef PB_SING_WRAPPER_HEADER
#define PB_SING_WRAPPER_HEADER
#include <vector>
#include "polybori.h"
#include "groebner_alg.h"
#include <mod2.h>
#include "ring.h"
#include "kbuckets.h"
#include "polys.h"
#include "ideals.h"

USING_NAMESPACE_PBORI
USING_NAMESPACE_PBORIGB
Polynomial sing2pb(poly p){
  int nvariables=rVar(currRing);
  Polynomial res;
  while(p!=NULL){
    int i;
    Monomial m;
    for(i=1;i<=nvariables;i++){
      int exponent=pGetExp(p,i);
      if (exponent>0) m=m*Variable(i-1);
    }
    res=res+m;
    pIter(p);
  }
  return res;
}
poly pb2sing(Polynomial p){
  Polynomial::exp_iterator it=p.expBegin();
  Polynomial::exp_iterator end=p.expEnd();
  kBucket_pt bucket=kBucketCreate();
  number one=nInit(1);
  while(it!=end){
    Exponent e=*it;
    poly m=pInit();
    int i;
    Exponent::const_iterator itit=e.begin();
    Exponent::const_iterator itend=e.end();
    
    while(itit!=itend){
      i=*itit;
      pSetExp(m,i+1,1);
      itit++;
    }
    pSetCoeff(m,nCopy(one));
    pSetm(m);
    int len=1;
    kBucket_Add_q(bucket,m,&len);
    
    it++;
  }
  nDelete(&one);
  kBucketCanonicalize(bucket);
  int dummy;
  poly res;
  kBucketClear(bucket,&res,&dummy);
  return res;
  
}
ideal do_lex_gb(ideal s){
  int nvariables=rVar(currRing);
  BoolePolyRing pbring(nvariables);
  int i_size=IDELEMS(s);
  int i;
  GroebnerStrategy strat;
  for(i=0;i<i_size;i++){
    poly p=s->m[i];
    Polynomial pb_p=sing2pb(p);
    strat.addGeneratorDelayed(pb_p);
  }
  strat.symmGB_F2();
  std::vector<Polynomial> res_pb=strat.minimalizeAndTailReduce();
  i_size=res_pb.size();
  ideal res=idInit(i_size,0);
  for(i=0;i<i_size;i++){
     res->m[i]=pb2sing(res_pb[i]);
  }
  return res;
}

#endif