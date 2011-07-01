// -*- c++ -*-
//*****************************************************************************
/** @file ll_red_nf.h 
 *
 * @author Michael Brickenstein
 * @date 2011-07-01
 *
 * This file includes the definition of the function @c ll_red_nf.
 *
 * @par Copyright:
 *   (c) 2006-2011 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_ll_red_nf_h_
#define polybori_groebner_ll_red_nf_h_

// include basic definitions
#include "groebner_defs.h"
#include "ReductionStrategy.h"
#include "LLReduction.h"

BEGIN_NAMESPACE_PBORIGB


template <bool have_redsb, bool single_call_for_noredsb, 
	  bool fast_multiplication>
inline Polynomial 
ll_red_nf_generic(const Polynomial&, const BooleSet&);

template <bool fast>
inline Polynomial
multiply(const Polynomial &p, const Polynomial& q){
  typedef CommutativeCacheManager<CCacheTypes::multiply_recursive>
    cache_mgr_type;
  
  return dd_multiply<fast>(cache_mgr_type(p.ring()), 
			   p.navigation(), q.navigation(),
			   BoolePolynomial(p.ring()));
}

template <bool have_redsb, bool single_call_for_noredsb, 
	  bool fast_multiplication>
inline Polynomial
ll_red_nf_generic(const Polynomial& p, MonomialSet::navigator r_nav){
  LLReduction<have_redsb, single_call_for_noredsb, fast_multiplication> 
    func(p.ring());
  
  return func(p, r_nav);
}

template <bool have_redsb, bool single_call_for_noredsb,
	  bool fast_multiplication>
inline Polynomial
ll_red_nf_generic(const Polynomial&  p, const BooleSet& reductors) {

  return ll_red_nf_generic<have_redsb, single_call_for_noredsb,
    fast_multiplication>(p, reductors.navigation());
}

inline Polynomial
ll_red_nf(const Polynomial& p,const BooleSet& reductors){
  return ll_red_nf_generic<true, false, false>(p,reductors);
}

inline Polynomial
ll_red_nf_noredsb(const Polynomial& p,const BooleSet& reductors){
  return ll_red_nf_generic<false, false, false>(p,reductors);
}

inline Polynomial
ll_red_nf_noredsb_single_recursive_call(const Polynomial& p,
					const BooleSet& reductors){
  return ll_red_nf_generic<false, true, false>(p,reductors);
}



END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_ll_red_nf_h_ */
