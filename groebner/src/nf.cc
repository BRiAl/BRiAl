/*
 *  nf.cc
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 25.04.06.
 *  Copyright 2006 The PolyBoRi Team. See LICENSE file.
 *
 */

#include <polybori/groebner/PolynomialSugar.h>
#include <polybori/groebner/LMLessComparePS.h>
#include <polybori/groebner/PSCompareByEl.h>
#include <polybori/groebner/IsEcart0Predicate.h>

#include <polybori/groebner/LexHelper.h>
#include <polybori/groebner/DegOrderHelper.h>
#include <polybori/groebner/BlockOrderHelper.h>
#include <polybori/groebner/LexOrderGreaterComparer.h>
#include <polybori/groebner/LLReduction.h>
#include <polybori/groebner/ExpGreater.h>
#include <polybori/groebner/PolyMonomialPairComparerLexLess.h>
#include <polybori/groebner/PolyMonomialPairComparerLess.h>
#include <polybori/groebner/MatrixMonomialOrderTables.h>


#ifdef HAVE_GD
#define DRAW_MATRICES
#endif
#ifdef DRAW_MATRICES
#include <stdio.h>
#include <gd.h>
#endif
#include <polybori/groebner/nf.h>
#include <polybori/groebner/polynomial_properties.h>
#include <polybori/groebner/lexbuckets.h>
#include <polybori/LexOrder.h>
#include <iostream>
#include <polybori/iterators/COrderedIter.h>
#ifdef HAVE_NTL
#include <NTL/GF2.h>
#include <NTL/mat_GF2.h>
NTL_CLIENT
#endif
#ifdef HAVE_M4RI
const int M4RI_MAXKAY = 16;
#endif
using std::cout;
using std::endl;

BEGIN_NAMESPACE_PBORIGB



void drawmatrix(mzd_t* mat, const char* filename){
    #ifdef DRAW_MATRICES
    int i,r,c,j;
    c=mat->ncols;
    r=mat->nrows;
    gdImagePtr im = gdImageCreate(c, r) ;
     FILE * out = fopen(filename, "wb") ;
 int black = gdImageColorAllocate(im, 0, 0, 0) ;
 int white = gdImageColorAllocate(im, 255, 255, 255); 
 gdImageFilledRectangle(im, 0, 0, c-1, r-1, white) ;
 
 for(i=0;i<r;i++){
     for(j=0;j<c;j++){
         if (mzd_read_bit(mat, i, j))
             gdImageSetPixel(im, j, i, black );
     }
 }
  

 gdImagePng(im, out);
 gdImageDestroy(im);
 fclose(out);
 #else
 std::cerr<<"warning: for drawing matrices compile with GD";
 #endif
}

static int log2_floor(int n){
    int i;
    for(i=0;__M4RI_TWOPOW(i)<=n;i++){}
    return i-1;
}
static int optimal_k_for_multiplication(int a,int b,int c,const GroebnerStrategy& strat){
    int res=std::min(M4RI_MAXKAY,std::max(1,log2_floor(b)));
    if UNLIKELY(strat.enabledLog)
        std::cout<<"optimal k for multiplication:"<<res<<std::endl;
    return res;
}
static int optimal_k_for_gauss(int m, int n, const GroebnerStrategy& strat){
    int l=std::min(n,m);
    int res=std::min(M4RI_MAXKAY,std::max(1,log2_floor(l)+1-log2_floor(log2_floor(l))));
    if UNLIKELY(strat.enabledLog)
        std::cout<<"optimal k for gauss:"<<res<<std::endl;
    return res;
}



Polynomial nf3(const ReductionStrategy& strat, Polynomial p, Monomial rest_lead){
  int index;
  while((index=strat.select1(rest_lead))>=0){
    assert(index<strat.size());
  
    const Polynomial* g=&strat[index].p;
    
    if //((strat.generators[index].deg==1)&&(lm!=strat.generators[index].lead)){
    (((strat.optBrutalReductions) && (rest_lead!=strat[index].lead))||((strat[index].length<4) &&(strat[index].ecart()==0) 
    && (rest_lead!=strat[index].lead))){
      wlen_type dummy;
      p=reduce_complete(p,strat[index], dummy);

    } else{
      //p=spoly(p,*g);
      Exponent exp=rest_lead.exp();
      p+=(exp-strat[index].leadExp)*(*g);
    }
    if (p.isZero())
        return p;
    else
        rest_lead=p.lead();
  }
  return p;
}


Polynomial nf3_lexbuckets(const GroebnerStrategy& strat, Polynomial p, Monomial rest_lead){
  int index;
  LexBucket bucket(p);
  //cout<<"huhu";
  while((index=strat.generators.select1(rest_lead))>=0){
    assert(index<strat.generators.size());
  
    const Polynomial* g=&strat.generators[index].p;
    
    if //((strat.generators[index].deg==1)&&(lm!=strat.generators[index].lead)){
    (((strat.generators.optBrutalReductions) && (rest_lead!=strat.generators[index].lead))||((strat.generators[index].length<4) &&(strat.generators[index].ecart()==0) 
    && (rest_lead!=strat.generators[index].lead))){
      //wlen_type dummy;
      Polynomial front=bucket.getFront();
      front/=strat.generators[index].leadExp;
      front*=*g;
      bucket+=front;
      
      //p=reduce_complete(p,strat.generators[index], dummy);

    } else{
      //p=spoly(p,*g);
      Exponent exp=rest_lead.exp();
      //Monomial lm=strat.generators[index].lead
      bucket+=(exp-strat.generators[index].leadExp)*(*g);
    }
    if (bucket.isZero())
      return p.ring().zero();
    else
      rest_lead=Monomial(bucket.leadExp(), p.ring());
  }
  return bucket.value();
}

Polynomial nf3_no_deg_growth(const ReductionStrategy& strat, Polynomial p, Monomial rest_lead){
  int index;
  while((index=select_no_deg_growth(strat,rest_lead))>=0){
    assert(index<strat.size());
  
    const Polynomial* g=&strat[index].p;
    
    if //((strat.generators[index].deg==1)&&(lm!=strat.generators[index].lead)){
    (((strat.optBrutalReductions) && (rest_lead!=strat[index].lead))|| 
			((strat[index].length<4) &&(strat[index].ecart()==0) && 
			(rest_lead!=strat[index].lead))){
			wlen_type dummy;
      p=reduce_complete(p,strat[index],dummy);

    } else{
      //p=spoly(p,*g);
      Exponent exp=rest_lead.exp();
      p+=(exp-strat[index].leadExp)*(*g);
    }
    if (p.isZero())
        return p;
    else
        rest_lead=p.lead();
  }
  return p;
}
Polynomial nf3_degree_order(const ReductionStrategy& strat, Polynomial p, Monomial lead){
    int index;
    int deg=p.deg();
    //Monomial lead=p.boundedLead(deg);
    Exponent exp=lead.exp();
    while((index=strat.select1(lead))>=0){
    assert(index<strat.size());
  
    const Polynomial* g=&strat[index].p;
    
    if //((strat.generators[index].deg==1)&&(lm!=strat.generators[index].lead)){
    (((strat.optBrutalReductions) && (lead!=strat[index].lead))||
			((strat[index].length<4) &&(strat[index].ecart()==0)
 			&& (lead!=strat[index].lead)))

{     wlen_type dummy;
      #ifndef NDEBUG
      Polynomial p_old=p;
      #endif
      
      assert(deg==p.leadDeg());
      Polynomial pg=p.gradedPart(deg);
      p=p-pg+reduce_complete(pg,strat[index],dummy);
      //p=reduce_complete(p,strat.generators[index],dummy);
      #ifndef NDEBUG
      assert(p.isZero()||p.lead()<p_old.lead());
      #endif

    } else{
      p+=(exp-strat[index].leadExp)*(*g);
      //p=spoly(p,*g);
    }
    if (!(p.isZero())){
        lead=p.boundedLead(deg);
        exp=lead.exp();
        deg=exp.deg();
    } else return p;
  }
  return p;
}

Polynomial nf3_short(const ReductionStrategy& strat, Polynomial p){
  int index;
  while((index=strat.select_short(p))>=0){
    assert(index<strat.size());
    const Polynomial* g=&strat[index].p;
    
    if //((strat.generators[index].deg==1)&&(lm!=strat.generators[index].lead)){
    ((strat[index].length<4) &&(strat[index].ecart()==0) && (p.lead()!=strat[index].lead)){
      wlen_type dummy;
      p=reduce_complete(p,strat[index].p,dummy);
      
    } else{
      p=spoly(p,*g);
    }
  }
  return p;
}

const int FARE_WORSE=10;
// Polynomial nf_delaying(GroebnerStrategy& strat, Polynomial p){
//   //parameter by value, so I can modify it
//   wlen_type initial=p.eliminationLength();
//   int index;
//   bool first=true;
//   while((index=select1(strat.generators,p))>=0){
//     Polynomial* g=&strat.generators[index].p;
//     if (g->nNodes()==1){
//       idx_type v=*(g->navigation());
//       if (g->length()==1)
//       {
//         p=Polynomial(p.diagram().subset0(v));
//       } else {
//         Polynomial p2=Polynomial(p.diagram().subset1(v));
//         p=Polynomial(p.diagram().subset0(v))+p2;
//       }
//     } else {
//       if ((first==true) ||(strat.generators[index].weightedLength<= FARE_WORSE*initial))
//         p=spoly(p,*g);
//       else {
//         strat.addGeneratorDelayed(p);
//         strat.log("Delay");
//         return Polynomial(false);
//       }
//     }
//     first=false;
//   }
//   return p;
// }

