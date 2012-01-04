// -*- c++ -*-
//*****************************************************************************
/** @file SlimgbReduction.h 
 *
 * @author Michael Brickenstein
 * @date 2011-06-30
 *
 * This file includes the definition of the class @c groebner_SlimgbReduction.
 *
 * @par Copyright:
 *   (c) by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_SlimgbReduction_h_
#define polybori_groebner_SlimgbReduction_h_

// include basic definitions
#include "groebner_defs.h"
#include <utility>
#include <vector>
#include "LMLessCompare.h"
#include "GroebnerStrategy.h"

BEGIN_NAMESPACE_PBORIGB

/** @class SlimgbReduction
 * @brief This class defines SlimgbReduction.
 *
 **/

const int SLIMGB_SIMPLEST=0;

template<int variant>
class SlimgbReduction{
private:
  const GroebnerStrategy* strat;
  std::priority_queue<Polynomial, std::vector<Polynomial>, LMLessCompare> to_reduce;
  public:
  std::vector<Polynomial> result;

  SlimgbReduction(GroebnerStrategy& strat){
    this->strat=&strat;
  }
  SlimgbReduction(){}
  void addPolynomial(const Polynomial& p);
  void reduce();
  //return zero at the end
  Polynomial nextResult();
};

template <int variant>
inline void
SlimgbReduction<variant>::addPolynomial(const Polynomial& p){
  if (!(p.isZero())){
    to_reduce.push(p);
  }
}

template <int variant>
inline Polynomial
SlimgbReduction<variant>::nextResult(){
  if (result.size()==0) 
    throw std::runtime_error("Empty result in SlimgbReduction.");
  Polynomial res=result.back();
  result.pop_back();
  return res;
}


template <>
inline void
SlimgbReduction<SLIMGB_SIMPLEST>::reduce(){
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
      PBORI_ASSERT(p_high.lead()==lm);
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
            PBORI_ASSERT(curr[i].lead()<lm);
            to_reduce.push(curr[i]);
          }
          
        }
        PBORI_ASSERT(!(reductor.isZero()));
        result.push_back(reductor);
      } else{
        PBORI_ASSERT(s==0);
        PBORI_ASSERT(!(curr[0].isZero()));
        result.push_back(curr[0]);
      }
    }
  
  }
  
}



END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_SlimgbReduction_h_ */
