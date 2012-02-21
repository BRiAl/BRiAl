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


#include <polybori/groebner/LexHelper.h>
#include <polybori/groebner/LexOrderGreaterComparer.h>

#include <polybori/groebner/ExpGreater.h>
#include <polybori/groebner/PolyMonomialPairComparerLexLess.h>
#include <polybori/groebner/PolyMonomialPairComparerLess.h>



#include <polybori/groebner/nf.h>
#include <polybori/groebner/linear_algebra_step.h>
#include <polybori/groebner/red_tail.h>
#include <polybori/groebner/ll_red_nf.h>
#include <polybori/groebner/add_up.h>
#include <polybori/groebner/polynomial_properties.h>
#include <polybori/groebner/LexBucket.h>
#include <polybori/LexOrder.h>
#include <iostream>
#include <polybori/iterators/COrderedIter.h>


using std::cout;
using std::endl;

BEGIN_NAMESPACE_PBORIGB



// static int log2_floor(int n){
//   unsigned i = 0;
//   for(;__M4RI_TWOPOW(i)<=(unsigned)n;i++){}
//     return i-1;
// }
// static int optimal_k_for_multiplication(int a,int b,int c,const GroebnerStrategy& strat){
//     int res=std::min(M4RI_MAXKAY,std::max(1,log2_floor(b)));
//     if PBORI_UNLIKELY(strat.enabledLog)
//         std::cout<<"optimal k for multiplication:"<<res<<std::endl;
//     return res;
// }
// static int optimal_k_for_gauss(int m, int n, const GroebnerStrategy& strat){
//     int l=std::min(n,m);
//     int res=std::min(M4RI_MAXKAY,std::max(1,log2_floor(l)+1-log2_floor(log2_floor(l))));
//     if PBORI_UNLIKELY(strat.enabledLog)
//         std::cout<<"optimal k for gauss:"<<res<<std::endl;
//     return res;
// }