static Polynomial exchange(GroebnerStrategy& strat , int i, const Polynomial & p){
  assert(p.lead()==strat.generators[i].lead);
  PolyEntry e(p);
  e.vPairCalculated=strat.generators[i].vPairCalculated;
  Polynomial res=spoly(strat.generators[i].p,p);
  strat.generators[i]=e;

  assert(p.ring().id() == strat.r.id());
  assert(e.lead.ring().id() == strat.r.id());
  assert(e.p.ring().id() == strat.r.id());


  return res;
}

static Polynomial exchange_with_promise(GroebnerStrategy& strat , int i, const Polynomial & p){
  assert(p.lead()==strat.generators[i].lead);
  assert(p.ring().id() == strat.r.id());
  //PolyEntry e(p);
  //e.vPairCalculated=strat.generators[i].vPairCalculated;
  bool minimal=strat.generators[i].minimal;
  Polynomial res=strat.generators[i].p;
  strat.generators[i].p=p;
  strat.generators[i].recomputeInformation();
  //strat.generators[i].minimal=false;
  
  if ((strat.generators[i].minimal)&&(strat.generators[i].length==2))
  //if ((strat.generators[i].length==2))
    strat.addNonTrivialImplicationsDelayed(strat.generators[i]);
  if (strat.generators[i].leadDeg==1)
    strat.propagate(strat.generators[i]);
  return res;
}

// Polynomial nf_delaying_exchanging(GroebnerStrategy& strat, Polynomial p){
//   //parameter by value, so I can modify it
//   wlen_type initial=p.eliminationLength();
//   int index;
//   bool first=true;
//   while((index=select1(strat,p))>=0){
//     Polynomial* g=&strat.generators[index].p;
//     if (g->nNodes()==1){
//       idx_type v=*(g->navigation());
//       if (g->length()==1)
//       {
//         p=Polynomial(p.diagram().subset0(v));
//       } else {
//         Polynomial p2=Polynomial(p.diagram().subset1(v));
//         p=Polynomial(p.diagram().subset0(v))+p2;
//       }
//     } else {
//       if ((p.lead()==strat.generators[index].lead) && (p.eliminationLength()<strat.generators[index].weightedLength)){
//         p=exchange(strat,index,p);
//         strat.log("Exchange");
//       } else{
//         if ((first==true) ||(strat.generators[index].weightedLength<= FARE_WORSE*initial))
//           p=spoly(p,*g);
//         else {
//           strat.addGeneratorDelayed(p);
//           strat.log("Delay");
//           return Polynomial(false);
//         }
//       }
//     }
//     first=false;
//   }
//   return p;
// }


template <> void SlimgbReduction<SLIMGB_SIMPLEST>::reduce(){
  while (!(to_reduce.empty())){
    //cout<<"looping"<<endl;
    std::vector<Polynomial> curr;
    curr.push_back(to_reduce.top());
    to_reduce.pop();
    //cout<<curr[0];
    Monomial lm=curr[0].lead();
    while ((!(to_reduce.empty())) && (to_reduce.top().lead()==lm)){
      curr.push_back(to_reduce.top());
      to_reduce.pop();
      //cout<<"same"<<endl;
      //cout.flush();
    }
    //cout<<lm;
    //cout.flush();
    int index=strat->generators.select1(lm);
    if (index>=0){
      Polynomial p_high=(lm/strat->generators[index].lead)*strat->generators[index].p;
      int i,s;
      s=curr.size();
      assert(p_high.lead()==lm);
      for(i=0;i<s;i++){
        curr[i]+=p_high;
        if (!(curr[i].isZero())){
          to_reduce.push(curr[i]);
        }
      }
    } else {
      //simly take the first, not so clever
      Polynomial reductor=curr.back();
      curr.pop_back();
      int i,s;
      s=curr.size();
      if (s>0){
        for(i=0;i<s;i++){
          curr[i]+=reductor;
          if (!(curr[i].isZero())){
            assert(curr[i].lead()<lm);
            to_reduce.push(curr[i]);
          }
          
        }
        assert(!(reductor.isZero()));
        result.push_back(reductor);
      } else{
        assert(s==0);
        assert(!(curr[0].isZero()));
        result.push_back(curr[0]);
      }
    }
  
  }
  
}





static void step_S(std::vector<PolynomialSugar>& curr, std::vector<Polynomial>& result, const BooleMonomial& lm, int index, GroebnerStrategy& strat){
  
  int s=curr.size();
  
  
  
  if ((strat.generators[index].length>2)||(lm==strat.generators[index].lead)){
    
    
    
    if //((strat.generators[index].deg==1)&&(lm!=strat.generators[index].lead)){
	  (((strat.generators.optBrutalReductions) && (lm!=strat.generators[index].lead)) ||
    ((strat.generators[index].length<4) &&(strat.generators[index].ecart()==0) 
		&& (lm!=strat.generators[index].lead))){
      //implies lmDeg==1, ecart=0
      //cout<<"REDUCE_COMPLETE\n";
      //assert(strat.generators[index].ecart()==0);
      //assert(strat.generators[index].leadDeg==1);
      //p=reduce_complete(p,strat.generators[index].p);
      
      for(int i=0;i<s;i++){
        
        //curr[i].add(p_high, deg_high);
        Polynomial to_red=curr[i].value();
        wlen_type new_len=curr[i].getLengthEstimation();
        to_red=reduce_complete(to_red,strat.generators[index],new_len);
        if (lm.ring().ordering().isTotalDegreeOrder())
            curr[i]=PolynomialSugar(to_red,curr[i].getSugar(),new_len);
        else
            curr[i]=PolynomialSugar(to_red);
      }
      
      
    }
    else{
      Polynomial p_high=(lm/strat.generators[index].lead)*strat.generators[index].p;
      
      wlen_type len_high=strat.generators[index].length;
      if (lm!=strat.generators[index].lead) len_high=p_high.length();
      if ((strat.reduceByTailReduced) && (p_high!=strat.generators[index].p)){
          p_high=red_tail(strat.generators, p_high);
          len_high=p_high.length();
        }
  deg_type deg_high=strat.generators[index].ecart()+lm.deg();
  assert(p_high.lead()==lm);
      for(int i=0;i<s;i++){
        
        curr[i].add(p_high, deg_high, len_high);
      }
    }
  } else {
    assert(strat.generators[index].length<=2);
    if (strat.generators[index].length==2){
      assert(strat.generators[index].p.length()==2);
      for(int i=0;i<s;i++){
        
        //curr[i].add(p_high, deg_high);
        Polynomial to_red=curr[i].value();
        wlen_type new_len=curr[i].getLengthEstimation();
        to_red=reduce_complete(to_red,strat.generators[index],new_len);
        //curr[i]=PolynomialSugar(to_red);
        if (lm.ring().ordering().isTotalDegreeOrder())
            curr[i]=PolynomialSugar(to_red,curr[i].getSugar(), new_len);
        else
            curr[i]=PolynomialSugar(to_red,to_red.deg(),new_len);
      }
    } else {
      ///@todo: check for sugar garanties
      assert(strat.generators[index].length==1);
      assert(strat.generators[index].p.length()==1);
    
      for(int i=0;i<s;i++){
        Polynomial to_red=curr[i].value();
        wlen_type new_len=curr[i].getLengthEstimation();
        to_red=reduce_complete(to_red,strat.generators[index],new_len);//BooleSet(to_red).diff(strat.generators[index].lead.multiples(to_red.usedVariables()));
        //curr[i]=PolynomialSugar(to_red);
        if (to_red.ring().ordering().isTotalDegreeOrder())
            curr[i]=PolynomialSugar(to_red,curr[i].getSugar(),new_len);
        else
            curr[i]=PolynomialSugar(to_red,to_red.deg(),new_len);
      }
      
    }
  }
}

