/*
 *  groebner_alg.cc
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 20.04.06.
 *  Copyright 2006 The PolyBoRi Team. See LICENSE file.
 *
 */

#include <polybori/groebner/CountCriterion.h>
#include <polybori/groebner/ChainCriterion.h>
#include <polybori/groebner/ChainVariableCriterion.h>
#include <polybori/groebner/HasTRepOrExtendedProductCriterion.h>
#include <polybori/groebner/SetBitUInt.h>
#include <polybori/groebner/ZeroFunction.h>
#include <polybori/groebner/ShorterEliminationLength.h>
#include <polybori/groebner/ShorterEliminationLengthModified.h>

#include <polybori/groebner/minimal_elements.h>
#include <polybori/groebner/contained_variables.h>


#include <polybori/groebner/groebner_alg.h>
#include <polybori/groebner/ll_red_nf.h>
#include <polybori/groebner/nf.h>
#include <polybori/groebner/interpolate.h>
#include <algorithm>
#include <set>

#define DANGEROUS_FIXED_PATH 1


#include <polybori/cache/CacheManager.h>


BEGIN_NAMESPACE_PBORIGB

//static MonomialSet divide_monomial_divisors_out(const BooleSet& s, const Monomial& lm);


//Variant for navigaor
template <class CacheMgr>
MonomialSet mod_var_set(const CacheMgr& cache_mgr,
                        MonomialSet::navigator a, MonomialSet::navigator v){

  idx_type a_index=*a;
  idx_type v_index=*v;
  while((v_index<a_index)||((v_index==a_index)&&(!(v.isConstant())))){
    if (v_index<a_index)
      v.incrementElse();
    if (v_index==a_index){
      a.incrementElse();
      v.incrementElse();
    }
    v_index=*v;
    a_index=*a;
  }

  if (v_index>a_index){
    if (v.isConstant()) return cache_mgr.generate(a);

    MonomialSet::navigator cached =
      cache_mgr.find(a, v);
    if (cached.isValid()) return cache_mgr.generate(cached);
    MonomialSet::navigator a_e=a.elseBranch();
    MonomialSet::navigator a_t=a.thenBranch();
    MonomialSet a0=mod_var_set(cache_mgr, a_e, v);
    MonomialSet a1=mod_var_set(cache_mgr, a_t, v);
    MonomialSet result(a0.ring());
    if (a1.isZero()) result=a0;
    else {
      if ((a1.navigation()==a_t)&&(a0.navigation()==a_e))
        result=cache_mgr.generate(a);
      else
        result=MonomialSet(a_index,a1,a0);
    }
    cache_mgr.insert(a,v,result.navigation());
    return result;
  }
  //so v_index==a_index and v.IsConstant
  return cache_mgr.generate(a);
}

// Variant for MonomialSet
MonomialSet mod_var_set(const MonomialSet& as, const MonomialSet& vs) {
  MonomialSet::navigator a=as.navigation();
  MonomialSet::navigator v=vs.navigation();
  typedef PBORI::CacheManager<CCacheTypes::mod_varset>
    cache_mgr_type;

  cache_mgr_type cache_mgr(as.ring());
  return mod_var_set(cache_mgr, a, v);
}

// Forward declaration
MonomialSet mod_deg2_set(const MonomialSet& as, const MonomialSet &vs);

template <class CacheMgr>
MonomialSet mod_deg2_set(const CacheMgr& cache_mgr, 
                         MonomialSet::navigator a, MonomialSet::navigator v){

  idx_type a_index=*a;
  idx_type v_index=*v;
  if (a.isConstant()) return cache_mgr.generate(a);
  while((v_index=*v)<(a_index=*a)){
        v.incrementElse();
    }
  if (v.isConstant()) return cache_mgr.generate(a);

  MonomialSet::navigator cached =
    cache_mgr.find(a, v);
  if (cached.isValid()) return cache_mgr.generate(cached);

  MonomialSet result(cache_mgr.zero());
  if (a_index==v_index){
    MonomialSet tmp = mod_var_set(cache_mgr.generate(a.thenBranch()), 
                                  cache_mgr.generate(v.thenBranch()));
    result=MonomialSet(a_index,
                       mod_deg2_set(cache_mgr, 
                                    tmp.navigation(), v.elseBranch()),
                       mod_deg2_set(cache_mgr, a.elseBranch(), v.elseBranch())
                       );
    
  } else {
    PBORI_ASSERT(v_index>a_index);
    result=MonomialSet(a_index,
                       mod_deg2_set(cache_mgr, a.thenBranch(), v),
                       mod_deg2_set(cache_mgr, a.elseBranch(), v) );
  }
  cache_mgr.insert(a,v,result.navigation());
  return result;
}

