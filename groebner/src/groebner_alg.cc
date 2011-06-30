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


#include <polybori/groebner/groebner_alg.h>
#include <polybori/groebner/nf.h>
#include <polybori/groebner/interpolate.h>
#include <algorithm>
#include <set>

#define DANGEROUS_FIXED_PATH 1


#include <polybori/cache/CacheManager.h>


BEGIN_NAMESPACE_PBORIGB
static MonomialSet divide_monomial_divisors_out(const BooleSet& s, const Monomial& lm);
//static MonomialSet minimal_elements_cudd_style(MonomialSet m);
static MonomialSet do_minimal_elements_cudd_style(MonomialSet m, MonomialSet mod);


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
    assert(v_index>a_index);
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


MonomialSet contained_variables_cudd_style(const MonomialSet& m){
    
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

MonomialSet contained_deg2_cudd_style(const MonomialSet& m){
    
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







static std::vector<idx_type> contained_variables(const MonomialSet& m){
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

MonomialSet minimal_elements_internal(const MonomialSet& s){
    if (s.isZero()) return s;
    if (Polynomial(s).isOne()) return s;
    MonomialSet::navigator nav=s.navigation();
    int i=*nav;
    
    
    if (Polynomial(s).hasConstantPart()) return MonomialSet(Polynomial(true, s.ring()));
    int l=s.length();
    if (l<=1) {
        return s;
    }
    
    if(l==2){
        MonomialSet::const_iterator it=s.begin();
        Monomial a=*it;
        Monomial b=*(++it);
        if (a.reducibleBy(b)) return MonomialSet(b.diagram());
        else return s;
    }
    
    MonomialSet s0_raw=s.subset0(i);
    MonomialSet s0=minimal_elements_internal(s0_raw);
    MonomialSet s1=minimal_elements_internal(s.subset1(i).diff(s0_raw));
    if (!(s0.isZero())){
      s1=s1.diff(s0.unateProduct(Polynomial(s1).usedVariablesExp().divisors(s.ring())));
        
    }
    return s0.unite(s1.change(i));

}

MonomialSet minimal_elements_internal2(MonomialSet s){
    if (s.isZero()) return s;
    if (Polynomial(s).isOne()) return s;
    
    
    
    
    if (Polynomial(s).hasConstantPart()) 
      return MonomialSet(Polynomial(true, s.ring()));
    MonomialSet result(s.ring());
    std::vector<idx_type> cv=contained_variables(s);
    if ((cv.size()>0) && (s.length()==cv.size())){
        return s;
    } else {
    
        int z;
        MonomialSet cv_set(s.ring());
        for(z=cv.size()-1;z>=0;z--){
            Monomial mv=Variable(cv[z], s.ring());
            cv_set=cv_set.unite(mv.diagram());
        }
        for(z=0;z<cv.size();z++){
            s=s.subset0(cv[z]);
        }
        result=cv_set;
    }
    
    if (s.isZero()) return result;
    assert(!(Polynomial(s).hasConstantPart()));
    
    
    
    MonomialSet::navigator nav=s.navigation();
    idx_type i;
    #if 1
    
    //first index of ZDD
    i=*nav;
    #else
    
    //first index of least lex. term
    while(!(nav.isConstant())){
        i=*nav;
        nav.incrementElse();
    }
    #endif
    
    
    
    /*MonomialSet s0=minimal_elements_internal2(s.subset0(i));
    MonomialSet s1=s.subset1(i);
    if ((s0!=s1)&&(!(s1.diff(s0).isZero()))){
        s1=minimal_elements_internal2(s1.unite(s0)).diff(s0);
    } else return s0;
    return s0.unite(s1.change(i));*/
    
    MonomialSet s0_raw=s.subset0(i);
    MonomialSet s0=minimal_elements_internal2(s0_raw);
    MonomialSet s1=minimal_elements_internal2(s.subset1(i).diff(s0_raw));
    if (!(s0.isZero())){
        s1=s1.diff(s0.unateProduct(Polynomial(s1).usedVariables().divisors()));
        
    }
    return s0.unite(s1.change(i)).unite(result);

}
std::vector<Exponent> minimal_elements_internal3(MonomialSet s){
    std::vector<Exponent> result;
    if (s.isZero()) return result;
    if ((Polynomial(s).isOne()) || (Polynomial(s).hasConstantPart())){
        result.push_back(Exponent());
        return result;
    }
    std::vector<idx_type> cv=contained_variables(s);
    int i;
    for(i=0;i<cv.size();i++){
            s=s.subset0(cv[i]);
            Exponent t;
            t.insert(cv[i]);
            result.push_back(t);
    }
    
    if (s.isZero()){
        return result;
    } else {
        std::vector<Exponent> exponents;
        //Pol sp=s;
        exponents.insert(exponents.end(), s.expBegin(),s.expEnd());
        int nvars=s.ring().nVariables();
        std::vector<std::vector<int> > occ_vecs(nvars);
        for(i=0;i<exponents.size()-1;i++){
            Exponent::const_iterator it=((const Exponent&) exponents[i]).begin();
            Exponent::const_iterator end=((const Exponent&) exponents[i]).end();
            while(it!=end){
                occ_vecs[*it].push_back(i);
                it++;
            }
        }
        //now exponents are ordered
        /*std::vector<std::set<int> > occ_sets(nvars);
        for(i=occ_sets.size()-1;i>=0;i--){
            occ_sets[i].insert(occ_vecs[i].begin(),occ_vecs[i].end());
        }*/
        std::vector<bool> still_minimal(exponents.size());
        for(i=exponents.size()-1;i>=0;i--){
            still_minimal[i]=true;
        }
        int result_orig=result.size();
        //cout<<"orig:"<<exponents.size()<<endl;
        //lex smalles is last so backwards
        for(i=exponents.size()-1;i>=0;i--){
            if (still_minimal[i]){
                //we assume, that each exponents has deg>0
                Exponent::const_iterator it=((const Exponent&) exponents[i]).begin();
                Exponent::const_iterator end=((const Exponent&) exponents[i]).end();
                int first_index=*it;
                std::vector<int> occ_set=occ_vecs[first_index];
                it++;
                while(it!=end){
                    
                    std::vector<int> occ_set_next;
                    set_intersection(
                        occ_set.begin(),
                        occ_set.end(),
                        occ_vecs[*it].begin(), 
                        occ_vecs[*it].end(),
                        std::back_insert_iterator<std::vector<int> >(occ_set_next));
                    occ_set=occ_set_next;
                    it++;
                }
                
                std::vector<int>::const_iterator oc_it= occ_set.begin();
                std::vector<int>::const_iterator  oc_end= occ_set.end();
                while(oc_it!=oc_end){
                    still_minimal[*oc_it]=false;
                    oc_it++;
                }
                
                it=((const Exponent&) exponents[i]).begin();
                while(it!=end){
                    std::vector<int> occ_set_difference;
                    set_difference(
                        occ_vecs[*it].begin(),
                        occ_vecs[*it].end(),
                        occ_set.begin(),
                        occ_set.end(),
                        std::back_insert_iterator<std::vector<int> >(occ_set_difference));
                    occ_vecs[*it]=occ_set_difference;
                    it++;
                }
                result.push_back(exponents[i]);
            }
        }
        //cout<<"after:"<<result.size()-result_orig<<endl;
        
    }
    return result;
}
MonomialSet minimal_elements(const MonomialSet& s){
#if 0
    //return minimal_elements_internal2(s);
    return minimal_elements_cudd_style_unary(s);
#else
#if 1
  return s.minimalElements();
#else
  MonomialSet minElts = minimal_elements_internal(s);

  if (minElts!=s.minimalElements()){

    std::cout <<"ERROR minimalElements"<<std::endl;
    std::cout <<"orig "<<s<< std::endl;
    std::cout <<"correct "<<minElts<< std::endl;
    std::cout <<"wrong "<<s.minimalElements()<< std::endl;
  }


  return minElts;
#endif
#endif
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
static MonomialSet divide_monomial_divisors_out_old(const MonomialSet& s, const Monomial& lm){
  /// deactivated, because segfaults on SatTestCase, AD
  ///   return s.existAbstract(lm.diagram());
  /**/
    MonomialSet m=s;
    Monomial::const_iterator it_lm=lm.begin();
        Monomial::const_iterator end_lm=lm.end();
        while(it_lm!=end_lm){
            idx_type i=*it_lm;
            m=m.subset0(i).unite(m.subset1(i));
            it_lm++;
        }
   return m;/**/
}
static MonomialSet do_divide_monomial_divisors_out(const MonomialSet& s, Monomial::const_iterator it, Monomial::const_iterator end){
    if(it==end) return s;
    if (s.isZero()) return s;
    
    Monomial::const_iterator itpp=it;
    itpp++;
    return (do_divide_monomial_divisors_out(s.subset0(*it),itpp,end).unite(do_divide_monomial_divisors_out(s.subset1(*it),itpp,end)));
    
}
static MonomialSet divide_monomial_divisors_out(const BooleSet& s, const Monomial& lm){
    return do_divide_monomial_divisors_out(s,lm.begin(),lm.end());
}


MonomialSet minimal_elements_cudd_style_unary(MonomialSet m){

  if (m.isZero()) return m;
  
  if (m.ownsOne()) return Polynomial(1, m.ring()).diagram();

  MonomialSet::navigator m_nav=m.navigation();
  MonomialSet::navigator ms0=m_nav.elseBranch();
  MonomialSet::navigator ms1=m_nav.thenBranch();

  
  typedef PBORI::CacheManager<CCacheTypes::minimal_elements>
    cache_mgr_type;



  cache_mgr_type cache_mgr(m.ring());
  PBORI::BoolePolynomial::navigator cached =
    cache_mgr.find(m_nav);


      
  if (cached.isValid() ){
    return cache_mgr.generate(cached);
  }
  
  MonomialSet minimal_else=minimal_elements_cudd_style_unary(cache_mgr.generate(ms0));
  MonomialSet minimal_then=minimal_elements_cudd_style_unary(mod_mon_set(cache_mgr.generate(ms1),minimal_else));
  MonomialSet result(m.ring());
  if ((minimal_else.navigation()==ms0) &&(minimal_then.navigation()==ms1)) result=m;
  else
    result= MonomialSet(*m_nav,minimal_then,minimal_else);//result0.unite(result1.change(index));

  cache_mgr.insert(m_nav, result.navigation());
  return result;
}

MonomialSet do_minimal_elements_cudd_style(MonomialSet m, MonomialSet mod){
  Polynomial p_mod=mod;
  if (m.isZero()) return m;
  if (mod.ownsOne())
    return MonomialSet(mod.ring());
  if (m.ownsOne()) return Polynomial(1,m.ring()).diagram();
  MonomialSet mod_cv=contained_variables_cudd_style(mod);
  m=mod_var_set(m,mod_cv);
  mod=mod_var_set(mod,mod_cv);
  MonomialSet mod_d2=contained_deg2_cudd_style(mod);
  mod=mod_deg2_set(mod,mod_d2);
  m=mod_deg2_set(m,mod_d2);
  MonomialSet cv=contained_variables_cudd_style(m);
  MonomialSet cv_orig=cv;
  cv=cv.diff(mod);
  mod=mod_var_set(mod,cv_orig);
  m=mod_var_set(m,cv_orig);
  m=m.diff(mod);
  if (m.isZero()) return cv;
  bool cv_empty=cv.isZero();
  
  MonomialSet result(m.ring());
  int index=*m.navigation();
  
  
  
  
  if (!mod.isZero())
  {
    MonomialSet::navigator nav_mod=mod.navigation();
    while((!(nav_mod.isConstant())) && (index>*nav_mod)){
      nav_mod.incrementElse();
     
    }
    mod=MonomialSet(nav_mod, m.ring());
  }
  
  MonomialSet::navigator m_nav=m.navigation();
  MonomialSet::navigator ms0=m_nav.elseBranch();
  MonomialSet::navigator ms1=m_nav.thenBranch();
  MonomialSet::navigator mod_nav=mod.navigation();
  
  typedef PBORI::CacheManager<CCacheTypes::minimal_mod>
    cache_mgr_type;



  cache_mgr_type cache_mgr(m.ring());
  PBORI::BoolePolynomial::navigator cached =
    cache_mgr.find(m_nav, mod_nav);


      
  if (cached.isValid() ){
    return cv.unite((MonomialSet)cache_mgr.generate(cached));
  }
  
  if (mod.isZero()){
    
    MonomialSet result0=do_minimal_elements_cudd_style(cache_mgr.generate(ms0),
                                                       mod); 
    MonomialSet result1= do_minimal_elements_cudd_style(cache_mgr.generate(ms1),
                                                        result0);
    result= MonomialSet(index,result1,result0);//result0.unite(result1.change(index));
     
  } else {
      if (*mod_nav>index){
        MonomialSet
          result0=do_minimal_elements_cudd_style(cache_mgr.generate(ms0), mod);
        MonomialSet result1= do_minimal_elements_cudd_style(
          cache_mgr.generate(ms1),result0.unite(mod));
        if (result1.isZero()) {result=result0;}
        else
          {result= MonomialSet(index,result1,result0);}
      } else {
      assert(index==*mod_nav);
      MonomialSet::navigator mod0=mod_nav.elseBranch();
      MonomialSet::navigator mod1=mod_nav.thenBranch();
      MonomialSet
        result0=do_minimal_elements_cudd_style(cache_mgr.generate(ms0), cache_mgr.generate(mod0));
      //MonomialSet mod1=mod.subset1(index);
      MonomialSet result1=
        do_minimal_elements_cudd_style(cache_mgr.generate(ms1), 
                                       result0.unite(cache_mgr.generate(ms0).unite(cache_mgr.generate(mod1))));
      result= MonomialSet(index,result1,result0);//result0.unite(result1.change(index));
    }
  }
  cache_mgr.insert(m.navigation(), mod.navigation(), result.navigation());
  if (cv_empty) return result;
  else
    return cv.unite(result);
}
MonomialSet minimal_elements_cudd_style(MonomialSet m){
  return do_minimal_elements_cudd_style(m, MonomialSet(m.ring()));
}


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
        assert(idx<*m);
        return MonomialSet(idx, m, p, MonomialSet::ring_type(mgr));
    }
}

// Wrapper for external use
MonomialSet recursively_insert(MonomialSet::navigator p, idx_type idx, MonomialSet mset){
  return recursively_insert(mset.ring(), p, idx, mset.navigation());
}

void addPolynomialToReductor(Polynomial& p, MonomialSet& m){
    Monomial lm=p.lead();
    assert (!(m.isZero()));
    idx_type lead_index=*(lm.begin());
    Exponent red_lead=*m.expBegin();
    if (std::find(red_lead.begin(),red_lead.end(),lead_index)==red_lead.end()){
        //this->log("linear lead reductor\n");
        p=ll_red_nf(p,m);
        assert(p.lead()==lm);
        m=ll_red_nf(m,MonomialSet(p.diagram())).diagram();
        //assert(ll_red_nf(m+m.lead(),m)==m+m.lead());
        m=recursively_insert(p.navigation().elseBranch(),
                             lead_index, m);
    }
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





static int get_first_variable_index(const Polynomial& p){
    return *(p.navigation());
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