static void step_S_T(std::vector<PolynomialSugar>& curr, std::vector<Polynomial>& result,  const BooleMonomial& lm, int index, GroebnerStrategy& strat){
  int s=curr.size();
  
  int found;
  wlen_type pivot_el;
  found=0;
  pivot_el=//curr[0].value().nNodes();
    curr[0].eliminationLength();
  
  for (int i=1;i<s;i++){
    wlen_type comp=//curr[i].value().nNodes();
      curr[0].eliminationLength();
    if (comp<pivot_el){
      found=i;
      pivot_el=comp;
    }
  }
  
  Polynomial pivot(strat.r);
  if (pivot_el<strat.generators[index].weightedLength){
    
    pivot_el=curr[found].eliminationLength();
    if (pivot_el<strat.generators[index].weightedLength){
        if (strat.generators.optRedTail)
            curr[found]=PolynomialSugar(red_tail(strat.generators,curr[found].value()));
        pivot_el=curr[found].eliminationLength();
    }
  }
  /*if (pivot_el<strat.generators[index].weightedLength){
      pivot=redTail(strat,curr[found].value());
      pivot_el=pivot.eliminationLength();
      curr[found]=pivot;
      }
    */  
  if (pivot_el<strat.generators[index].weightedLength){
    wlen_type pivot_len=curr[found].value().length();
    for(int i=0;i<s;i++){
      if(i==found) continue;
      curr[i].add(curr[found].value(), curr[found].getSugar(),pivot_len);
      ///@todo different prototpye
    }
    #if 1
    if ((pivot.deg()<=strat.generators[index].deg) &&(lm.deg()==strat.generators[index].leadDeg)){
      assert(lm==strat.generators[index].lead);
      assert(curr[found].getSugar()>=curr[found].value().deg());
      assert(curr[found].value().lead()==lm);
      wlen_type old_length=strat.generators[index].length;
      deg_type old_deg=strat.generators[index].deg;
      curr[found]=PolynomialSugar(exchange_with_promise(strat, index, curr[found].value()),old_deg,old_length);
      strat.log("Exchange");
    }
    #endif
    
    deg_type deg_high=strat.generators[index].ecart()+lm.deg();
    curr[found].add((lm/strat.generators[index].lead)*strat.generators[index].p, deg_high,strat.generators[index].length);
    //assert(!(curr[found].value().isZero()));
  } else 
    step_S(curr,result,lm, index,strat);
  
}


static void step_T_simple(std::vector<PolynomialSugar>& curr, std::vector<Polynomial>& result,  const BooleMonomial& lm,GroebnerStrategy& strat){
  int s=curr.size();
  Polynomial reductor(strat.r);
  int found;
  wlen_type pivot_el;
  found=0;
  pivot_el=curr[0].eliminationLength();
  int i;
  for (i=1;i<s;i++){
    wlen_type comp=curr[i].eliminationLength();
    if (comp<pivot_el){
      found=i;
      pivot_el=comp;
    }
  }
  reductor=curr[found].value();
  assert(reductor.lead()==lm);
  wlen_type length=reductor.length();//curr[found].getLengthEstimation();
  curr.erase(curr.begin()+found);
  
  s=s-1;
  
  //will calculate elimLength later so does not increase assymptotic complexity
  deg_type deg_high=reductor.deg();
  for(i=0;i<s;i++){
    //if (i==found) continue;
    assert(curr[i].lead()==lm);
    assert(curr[i].lead()==curr[i].value().lead());
    curr[i].add(reductor, deg_high,length);
    if (!(curr[i].isZero())){
      //if (!(curr[i].lead()<lm)){
    //    cout<<curr[i].lead()<<endl<<lm<<endl;
     //}
      assert(curr[i].lead()<lm);
    }
    
  }
  assert(!(reductor.isZero()));
  result.push_back(reductor);
  

}



int sum_size(const MonomialSet& s1, const MonomialSet& s2){
  MonomialSet m1=s1;
  MonomialSet m2=s2;
  Monomial lm=Polynomial(m1).lead();
  int d=lm.deg()/2;
  int i;
  Monomial::const_iterator iter=lm.begin();
  for(i=0;i<d;i++){
    assert(iter!=lm.end());
    m1=m1.subset1(*iter);
    m2=m2.subset1(*iter);
    iter++;
    
  }
  return m1.length()+m2.length()-2*m1.intersect(m2).length();
}


static void step_T_complex(std::vector<PolynomialSugar>& curr, std::vector<Polynomial>& result,  const BooleMonomial& lm,GroebnerStrategy& strat){
  std::sort(curr.begin(), curr.end(), PSCompareByEl());
  const int max_cans=5;
  int s=curr.size();
  Polynomial reductor(strat.r);
  int found;
  wlen_type pivot_el;
  
  pivot_el=curr[0].eliminationLength();
  
  int i,j;
  for (i=s-1;i>0;i--){
    int found=0;
    MonomialSet as_set(curr[i].value());
    int c_size=sum_size(as_set, MonomialSet(curr[0].value()));
    for (j=1;j<std::min(i,max_cans);j++){ 
      int size2=sum_size(as_set, MonomialSet(curr[j].value()));
      if (size2<c_size){
        found=j;
        c_size=size2;
      }
    }
    curr[i].add(curr[found].value(), curr[found].getSugar(), curr[found].getLengthEstimation());
  }
  reductor=curr[0].value();
  curr.erase(curr.begin());
  assert(!(reductor.isZero()));
  result.push_back(reductor);
  
  
}

static void step_T(std::vector<PolynomialSugar>& curr, std::vector<Polynomial>& result,  const BooleMonomial& lm,GroebnerStrategy& strat){
  int s=curr.size();

  if (s>2) return step_T_complex(curr,result, lm, strat);
  else
    step_T_complex(curr,result, lm, strat);


  
  
}


std::vector<Polynomial> parallel_reduce(std::vector<Polynomial> inp, GroebnerStrategy& strat, int average_steps, double delay_f){

  
  std::vector<Polynomial> result;
  int i,s;
  s=inp.size();
  int max_steps=average_steps*s;
  int steps=0;
  std::priority_queue<PolynomialSugar, std::vector<PolynomialSugar>, LMLessComparePS> to_reduce;
  deg_type max_sugar=0;
  Polynomial::size_type max_length=0;
  
  for(i=0;i<s;i++){
    if (inp[i].isOne()){
      result.push_back(inp[i]);
      return result;
    }
		if (inp[i].isZero()) continue;
    PolynomialSugar to_push=PolynomialSugar(inp[i]);
    //max_length=std::max(max_length,inp[i].length());
    
    max_sugar=std::max(max_sugar,to_push.getSugar());
    
    to_reduce.push(to_push);
  }
  const idx_type last_block_start=strat.r.ordering().lastBlockStart();
  while (!(to_reduce.empty())){

    std::vector<PolynomialSugar> curr;
    curr.push_back(to_reduce.top());
    to_reduce.pop();

    Monomial lm=curr[0].lead();
    #ifdef HAVE_M4RI
    if (strat.optLinearAlgebraInLastBlock){
        if (!(lm.deg()==0)){
            
            if((*(lm.begin()))>=last_block_start){
                strat.log("switching to la\n");
                std::vector<Polynomial> la;
                la.push_back(curr[0].value());
                while(!(to_reduce.empty())){
                    la.push_back(to_reduce.top().value());
                    to_reduce.pop();
                }
                la=strat.faugereStepDense(la);
                
                //result.insert(result.size(),la.begin(),la.end());
                for(i=0;i<la.size();i++){
                    assert(!(la[i].isZero()));
                    result.push_back(la[i]);
                }
                return result;
                
            }
        }
    }
    #endif 
    while ((!(to_reduce.empty())) && (to_reduce.top().lead()==lm)){
      curr.push_back(to_reduce.top());
      to_reduce.pop();

    }
 
    int index=strat.generators.select1(lm);
    if (index>=0){
      steps=steps+curr.size();
      if ((strat.optExchange) && (curr.size()>1)){
        if (strat.generators[index].leadDeg==lm.deg())
          step_S_T(curr,result,lm, index,strat);
        else
          step_S(curr,result,lm, index,strat);
      } else{
        step_S(curr,result,lm, index,strat);
      }
    } else {
      //simly take the first, not so clever
     
      int i,s;
      s=curr.size();
      if (s>1){
        steps+=curr.size()-1;
        step_T_simple(curr,result,lm,strat);
      } else{
        assert(s==1);
				if (!(curr[0].isZero()))
        	result.push_back(curr[0].value());
        curr.clear();
      }
      
    }
    
    //after each loop push back
    s=curr.size();
    for(i=0;i<s;i++){
      if (!(curr[i].isZero())){
        if (((!strat.optLazy) ||((curr[i].getSugar()<=max_sugar)
       ))||(curr[i].isOne())){
          if (curr[i].isOne()){
            result.clear();
            result.push_back(curr[i].value());
            return result;
          }
          to_reduce.push(curr[i]);
        } else {
          strat.log("Delaying");
          cout.flush();
          strat.addGeneratorDelayed(curr[i].value());
        }
      }
    }
    curr.clear();
    if ((strat.optStepBounded) &&(steps>max_steps)){
        strat.log("Too many steps\n");
        while (!(to_reduce.empty())){
            assert(!(to_reduce.top().isZero()));
            Monomial lm=to_reduce.top().lead();
            if (strat.generators.select1(lm)>=0){
                while((!(to_reduce.empty()))&&(to_reduce.top().lead()==lm)){
                    strat.addGeneratorDelayed(to_reduce.top().value());
                    to_reduce.pop();
                } 
            }else {
                std::vector<PolynomialSugar> this_lm;
                while((!(to_reduce.empty()))&&(to_reduce.top().lead()==lm)){
                    this_lm.push_back(to_reduce.top());
                    to_reduce.pop();
                }
                std::vector<PolynomialSugar>::iterator for_basis=std::min_element(this_lm.begin(),this_lm.end(),PSCompareByEl());
                int i;
                for(i=0;i<this_lm.size();i++){
                    if (this_lm[i].value()!=for_basis->value()){
                        strat.addGeneratorDelayed(this_lm[i].value());
                    }
                }
                assert(!(for_basis->value().isZero()));
                result.push_back((*for_basis).value());
            }
            
            
            
 
        }
        return result;
    }
  }
  return result;
  
}
typedef LessWeightedLengthInStratModified StratComparerForSelect;