// Variant for MonomialSet
MonomialSet mod_deg2_set(const MonomialSet& as, const MonomialSet &vs){
  MonomialSet::navigator a=as.navigation();
  MonomialSet::navigator v=vs.navigation();
  typedef PBORI::CacheManager<CCacheTypes::mod_deg2_set>
    cache_mgr_type;
  cache_mgr_type cache_mgr(as.ring());

  return mod_deg2_set(cache_mgr, a, v);
}

/// assumes that divisibility condition is fullfilled

Polynomial reduce_by_monom(const Polynomial& p, const Monomial& m){
  
  if (m.deg()==1){
    //cout<<"branch 1\n";
    //cout.flush();
    Monomial::const_iterator it=m.begin();
    return Polynomial(BooleSet(p).subset0(*it));
  }
#if 0
  //buggy variant 1
  return p%m;
#endif
#if 1
  //working variant

  Monomial::const_iterator it=m.begin();
  Monomial::const_iterator end=m.end();
  BooleSet dividing_terms
  //=BooleSet(p/m);
  =BooleSet(p);
  
  while(it!=end){
    dividing_terms=dividing_terms.subset1(*it);
    it++;
  }
#endif
#if 0
  //buggy variant 2
  if (p==Polynomial(m))
    return p.ring().zero();
  BooleSet dividing_terms
    =BooleSet(p/m);
    
#endif
  //fast multiply back
  //cout<<"branch2\n";
  //cout.flush();
  
  dividing_terms = dividing_terms.unateProduct(m.diagram());
  return Polynomial(BooleSet(p).diff(dividing_terms));
  //return Polynomial(BooleSet(p).diff(BooleSet(m*Polynomial(dividing_terms))));
}



Polynomial reduce_complete(const Polynomial& p, const Polynomial& reductor){

  
  Monomial p_lead=p.lead();
  Monomial reductor_lead=reductor.lead();
  Polynomial reductor_tail=reductor-reductor_lead;

  
  
  //Polynomial p_reducible_base=p;
  Monomial::const_iterator it=reductor_lead.begin();
  Monomial::const_iterator end=reductor_lead.end();
  
  BooleSet dividing_terms
    //=BooleSet(p/m);
    =BooleSet(p);
  
  while(it!=end){
    dividing_terms=dividing_terms.subset1(*it);
    it++;
  }
  
  
  
  Polynomial canceled_lead(BooleSet(p).diff(dividing_terms.unateProduct(reductor_lead.diagram())));
  
  Polynomial::const_iterator it_r=reductor_tail.begin();
  Polynomial::const_iterator end_r=reductor_tail.end();
  Polynomial res=canceled_lead;
  while(it_r!=end_r){
    Monomial m=(*it_r);
    Monomial b_p_gcd=m.GCD(reductor_lead);
    Polynomial first_mult_half=dividing_terms.unateProduct(BooleSet(Polynomial(b_p_gcd)));
    Polynomial multiplied=(m/b_p_gcd)*first_mult_half;
    res+=multiplied;
    ++it_r;
    
  }
  return res;
}


