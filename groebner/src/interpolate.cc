//
//  Interpolation
//
//  Created by Michael Brickenstein on 2007-07-02.
//  Copyright (c) 2007 The PolyBoRi Team. All rights reserved.

#include "groebner_defs.h"
#include <CCacheManagement.h>
#include <CacheManager.h>
#include <algorithm>
#include <iostream>
#include "interpolate.h"

BEGIN_NAMESPACE_PBORIGB
MonomialSet zeroes(Polynomial p, MonomialSet candidates){
    MonomialSet s=p.diagram();
    MonomialSet result;
    MonomialSet::navigator p_nav=s.navigation();
    if (candidates.emptiness()) return candidates;
    if (p.isOne()) return MonomialSet();
    if (p.isZero()) return candidates;
    if (Polynomial(candidates).isOne()){
        if (p.hasConstantPart()) return MonomialSet();
        else return candidates;
    }
    MonomialSet::navigator can_nav=candidates.navigation();
    idx_type index=*can_nav;
    while ((*p_nav)<index){
        p_nav.incrementElse();
    }
    typedef PBORI::CacheManager<CCacheTypes::zeroes>
      cache_mgr_type;
    cache_mgr_type cache_mgr;
    MonomialSet::navigator cached=cache_mgr.find(p_nav, can_nav);
    if (cached.isValid() ){
       return cached;
    }
    s=MonomialSet(p_nav);
    
    MonomialSet p1=s.subset1(index);
    MonomialSet p0=s.subset0(index);
    MonomialSet c1=candidates.subset1(index);
    MonomialSet c0=candidates.subset0(index);
    
    MonomialSet z00=zeroes(p0,c0);
    MonomialSet z01=zeroes(p0,c1);
    MonomialSet z11=zeroes(p1,c1);
    //MonomialSet then_branch=z01.intersect(z11).unite(c1.diff(z01.unite(z11)));
    MonomialSet then_branch=c1.diff(z01.Xor(z11));
    assert (*then_branch.navigation()>index);
    assert (*z00.navigation()>index);
    result=MonomialSet(index,then_branch,z00);
    cache_mgr.insert(p_nav,can_nav,result.navigation());
    return result;
}
Polynomial interpolate(MonomialSet to_zero,MonomialSet to_one){
    //std::cout<<"to_one:"<<(Polynomial) to_one<<"to_zero:"<<(Polynomial) to_zero<<std::endl;
    assert(to_zero.intersect(to_one).emptiness());
    if (to_zero.emptiness()) return Polynomial(1);
    if (to_one.emptiness()) return Polynomial(0);
    typedef PBORI::CacheManager<CCacheTypes::interpolate>
      cache_mgr_type;
    cache_mgr_type cache_mgr;
    MonomialSet::navigator cached=cache_mgr.find(to_zero.navigation(), to_one.navigation());
    if (cached.isValid() ){
       return cached;
    }
    idx_type index=std::min(*to_zero.navigation(),*to_one.navigation());
    //std::cout<<"else"<<std::endl;
    Polynomial p0=interpolate(to_zero.subset0(index),to_one.subset0(index));
    //std::cout<<"then"<<std::endl;
    Polynomial p1=interpolate(to_zero.subset1(index),to_one.subset1(index))+p0;
    MonomialSet result(index,p1.diagram(),p0.diagram());   
    cache_mgr.insert(to_zero.navigation(),to_one.navigation(),result.navigation());
    return result;
}
/*Polynomial interpolate_smallest_lex(MonomialSet to_zero,MonomialSet to_one){
    assert(to_zero.intersect(to_one).emptiness());
    if (to_zero.emptiness()) return Polynomial(1);
    if (to_one.emptiness()) return Polynomial(0);
    typedef PBORI::CacheManager<CCacheTypes::interpolate_smallest_lex>
      cache_mgr_type;
    cache_mgr_type cache_mgr;
    MonomialSet::navigator cached=cache_mgr.find(to_zero.navigation(), to_one.navigation());
    if (cached.isValid() ){
       return cached;
    }
  
    idx_type index=std::min(*to_zero.navigation(),*to_one.navigation());
    MonomialSet to_zero1=to_zero.subset1(index);
    MonomialSet to_zero0=to_zero.subset0(index);
    MonomialSet to_one1=to_one.subset1(index);
    MonomialSet to_one0=to_one.subset0(index);
    MonomialSet to_zerou=to_zero1.unite(to_zero0);
    MonomialSet to_oneu=to_one1.unite(to_one0);
    MonomialSet result;
    if (to_zerou.intersect(to_oneu).emptiness()){
        //std::cout<<"then branch"<<std::endl;
        result=interpolate_smallest_lex(to_zerou,to_oneu);
    } else {
        Polynomial p0=interpolate_smallest_lex(to_zero0,to_one0);
        Polynomial p1=interpolate_smallest_lex(to_zero1,to_one1)+p0;
        result=MonomialSet(index,p1.diagram(),p0.diagram());
        //std::cout<<"else branch"<<std::endl;
    }
   
    cache_mgr.insert(to_zero.navigation(),to_one.navigation(),result.navigation());
    return result;
}
*/

