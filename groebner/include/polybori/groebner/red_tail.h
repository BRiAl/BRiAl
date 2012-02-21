// -*- c++ -*-
//*****************************************************************************
/** @file red_tail.h 
 *
 * @author Michael Brickenstein
 * @date 2011-07-01
 *
 * This file includes the definition of the @c red_tail functions.
 *
 * @par Copyright:
 *   (c) 2006-2011 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_red_tail_h_
#define polybori_groebner_red_tail_h_

// include basic definitions
#include "groebner_defs.h"
#include "LexHelper.h"
#include "DegOrderHelper.h"
#include "BlockOrderHelper.h"
#include "GroebnerStrategy.h"

BEGIN_NAMESPACE_PBORIGB

inline Polynomial
red_tail_general(const ReductionStrategy& strat, Polynomial p){

  PBORI_ASSERT(!p.isZero());

  // int deg_bound=p.deg(); /// @todo unused?
  std::vector<Polynomial> res_vec;
  Polynomial orig_p=p;
  bool changed=false;

  // assuming non-zero
  Monomial lm=p.lead();
  res_vec.push_back(lm);
  p=Polynomial(p.diagram().diff(lm.diagram()));

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
    
    if PBORI_UNLIKELY((!(changed))&& (it==end)) return orig_p;
    //@todo: if it==end irr_p=p, p=Polnomial(0)
    Polynomial irr_p(p.ring());
    if PBORI_LIKELY(it!=end) {
      irr_p=add_up_generic(irr, p.ring().zero());
        rest_lead=*it;
        }
    else irr_p=p;
    int s;
    s=irr.size();
    PBORI_ASSERT(s==irr_p.length());
    //if (s!=irr_p.length()) cout<<"ADDUP FAILED!!!!!!!!!!!!!!!!!!!!!!!!\n";
    //for(i=0;i<s;i++){
    //    res_vec.push_back(irr[i]);
    //}
    res_vec.push_back(irr_p);
    //p=p-irr_p;
    p=Polynomial(p.diagram().diff(irr_p.diagram()));
    if PBORI_UNLIKELY(p.isZero()) break;
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
  return add_up_polynomials(res_vec, p.ring().zero());
}


template <class Helper>
inline Polynomial
red_tail_generic(const ReductionStrategy& strat, Polynomial p){

  PBORI_ASSERT(!p.isZero());

  // int deg_bound=p.deg(); /// @todo unused?
  std::vector<Polynomial> res_vec;
  Polynomial orig_p=p;
  bool changed=false;

  // assuming non-zero
  Monomial lm = p.lead();
  res_vec.push_back(lm);
  p = Polynomial(p.diagram().diff(lm.diagram()));

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
    if PBORI_LIKELY(strat.canRewrite(p)){
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
            if PBORI_UNLIKELY(Helper::knowRestIsIrreducible(it,strat)){
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
    
    if PBORI_UNLIKELY((!(changed))&& (it==end)) return orig_p;
    //@todo: if it==end irr_p=p, p=Polnomial(0)
    Polynomial irr_p(p.ring());
    if PBORI_LIKELY((it!=end) &&(!(rest_is_irreducible))) {
      irr_p=Helper::sum_range(irr,it_orig,it, p.ring().zero());//add_up_monomials(irr);
        rest_lead=*it;
        
        }
    else irr_p=p;
    size_t s;
    s=irr.size();

    PBORI_ASSERT((s==irr_p.length())||(rest_is_irreducible));

    res_vec.push_back(irr_p);

    p=Polynomial(p.diagram().diff(irr_p.diagram()));
    if PBORI_UNLIKELY(p.isZero()) break;
    p=Helper::nf(strat,p,rest_lead);
    changed=true;
  }
  
  //should use already added irr_p's
  return add_up_polynomials(res_vec, p.ring().zero());
}



inline Polynomial
red_tail(const ReductionStrategy& strat, Polynomial p){
  if PBORI_UNLIKELY(p.isZero()) return p;

  if (p.ring().ordering().isLexicographical())
    return red_tail_generic<LexHelper>(strat,p);
  if (p.ring().ordering().isDegreeOrder())
    return red_tail_generic<DegOrderHelper>(strat,p);
  if (p.ring().ordering().isBlockOrder())
    return red_tail_generic<BlockOrderHelper>(strat,p);
  return red_tail_general(strat,p);
}

inline Polynomial
red_tail_short(const ReductionStrategy& strat, Polynomial p){
  Polynomial res(p.ring());
  while(!(p.isZero())){
    Polynomial lm=p.lead();
    res+=lm;
    p-=lm;
    p=nf3_short(strat,p);
  }
  return res;
}

inline Polynomial
red_tail_in_last_block(const GroebnerStrategy& strat, Polynomial p){
    Polynomial::navigator nav=p.navigation();
    idx_type last=p.ring().ordering().lastBlockStart();
    if ((*nav)>=last) //includes constant polynomials
        return p;
    while ((*nav)<last){
        nav.incrementElse();
    }
    if (nav.isConstant()){
        //we don't check for strat containing one at this point
        return p;
    }
    Polynomial l1(nav, p.ring());
    Polynomial l2=strat.nf(l1);
    if (!(l2.isZero())) l2=red_tail(strat.generators,l2);
    return p+(l1+l2);
}


END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_red_tail_h_ */