Polynomial reduce_complete(const Polynomial &p, const PolyEntry& reductor, wlen_type & len){
  //cout<<"red complete"<<endl;
  if (reductor.length==2){
      return reduce_by_binom(p,reductor.p);
  }
  MonomialSet rewriteable_terms_divided=p.diagram();
  Exponent::const_iterator it=reductor.leadExp.begin();
  Exponent::const_iterator end=reductor.leadExp.end();
  while (it!=end){
    rewriteable_terms_divided=rewriteable_terms_divided.subset1(*it);
    it++;
  }
  //divide_monomial_divisors_out(p.diagram(),reductor.lm);
  //Polynomial res=p-reductor.lm*(Polynomial)rewriteable_terms_divided;
  Polynomial factor_reductor=reductor.p;//tail;
  size_t factor_reductor_len=reductor.length;//factor_reductor.length();
  size_t rewriteable_terms_len=rewriteable_terms_divided.length();
  len=len+(factor_reductor_len-2)*rewriteable_terms_len;
  Polynomial product=
#if 1
    factor_reductor*(Polynomial) rewriteable_terms_divided;
    //multiply_recursively2(factor_reductor,rewriteable_terms_divided);
#else
    multiply(factor_reductor,factor_reductor_len,rewriteable_terms_divided,rewriteable_terms_len);
#endif

  /*if (factor_reductor_len<rewriteable_terms_len){
    product=factor_reductor*((Polynomial)rewriteable_terms_divided);
  
  } else {
    product=((Polynomial)rewriteable_terms_divided)*((Polynomial) factor_reductor);
  }*/
  //res=res+product;
  return p+product;
}









Polynomial translate_indices(const Polynomial& p, const std::vector<idx_type>& table){
    if (p.isConstant()) return p;
    int index=*(p.navigation());
    int index_mapped=table[index];
    
#if 0
return
  dd_backward_transform(p.navigation(), p.ring().zero(), 
                            mapped_new_node<std::vector<idx_type>,
                            Variable, Monomial, Polynomial>(table), 
                            integral_constant<bool, true, Polynomial>());


#else

    MonomialSet s1=p.diagram().subset1(index);
    MonomialSet s0=p.diagram().subset0(index);
    if (s1!=s0){
      s1=translate_indices(s1,table);
      s1=s1.change(index_mapped);
      s0=translate_indices(s0,table);
    } else {
      s0=translate_indices(s0,table);
      s1=s0.change(index_mapped);
      
    }
    return s1.unite(s0);
    //we can assume that index_mapped does not occur in the translated subpolynomial, in which index index does not occurr
   // return translate_indices(p.diagram().subset1(index),table).diagram().change(index_mapped).unite(
  //    translate_indices(p.diagram().subset0(index), table));
#endif


}







//static Monomial oper(int i){
//    return Monomial(Variable(i));
//}
// static MonomialSet divide_monomial_divisors_out_old(const MonomialSet& s, const Monomial& lm){
//   /// deactivated, because segfaults on SatTestCase, AD
//   ///   return s.existAbstract(lm.diagram());
//   /**/
//     MonomialSet m=s;
//     Monomial::const_iterator it_lm=lm.begin();
//         Monomial::const_iterator end_lm=lm.end();
//         while(it_lm!=end_lm){
//             idx_type i=*it_lm;
//             m=m.subset0(i).unite(m.subset1(i));
//             it_lm++;
//         }
//    return m;/**/
// }
// static MonomialSet do_divide_monomial_divisors_out(const MonomialSet& s, Monomial::const_iterator it, Monomial::const_iterator end){
//     if(it==end) return s;
//     if (s.isZero()) return s;
    
//     Monomial::const_iterator itpp=it;
//     itpp++;
//     return (do_divide_monomial_divisors_out(s.subset0(*it),itpp,end).unite(do_divide_monomial_divisors_out(s.subset1(*it),itpp,end)));
    
// }

// static MonomialSet divide_monomial_divisors_out(const BooleSet& s, const Monomial& lm){
//     return do_divide_monomial_divisors_out(s,lm.begin(),lm.end());
// }


// Core function which uses the manager given as firt argument
template <class MgrType>
MonomialSet recursively_insert(const MgrType& mgr,
                               MonomialSet::navigator p, 
                               idx_type idx, MonomialSet::navigator m){
    //MonomialSet::navigation nav=m.navigator();
    if (idx>*m){
      return MonomialSet(*m,recursively_insert(mgr, p,idx,m.thenBranch()),
                         MonomialSet( m.elseBranch(), 
                                      MonomialSet::ring_type(mgr)));
    } else{
        PBORI_ASSERT(idx<*m);
        return MonomialSet(idx, m, p, MonomialSet::ring_type(mgr));
    }
}

