/*
 *  literal_factorization.cc
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 29.05.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "literal_factorization.h"
#include <iostream>
#include <CCacheManagement.h>
BEGIN_NAMESPACE_PBORIGB

static Polynomial do_has_factor_x(const MonomialSet& m,const Variable& x){
    if (m.emptiness()) return true;
    MonomialSet::navigator nav=m.navigation();
    idx_type x_idx=x.index();
    while((nav.elseBranch().isEmpty()) && (!(nav.isConstant())) &&(*nav<x_idx)){
        nav.incrementThen();
    }

    idx_type m_idx=*nav;

    if (m_idx==x_idx){
        return nav.elseBranch().isEmpty()?1:0;
    } else {
        assert(!(nav.isEmpty()));
        if (m_idx>x_idx){
            return 0;//False
        } else {
            assert (*nav<x_idx);
            typedef PBORI::CacheManager<CCacheTypes::has_factor_x>
                 cache_mgr_type;
            
            cache_mgr_type cache_mgr;
            MonomialSet::navigator cached=cache_mgr.find(nav, ((Polynomial) x).diagram().navigation());
            if (cached.isValid() ){
                   return cached;
            }
            Polynomial res0=do_has_factor_x(nav.elseBranch(),x);
            Polynomial res=res0;
            if (res0==1){
                res=do_has_factor_x(nav.thenBranch(),x);
                
            }
            cache_mgr.insert(nav,((Polynomial) x).diagram().navigation(),res.diagram().navigation());
            return res;
        }
    }
}



static Polynomial do_left_equals_right_x_branch_and_r_has_fac_x(const MonomialSet& left,const MonomialSet& right, const Variable& x){
    if (left.emptiness()) return right.emptiness()?1:0;
    MonomialSet::navigator nav_l=left.navigation();
    MonomialSet::navigator nav_r=right.navigation();
    typedef PBORI::CacheManager<CCacheTypes::left_equals_right_x_branch_and_r_has_fac_x>
         cache_mgr_type;
    if (*nav_l<*nav_r) return 0;
    idx_type x_idx=x.index();
    if (*nav_r>x_idx) return 0;
    if ((*nav_l!=*nav_r) && (*nav_r!=x_idx)) return 0;
    
    if (*nav_l==x_idx) return 0;

    if (*nav_r==x_idx){
        return ((nav_r.thenBranch()==nav_l) && (nav_r.elseBranch().isEmpty()))?1:0;
    } else {
        assert(!(nav_r.isEmpty()));
        assert(*nav_r<x_idx);
    
            assert (*nav_r<x_idx);
            if (*nav_r!=*nav_l) return 0;
            
            
            cache_mgr_type cache_mgr;
            MonomialSet::navigator cached=cache_mgr.find(nav_l,nav_r, ((Polynomial) x).diagram().navigation());
            if (cached.isValid() ){
                   return cached;
            }
            Polynomial res0=do_left_equals_right_x_branch_and_r_has_fac_x(nav_l.elseBranch(),nav_r.elseBranch(),x);
            Polynomial res=res0;
            if (res0==1){
                res=do_left_equals_right_x_branch_and_r_has_fac_x(nav_l.thenBranch(),nav_r.thenBranch(),x);
                
            }
            cache_mgr.insert(nav_l,nav_r,((Polynomial) x).diagram().navigation(),res.diagram().navigation());
            return res;
        
    }
}

static Polynomial do_has_factor_x_plus_y(const MonomialSet& m,const Variable& x,const Variable& y){
    if (m.emptiness()) return true;
    assert(!(x==y));
    MonomialSet::navigator nav=m.navigation();
    idx_type min_idx=std::min(x.index(),y.index());
    
    while((nav.elseBranch().isEmpty()) && (!(nav.isConstant())) &&(*nav<min_idx)){
        nav.incrementThen();
    }

    idx_type m_idx=*nav;

    if (m_idx==min_idx){
        Variable other;
        if (min_idx!=x.index()) other=x; else other=y;
        return do_left_equals_right_x_branch_and_r_has_fac_x(nav.thenBranch(),nav.elseBranch(),other)==1?1:0;
    } else {
        assert(!(nav.isEmpty()));
        if (m_idx>min_idx){
            return 0;//False
        } else {
            assert (*nav<min_idx);
            typedef PBORI::CacheManager<CCacheTypes::has_factor_x_plus_y>
                 cache_mgr_type;
            
            cache_mgr_type cache_mgr;
            MonomialSet::navigator x_nav=((Polynomial) x).diagram().navigation();
            MonomialSet::navigator y_nav=((Polynomial) y).diagram().navigation();
            MonomialSet::navigator cached=cache_mgr.find(nav, x_nav,y_nav);
            if (cached.isValid() ){
                   return cached;
            }
            Polynomial res0=do_has_factor_x_plus_y(nav.elseBranch(),x,y);
            Polynomial res=res0;
            if (res0==1){
                res=do_has_factor_x_plus_y(nav.thenBranch(),x,y);
                
            }
            cache_mgr.insert(nav,x_nav,y_nav,res.diagram().navigation());
            return res;
        }
    }
}
//struct has_factor_x_plus_y: public ternary_cache_tag { };
//struct left_equals_right_x_branch: public ternary_cache_tag { };
static Polynomial do_has_factor_x_plus_one(const MonomialSet& m,const Variable& x){
    if (m.emptiness()) return true;
    MonomialSet::navigator nav=m.navigation();
    idx_type x_idx=x.index();
    while((nav.elseBranch().isEmpty()) && (!(nav.isConstant())) &&(*nav<x_idx)){
        nav.incrementThen();
    }

    idx_type m_idx=*nav;

    if (m_idx==x_idx){
        return nav.elseBranch()==nav.thenBranch()?1:0;
    } else {
        assert(!(nav.isEmpty()));
        if (m_idx>x_idx){
            return 0;//False
        } else {
            assert (*nav<x_idx);
            typedef PBORI::CacheManager<CCacheTypes::has_factor_x_plus_one>
                 cache_mgr_type;
            
            cache_mgr_type cache_mgr;
            MonomialSet::navigator cached=cache_mgr.find(nav, ((Polynomial) x).diagram().navigation());
            if (cached.isValid() ){
                   return cached;
            }
            Polynomial res0=do_has_factor_x_plus_one(nav.elseBranch(),x);
            Polynomial res=res0;
            if (res0==1){
                res=do_has_factor_x_plus_one(nav.thenBranch(),x);
                
            }
            cache_mgr.insert(nav,((Polynomial) x).diagram().navigation(),res.diagram().navigation());
            return res;
        }
    }
}

static bool has_factor_x(const MonomialSet& m, const Variable& x){
    return do_has_factor_x(m,x)==1;
}

static bool has_factor_x_plus_one(const MonomialSet& m, const Variable& x){
    return do_has_factor_x_plus_one(m,x)==1;
}
static bool has_factor_x_plus_y(const MonomialSet& m, const Variable& x,const Variable& y){
    return do_has_factor_x_plus_y(m,x,y)==1;
}
//#define ELEMENTAR_FACTORIZATION 1
LiteralFactorization::LiteralFactorization(const Polynomial& p){
  assert(!(p.isZero()));
  Exponent lead_variables=p.leadExp();
  lmDeg=p.lmDeg();
  Exponent other_variables=p.usedVariablesExp()-lead_variables;
  //only vars in the lead can factor out, independently of the order
  BooleSet r(p);
  Exponent::const_iterator it=lead_variables.begin();
  const Exponent::const_iterator end=lead_variables.end();
  
  int len_r=r.length();
  
  
  
  while(it!=end){
    assert(len_r==r.length());
    idx_type v=*it;
    #ifndef ELEMENTAR_FACTORIZATION
    BooleSet s0=r.subset0(v);
    assert (s0.emptiness()==has_factor_x(r,Variable(v)));
    if (s0.emptiness()){
    #else
    Variable v_var(v);
    if (has_factor_x(r,Variable(v_var))){
    #endif
      factors[v]=0;// var(v) is factor
      assert(r.change(v)==r.subset1(v));
      r=r.change(v);//equivalently subset(1)
      //cout<<"found factor0"<<endl;
    } else {
 
    #ifndef ELEMENTAR_FACTORIZATION
      if ((len_r%2==0)&&(s0.length()*2==len_r)){
    #else
      if ((len_r%2==0)){
    #endif
    #ifndef ELEMENTAR_FACTORIZATION
        BooleSet s1=r.subset1(v);
        assert((s1==s0)==has_factor_x_plus_one(r,Variable(v)));
            if (s1==s0){
    #else
       if (has_factor_x_plus_one(r,v_var)){
    #endif
                factors[v]=1;//var(v)+1 is factor
    #ifndef ELEMENTAR_FACTORIZATION
                r=s1;//==s0
    #else
                r=r.subset1(v);
    #endif
                len_r/=2;
                //cout<<"found factor1"<<endl; 
            } else {
                if (!(Polynomial(r).hasConstantPart())){
                Exponent::const_iterator other_it=other_variables.begin();
                Exponent::const_iterator other_end=other_variables.end();
                //++other_it;//explicit is better than implicit
                while(other_it!=other_end){
                    
                    idx_type v2=*other_it;
                    #ifdef ELEMENTAR_FACTORIZATION
                    Variable v2_var(v2);
                    #endif
                    //cout<<"testing var"<<v2<<endl;
                    //v occurs in the lead, v2 not
                    assert(v2!=v);
                    #ifndef ELEMENTAR_FACTORIZATION
                    assert((r.subset1(v2)==s1)==has_factor_x_plus_y(r,Variable(v),Variable(v2)));
                    if (r.subset1(v2)==s1){
                    #else
                    if (has_factor_x_plus_y(r,v,v2_var)){
                    #endif
                    
                        assert(BoolePolyRing::compare(v, v2)==BoolePolyRing::greater_than);
                        var2var_map[v]=v2;
                        assert(r.subset1(v2).change(v2)==r.subset0(v));
                        assert(r.subset1(v2).subset1(v).emptiness());
                        assert(r.subset0(v).subset0(v2).emptiness());
                    #ifndef ELEMENTAR_FACTORIZATION
                        r=s1;
                    #else
                        r=r.subset1(v);
                    #endif
                        len_r/=2;

                        //cout<<"found factor var2var"<<v<<":"<<v2<<endl;
                        break;
                    }
                    
                    ++other_it;
                }
                }
            }
      }
    }

    it++;
  }
  rest=r;
  assert((!(rest.isZero())));
}
deg_type common_literal_factors_deg(const LiteralFactorization& a, const LiteralFactorization& b){
  deg_type res=0;
  
  {
  LiteralFactorization::map_type::const_iterator it=a.factors.begin();
  const LiteralFactorization::map_type::const_iterator end=a.factors.end();
  const LiteralFactorization::map_type::const_iterator b_end=b.factors.end();
  while(it!=end){

    int val=it->second;
    idx_type variable=it->first;
    LiteralFactorization::map_type::const_iterator found_res=b.factors.find(variable);
    if (found_res!=b_end){
      if (val==found_res->second){
        ++res;
      }
    }
    
    it++;
  }
  }
  
  {
  LiteralFactorization::var2var_map_type::const_iterator vit=a.var2var_map.begin();
  const LiteralFactorization::var2var_map_type::const_iterator vend=a.var2var_map.end();
  const LiteralFactorization::var2var_map_type::const_iterator vb_end=b.var2var_map.end();
  while(vit!=vend){

    idx_type val=vit->second;
    idx_type variable=vit->first;
    LiteralFactorization::var2var_map_type::const_iterator found_res=b.var2var_map.find(variable);
    if (found_res!=vb_end){
      if (val==found_res->second){
        ++res;
      }
    }
    
    vit++;
  }
  }
  
  if (a.rest==b.rest){
    res+=a.rest.lmDeg();
  }
  
  return res;
}


bool LiteralFactorization::trivial() const{
  return (((this->factors.size()==0) && 
    (this->var2var_map.size()==0) )
    || ((this->factors.size()==1) && (this->var2var_map.size()==0) && this->rest.isOne())
    || ((this->factors.size()==0) && (this->var2var_map.size()==1) && this->rest.isOne()))
    ;
}
bool LiteralFactorization::occursAsLeadOfFactor(idx_type v) const{
  if (factors.count(v)>0){
    return true;
  }
  else{
    if (rest.lmDeg()==1){
      Exponent m=rest.leadExp();
      return ((*(m.begin()))==v);
    }
    if (var2var_map.count(v)>0) return true;
  }
  return false;
  //return ((factors.count(v)>0)||((rest.lmDeg()==1)&& ((*(rest.lead().begin()))==v)));
}

//LiteralFactorization::var_pair_type LiteralFactorization::constr_two_var_factor(int i, int j){
//    return var_pair_type(std::min(i,j),std::max(i,j));
//}

bool maps_to_one( const std::pair<const polybori::groebner::idx_type, int> v){
    return (v.second==1);
}
bool maps_to_zero( const std::pair<const polybori::groebner::idx_type, int>  v){
    return (v.second==0);
}
bool LiteralFactorization::is11Factorization(){
    if (this->factors.size()==lmDeg){
        if (find_if(factors.begin(),factors.end(), maps_to_zero)==factors.end()){
            return true;
        } else return false;
    }
    return false;
}
bool LiteralFactorization::is00Factorization(){
    if (this->factors.size()==lmDeg){
        if (find_if(factors.begin(),factors.end(), maps_to_one)==factors.end()){
            return true;
        } else return false;
    }
    return false;
}
END_NAMESPACE_PBORIGB
