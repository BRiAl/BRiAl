//
//  Interpolation
//
//  Created by Michael Brickenstein on 2007-07-02.
//  Copyright (c) 2007 The PolyBoRi Team. See LICENSE file.

#include <polybori/groebner/groebner_defs.h>
#include <polybori/cache/CCacheManagement.h>
#include <polybori/cache/CacheManager.h>
#include <algorithm>
#include <iostream>
#include <polybori/groebner/interpolate.h>
#include <polybori/groebner/randomset.h>
#include <polybori/groebner/nf.h>
#include <polybori/groebner/add_up.h>

BEGIN_NAMESPACE_PBORIGB

std::vector<Polynomial> variety_lex_groebner_basis(const MonomialSet& points,const Monomial & variables){
    MonomialSet leads=variety_lex_leading_terms(points,variables);
    std::vector<Polynomial> res;
    MonomialSet::const_iterator it=leads.begin();
    MonomialSet::const_iterator end=leads.end();
    while(it!=end){
        Monomial lm=*it;
        Polynomial p=nf_lex_points(lm,points);
        res.push_back(lm+p);
        ++it;
    }
    return res;
}
        

            
MonomialSet nf_lex_points(const Polynomial& f,const MonomialSet& p){
    MonomialSet z=zeros(f,p);
    return interpolate_smallest_lex(z,p.diff(z));
}
MonomialSet gen_random_subset(const BoolePolyRing& ring,
			      const std::vector<Monomial>& vec,bool_gen_type&
bit_gen){
    std::vector<Monomial> chosen;
    std::vector<Monomial>::const_iterator it=vec.begin();
    std::vector<Monomial>::const_iterator end=vec.end();
    while(it!=end){
        if (bit_gen()){
            chosen.push_back(*it);
        }
        ++it;
    }
    return add_up_monomials(chosen, ring.zero()).diagram();
}
MonomialSet random_interpolation(const MonomialSet& as_set, const std::vector<Monomial>& as_vector, bool_gen_type& bit_gen){
  MonomialSet s1=gen_random_subset(as_set.ring(), as_vector,bit_gen);
    return interpolate_smallest_lex(as_set.diff(s1),s1);
}
MonomialSet variety_lex_leading_terms(const MonomialSet& points, const Monomial& variables){
    base_generator_type generator(static_cast<unsigned int>(std::time(0)));

    std::vector<Monomial> points_vec(points.size(), points.ring());

    std::copy(points.begin(),points.end(),points_vec.begin());
    bool_gen_type bit_gen(generator,distribution_type(0,1));
    MonomialSet vars_div=variables.divisors();
    MonomialSet standards(points.ring());
    if (points!=vars_div){
      standards = points.ring().one();
    }
    unsigned int len_standards=standards.size();
    unsigned int n_points=points.size();
    MonomialSet standards_old=standards;
    while (len_standards<n_points){
        
        standards=standards.unite(random_interpolation(points,points_vec,bit_gen));
        if (standards!=standards_old){
            standards=include_divisors(standards);
            len_standards=standards.size();
            standards_old=standards;
        }
    }
    MonomialSet res=vars_div.diff(standards);
    res=res.minimalElements();
    return res;
}
MonomialSet zeros(Polynomial p, MonomialSet candidates){
    MonomialSet s=p.diagram();
    MonomialSet result(s.ring());
    MonomialSet::navigator p_nav=s.navigation();
    if (candidates.isZero()) return candidates;
    if (p.isOne()) return MonomialSet(p.ring());
    if (p.isZero()) return candidates;
    if (Polynomial(candidates).isOne()){
      if (p.hasConstantPart()) return MonomialSet(p.ring());
        else return candidates;
    }
    MonomialSet::navigator can_nav=candidates.navigation();
    idx_type index=*can_nav;
    while ((*p_nav)<index){
        p_nav.incrementElse();
    }
    typedef PBORI::CacheManager<CCacheTypes::zeros>
      cache_mgr_type;
    cache_mgr_type cache_mgr(candidates.ring());
    MonomialSet::navigator cached=cache_mgr.find(p_nav, can_nav);
    if (cached.isValid() ){
      return cache_mgr.generate(cached);
    }
    s=MonomialSet(cache_mgr.generate(p_nav));
    
    MonomialSet p1=s.subset1(index);
    MonomialSet p0=s.subset0(index);
    MonomialSet c1=candidates.subset1(index);
    MonomialSet c0=candidates.subset0(index);
    
    MonomialSet z00=zeros(p0,c0);
    MonomialSet z01=zeros(p0,c1);
    MonomialSet z11=zeros(p1,c1);
    //MonomialSet then_branch=z01.intersect(z11).unite(c1.diff(z01.unite(z11)));
    MonomialSet then_branch=c1.diff(z01.Xor(z11));
    PBORI_ASSERT (*then_branch.navigation()>index);
    PBORI_ASSERT (*z00.navigation()>index);
    result=MonomialSet(index,then_branch,z00);
    cache_mgr.insert(p_nav,can_nav,result.navigation());
    return result;
}
Polynomial interpolate(MonomialSet to_zero,MonomialSet to_one){
    //std::cout<<"to_one:"<<(Polynomial) to_one<<"to_zero:"<<(Polynomial) to_zero<<std::endl;
    PBORI_ASSERT(to_zero.intersect(to_one).isZero());

    typedef PBORI::CacheManager<CCacheTypes::interpolate>
      cache_mgr_type;
    cache_mgr_type cache_mgr(to_zero.ring());
    if (to_zero.isZero()) return cache_mgr.one();
    if (to_one.isZero()) return cache_mgr.zero();

    MonomialSet::navigator cached=cache_mgr.find(to_zero.navigation(), to_one.navigation());
    if (cached.isValid() ){
      return cache_mgr.generate(cached);
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
    PBORI_ASSERT(to_zero.intersect(to_one).isZero());

    typedef PBORI::CacheManager<CCacheTypes::interpolate_smallest_lex>
      cache_mgr_type;
    cache_mgr_type cache_mgr;

    if (to_zero.isZero()) return cache_mgr.one();
    if (to_one.isZero()) return cache_mgr.zero();

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
    if (to_zerou.intersect(to_oneu).isZero()){
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
    PBORI_ASSERT(to_zero.intersect(to_one).isZero());

    typedef PBORI::CacheManager<CCacheTypes::interpolate_smallest_lex>
      cache_mgr_type;
    cache_mgr_type cache_mgr(to_zero.ring());
    if (to_zero.isZero()) return cache_mgr.one();
    if (to_one.isZero()) return cache_mgr.zero();

    MonomialSet::navigator cached=cache_mgr.find(to_zero.navigation(), to_one.navigation());
    if (cached.isValid() ){
      return cache_mgr.generate(cached);
    }
  
    idx_type index=std::min(*to_zero.navigation(),*to_one.navigation());
    MonomialSet to_zero1=to_zero.subset1(index);
    MonomialSet to_zero0=to_zero.subset0(index);
    MonomialSet to_one1=to_one.subset1(index);
    MonomialSet to_one0=to_one.subset0(index);
    MonomialSet to_zerou=to_zero1.unite(to_zero0);
    MonomialSet to_oneu=to_one1.unite(to_one0);
    
    MonomialSet result(to_zero.ring());
    if (to_zerou.intersect(to_oneu).isZero()){
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
        not_affected=zeros(p1,not_affected);
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
    cache_mgr_type cache_mgr(m.ring());
    MonomialSet::navigator cached=cache_mgr.find(nav);
    if (cached.isValid() ){
      return cache_mgr.generate(cached);
    }
    MonomialSet::navigator tb=nav.thenBranch();
    MonomialSet::navigator eb=nav.elseBranch();
    MonomialSet itb=include_divisors(cache_mgr.generate(tb));
    MonomialSet r0=include_divisors(cache_mgr.generate(eb)).unite(itb);
    MonomialSet result=MonomialSet(*nav,itb,r0);
    cache_mgr.insert(nav,result.navigation());
    return result;
}
END_NAMESPACE_PBORIGB