Polynomial nf3(const ReductionStrategy& strat, Polynomial p, Monomial rest_lead){
  int index;
  while((index=strat.select1(rest_lead))>=0){
    PBORI_ASSERT(index<strat.size());
  
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
    PBORI_ASSERT(index<strat.generators.size());
  
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
    PBORI_ASSERT(index<strat.size());
  
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
    PBORI_ASSERT(index<strat.size());
  
    const Polynomial* g=&strat[index].p;
    
    if //((strat.generators[index].deg==1)&&(lm!=strat.generators[index].lead)){
    (((strat.optBrutalReductions) && (lead!=strat[index].lead))||
			((strat[index].length<4) &&(strat[index].ecart()==0)
 			&& (lead!=strat[index].lead)))

{     wlen_type dummy;
      #ifndef PBORI_NDEBUG
      Polynomial p_old=p;
      #endif
      
      PBORI_ASSERT(deg==p.leadDeg());
      Polynomial pg=p.gradedPart(deg);
      p=p-pg+reduce_complete(pg,strat[index],dummy);
      //p=reduce_complete(p,strat.generators[index],dummy);
      #ifndef PBORI_NDEBUG
      PBORI_ASSERT(p.isZero()||p.lead()<p_old.lead());
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
    PBORI_ASSERT(index<strat.size());
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

// static Polynomial exchange(GroebnerStrategy& strat , int i, const Polynomial & p){
//   PBORI_ASSERT(p.lead() == strat.generators[i].lead);
//   PolyEntry e(p);

//   e.vPairCalculated=strat.generators[i].vPairCalculated;
//   Polynomial res=spoly(strat.generators[i].p,p);
//   strat.generators.exchange(i, e);

//   return res;
// }

static Polynomial exchange_with_promise(GroebnerStrategy& strat , int i, const Polynomial & p){
  PBORI_ASSERT(p.lead()==strat.generators[i].lead);

  //PolyEntry e(p);
  //e.vPairCalculated=strat.generators[i].vPairCalculated;
  Polynomial res=strat.generators[i].p;
  strat.generators.exchange(i,p);

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





static void step_S(std::vector<PolynomialSugar>& curr, std::vector<Polynomial>& result, const BooleMonomial& lm, int index, GroebnerStrategy& strat){
  
  int s=curr.size();
  
  
  
  if ((strat.generators[index].length>2)||(lm==strat.generators[index].lead)){
    
    
    
    if //((strat.generators[index].deg==1)&&(lm!=strat.generators[index].lead)){
	  (((strat.generators.optBrutalReductions) && (lm!=strat.generators[index].lead)) ||
    ((strat.generators[index].length<4) &&(strat.generators[index].ecart()==0) 
		&& (lm!=strat.generators[index].lead))){
      //implies lmDeg==1, ecart=0
      //cout<<"REDUCE_COMPLETE\n";
      //PBORI_ASSERT(strat.generators[index].ecart()==0);
      //PBORI_ASSERT(strat.generators[index].leadDeg==1);
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
  PBORI_ASSERT(p_high.lead()==lm);
      for(int i=0;i<s;i++){
        
        curr[i].add(p_high, deg_high, len_high);
      }
    }
  } else {
    PBORI_ASSERT(strat.generators[index].length<=2);
    if (strat.generators[index].length==2){
      PBORI_ASSERT(strat.generators[index].p.length()==2);
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
      PBORI_ASSERT(strat.generators[index].length==1);
      PBORI_ASSERT(strat.generators[index].p.length()==1);
    
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
  
  Polynomial pivot(lm.ring());
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
      PBORI_ASSERT(lm==strat.generators[index].lead);
      PBORI_ASSERT(curr[found].getSugar()>=curr[found].value().deg());
      PBORI_ASSERT(curr[found].value().lead()==lm);
      wlen_type old_length=strat.generators[index].length;
      deg_type old_deg=strat.generators[index].deg;
      curr[found]=PolynomialSugar(exchange_with_promise(strat, index, curr[found].value()),old_deg,old_length);
      strat.log("Exchange");
    }
    #endif
    
    deg_type deg_high=strat.generators[index].ecart()+lm.deg();
    curr[found].add((lm/strat.generators[index].lead)*strat.generators[index].p, deg_high,strat.generators[index].length);
    //PBORI_ASSERT(!(curr[found].value().isZero()));
  } else 
    step_S(curr,result,lm, index,strat);
  
}


static void step_T_simple(std::vector<PolynomialSugar>& curr, std::vector<Polynomial>& result,  const BooleMonomial& lm,GroebnerStrategy& strat){
  int s=curr.size();
  Polynomial reductor(lm.ring());
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
  PBORI_ASSERT(reductor.lead()==lm);
  wlen_type length=reductor.length();//curr[found].getLengthEstimation();
  curr.erase(curr.begin()+found);
  
  s=s-1;
  
  //will calculate elimLength later so does not increase assymptotic complexity
  deg_type deg_high=reductor.deg();
  for(i=0;i<s;i++){
    //if (i==found) continue;
    PBORI_ASSERT(curr[i].lead()==lm);
    PBORI_ASSERT(curr[i].lead()==curr[i].value().lead());
    curr[i].add(reductor, deg_high,length);
    if (!(curr[i].isZero())){
      //if (!(curr[i].lead()<lm)){
    //    cout<<curr[i].lead()<<endl<<lm<<endl;
     //}
      PBORI_ASSERT(curr[i].lead()<lm);
    }
    
  }
  PBORI_ASSERT(!(reductor.isZero()));
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
    PBORI_ASSERT(iter!=lm.end());
    m1=m1.subset1(*iter);
    m2=m2.subset1(*iter);
    iter++;
    
  }
  return m1.length()+m2.length()-2*m1.intersect(m2).length();
}


// static void step_T_complex(std::vector<PolynomialSugar>& curr, std::vector<Polynomial>& result,  const BooleMonomial& lm,GroebnerStrategy& strat){
//   std::sort(curr.begin(), curr.end(), PSCompareByEl());
//   const int max_cans=5;
//   int s=curr.size();
//   Polynomial reductor(lm.ring());

//   wlen_type pivot_el;
  
//   pivot_el=curr[0].eliminationLength();
  
//   int i,j;
//   for (i=s-1;i>0;i--){
//     int found=0;
//     MonomialSet as_set(curr[i].value());
//     int c_size=sum_size(as_set, MonomialSet(curr[0].value()));
//     for (j=1;j<std::min(i,max_cans);j++){ 
//       int size2=sum_size(as_set, MonomialSet(curr[j].value()));
//       if (size2<c_size){
//         found=j;
//         c_size=size2;
//       }
//     }
//     curr[i].add(curr[found].value(), curr[found].getSugar(), curr[found].getLengthEstimation());
//   }
//   reductor=curr[0].value();
//   curr.erase(curr.begin());
//   PBORI_ASSERT(!(reductor.isZero()));
//   result.push_back(reductor);
  
  
// }

// static void step_T(std::vector<PolynomialSugar>& curr, std::vector<Polynomial>& result,  const BooleMonomial& lm,GroebnerStrategy& strat){
//   int s=curr.size();

//   if (s>2) return step_T_complex(curr,result, lm, strat);
//   else
//     step_T_complex(curr,result, lm, strat);


  
  
// }


std::vector<Polynomial> parallel_reduce(std::vector<Polynomial> inp, GroebnerStrategy& strat, int average_steps, double delay_f){

  
  std::vector<Polynomial> result;
  if (inp.empty())
    return result;

  std::size_t s=inp.size();
  int max_steps=average_steps*s;
  int steps=0;
  std::priority_queue<PolynomialSugar, std::vector<PolynomialSugar>, LMLessComparePS> to_reduce;
  deg_type max_sugar=0;
  ///  Polynomial::size_type max_length=0;
  
  for(std::size_t i=0;i<s;i++){
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
  const idx_type last_block_start=inp[0].ring().ordering().lastBlockStart();
  while (!(to_reduce.empty())){

    std::vector<PolynomialSugar> curr;
    curr.push_back(to_reduce.top());
    to_reduce.pop();

    Monomial lm=curr[0].lead();
    #ifdef PBORI_HAVE_M4RI
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
                for(std::size_t i=0;i<la.size();i++){
                    PBORI_ASSERT(!(la[i].isZero()));
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
     
      std::size_t s = curr.size();
      if (s>1){
        steps+=curr.size()-1;
        step_T_simple(curr,result,lm,strat);
      } else{
        PBORI_ASSERT(s==1);
				if (!(curr[0].isZero()))
        	result.push_back(curr[0].value());
        curr.clear();
      }
      
    }
    
    //after each loop push back
    s=curr.size();
    for(std::size_t i=0; i < s; i++){
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
            PBORI_ASSERT(!(to_reduce.top().isZero()));
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
                for(std::size_t i=0;i<this_lm.size();i++){
                    if (this_lm[i].value()!=for_basis->value()){
                        strat.addGeneratorDelayed(this_lm[i].value());
                    }
                }
                PBORI_ASSERT(!(for_basis->value().isZero()));
                result.push_back((*for_basis).value());
            }
            
            
            
 
        }
        return result;
    }
  }
  return result;
  
}


// Doxygen problems with the following syntax, so skip during docs generation




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
        int index=strat.index(curr);
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

// static Polynomial nf4(GroebnerStrategy& strat, Polynomial p){
//   int index;
//   while((index=strat.generators.select1(p))>=0){
//     PBORI_ASSERT(index<strat.generators.size());
//     const Polynomial* g=&strat.generators[index].p;
    
//     if((strat.generators[index].ecart()==0) && (strat.generators[index].length<=4) &&(strat.generators[index].lead!=p.lead())){
//       wlen_type dummy;
//       p=reduce_complete(p,strat.generators[index],dummy);
      
//     } else{
//       p=spoly(p,*g);
//     }
//   }
//   return p;
  
// }

Polynomial do_plug_1(const Polynomial& p, const MonomialSet& m_plus_ones){
    MonomialSet::navigator m_nav=m_plus_ones.navigation();
    
    if (m_nav.isConstant()){
        return p;
    }
    Polynomial::navigator p_nav=p.navigation();
    if PBORI_UNLIKELY(p_nav.isConstant()) return p;
    idx_type p_index=*p_nav;
    while(p_index>*m_nav){
        PBORI_ASSERT(!(m_nav.isConstant()));
        m_nav.incrementElse();
    }
    PBORI_ASSERT (p_index==*p_nav);
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
        PBORI_ASSERT(p_index<*m_nav);
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



Polynomial
cheap_reductions(const ReductionStrategy& strat, Polynomial p){
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


MonomialSet mod_mon_set(const MonomialSet& as, const MonomialSet &vs){
  typedef PBORI::CacheManager<CCacheTypes::mod_mon_set>
     cache_mgr_type;
 cache_mgr_type cache_mgr(as.ring());
 return dd_modulo_monomials(cache_mgr, as.navigation(), vs.navigation(),
			    MonomialSet(as.ring()));
}



END_NAMESPACE_PBORIGB
