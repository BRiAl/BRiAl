// -*- c++ -*-
//*****************************************************************************
/** @file LLReduction.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30 
 *
 * This file includes the definition of the class @c LLReduction.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_LLReduction_h_
#define polybori_groebner_LLReduction_h_

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class LLReduction
 * @brief This class defines LLReduction.
 *
 **/
template <bool have_redsb, bool single_call_for_noredsb,
          bool fast_multiplication>
class LLReduction {
public:

  template<class RingType>
  LLReduction(const RingType& ring): cache_mgr(ring) {}

  Polynomial multiply(const Polynomial &p, const Polynomial& q){
    typedef CommutativeCacheManager<CCacheTypes::multiply_recursive>
      cache_mgr_type;
    
    return dd_multiply<fast_multiplication>(cache_mgr_type(p.ring()), 
                                            p.navigation(), q.navigation(),
                                            BoolePolynomial(p.ring()));
  }

  Polynomial operator()(const Polynomial& p, MonomialSet::navigator r_nav);

protected:
  typedef PBORI::CacheManager<CCacheTypes::ll_red_nf> cache_mgr_type;
  cache_mgr_type cache_mgr;
};


template <bool have_redsb, bool single_call_for_noredsb, bool fast_multiplication>
Polynomial
LLReduction<have_redsb, single_call_for_noredsb,
            fast_multiplication>::operator() (const Polynomial& p,
                                              MonomialSet::navigator r_nav) {     

  if PBORI_UNLIKELY(p.isConstant()) return p;
    
  MonomialSet::navigator p_nav=p.navigation();
  idx_type p_index=*p_nav;
  
  while((*r_nav)<p_index) {
    r_nav.incrementThen();
  }
  
  if PBORI_UNLIKELY(r_nav.isConstant())
    return p;

  MonomialSet::navigator cached = cache_mgr.find(p_nav, r_nav);

  if PBORI_LIKELY(cached.isValid()) 
    return MonomialSet(cache_mgr.generate(cached));

  Polynomial res(0, p.ring());

  Polynomial p_nav_else(cache_mgr.generate(p_nav.elseBranch()));
  Polynomial p_nav_then(cache_mgr.generate(p_nav.thenBranch()));

  if ((*r_nav) == p_index){
    Polynomial r_nav_else(cache_mgr.generate(r_nav.elseBranch()));

    if ((!have_redsb) && single_call_for_noredsb) { 
      res = operator()(p_nav_else + multiply(r_nav_else, p_nav_then), 
                       r_nav.thenBranch());
    }
    else {
      Polynomial tmp1 = operator()(p_nav_else, r_nav.thenBranch());
      Polynomial tmp2 = operator()(p_nav_then, r_nav.thenBranch());

      Polynomial tmp( have_redsb? r_nav_else:
                      operator()(r_nav_else, r_nav.thenBranch()) );
      res = tmp1 + multiply(tmp, tmp2);
    }
  } 
  else{
    PBORI_ASSERT((*r_nav)>p_index);
    res = MonomialSet( p_index,
                       operator()(p_nav_then, r_nav).diagram(),
                       operator()(p_nav_else, r_nav).diagram());
      
  }

  cache_mgr.insert(p_nav, r_nav, res.navigation());
  return res;
}

END_NAMESPACE_PBORIGB

#endif /* polybori_LLReduction_h_ */
