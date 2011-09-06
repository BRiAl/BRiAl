// -*- c++ -*-
//*****************************************************************************
/** @file fixed_path_divisors.h 
 *
 * @author Michael Brickenstein
 * @date 2011-07-01
 *
 * This file includes the definition of the class @c fixed_path_divisors.
 *
 * @par Copyright:
 *   (c) 2006-2011 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_fixed_path_divisors_h_
#define polybori_groebner_fixed_path_divisors_h_

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

#ifndef DANGEROUS_FIXED_PATH
  typedef PBORI::CacheManager<CCacheTypes::divisorsof_fixedpath>
#else
  typedef PBORI::CacheManager<CCacheTypes::divisorsof>
#endif
  fixed_divisors_cache_type;

// Variant navigator
inline MonomialSet
do_fixed_path_divisors(const fixed_divisors_cache_type & cache_mgr, 
                       MonomialSet::navigator a_nav,
                       MonomialSet::navigator m_nav,
                       MonomialSet::navigator n_nav){

  if (n_nav.isTerminated()) return 
    MonomialSet(cache_mgr.generate(a_nav)).firstDivisorsOf(cache_mgr.generate(m_nav));
  PBORI_ASSERT(!(n_nav.isConstant()&&(!(n_nav.terminalValue()))));

  if (a_nav.isConstant()) return cache_mgr.generate(a_nav);

  PBORI_ASSERT(!(n_nav.isConstant()));
  PBORI_ASSERT(!(m_nav.isConstant()));
  int m_index=*m_nav;
  int n_index=*n_nav;
  int a_index=*a_nav;
   
  PBORI_ASSERT(m_index<=n_index);
  
  
  //here we rely on the fact, that in Cudd deref of constant nav. gives a bigger index than allow allowed real indices
  while((a_index=*a_nav)!=(m_index=*m_nav)){
     if (a_index<m_index) a_nav.incrementElse();
     else{
       n_index=*n_nav;
       PBORI_ASSERT(n_index>=m_index);
       if (m_index==n_index){
         n_nav.incrementThen();
         m_nav.incrementThen();
       } else {
         m_nav.incrementThen();
       }
     }
   
  }
  n_index=*n_nav;
 
  if (a_nav.isConstant()){
    return cache_mgr.generate(a_nav);
  }
  PBORI_ASSERT((*a_nav)==(*m_nav));

  MonomialSet::navigator cached;
  #ifndef DANGEROUS_FIXED_PATH
  cached =
    cache_mgr.find(a_nav, m_nav,n_nav);
    if (cached.isValid() ){
      return  cache_mgr.generate(cached);
    }
  #else
  //MonomialSet::navigator cached =
    //cache_mgr.find(a_nav, m_nav);
  #endif

    
  // here it is theoretically possible to get answers which don't contain the
  // fixed path n, but that doesn't matter in practice, 
  // as it is optimization anyway
  typedef PBORI::CacheManager<CCacheTypes::divisorsof>
    cache_mgr_type2;

  cache_mgr_type2 cache_mgr2(cache_mgr.manager());
  
  cached =
    cache_mgr2.find(a_nav, m_nav);
  
  if (cached.isValid()){
    return cache_mgr2.generate(cached);
  }
    
  PBORI_ASSERT(a_index==m_index);
  int index=m_index;
  MonomialSet result(cache_mgr.zero());
  if (m_index==n_index){
    result=do_fixed_path_divisors(cache_mgr, a_nav.thenBranch(), 
                                  m_nav.thenBranch(), n_nav.thenBranch());
    if (!(result.isZero())) 
      result = MonomialSet(index, result, cache_mgr.zero());
  } else {
    MonomialSet
      then_path=do_fixed_path_divisors(cache_mgr, a_nav.thenBranch(),
                                       m_nav.thenBranch(), n_nav);
    MonomialSet
      else_path=do_fixed_path_divisors(cache_mgr, a_nav.elseBranch(),
                                       m_nav.thenBranch(), n_nav);
    if (then_path.isZero()){
      result=else_path;
    } else {
      result=MonomialSet(index,then_path,else_path);
    }
  }
#ifndef DANGEROUS_FIXED_PATH
  cache_mgr.insert(a_nav,m_nav,n_nav,result.navigation());
#else
 cache_mgr2.insert(a_nav,m_nav,result.navigation());
#endif
  return result;
}
// end of variant for navigator

// variant for MonomialSet
inline MonomialSet
do_fixed_path_divisors(MonomialSet a, 
                                          MonomialSet m, MonomialSet n){

  //we assume that m is a multiple of n
  MonomialSet::navigator m_nav=m.navigation();
  MonomialSet::navigator n_nav=n.navigation();

  MonomialSet::navigator a_nav=a.navigation();

  typedef fixed_divisors_cache_type cache_mgr_type;
  cache_mgr_type cache_mgr(a.ring());

  return do_fixed_path_divisors(cache_mgr, a_nav, m_nav, n_nav);
}


inline MonomialSet
fixed_path_divisors(MonomialSet a, Monomial m, Monomial n){
   PBORI_ASSERT(m.reducibleBy(n));
   return do_fixed_path_divisors(a,m.diagram(),n.diagram());
}


END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_fixed_path_divisors_h_ */