// Doxygen problems with the following syntax, so skip during docs generation

 
int ReductionStrategy::select_short(const Polynomial& p) const{
  MonomialSet ms=leadingTerms.intersect(p.leadDivisors());
  //Polynomial workaround =Polynomial(ms);
  
  if (ms.isZero())
    return -1;
  else {
    
    //Monomial min=*(std::min_element(ms.begin(),ms.end(), LessWeightedLengthInStrat(strat)));
    Monomial min=*(std::min_element(ms.begin(),ms.end(), LessWeightedLengthInStrat(*this)));
    
    int res=lm2Index.find(min)->second;
    if (((*this)[res].weightedLength<=2)/*||(strat.generators[res].ecart()==0)*/) return res;
    else return -1;
  }
  
}

int ReductionStrategy::select_short(const Monomial& m) const{
  MonomialSet ms=leadingTerms.intersect(m.divisors());
  if (ms.isZero())
    return -1;
  else {
    //Monomial min=*(std::min_element(ms.begin(),ms.end(), LessWeightedLengthInStrat(strat)));
    Monomial min=*(std::min_element(ms.begin(),ms.end(), LessWeightedLengthInStrat(*this)));
    int res=lm2Index.find(min)->second;
    if (((*this)[res].weightedLength<=2)/*||(strat.generators[res].ecart()==0)*/) return res;
    else return -1;

  }
}

int ReductionStrategy::select1( const Polynomial& p) const{
  MonomialSet ms=leadingTerms.divisorsOf(p.lead());//strat.leadingTerms.intersect(p.leadDivisors());
  //Polynomial workaround =Polynomial(ms);
  
  if (ms.isZero())
    return -1;
  else {
#ifdef LEX_LEAD_RED_STRAT
    if (p.ring().ordering().isLexicographical()){
      Exponent min=*(ms.expBegin());
      return exp2Index.find(min)->second;
    }
#endif
    //Monomial min=*(std::min_element(ms.begin(),ms.end(), LessWeightedLengthInStrat(strat)));
    Exponent min=*(std::min_element(ms.expBegin(),ms.expEnd(), StratComparerForSelect(*this)));

    return exp2Index.find(min)->second;
     
  }
  
}
int ReductionStrategy::select1(const Monomial& m) const {
  MonomialSet ms=leadingTerms.divisorsOf(m);
  if (ms.isZero())
    return -1;
  else {
    //Monomial min=*(std::min_element(ms.begin(),ms.end(), LessWeightedLengthInStrat(strat)));
    Exponent min=*(std::min_element(ms.expBegin(),ms.expEnd(), StratComparerForSelect(*this)));
    return exp2Index.find(min)->second;
  }
}

int select_largest_degree(const ReductionStrategy& strat, const Monomial& m){
    MonomialSet ms=strat.leadingTerms.divisorsOf(m);
    if (ms.isZero())
      return -1;
    else {
      //Monomial min=*(std::min_element(ms.begin(),ms.end(), LessWeightedLengthInStrat(strat)));
      Exponent min=*(std::min_element(ms.expBegin(),ms.expEnd(), LargerDegreeComparer()));
      return strat.exp2Index.find(min)->second;
    }
}








int select_no_deg_growth(const ReductionStrategy& strat, const Monomial& m){
  MonomialSet ms=strat.leadingTerms.divisorsOf(m);
  if (ms.isZero())
    return -1;
  else {
    //Monomial min=*(std::min_element(ms.begin(),ms.end(), LessWeightedLengthInStrat(strat)));
    //Exponent min=*(std::min_element(ms.expBegin(),ms.expEnd(), StratComparerForSelect(strat)));
    int selected=-1;
    wlen_type selected_wlen=-1;
    MonomialSet::exp_iterator it=ms.expBegin();
    MonomialSet::exp_iterator end=ms.expEnd();
    while(it!=end){
        Exponent curr=*it;
        int index=strat.exp2Index.find(curr)->second;
        if (strat[index].ecart()==0){
            if (selected<0){
                selected=index;
                selected_wlen=wlen_literal_exceptioned(strat[index]);
            } else {
                if (wlen_literal_exceptioned(strat[index])<selected_wlen){
                    selected=index;
                    selected_wlen=wlen_literal_exceptioned(strat[index]);
                }
            }
        }
        
        it++;
    }
    if ((selected<0)&&(!(LexHelper::irreducible_lead(m,strat)))) std::cerr<<"select_no_Deg_growth buggy";
  return selected; 
  }
  
}

static Polynomial nf4(GroebnerStrategy& strat, Polynomial p){
  int index;
  while((index=strat.generators.select1(p))>=0){
    assert(index<strat.generators.size());
    Polynomial* g=&strat.generators[index].p;
    
    if((strat.generators[index].ecart()==0) && (strat.generators[index].length<=4) &&(strat.generators[index].lead!=p.lead())){
      wlen_type dummy;
      p=reduce_complete(p,strat.generators[index],dummy);
      
    } else{
      p=spoly(p,*g);
    }
  }
  return p;
  
}


static MonomialSet add_up_lex_sorted_monomials(const BoolePolyRing& ring,
					       std::vector<Monomial>& vec, int start, int end){
    assert(end<=vec.size());
    assert(start>=0);
    int d=end-start;
    assert(d>=0);
    if (d<=2){
        switch(d){
            case 0:return MonomialSet(ring);
            case 1:return vec[start].diagram();
            case 2: 
              return (vec[start]+vec[start+1]).diagram();
        }

    
    }
    
    //more than two monomial, lex sorted, so if first is  constant, all are constant
    if (vec[start].isOne()) return Polynomial(end-start, vec[start].ring()).diagram();
    assert (!(vec[start].isOne()));
    idx_type idx=*vec[start].begin();
    int limes=end;
    vec[start].popFirst();
    for(limes=start+1;limes<end;limes++){
        if (vec[limes].isOne()||(*vec[limes].begin()!=idx)){
            assert((vec[limes].isOne())||(*vec[limes].begin()>idx));
            break;
        } else 
           vec[limes].popFirst();
            //vec[limes].changeAssign(idx);
    }
    
    return MonomialSet(idx,add_up_lex_sorted_monomials(ring, vec,start,limes),add_up_lex_sorted_monomials(ring,vec,limes,end));
}

static MonomialSet
add_up_lex_sorted_exponents(const BoolePolyRing& ring,
                            std::vector<Exponent>& vec, int start, int end){
    assert(end<=vec.size());
    assert(start>=0);
    int d=end-start;
    assert(d>=0);
    if (d<=2){
        switch(d){
            case 0:return MonomialSet(ring);
        case 1:return Monomial(vec[start], ring).diagram();
            case 2: 
              Polynomial res=Monomial(vec[start], ring) + 
                Monomial(vec[start+1],ring);
              return MonomialSet(res.diagram());
        }

    
    }
    
    //more than two monomial, lex sorted, so if first is  constant, all are constant
    if (vec[start].deg()==0) return Polynomial(end-start, ring).diagram();
    assert (!(vec[start].deg()==0));
    idx_type idx=*vec[start].begin();
    int limes=end;
    vec[start].popFirst();
    for(limes=start+1;limes<end;limes++){
        if (UNLIKELY((vec[limes].deg()==0)||(*vec[limes].begin()!=idx))){
            assert((vec[limes].deg()==0)||(*vec[limes].begin()>idx));
            break;
        } else 
           vec[limes].popFirst();
            //vec[limes].changeAssign(idx);
    }
    
    return MonomialSet(idx, add_up_lex_sorted_exponents(ring, vec,start,limes),
                       add_up_lex_sorted_exponents(ring, vec,limes,end));
}

/// @note This function is deactivated, because it always uses the active manager!
/// @todo activate and make save, when used
#if 0
static MonomialSet add_up_lex_sorted_monomial_navs(const BoolePolyRing& ring,
  std::vector<Monomial::const_iterator>& vec, int start, int end){
    assert(end<=vec.size());
    assert(start>=0);
    int d=end-start;
    assert(d>=0);
    if (d<=2){
        switch(d){
            case 0:return MonomialSet(const BoolePolyRing& ring,);
            case 1:return MonomialSet(vec[start]);
            case 2: 
              Polynomial res=Polynomial(vec[start])+Polynomial(vec[start+1]);
              return MonomialSet(res.diagram());
        }

    
    }
    
    //more than two monomial, lex sorted, so if first is  constant, all are constant
    if (vec[start].isConstant()) return Polynomial(end-start).diagram();
    assert (!(vec[start].isConstant()));
    idx_type idx=*vec[start];
    int limes=end;
    vec[start]++;
    for(limes=start+1;limes<end;limes++){
        if (vec[limes].isConstant()||(*vec[limes]!=idx)){
            assert((vec[limes].isTerminated())||(*vec[limes]>idx));
            break;
        } else 
           vec[limes]++;
            //vec[limes].changeAssign(idx);
    }
    
    return MonomialSet(idx,add_up_lex_sorted_monomial_navs(vec,start,limes),add_up_lex_sorted_monomial_navs(vec,limes,end));
}
#endif

