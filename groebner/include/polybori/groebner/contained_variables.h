// -*- c++ -*-
//*****************************************************************************
/** @file contained_variables.h 
 *
 * @author Michael Brickenstein
 * @date 2011-06-30
 *
 * This file includes the definition of the @c contained_variables functions.
 *
 * @par Copyright:
 *   (c) 2006-2011 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_contained_variables_h_
#define polybori_groebner_contained_variables_h_

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

inline MonomialSet
contained_variables_cudd_style(const MonomialSet& m){
    
    MonomialSet::navigator nav=m.navigation();
    MonomialSet::navigator orig=nav;
    typedef PBORI::CacheManager<CCacheTypes::contained_variables>
      cache_mgr_type;

    cache_mgr_type cache_mgr(m.ring());


    while (!(nav.isConstant())){
        MonomialSet::navigator cached =
          cache_mgr.find(nav);
        if (cached.isValid()) return cache_mgr.generate(cached);
        idx_type v=*nav;
        MonomialSet::navigator check_nav=nav.thenBranch();
        while(!(check_nav.isConstant())){
            check_nav.incrementElse();
        }
        if (check_nav.isTerminated()){
            idx_type result_index=v;
            MonomialSet result=MonomialSet(result_index,Polynomial(cache_mgr.one()).diagram(),contained_variables_cudd_style(cache_mgr.generate(nav.elseBranch())));
            MonomialSet::navigator r_nav=result.navigation();
            while(1){
              MonomialSet::navigator last=orig;
              cache_mgr.insert(orig, r_nav);
              orig.incrementElse();
              if(last==nav) break;
            }
            return result;
        }
        nav.incrementElse();
        
    }
    return MonomialSet(cache_mgr.zero());
}

inline MonomialSet
contained_deg2_cudd_style(const MonomialSet& m){
    
    MonomialSet::navigator nav=m.navigation();
    
    typedef PBORI::CacheManager<CCacheTypes::contained_deg2>
      cache_mgr_type;

    cache_mgr_type cache_mgr(m.ring());


    if (!(nav.isConstant())){
        MonomialSet::navigator cached =
          cache_mgr.find(nav);
        if (cached.isValid()) return cache_mgr.generate(cached);
        MonomialSet::navigator then_branch=nav.thenBranch();
        MonomialSet::navigator else_branch=nav.elseBranch();
        MonomialSet then_res=contained_variables_cudd_style(cache_mgr.generate(then_branch));
        MonomialSet else_res=contained_deg2_cudd_style(cache_mgr.generate(else_branch));
        MonomialSet result=MonomialSet(*nav,then_res,else_res);
        cache_mgr.insert(nav,result.navigation());
        return result;
    }
    else return MonomialSet(cache_mgr.zero());
}



inline std::vector<idx_type>
contained_variables(const MonomialSet& m){
    std::vector<idx_type> result;
    MonomialSet::navigator nav=m.navigation();
    while (!(nav.isConstant())){
        idx_type v=*nav;
        MonomialSet::navigator check_nav=nav.thenBranch();
        while(!(check_nav.isConstant())){
            check_nav.incrementElse();
        }
        if (check_nav.isTerminated()){
            result.push_back(v);
        }
        nav.incrementElse();
        
    }
    return result;
}


END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_contained_variables_h_ */