Polynomial interpolate_smallest_lex(MonomialSet to_zero,MonomialSet to_one){
    assert(to_zero.intersect(to_one).emptiness());
    if (to_zero.emptiness()) return Polynomial(1);
    if (to_one.emptiness()) return Polynomial(0);
    typedef PBORI::CacheManager<CCacheTypes::interpolate_smallest_lex>
      cache_mgr_type;
    cache_mgr_type cache_mgr;
    MonomialSet::navigator cached=cache_mgr.find(to_zero.navigation(), to_one.navigation());
    if (cached.isValid() ){
       return cached;
    }
  
    idx_type index=std::min(*to_zero.navigation(),*to_one.navigation());
    MonomialSet to_zero1=to_zero.subset1(index);
    MonomialSet to_zero0=to_zero.subset0(index);
    MonomialSet to_one1=to_one.subset1(index);
    MonomialSet to_one0=to_one.subset0(index);
    MonomialSet to_zerou=to_zero1.unite(to_zero0);
    MonomialSet to_oneu=to_one1.unite(to_one0);
    
    MonomialSet result;
    if (to_zerou.intersect(to_oneu).emptiness()){
        //std::cout<<"then branch"<<std::endl;
        result=interpolate_smallest_lex(to_zerou,to_oneu);
    } else {
        MonomialSet united0=to_zero0.unite(to_one0);
        MonomialSet to_one1_not_in0=to_one1.diff(united0);
        MonomialSet to_zero1_not_in0=to_zero1.diff(united0);
        
        to_zero1=to_zero1.diff(to_zero1_not_in0);
        //to_zero0=to_zero0.unite(to_zero1_not_in0);
        
        to_one1=to_one1.diff(to_one1_not_in0);
        //to_one0=to_one0.unite(to_one1_not_in0);
        
        Polynomial p1=interpolate_smallest_lex(to_zero1.intersect(to_zero0).unite(to_one1.intersect(to_one0)),to_one1.intersect(to_zero0).unite(to_zero1.intersect(to_one0)));
        
        
        MonomialSet not_affected=to_zero1_not_in0.unite(to_one1_not_in0);
        not_affected=zeroes(p1,not_affected);
        Polynomial p0=interpolate_smallest_lex(to_zero0.unite(not_affected.intersect(to_zero1_not_in0)).unite(to_one1_not_in0.diff(not_affected)),
                to_one0.unite(not_affected.intersect(to_one1_not_in0)).unite(to_zero1_not_in0.diff(not_affected)));
        result=MonomialSet(index,p1.diagram(),p0.diagram());
        //std::cout<<"else branch"<<std::endl;
    }
   
    cache_mgr.insert(to_zero.navigation(),to_one.navigation(),result.navigation());
    return result;
}

MonomialSet include_divisors(const MonomialSet& m){
    MonomialSet::navigator nav=m.navigation();
    if (nav.isConstant()) return m;
    typedef PBORI::CacheManager<CCacheTypes::include_divisors>
      cache_mgr_type;
    cache_mgr_type cache_mgr;
    MonomialSet::navigator cached=cache_mgr.find(nav);
    if (cached.isValid() ){
       return cached;
    }
    MonomialSet::navigator tb=nav.thenBranch();
    MonomialSet::navigator eb=nav.elseBranch();
    MonomialSet itb=include_divisors(tb);
    MonomialSet r0=include_divisors(eb).unite(itb);
    MonomialSet result=MonomialSet(*nav,itb,r0);
    cache_mgr.insert(nav,result.navigation());
    return result;
}
END_NAMESPACE_PBORIGB