Polynomial add_up_monomials(const std::vector<Monomial>& vec,
			    const Polynomial& init){
  return add_up_generic(vec, init);

}
Polynomial add_up_polynomials(const std::vector<Polynomial>& vec,
			      const Polynomial& init){
  return add_up_generic(vec, init);

}
Polynomial add_up_exponents(const std::vector<Exponent>& vec,
			    const Polynomial& init){
    //return add_up_generic(vec);
    std::vector<Exponent> vec_sorted=vec;
    std::sort(vec_sorted.begin(),vec_sorted.end(),LexOrderGreaterComparer());
    
   
    return add_up_lex_sorted_exponents(init.ring(),
				       vec_sorted,0,vec_sorted.size());
}



static Polynomial unite_polynomials(const std::vector<Polynomial>& res_vec, int
start, int end, Polynomial init){
    //we assume the polynomials to be pairwise different
    int s=end-start;
    if UNLIKELY(s==0) return init;
    if (s==1) return res_vec[start];
    int h=s/2;
    return Polynomial(unite_polynomials(res_vec,start,start+h,
             init).diagram().unite(unite_polynomials(res_vec,start+h,end, 
                                                     init).diagram()));
    //return add_up_monomials(res_vec,start,start+h)+add_up_monomials(res_vec,start+h,end);
}
static Polynomial unite_polynomials(const std::vector<Polynomial>& res_vec,
                                    Polynomial init){
    //we assume the polynomials to be pairwise different
    int s=res_vec.size();
    if UNLIKELY(s==0) return init;
    if (s==1) return res_vec[0];
    int h=s/2;
    
    return Polynomial(unite_polynomials(res_vec,0,h, init).diagram().unite(unite_polynomials(res_vec,h,s,init).diagram()));
}

// static Polynomial add_up_polynomials(const std::vector<Polynomial>& res_vec, int
// start, int end, Polynomial init){
//     //we assume the polynomials to be pairwise different
//     int s=end-start;
//     if (s==0) return init;
//     if (s==1) return res_vec[start];
//     int h=s/2;
//     return add_up_polynomials(res_vec,start,start+h,
//              init)+add_up_polynomials(res_vec,start+h,end, 
//                                                      init);
//     //return add_up_monomials(res_vec,start,start+h)+add_up_monomials(res_vec,start+h,end);
// }
// static Polynomial add_up_polynomials(const std::vector<Polynomial>& res_vec,
//                                     Polynomial init){
//     //we assume the polynomials to be pairwise different
//     int s=res_vec.size();
//     if (s==0) return init;
//     if (s==1) return res_vec[0];
//     int h=s/2;
//     
//     return add_up_polynomials(res_vec,0,h, init)+add_up_polynomials(res_vec,h,s,init);
// }

#if 0
Polynomial red_tail(const GroebnerStrategy& strat, Polynomial p){
  Polynomial res;
  int deg_bound=p.deg();
  std::vector<Monomial> res_vec;
  Polynomial orig_p=p;
  bool changed=false;
  if (!(p.isZero())){
    Monomial lm=p.lead();
    res_vec.push_back(lm);
    p=Polynomial(p.diagram().diff(lm.diagram()));
  }
  while(!(p.isZero())){
    
    //res+=lm;

    
    //p-=lm;
    std::vector<Monomial> irr;
    Polynomial::const_iterator it=p.begin();
    Polynomial::const_iterator end=p.end();
    while((it!=end)&& (irreducible_lead(*it,strat))){
        irr.push_back(*it);
        it++;
    }
    if ((!(changed))&& (it==end)) return orig_p;
    Polynomial irr_p=add_up_monomials(irr, p.ring().zero());
    int s,i;
    s=irr.size();
    assert(s==irr_p.length());
    //if (s!=irr_p.length()) cout<<"ADDUP FAILED!!!!!!!!!!!!!!!!!!!!!!!!\n";
    for(i=0;i<s;i++){
        res_vec.push_back(irr[i]);
    }
    
    //p=p-irr_p;
    p=Polynomial(p.diagram().diff(irr_p.diagram()));
    if(p.isZero()) break;
    //Monomial lm=p.lead();
    //res_vec.push_back(lm);
    
    
    //p=Polynomial(p.diagram().diff(lm.diagram()));
    p=nf3(strat.generators,p);
    changed=true;
  }
  
  //should use already added irr_p's
  res=add_up_monomials(res_vec, p.ring().zero());
  return res;
}
#else
Polynomial red_tail_general(const ReductionStrategy& strat, Polynomial p){
  Polynomial res(p.ring().zero());
  int deg_bound=p.deg();
  std::vector<Polynomial> res_vec;
  Polynomial orig_p=p;
  bool changed=false;
  if LIKELY(!(p.isZero())){
    Monomial lm=p.lead();
    res_vec.push_back(lm);
    p=Polynomial(p.diagram().diff(lm.diagram()));
  }
  while(!(p.isZero())){
    
    //res+=lm;

    
    //p-=lm;
    std::vector<Monomial> irr;
    Polynomial::ordered_iterator it=p.orderedBegin();
    Polynomial::ordered_iterator end=p.orderedEnd();
    while((it!=end)&& (irreducible_lead(*it,strat))){
        irr.push_back(*it);
        it++;
    }
    Monomial rest_lead(p.ring());
    
    if UNLIKELY((!(changed))&& (it==end)) return orig_p;
    //@todo: if it==end irr_p=p, p=Polnomial(0)
    Polynomial irr_p(p.ring());
    if LIKELY(it!=end) {
      irr_p=add_up_generic(irr, p.ring().zero());
        rest_lead=*it;
        }
    else irr_p=p;
    int s,i;
    s=irr.size();
    assert(s==irr_p.length());
    //if (s!=irr_p.length()) cout<<"ADDUP FAILED!!!!!!!!!!!!!!!!!!!!!!!!\n";
    //for(i=0;i<s;i++){
    //    res_vec.push_back(irr[i]);
    //}
    res_vec.push_back(irr_p);
    //p=p-irr_p;
    p=Polynomial(p.diagram().diff(irr_p.diagram()));
    if UNLIKELY(p.isZero()) break;
    //Monomial lm=p.lead();
    //res_vec.push_back(lm);
    
    
    //p=Polynomial(p.().diff(lm.diagram()));
    if (!(p.ring().ordering().isDegreeOrder()))
        p=nf3(strat,p, rest_lead);
    else{
        p=nf3_degree_order(strat,p,rest_lead);
    }
    changed=true;
  }
  
  //should use already added irr_p's
  res=add_up_polynomials(res_vec, p.ring().zero());
  return res;
}

Polynomial cheap_reductions(const ReductionStrategy& strat, Polynomial p){
  Polynomial p_bak(p.ring());
    while(!(p.isZero())){
        p_bak=p;
      //res+=lm;
         p=mod_mon_set(p.diagram(),strat.monomials);

         p=plug_1(p,strat.monomials_plus_one);
         if (strat.optLL){
           
           p=ll_red_nf(p,strat.llReductor);
           if (p_bak!=p){
               p=mod_mon_set(p.diagram(),strat.monomials);
           }
         }
         
       if (p.isZero()) break;
       if (p_bak==p) return p;
       
   }
   return p;
}

template <class Helper> Polynomial red_tail_generic(const ReductionStrategy& strat, Polynomial p){
  Polynomial res(p.ring().zero());
  int deg_bound=p.deg();
  std::vector<Polynomial> res_vec;
  Polynomial orig_p=p;
  bool changed=false;
  if LIKELY(!(p.isZero())){
    Monomial lm=p.lead();
    res_vec.push_back(lm);
    p=Polynomial(p.diagram().diff(lm.diagram()));
  }
  while(!(p.isZero())){
  {
      Polynomial p_bak=p;
      p = cheap_reductions(strat, p);    
      if (p!=p_bak){
          changed=true;
      }
  }
  
    //p-=lm;
    std::vector<Monomial> irr;
    typename Helper::iterator_type it=Helper::begin(p);
    typename Helper::iterator_type it_orig=it;
    typename Helper::iterator_type end=Helper::end(p);
    bool rest_is_irreducible=false;
    //typedef  (typename Helper::iterator_type) it_type;
    //typedef  (typename it_type::value_type) mon_type;
    //Monomial mymon;
    if LIKELY(strat.canRewrite(p)){
        Polynomial irreducible_part=mod_mon_set(p.diagram(),strat.minimalLeadingTerms);
        if (!(irreducible_part.isZero())){
            res_vec.push_back(irreducible_part);
            Polynomial p2=p+irreducible_part;
            it=Helper::begin(p2);
            it_orig=it;
            end=Helper::end(p2);
            p=p2;
        }

        while((it!=end)&& (Helper::irreducible_lead(*it,strat))){
            if UNLIKELY(Helper::knowRestIsIrreducible(it,strat)){
                rest_is_irreducible=true;
                break;
            } else{
                irr.push_back(*it);
                it++;

            }
        }
    } else {
        rest_is_irreducible=true;
    }
    Monomial rest_lead(p.ring());
    
    if UNLIKELY((!(changed))&& (it==end)) return orig_p;
    //@todo: if it==end irr_p=p, p=Polnomial(0)
    Polynomial irr_p(p.ring());
    if LIKELY((it!=end) &&(!(rest_is_irreducible))) {
      irr_p=Helper::sum_range(irr,it_orig,it, p.ring().zero());//add_up_monomials(irr);
        rest_lead=*it;
        
        }
    else irr_p=p;
    int s,i;
    s=irr.size();

    assert((s==irr_p.length())||(rest_is_irreducible));

    res_vec.push_back(irr_p);

    p=Polynomial(p.diagram().diff(irr_p.diagram()));
    if UNLIKELY(p.isZero()) break;
    p=Helper::nf(strat,p,rest_lead);
    changed=true;
  }
  
  //should use already added irr_p's
  res=add_up_polynomials(res_vec, p.ring().zero());
  return res;
}



