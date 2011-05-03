/*
 *  literal_factorization.cc
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 29.05.06.
 *  Copyright 2006 The PolyBoRi Team. See LICENSE file.
 *
 */

#include <polybori/groebner/literal_factorization.h>
#include <iostream>
#include <polybori/cache/CCacheManagement.h>
BEGIN_NAMESPACE_PBORIGB

std::vector<Polynomial> easy_linear_factors(const Polynomial &p){
    LiteralFactorization factorization(p);
    std::vector<Polynomial> res;
    LiteralFactorizationIterator it1=factorization.begin();
    LiteralFactorizationIterator end1=factorization.end();
    while(it1!=end1){
        res.push_back(*it1);
        ++it1;
    }
    return res;
}

static Polynomial do_has_factor_x(const MonomialSet& m,const Variable& x){
  if (m.isZero()) return m.ring().one();
    MonomialSet::navigator nav=m.navigation();
    idx_type x_idx=x.index();
    while((!(nav.isConstant())) && (nav.elseBranch().isEmpty())  &&(*nav<x_idx)){
        nav.incrementThen();
    }

    idx_type m_idx=*nav;

    if (m_idx==x_idx){
      return Polynomial(nav.elseBranch().isEmpty(), m.ring());
    } else {
        assert(!(nav.isEmpty()));
        if (m_idx>x_idx){
          return m.ring().zero();//False
        } else {
            assert (*nav<x_idx);
            typedef PBORI::CacheManager<CCacheTypes::has_factor_x>
                 cache_mgr_type;
            
            cache_mgr_type cache_mgr(m.ring());
            MonomialSet::navigator cached=cache_mgr.find(nav, ((Polynomial) x).diagram().navigation());
            if (cached.isValid() ){
              return cache_mgr.generate(cached);
            }
            Polynomial res0=do_has_factor_x(cache_mgr.generate(nav.elseBranch()),x);
            Polynomial res=res0;
            if (res0==1){
              res=do_has_factor_x(cache_mgr.generate(nav.thenBranch()),x);
                
            }
            cache_mgr.insert(nav,((Polynomial) x).diagram().navigation(),res.diagram().navigation());
            return res;
        }
    }
}



static Polynomial do_left_equals_right_x_branch_and_r_has_fac_x(const MonomialSet& left,const MonomialSet& right, const Variable& x){
    if (left.isZero()) 
      return Polynomial(right.isZero(), left.ring());
    MonomialSet::navigator nav_l=left.navigation();
    MonomialSet::navigator nav_r=right.navigation();
    typedef PBORI::CacheManager<CCacheTypes::left_equals_right_x_branch_and_r_has_fac_x>
         cache_mgr_type;
    if (*nav_l<*nav_r) return left.ring().zero();
    idx_type x_idx=x.index();
    if (*nav_r>x_idx) return left.ring().zero();
    if ((*nav_l!=*nav_r) && (*nav_r!=x_idx)) return left.ring().zero();
    
    if (*nav_l==x_idx) return left.ring().zero();

    if (*nav_r==x_idx){
      return Polynomial((nav_r.thenBranch()==nav_l) &&
                        (nav_r.elseBranch().isEmpty()), left.ring());
    } else {
        assert(!(nav_r.isEmpty()));
        assert(*nav_r<x_idx);
    
            assert (*nav_r<x_idx);
            if (*nav_r!=*nav_l) return left.ring().zero();
            
            
            cache_mgr_type cache_mgr(left.ring());
            MonomialSet::navigator cached=cache_mgr.find(nav_l,nav_r, ((Polynomial) x).diagram().navigation());
            if (cached.isValid() ){
              return cache_mgr.generate(cached);
            }
            Polynomial res0=do_left_equals_right_x_branch_and_r_has_fac_x(cache_mgr.generate(nav_l.elseBranch()),cache_mgr.generate(nav_r.elseBranch()),x);
            Polynomial res=res0;
            if (res0==1){
              res=do_left_equals_right_x_branch_and_r_has_fac_x(cache_mgr.generate(nav_l.thenBranch()),cache_mgr.generate(nav_r.thenBranch()),x);
                
            }
            cache_mgr.insert(nav_l,nav_r,((Polynomial) x).diagram().navigation(),res.diagram().navigation());
            return res;
        
    }
}

