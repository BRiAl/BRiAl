//
//  untitled
//
//  Created by  on 2008-04-16.
//  Copyright (c) 2008 The PolyBoRi Team. See LICENSE file.
//  PolyBoRi Project
#include <polybori/groebner/groebner_defs.h>
#include <polybori/groebner/polynomial_properties.h>
#include <iostream>
using namespace std;
BEGIN_NAMESPACE_PBORIGB

Polynomial do_is_rewriteable(const Polynomial& p, const MonomialSet& leading_terms){
    //don't change ordering
    if (p.isZero()) return p.ring().zero();
    if (leading_terms.ownsOne()) return p.ring().one();
    //case leading terms contains one is checked
    MonomialSet::navigator l_nav=leading_terms.navigation();
    Polynomial::navigator p_nav=p.navigation();
    idx_type p_i=*p_nav;
    idx_type l_i=*l_nav;
    bool changed=true;
    while (changed){
        changed=false;
        while ((l_i=(*l_nav))<p_i){
            l_nav.incrementElse();
            changed=true;
        }
        //idx_type l_i=*l_nav;
        while(((p_i=*p_nav)<l_i)&& (p_nav.elseBranch().isEmpty())){
            p_nav.incrementThen();
            changed=true;
        }
        while((!(p_nav.isConstant()))&&(l_i==p_i) &&(p_nav.elseBranch().isEmpty()) && (l_nav.elseBranch().isEmpty())){
            p_nav.incrementThen();
            l_nav.incrementThen();
            l_i=*l_nav;
            p_i=*p_nav;
            changed=true;
        }
    }
    PBORI_ASSERT(l_i==*l_nav);
    PBORI_ASSERT(p_i==*p_nav);
    PBORI_ASSERT(p_i<=l_i);

    
    
    typedef PBORI::CacheManager<CCacheTypes::mod_mon_set>
      cache_mgr_type_mod_mon_set;
    cache_mgr_type_mod_mon_set cache_mgr_mod_mon_set(p.ring());
    typedef PBORI::CacheManager<CCacheTypes::is_rewriteable>
      cache_mgr_type;
    cache_mgr_type cache_mgr(p.ring());
    PBORI_ASSERT(!(p.isZero()));
    PBORI_ASSERT(!(p_nav.isEmpty()));
    if (cache_mgr.generate(l_nav).ownsOne()){
        return cache_mgr.one();
    }
    PBORI_ASSERT(!(cache_mgr.generate(l_nav).ownsOne()));
    
    if (p_nav.isTerminated()){
        return cache_mgr.zero();
    }
    if (l_nav.isEmpty()){
        return cache_mgr.zero();
    }
    PBORI_ASSERT(!(p_nav.isConstant()));
    
    Polynomial::navigator cached= cache_mgr.find(p_nav,l_nav);
    if (cached.isValid()) {
        return Polynomial(cache_mgr.generate(cached));
        
    }
    MonomialSet::navigator cached_mod_mon_set= cache_mgr.find(p_nav,l_nav);
    if (cached_mod_mon_set.isValid()){
        return (cached_mod_mon_set==p_nav)?cache_mgr.zero():cache_mgr.one();
    }
    
    PBORI_ASSERT (!(l_nav.isConstant()));
    Polynomial p0=cache_mgr.generate(p_nav.elseBranch());
    Polynomial p1=cache_mgr.generate(p_nav.thenBranch());
    Polynomial res(p.ring());
    if (l_i>p_i){
        MonomialSet l_curr=cache_mgr.generate(l_nav);
        if (((do_is_rewriteable(p0,l_curr)).isOne())||(do_is_rewriteable(p1,l_curr).isOne())){
            res=cache_mgr.one();
        }else{
            res= cache_mgr.zero();
        }
    } else{
        PBORI_ASSERT(l_i==p_i);
        MonomialSet l0=cache_mgr.generate(l_nav.elseBranch());
        MonomialSet l1=cache_mgr.generate(l_nav.thenBranch());
        if ((do_is_rewriteable(p0,l0).isOne())||(do_is_rewriteable(p1,l1).isOne())||(do_is_rewriteable(p1,l0).isOne())){
            res=cache_mgr.one();
        } else{
            res= cache_mgr.zero();
        }
    }
    cache_mgr.insert(p_nav,l_nav,res.navigation());
    return res;

}
bool is_rewriteable(const Polynomial& p, const MonomialSet& leading_terms){
    return (do_is_rewriteable(p,leading_terms).isOne())?true:false;
}
END_NAMESPACE_PBORIGB