// Wrapper for external use
MonomialSet recursively_insert(MonomialSet::navigator p, idx_type idx, MonomialSet mset){
  return recursively_insert(mset.ring(), p, idx, mset.navigation());
}






template <class CacheMgr>
Polynomial map_every_x_to_x_plus_one(const CacheMgr& cache_mgr,
                                     MonomialSet::navigator nav){

    if (nav.isConstant()) return cache_mgr.generate(nav);
    
    idx_type idx=*nav;

    MonomialSet::navigator cached=cache_mgr.find(nav);
    if (cached.isValid() ){
      return cache_mgr.generate(cached);
    }
    
    Polynomial then_mapped = 
      map_every_x_to_x_plus_one(cache_mgr, nav.thenBranch());
    Polynomial res0 = 
      map_every_x_to_x_plus_one(cache_mgr, nav.elseBranch()) + 
      Polynomial(then_mapped);
    Polynomial res = 
      MonomialSet(idx,
                  map_every_x_to_x_plus_one(cache_mgr, 
                                            nav.thenBranch()).diagram(), 
                  res0.diagram());

    cache_mgr.insert(nav,res.diagram().navigation());
    return res;
}

Polynomial map_every_x_to_x_plus_one(Polynomial p){
  MonomialSet::navigator nav=p.diagram().navigation();
  typedef PBORI::CacheManager<CCacheTypes::map_every_x_to_x_plus_one>
    cache_mgr_type;
  
  cache_mgr_type cache_mgr(p.ring());
  return map_every_x_to_x_plus_one(cache_mgr, nav);
}




Polynomial mult_fast_sim(const std::vector<Polynomial>& vec,
			 const BoolePolyRing& ring){
    std::vector<Polynomial> new_vec;
    
    int i;
    int s=vec.size();
    Polynomial::idx_type index=-1;
    for(i=0;i<s;i++){
      if (vec[i].isZero()) return vec[i]; // is 0
        if (!(vec[i].isOne())){
    
            new_vec.push_back(vec[i]);
    
            if (index<0){
                index=*(vec[i].navigation());
    
                }
            else {
    
                //int a_i=*p.navigation();
                index=std::min(index,*(vec[i].navigation()));
            }
        }
    }
    
    
    if (new_vec.size()==0) return Polynomial(1, ring);
    if (new_vec.size()==1) return new_vec[0];
    s=new_vec.size();
    std::vector<Polynomial> s0_vec(s, ring);
    std::vector<Polynomial> s1_vec(s, ring);
    for(i=0;i<s;i++){
        s0_vec[i]=new_vec[i].diagram().subset0(index);
        s1_vec[i]=new_vec[i].diagram().subset1(index).Xor(new_vec[i].diagram().subset0(index));
    }
    
    Polynomial s0=mult_fast_sim(s0_vec, ring);
    Polynomial s1=mult_fast_sim(s1_vec, ring);
    
    return ((Monomial) Variable(index, s0.ring()))*(s1+s0)+s0;
}
std::vector<Polynomial> full_implication_gb(const Polynomial & p, CacheManager& cache, GroebnerStrategy& strat_param){
    bool succ;
    CacheManager::res_type cache_res=cache.lookup(p,succ);
    if (!(succ)){
      GroebnerStrategy strat(p.ring());
    strat.cache=strat_param.cache;
    //strat.log("Recursive call");
    strat.optAllowRecursion=true;
    strat.addGenerator(p);
    strat.symmGB_F2();
    std::vector<Polynomial> res=strat.minimalizeAndTailReduce();
    cache.insert(p,res);
    return res;
    } else
    {   
        strat_param.log("used dynamic cache");
        std::vector<Polynomial> res(*cache_res);
        return res;
    }
}
END_NAMESPACE_PBORIGB