Polynomial red_tail(const ReductionStrategy& strat, Polynomial p){
  if (p.ring().ordering().isLexicographical())
    return red_tail_generic<LexHelper>(strat,p);
  if (p.ring().ordering().isDegreeOrder())
    return red_tail_generic<DegOrderHelper>(strat,p);
  if (p.ring().ordering().isBlockOrder())
    return red_tail_generic<BlockOrderHelper>(strat,p);
  return red_tail_general(strat,p);
}
#endif
Polynomial red_tail_short(const ReductionStrategy& strat, Polynomial p){
  Polynomial res(p.ring());
  while(!(p.isZero())){
    Polynomial lm=p.lead();
    res+=lm;
    p-=lm;
    p=nf3_short(strat,p);
  }
  return res;
}


template <bool have_redsb, bool single_call_for_noredsb, bool fast_multiplication> Polynomial 
ll_red_nf_generic(const Polynomial&, const BooleSet&);

template  <bool fast> Polynomial multiply(const Polynomial &p, const Polynomial& q){
    typedef CommutativeCacheManager<CCacheTypes::multiply_recursive>
      cache_mgr_type;

    return dd_multiply<fast>(cache_mgr_type(p.ring()), 
                             p.navigation(), q.navigation(),
                             BoolePolynomial(p.ring()));
}

template <bool have_redsb, bool single_call_for_noredsb, bool fast_multiplication>
Polynomial
ll_red_nf_generic(const Polynomial& p,MonomialSet::navigator r_nav){
  LLReduction<have_redsb, single_call_for_noredsb, fast_multiplication> 
    func(p.ring());

  return func(p, r_nav);
}

template <bool have_redsb, bool single_call_for_noredsb, bool fast_multiplication>
Polynomial
ll_red_nf_generic(const Polynomial&  p, const BooleSet& reductors) {

  return ll_red_nf_generic<have_redsb, single_call_for_noredsb,
    fast_multiplication>(p, reductors.navigation());
}

Polynomial ll_red_nf(const Polynomial& p,const BooleSet& reductors){
    return ll_red_nf_generic<true, false, false>(p,reductors);
}
Polynomial ll_red_nf_noredsb(const Polynomial& p,const BooleSet& reductors){
    return ll_red_nf_generic<false, false, false>(p,reductors);
}
Polynomial ll_red_nf_noredsb_single_recursive_call(const Polynomial& p,const BooleSet& reductors){
    return ll_red_nf_generic<false, true, false>(p,reductors);
}

Polynomial do_plug_1(const Polynomial& p, const MonomialSet& m_plus_ones){
    MonomialSet::navigator m_nav=m_plus_ones.navigation();
    
    if (m_nav.isConstant()){
        return p;
    }
    Polynomial::navigator p_nav=p.navigation();
    if UNLIKELY(p_nav.isConstant()) return p;
    idx_type p_index=*p_nav;
    while(p_index>*m_nav){
        assert(!(m_nav.isConstant()));
        m_nav.incrementElse();
    }
    assert (p_index==*p_nav);
    typedef PBORI::CacheManager<CCacheTypes::plug_1>
      cache_mgr_type;
    cache_mgr_type cache_mgr(p.ring());
    MonomialSet::navigator cached =
      cache_mgr.find(p_nav,m_nav);
    if (cached.isValid()) return cache_mgr.generate(cached);
    MonomialSet res(p.ring());
    if (p_index==*m_nav){  
      MonomialSet m1(cache_mgr.generate(m_nav.thenBranch()));
      MonomialSet m0(cache_mgr.generate(m_nav.elseBranch()));
      MonomialSet p1(cache_mgr.generate(p_nav.thenBranch()));
    MonomialSet p1_irr_s1=mod_mon_set(p1,m1);
    MonomialSet p1_red_s1=p1.diff(p1_irr_s1);
    MonomialSet p0=cache_mgr.generate(p_nav.elseBranch());
    Polynomial res0=do_plug_1(p1_red_s1,m1)+do_plug_1(p0,m0);
    Polynomial res1=do_plug_1(p1_irr_s1,m0);
    res=MonomialSet(p_index,res1.diagram(),res0.diagram());
    } else {
        assert(p_index<*m_nav);
        res=MonomialSet(p_index,do_plug_1(cache_mgr.generate(p_nav.thenBranch()),m_plus_ones).diagram(),do_plug_1(cache_mgr.generate(p_nav.elseBranch()),m_plus_ones).diagram());
    }
    cache_mgr.insert(p_nav,m_nav,res.navigation());
    
    return res;
}

Polynomial plug_1_top(const Polynomial& p, const MonomialSet& m_plus_ones){
    Polynomial  irr=mod_mon_set(p.diagram(),m_plus_ones);
    Polynomial red=p.diagram().diff(irr);
    return irr+do_plug_1(red,m_plus_ones);
}
Polynomial plug_1(const Polynomial& p, const MonomialSet& m_plus_ones){
  Polynomial p1(p),p2(plug_1_top(p1,m_plus_ones));
    while(p1!=p2){
        Polynomial h=p2;
        p2=plug_1_top(p1,m_plus_ones);
        p1=h;
    }
    return p2;
}






using std::vector;
#if  defined(HAVE_NTL) || defined(HAVE_M4RI)
using std::vector;
using std::pair;


//typedef std::map<int,Monomial> to_term_map_type;
typedef Exponent::idx_map_type from_term_map_type;



static void fix_point_iterate(const GroebnerStrategy& strat,vector<Polynomial> extendable_system, vector<Polynomial>& res1,MonomialSet& res_terms,MonomialSet& leads_from_strat){

    BoolePolyRing current_ring(res_terms.ring());
    leads_from_strat=MonomialSet(current_ring);
    res_terms=MonomialSet(current_ring);

    int i;
        for(i=0;i<extendable_system.size();i++){
            Polynomial p=extendable_system[i];
	    assert(p.ring().id() == current_ring.id());

            if UNLIKELY(p.isZero()) continue;
            
            p=cheap_reductions(strat.generators, p);
            
            // Polynomial p=mod_mon_set(p_orig.diagram(),strat.generators.monomials);
            // if (strat.generators.optLL){
            //     Polynomial p_bak2=p;
            //     p=ll_red_nf(p,strat.generators.llReductor);
            //     if (p!=p_bak2) p=mod_mon_set(p.diagram(),strat.generators.monomials);
            // }
            MonomialSet new_terms=p.diagram().diff(res_terms);
            MonomialSet::const_iterator it=new_terms.begin();
            MonomialSet::const_iterator end=new_terms.end();
            while(it!=end){
                Monomial m=*it;

                int index=select_largest_degree(strat.generators, m);
                if LIKELY(index>=0){

                        Monomial m2=m/strat.generators[index].lead;
                        Polynomial p2=m2*strat.generators[index].p;
                        extendable_system.push_back(p2);
			assert(current_ring.id() ==  strat.generators[index].lead.ring().id());
			assert(current_ring.id() ==  strat.generators[index].p.ring().id());
			assert(current_ring.id() ==  m.ring().id());
			assert(current_ring.id() ==  m2.ring().id());
			assert(current_ring.id() ==  p2.ring().id());
                }
                ++it;
            }
            res_terms=res_terms.unite(new_terms);
            res1.push_back(p);
        }
    leads_from_strat=res_terms.diff(mod_mon_set(res_terms,strat.generators.minimalLeadingTerms));
}
void fill_matrix(mzd_t* mat,vector<Polynomial> polys, from_term_map_type from_term_map){
    int i;
    for(i=0;i<polys.size();i++){
        Polynomial::exp_iterator it=polys[i].expBegin();//not order dependend
        Polynomial::exp_iterator end=polys[i].expEnd();
        while(it!=end){
            #ifndef HAVE_M4RI
            mat[i][from_term_map[*it]]=1;
            #else
            from_term_map_type::const_iterator from_it=from_term_map.find(*it);
            assert(from_it!=from_term_map.end());
            mzd_write_bit(mat,i,from_it->second,1);
            #endif
            it++;
        }
    }
}

