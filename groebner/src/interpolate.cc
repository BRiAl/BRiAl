//
//  Interpolation
//
//  Created by Michael Brickenstein on 2007-07-02.
//  Copyright (c) 2007 Mathematisches Forschungsinstitut Oberwolfach. All rights reserved.
//  PolyBoRi Project

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
        Polynomial p0=interpolate_smallest_lex(to_zero0,to_one0);
        MonomialSet affected=to_zero1.unite(to_one1);
        affected=affected.diff(zeroes(p0,affected));
        Polynomial p1=interpolate_smallest_lex(to_zero1.Xor(affected),to_one1.Xor(affected));
        result=MonomialSet(index,p1.diagram(),p0.diagram());
        //std::cout<<"else branch"<<std::endl;
    }
   
    cache_mgr.insert(to_zero.navigation(),to_one.navigation(),result.navigation());
    return result;
}
END_NAMESPACE_PBORIGB
