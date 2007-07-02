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

#include "interpolate.h"

BEGIN_NAMESPACE_PBORIGB
Polynomial interpolate(MonomialSet to_zero,MonomialSet to_one){
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
    Polynomial p0=interpolate(to_zero.subset0(index),to_one.subset0(index));
    Polynomial p1=interpolate(to_zero.subset1(index),to_one.subset1(index))+p0;
    MonomialSet result(index,p1.diagram(),p0.diagram());   
    cache_mgr.insert(to_zero.navigation(),to_one.navigation(),result.navigation());
    return result;
}
END_NAMESPACE_PBORIGB