void translate_back(vector<Polynomial>& polys, MonomialSet leads_from_strat,mzd_t* mat,const vector<int>& ring_order2lex, const vector<Exponent>& terms_as_exp,const vector<Exponent>& terms_as_exp_lex,int rank){
    int cols=mat->ncols;
    int rows=mat->nrows;
    
    int i;
    for(i=0;i<rank;i++){
        int j;
        vector<int> p_t_i;
    
        bool from_strat=false;
        for(j=0;j<cols;j++){
            #ifndef HAVE_M4RI
            if (mat[i][j]==1){
            #else
            if UNLIKELY(mzd_read_bit(mat,i,j)==1){
            #endif
                if (p_t_i.size()==0){
                    if (leads_from_strat.owns(terms_as_exp[j])) {
                        from_strat=true;break;
                    }
                }
                p_t_i.push_back(ring_order2lex[j]);
            }
        }
        if (!(from_strat)){
            vector<Exponent> p_t(p_t_i.size());
            std::sort(p_t_i.begin(),p_t_i.end(),std::less<int>());            
            for(j=0;j<p_t_i.size();j++){
                p_t[j]=terms_as_exp_lex[p_t_i[j]];
            }
            polys.push_back(add_up_lex_sorted_exponents(leads_from_strat.ring(),
							p_t,0,p_t.size()));
            assert(!(polys[polys.size()-1].isZero()));
        }
    }
}


static void linalg_step(vector<Polynomial>& polys, MonomialSet terms,MonomialSet leads_from_strat, bool log, bool optDrawMatrices=false, const char* matrixPrefix="mat"){
    if UNLIKELY(polys.size()==0) return;
 
    static int round=0;

    int rows=polys.size();
    int cols=terms.size();
    if UNLIKELY(log){
        std::cout<<"ROWS:"<<rows<<"COLUMNS:"<<cols<<std::endl;
    }
    #ifndef HAVE_M4RI
    mat_GF2 mat(INIT_SIZE,rows,cols);
    #else
    mzd_t* mat=mzd_init(rows,cols);
    #endif
    MatrixMonomialOrderTables  tabs(terms);

    fill_matrix(mat,polys,tabs.from_term_map);

    polys.clear();
    #ifndef HAVE_M4RI
    int rank=gauss(mat);
    #else
    if UNLIKELY(optDrawMatrices){
         char matname[255];
         ++round;
         snprintf(matname,255,"%s%d.png", matrixPrefix, round);
         
         drawmatrix(mat,matname);
     }
    int rank=mzd_echelonize_m4ri(mat, TRUE, 0);//optimal_k_for_gauss(mat->nrows,mat->ncols,strat));
    #endif
    if UNLIKELY(log){
        std::cout<<"finished gauss"<<std::endl;
    }
    translate_back(polys, leads_from_strat, mat,tabs.ring_order2lex, tabs.terms_as_exp,tabs.terms_as_exp_lex,rank);
    
    #ifdef HAVE_M4RI
    mzd_free(mat);
    #endif
}

static void printPackedMatrixMB(mzd_t* mat){
    int i,j;
    for(i=0;i<mat->nrows;i++){
        for(j=0;j<mat->ncols;j++){
            std::cout<<(int)mzd_read_bit(mat,i,j);
        }
        std::cout<<std::endl;
    }
}
static mzd_t* transposePackedMB(mzd_t* mat){
    return mzd_transpose(NULL,mat);
    /*mzd_t* res=mzd_init(mat->ncols,mat->nrows);
    int i,j;
    for(i=0;i<mat->nrows;i++){
        for(j=0;j<mat->ncols;j++){
            mzd_write_bit(res,j,i,mzd_read_bit(mat,i,j));
        }
    }
    return res;*/
}


static void 
linalg_step_modified(vector < Polynomial > &polys, MonomialSet terms, MonomialSet leads_from_strat, bool log, bool optDrawMatrices, const char* matrixPrefix)
{
    BoolePolyRing current_ring(terms.ring());
    assert(current_ring.id() ==  leads_from_strat.ring().id());
#ifndef NDEBUG
    vector < Polynomial >::const_iterator start(polys.begin()),
      finish(polys.end());

    while (start != finish) {
      assert(current_ring.id() == start->ring().id());
      ++start;
    }
#endif
    int unmodified_rows=polys.size();
    int unmodified_cols=terms.size();
    if UNLIKELY(((long long) unmodified_cols)*((long long) unmodified_rows)>20000000000ll){
      PBoRiError error(CTypes::matrix_size_exceeded);
      throw error;
    }
    static int round=0;
    round++;
    const int russian_k=16;
    MonomialSet     terms_unique(current_ring);
    vector < Monomial > terms_unique_vec;
    MonomialSet     terms_step1(current_ring);
    int             i;
    vector < pair < Polynomial, Monomial > >polys_lm;
    for (i = 0; i < polys.size(); i++) {
        if LIKELY(!(polys[i].isZero()))
            polys_lm.push_back(pair < Polynomial, Monomial > (polys[i], polys[i].lead()));
    }
std::  sort(polys_lm.begin(), polys_lm.end(), PolyMonomialPairComparerLess());
    polys.clear();
    
    //special cases
    if UNLIKELY(polys_lm.size() == 0)
        return;
    Monomial        last(current_ring);
    if UNLIKELY(polys_lm[0].second.deg() == 0) {
        assert(polys_lm[0].first.isOne());
        polys.resize(1, current_ring);
        polys[0] = 1;

        return;
    }
    
    vector < Polynomial > polys_triangular;
    vector < Polynomial > polys_rest;

    {
vector < pair < Polynomial, Monomial > >::iterator it = polys_lm.begin();
vector < pair < Polynomial, Monomial > >::iterator end = polys_lm.end();

        while (it != end) {
            if LIKELY(it->second != last) {
                last = it->second;
                polys_triangular.push_back(it->first);
		

        assert(std::   find(terms_unique_vec.begin(), terms_unique_vec.end(), it->second) == terms_unique_vec.end());

                terms_unique_vec.push_back(it->second);
                terms_step1=terms_step1.unite(it->first.diagram());
            } else
                polys_rest.push_back(it->first);
            it++;
        }
    }
    polys.clear();
    std::reverse(polys_triangular.begin(), polys_triangular.end());
    terms_unique = add_up_generic(terms_unique_vec, current_ring.zero());
    assert(terms_step1.diff(terms).isZero());
    assert(polys_triangular.size()!=0);
    from_term_map_type eliminated2row_number;
    int remaining_cols;
    mzd_t* mat_step1;
    vector<int> compactified_columns2old_columns;
    int rows_step1;
    vector<int> row_start;
    //vector<Polynomial> result;
    MatrixMonomialOrderTables step1(terms_step1);
    //vector<Exponent> terms_as_exp_step1;
    {
        int rows=polys_triangular.size();
        int cols=terms_step1.size();
        rows_step1=rows;
        if UNLIKELY(log){
            std::cout<<"STEP1: ROWS:"<<rows<<"COLUMNS:"<<cols<<std::endl;
        }

        mat_step1=mzd_init(rows,cols);
        //vector<Exponent> terms_as_exp_lex_step1;
        //vector<int> ring_order2lex_step1;
        //vector<int> lex_order2ring_step1;
  //      from_term_map_type from_term_map_step1;
//setup_order_tables(terms_as_exp_step1,terms_as_exp_lex_step1,ring_order2lex_step1,lex_order2ring_step1,from_term_map_step1, terms_step1);
        fill_matrix(mat_step1,polys_triangular,step1.from_term_map);

        polys_triangular.clear();
        
        if UNLIKELY(optDrawMatrices) {
        char matname[255];
        snprintf(matname,255, "%s%d_step1.png", matrixPrefix, round);
        
        drawmatrix(mat_step1,matname);
        }
        //optimize: call back subst directly
        mzd_top_echelonize_m4ri
            (mat_step1,0);

        if UNLIKELY(log){
            std::cout<<"finished gauss"<<std::endl;
        }
        int rank=mat_step1->nrows;

        //sort rows
        int pivot_row=0;
        row_start.resize(rows);
        assert(cols>=rows);
        remaining_cols=cols-rows;
        compactified_columns2old_columns.resize(remaining_cols);
        for(i=0;i<cols;i++){
            int j;
            for(j=pivot_row;j<rows;j++){
                if UNLIKELY(mzd_read_bit(mat_step1,j,i)==1){
                    if (j!=pivot_row)
                        mzd_row_swap(mat_step1,j,pivot_row);
                    
                    eliminated2row_number[step1.terms_as_exp[i]]=pivot_row;
                    row_start[pivot_row]=i;
                    pivot_row++;
                    
                    break;
                }
            }
            if UNLIKELY(j==rows){
                assert(i>=pivot_row);
                compactified_columns2old_columns[i-pivot_row]=i;
            }
            
        }
        if UNLIKELY(log){
            std::cout<<"finished sort"<<std::endl;
        }
        assert(pivot_row==rows);

        translate_back(polys, leads_from_strat, mat_step1,step1.ring_order2lex, step1.terms_as_exp,step1.terms_as_exp_lex,rank);
        
        if UNLIKELY(log){
            std::cout<<"finished translate"<<std::endl;
        }

        //delete columns
        mzd_t* transposed_step1=mzd_transpose(NULL,mat_step1);
        if UNLIKELY(log){
            std::cout<<"finished transpose"<<std::endl;
        }
        mzd_free(mat_step1);

        for(i=0;i<remaining_cols;i++){
            int source=compactified_columns2old_columns[i];
            assert(i<=source);
            assert(source<=transposed_step1->nrows);
            if LIKELY(i!=source) mzd_row_swap(transposed_step1,source,i);
            
        }
        if UNLIKELY(log){
            std::cout<<"finished permute"<<std::endl;
        }

        //cols, rows arguments are swapped, as matrix is transposed

        mzd_t* sub_step1=mzd_submatrix(NULL,transposed_step1,0,0,remaining_cols,rows);

        if UNLIKELY(log){
            std::cout<<"finished submat"<<std::endl;
        }
        mzd_free(transposed_step1);
        mat_step1=mzd_transpose(NULL,sub_step1);
        if UNLIKELY(log){
            std::cout<<"finished transpose"<<std::endl;
        }
        mzd_free(sub_step1);
    }
    MonomialSet terms_step2=terms.diff(terms_unique);
    const int rows_step2=polys_rest.size();
    const int cols_step2=terms_step2.size();
    mzd_t* mat_step2=mzd_init(rows_step2,cols_step2);
    mzd_t* mat_step2_factor=mzd_init(rows_step2,mat_step1->nrows);
    MatrixMonomialOrderTables step2(terms_step2);
    // vector<Exponent> step2.terms_as_exp;
    //    vector<Exponent> step2.terms_as_exp_lex;
    //    vector<int> step2.ring_order2lex;
    //    vector<int> step2.lex_order2ring;
    //    from_term_map_type step2.from_term_map;
    // setup_order_tables(step2.terms_as_exp,step2.terms_as_exp_lex,step2.ring_order2lex,step2.lex_order2ring,step2.from_term_map, terms_step2);
    
    
    for(i=0;i<polys_rest.size();i++){
        Polynomial p_r=polys_rest[i];
        Polynomial p_t=p_r.diagram().intersect(terms_step2);
        Polynomial p_u=p_r.diagram().diff(p_t.diagram());
        Polynomial::exp_iterator it(p_u.expBegin()), end(p_u.expEnd());
        
        while(it!=end){
            Exponent e=*it; 
                from_term_map_type::const_iterator from_it=eliminated2row_number.find(e);
                assert(step1.terms_as_exp[row_start[from_it->second]]==e);
                assert(from_it!=eliminated2row_number.end());
                int index=from_it->second;//...translate e->line number;
                mzd_write_bit(mat_step2_factor,i,index,1);
            it++;
        }
        it=p_t.expBegin();
        end=p_t.expEnd();
        while(it!=end){
            Exponent e=*it;
                from_term_map_type::const_iterator from_it=step2.from_term_map.find(e);
                assert(from_it!=step2.from_term_map.end());
                int index=from_it->second;
                mzd_write_bit(mat_step2,i,index,1);
            it++;       
        }
    }
    
    if UNLIKELY(log){
        std::cout<<"iterate over rest polys"<<std::endl;
    }
    
    vector<int> remaining_col2new_col(remaining_cols);
    for(i=0;i<remaining_cols;i++){
        remaining_col2new_col[i]=step2.from_term_map[step1.terms_as_exp[compactified_columns2old_columns[i]]];
    }
    assert(mat_step2_factor->ncols==mat_step1->nrows);
    assert(mat_step1->nrows==terms_unique.size());
    assert(mat_step1->ncols==remaining_cols);

    if UNLIKELY(optDrawMatrices)
    {
    
    char matname[255];
    snprintf(matname,255, "%s%d_mult_A.png", matrixPrefix, round);
    drawmatrix(mat_step2_factor,matname);
    snprintf(matname,255, "%s%d_mult_B.png", matrixPrefix, round);
    drawmatrix(mat_step1,matname);
    }
    if UNLIKELY(log){
        std::cout<<"start mult"<<std::endl;
    }
    
    mzd_t* eliminated;
    if LIKELY((mat_step2_factor->nrows!=0) && (mat_step1->nrows!=0) && (mat_step2_factor->ncols!=0) && (mat_step1->ncols!=0))   
        eliminated=mzd_mul_m4rm(NULL,mat_step2_factor,mat_step1,0);
    else
        eliminated=mzd_init(mat_step2_factor->nrows,mat_step1->ncols);

    mzd_free(mat_step2_factor);
    if UNLIKELY(log){
        std::cout<<"end mult"<<std::endl;
    }
    mzd_free(mat_step1);
    assert(polys_rest.size()==eliminated->nrows);
    assert(mat_step2->nrows==eliminated->nrows);
    for(i=0;i<polys_rest.size();i++){
        int j;
        assert(remaining_cols==eliminated->ncols);
        for(j=0;j<remaining_cols;j++){
            if UNLIKELY(mzd_read_bit(eliminated,i,j)==1){
                assert(step2.terms_as_exp[remaining_col2new_col[j]]==step1.terms_as_exp[compactified_columns2old_columns[j]]);
                
                if UNLIKELY(mzd_read_bit(mat_step2,i,remaining_col2new_col[j])==1){
                    mzd_write_bit(mat_step2,i,remaining_col2new_col[j],0);
                        } else mzd_write_bit(mat_step2,i,remaining_col2new_col[j],1);
            }
        }
    }

    mzd_free(eliminated);
    
     if UNLIKELY(log){
            std::cout<<"STEP2: ROWS:"<<rows_step2<<"COLUMNS:"<<cols_step2<<std::endl;
        }
    if UNLIKELY(optDrawMatrices)
    {
        char matname[255];
        snprintf(matname,255, "%s%d_step2.png", matrixPrefix, round);
        drawmatrix(mat_step2,matname);
    }


    int rank_step2;
    if ((mat_step2->ncols>0) &&( mat_step2->nrows>0)){
        rank_step2=
        mzd_echelonize_m4ri(mat_step2,TRUE,0);
        //mzd_echelonize_pluq(mat_step2,TRUE);
    } else
        rank_step2=0;

        if UNLIKELY(log){
            std::cout<<"finished gauss"<<std::endl;
        }

    translate_back(polys, leads_from_strat, mat_step2,step2.ring_order2lex, step2.terms_as_exp,step2.terms_as_exp_lex,rank_step2);
    mzd_free(mat_step2);
    

}


vector<Polynomial> gauss_on_polys(const vector<Polynomial>& orig_system){

  if (orig_system.empty())
    return orig_system;

  Polynomial init(0, orig_system[0].ring());
  MonomialSet terms=unite_polynomials(orig_system, init);
  MonomialSet from_strat(init.ring());//no strat
  vector<Polynomial> polys(orig_system);
  linalg_step(polys, terms, from_strat, false);
  return polys;
}
vector<Polynomial> GroebnerStrategy::faugereStepDense(const vector<Polynomial>& orig_system){
  if (orig_system.empty())
    return orig_system;
    vector<Polynomial> polys;
    //vector<Monomial> leads_from_strat_vec;

    int i;
    MonomialSet terms(orig_system[0].ring());
    MonomialSet leads_from_strat(terms.ring());
    assert(orig_system[0].ring().id() ==  terms.ring().id());

#ifndef NDEBUG
    {
      vector < Polynomial >::const_iterator start(orig_system.begin()),
	finish(orig_system.end());
      
      while (start != finish) {
	assert(terms.ring().id() == start->ring().id());
	++start;
      }
    }
#endif
    fix_point_iterate(*this,orig_system,polys,terms,leads_from_strat);
    if (optModifiedLinearAlgebra){
    linalg_step_modified(polys,terms,leads_from_strat, enabledLog, optDrawMatrices, matrixPrefix.data());
    } else {
        linalg_step(polys,terms,leads_from_strat, enabledLog, optDrawMatrices, matrixPrefix.data());
    }
    //leads_from_strat=terms.diff(mod_mon_set(terms,generators.minimalLeadingTerms));


    return polys;
}
#endif



MonomialSet mod_mon_set(const MonomialSet& as, const MonomialSet &vs){
  typedef PBORI::CacheManager<CCacheTypes::mod_mon_set>
     cache_mgr_type;
 cache_mgr_type cache_mgr(as.ring());
 return dd_modulo_monomials(cache_mgr, as.navigation(), vs.navigation(),
			    MonomialSet(as.ring()));
}


Polynomial ReductionStrategy::reducedNormalForm(Polynomial p) const{
    if UNLIKELY(p.isZero()) return p;
    
    Polynomial res(p.ring());
    if (p.ring().ordering().isDegreeOrder()) res=nf3_degree_order(*this,p,p.lead());
    else res=nf3(*this,p,p.lead());
    if ((res.isZero())) return res;
    res=red_tail(*this,res);
    return res;
}
Polynomial ReductionStrategy::headNormalForm(Polynomial p) const{
    if UNLIKELY(p.isZero()) return p;
    
    Polynomial res(p.ring());
    if (p.ring().ordering().isDegreeOrder()) res=nf3_degree_order(*this,p,p.lead());
    else res=nf3(*this,p,p.lead());
    return res;
}
Polynomial ReductionStrategy::nf(Polynomial p) const{
    if (optRedTail) return reducedNormalForm(p);
    else return headNormalForm(p);
}

END_NAMESPACE_PBORIGB
