/*  Copyright (c) 2005-2007 by The PolyBoRi Team */

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
#include "kutil.h"
USING_NAMESPACE_PBORI
USING_NAMESPACE_PBORIGB
Polynomial sing2pb(poly p, ring r){
  int nvariables=rVar(r);
  Polynomial res;
  while(p!=NULL){
    int i;
    Monomial m;
    for(i=1;i<=nvariables;i++){
      int exponent=p_GetExp(p,i,r);
      if (exponent>0) m=m*Variable(i-1);
    }
    res=res+m;
    pIter(p);
  }
  return res;
}
poly pb2sing(Polynomial p, ring r){
  Polynomial::exp_iterator it=p.expBegin();
  Polynomial::exp_iterator end=p.expEnd();
  kBucket_pt bucket=kBucketCreate(r);
  number one=n_Init(1,r);
  while(it!=end){
    Exponent e=*it;
    poly m=p_Init(r);
    int i;
    Exponent::const_iterator itit=e.begin();
    Exponent::const_iterator itend=e.end();
    
    while(itit!=itend){
      i=*itit;
      p_SetExp(m,i+1,1,r);
      itit++;
    }
    p_SetCoeff(m,n_Copy(one,r),r);
    p_Setm(m,r);
    int len=1;
    kBucket_Add_q(bucket,m,&len);
    
    it++;
  }
  n_Delete(&one,r);
  kBucketCanonicalize(bucket);
  int dummy;
  poly res;
  kBucketClear(bucket,&res,&dummy);
  kBucketDestroy(&bucket);
  return res;
  
}
poly gen_fe(int i,ring r){
  number one=n_Init(1,r);
  poly square=p_Init(r);
  poly lin=p_Init(r);
  p_SetCoeff(square,n_Copy(one,r),r);
  p_SetCoeff(lin,n_Copy(one,r),r);
  p_SetExp(square,i,2,r);
  p_SetExp(lin,i,1,r);
  p_Setm(square,r);
  p_Setm(lin,r);
  pNext(square)=lin;
  n_Delete(&one,r);
  return square;
}

ideal do_lex_gb(ideal s, bool generate_fe=true, bool use_singular_options=true){
  int nvariables=rVar(currRing);
  BoolePolyRing pbring(nvariables);
  int i_size=IDELEMS(s);
  int i;
  
  GroebnerStrategy strat;
  for(i=0;i<i_size;i++){
    poly p=s->m[i];
    Polynomial pb_p=sing2pb(p,currRing);
    strat.addGeneratorDelayed(pb_p);
  }
  /* todo; options */
/*   if (use_singular_options){ */
/*     if (TEST_OPT_REDTAIL){ */
/*       strat.optRedTail=true; */
/*     } else */
/*     strat.optRedTail=false; */
/*     if (K_TEST_OPT_REDTHROUGH){ */
/*       strat.optLazy=true; */
/*     } else */
/*     strat.optLazy=false; */
/*   } */
  //todo: deg_bound
  strat.symmGB_F2();
  std::vector<Polynomial> res_pb;

  //  if ((use_singular_options)&&(!(TEST_OPT_REDSB)))
   res_pb=strat.minimalize();
   //  else
   //   res_pb=strat.minimalizeAndTailReduce();
  i_size=res_pb.size();
  
  bool contains_one=false;
  std::vector<int> fe_needed(nvariables);
  for(i=0;i<nvariables;i++){
    fe_needed[i]=true;
  }
  int fe_n=nvariables;
  for(i=0;i<i_size;i++){
    Polynomial p=res_pb[i];
     assume(!(p.isZero()));
     Exponent lead=p.leadExp();
     if (lead.deg()<=1){
        if (lead.deg()==0){
          contains_one=true;
        } else{
          assume(lead.deg()==1);
          fe_needed[*lead.begin()]=false;
          fe_n--;
        }
      }
  }
  if (contains_one) fe_n=0;
  if (!(generate_fe)) fe_n=0;
  ideal res=idInit(i_size+fe_n,0);
  for(i=0;i<i_size;i++){
     Polynomial p=res_pb[i];
     assume(!(p.isZero()));
     
     
     res->m[i]=pb2sing(p,currRing);
  }
  int fe_c=0;
  //todo: minimal field equations
  if ((!(contains_one))&& (generate_fe)){
    for(i=1;i<=nvariables;i++){
      if (fe_needed[i-1]){
      
        res->m[i_size+fe_c]=gen_fe(i,currRing);
        fe_c++;
      }
    }
  }
  
  return res;
}

#endif