static Polynomial do_has_factor_x_plus_y(const MonomialSet& m,const Variable& x,const Variable& y){
    if (m.isZero()) return m.ring().one();
    assert(!(x==y));
    MonomialSet::navigator nav=m.navigation();
    idx_type min_idx=std::min(x.index(),y.index());
    
    while((!(nav.isConstant())) && (nav.elseBranch().isEmpty()) && (*nav<min_idx)){
        nav.incrementThen();
    }

    idx_type m_idx=*nav;

    if (m_idx==min_idx){
        Variable other(0, m.ring());
        if (min_idx!=x.index()) other=x; else other=y;
        return
          Polynomial(do_left_equals_right_x_branch_and_r_has_fac_x(
              MonomialSet::dd_type(m.ring(), nav.thenBranch()), 
              MonomialSet::dd_type(m.ring(), nav.elseBranch()), 
              other).isOne(), m.ring());
    } else {
        assert(!(nav.isEmpty()));
        if (m_idx>min_idx){
          return m.ring().zero();//False
        } else {
            assert (*nav<min_idx);
            typedef PBORI::CacheManager<CCacheTypes::has_factor_x_plus_y>
                 cache_mgr_type;
            
            cache_mgr_type cache_mgr(m.ring());
            MonomialSet::navigator x_nav=((Polynomial) x).diagram().navigation();
            MonomialSet::navigator y_nav=((Polynomial) y).diagram().navigation();
            MonomialSet::navigator cached=cache_mgr.find(nav, x_nav,y_nav);
            if (cached.isValid() ){
              return cache_mgr.generate(cached);
            }
            Polynomial
              res0=do_has_factor_x_plus_y(cache_mgr.generate(nav.elseBranch()),
                                          x,y); 
            Polynomial res=res0;
            if (res0==1){
              res=do_has_factor_x_plus_y(cache_mgr.generate(nav.thenBranch()),x,y);
                
            }
            cache_mgr.insert(nav,x_nav,y_nav,res.diagram().navigation());
            return res;
        }
    }
}
//struct has_factor_x_plus_y: public ternary_cache_tag { };
//struct left_equals_right_x_branch: public ternary_cache_tag { };
static Polynomial do_has_factor_x_plus_one(const MonomialSet& m,const Variable& x){
  if (m.isZero()) return m.ring().one();
    MonomialSet::navigator nav=m.navigation();
    idx_type x_idx=x.index();
    while((!(nav.isConstant())) && (nav.elseBranch().isEmpty())  &&(*nav<x_idx)){
        nav.incrementThen();
    }

    idx_type m_idx=*nav;

    if (m_idx==x_idx){
      return Polynomial(nav.elseBranch()==nav.thenBranch(), m.ring());
    } else {
        assert(!(nav.isEmpty()));
        if (m_idx>x_idx){
          return m.ring().zero();//False
        } else {
            assert (*nav<x_idx);
            typedef PBORI::CacheManager<CCacheTypes::has_factor_x_plus_one>
                 cache_mgr_type;
            
            cache_mgr_type cache_mgr(m.ring());
            MonomialSet::navigator cached=cache_mgr.find(nav, ((Polynomial) x).diagram().navigation());
            if (cached.isValid() ){
              return cache_mgr.generate(cached);
            }
            Polynomial res0=do_has_factor_x_plus_one(cache_mgr.generate(nav.elseBranch()),x);
            Polynomial res=res0;
            if (res0==1){
              res=do_has_factor_x_plus_one(cache_mgr.generate(nav.thenBranch()),
                                           x);
                
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
#define ELEMENTAR_FACTORIZATION 1
LiteralFactorization::LiteralFactorization(const Polynomial& p){
  assert(!(p.isZero()));
  Exponent lead_variables=p.leadExp();
  lmDeg=p.leadDeg();
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
    assert (s0.isZero()==has_factor_x(r,Variable(v, s0.ring())));
    if (s0.isZero()){
    #else
      Variable v_var(v, p.ring());
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
        assert((s1==s0)==has_factor_x_plus_one(r,Variable(v, s1.ring())));
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
                    if (UNLIKELY(BooleEnv::ordering().compare(v, v2)!=BoolePolyRing::greater_than)){
                        ++other_it;
                        continue;
                    }
                    #ifdef ELEMENTAR_FACTORIZATION
                    Variable v2_var(v2, r.ring());
                    #endif
                    //cout<<"testing var"<<v2<<endl;
                    //v occurs in the lead, v2 not
                    assert(v2!=v);
                    #ifndef ELEMENTAR_FACTORIZATION
                    assert((r.subset1(v2)==s1)==has_factor_x_plus_y(r,Variable(v,r.ring()),Variable(v2, r.ring())));
                    if (r.subset1(v2)==s1){
                    #else
                      if (has_factor_x_plus_y(r,Variable(v, r.ring()),v2_var)){
                    #endif
                    
                      assert(BooleEnv::ordering().compare(v, v2)==BoolePolyRing::greater_than);
                        var2var_map[v]=v2;
                        assert(r.subset1(v2).change(v2)==r.subset0(v));
                        assert(r.subset1(v2).subset1(v).isZero());
                        assert(r.subset0(v).subset0(v2).isZero());
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
    res+=a.rest.leadDeg();
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
    if (rest.leadDeg()==1){
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
LiteralFactorization::const_iterator LiteralFactorization::begin(){
    return LiteralFactorizationIterator(this, factors.begin(), var2var_map.begin());
}
LiteralFactorization::const_iterator LiteralFactorization::end(){
    return LiteralFactorizationIterator(this, factors.end(), var2var_map.end());
}
LiteralFactorizationIterator::self& LiteralFactorizationIterator::operator++(){
    if (var2const_iter!=literal_factorization->factors.end()){
        ++var2const_iter;
        return *this;
          }
          if (var2var_iter!=literal_factorization->var2var_map.end()){
              ++var2var_iter;
              return *this;
          }
}
Polynomial LiteralFactorizationIterator::operator*() const{
    if (var2const_iter!=literal_factorization->factors.end()){
      return Variable(var2const_iter->first, m_ring)
        + Polynomial(var2const_iter->second, m_ring);
       }
       if (var2var_iter!=literal_factorization->var2var_map.end()){
         return Variable(var2var_iter->first, m_ring)
           + Variable(var2var_iter->second,m_ring);
       }
}

END_NAMESPACE_PBORIGB
