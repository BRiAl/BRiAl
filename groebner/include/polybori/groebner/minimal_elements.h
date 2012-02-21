// -*- c++ -*-
//*****************************************************************************
/** @file minimal_elements.h 
 *
 * @author Michael Brickenstein
 * @date 2011-06-30
 *
 * This file includes the definition of the @c minimal_elements functions..
 *
 * @par Copyright:
 *   (c) 2005-2011 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_minimal_elements_h_
#define polybori_groebner_minimal_elements_h_

// include definitions
#include "groebner_defs.h"
#include "contained_variables.h"
//#include "nf.h"

#include <vector>
#include <algorithm>

BEGIN_NAMESPACE_PBORIGB
MonomialSet mod_mon_set(const MonomialSet& as, const MonomialSet &vs); // nf.cc
MonomialSet mod_deg2_set(const MonomialSet& as, const MonomialSet &vs);  // groebner_alg.cc
MonomialSet mod_var_set(const MonomialSet& as, const MonomialSet& vs);   // groebner_alg.cc

inline MonomialSet
minimal_elements_internal(const MonomialSet& s){
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

inline MonomialSet
minimal_elements_internal2(MonomialSet s){
    if (s.isZero()) return s;
    if (Polynomial(s).isOne()) return s;
    
    
    
    
    if (Polynomial(s).hasConstantPart()) 
      return MonomialSet(Polynomial(true, s.ring()));
    MonomialSet result(s.ring());
    std::vector<idx_type> cv=contained_variables(s);
    if ((cv.size()>0) && (s.length()==cv.size())){
        return s;
    } else {
    
        MonomialSet::size_type z;
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
    PBORI_ASSERT(!(Polynomial(s).hasConstantPart()));
    
    
    
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

inline std::vector<Exponent>
minimal_elements_internal3(MonomialSet s){
    std::vector<Exponent> result;
    if (s.isZero()) return result;
    if ((Polynomial(s).isOne()) || (Polynomial(s).hasConstantPart())){
        result.push_back(Exponent());
        return result;
    }
    std::vector<idx_type> cv=contained_variables(s);
    for(MonomialSet::size_type i=0;i<cv.size();i++){
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
        for(MonomialSet::size_type i=0;i<exponents.size()-1;i++){
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
        for(MonomialSet::size_type i=exponents.size()-1;i>=0;i--){
            still_minimal[i]=true;
        }

        //lex smalles is last so backwards
        for(MonomialSet::size_type i=exponents.size()-1;i>=0;i--){
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
        
    }
    return result;
}

inline MonomialSet
minimal_elements(const MonomialSet& s){
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



inline MonomialSet
minimal_elements_cudd_style_unary(MonomialSet m){

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

inline MonomialSet
do_minimal_elements_cudd_style(MonomialSet m, MonomialSet mod){
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
      PBORI_ASSERT(index==*mod_nav);
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

inline MonomialSet
minimal_elements_cudd_style(MonomialSet m){
  return do_minimal_elements_cudd_style(m, MonomialSet(m.ring()));
}





inline MonomialSet
minimal_elements_multiplied_monoms(MonomialSet m, Monomial lm){
  
    if (m.divisorsOf(lm).isZero()){
       m = m.existAbstract(lm);
       m = minimal_elements_cudd_style_unary(m);

       return m.unateProduct(lm.set());
    }
    return lm.set() ;
}

inline std::vector<Monomial>
minimal_elements_multiplied(MonomialSet m, Monomial lm){ 

  MonomialSet result = minimal_elements_multiplied_monoms(m, lm);
  return std::vector<Monomial>(result.begin(), result.end());
}
  


//#define MIN_ELEMENTS_BINARY 1
#ifdef MIN_ELEMENTS_BINARY
inline MonomialSet
minimal_elements_divided(MonomialSet m, Monomial lm, MonomialSet mod){

    if (m.divisorsOf(lm).isZero()){
        m=divide_monomial_divisors_out(m,lm);
        //mod=divide_monomial_divisors_out(mod,lm);
        return do_minimal_elements_cudd_style(m,mod);
    }
    return m.ring().one();
}


#else

inline MonomialSet
minimal_elements_divided(MonomialSet m, Monomial lm, MonomialSet mod){

    if (m.divisorsOf(lm).isZero()){

        m=m.existAbstract(lm);
        mod=mod.existAbstract(lm);
        //mod=divide_monomial_divisors_out(mod,lm);
        m=mod_mon_set(m,mod);

        return minimal_elements_cudd_style_unary(m);
    }
    return m.ring().one();
}
#endif

inline void
minimal_elements_divided(MonomialSet m, Monomial lm, MonomialSet mod,
                         std::vector<Exponent>& result){
  MonomialSet elements = minimal_elements_divided(m, lm, mod);
  result.resize(elements.length());
  std::copy(elements.expBegin(), elements.expEnd(), result.begin());
}


END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_minimal_elements_h_ */
