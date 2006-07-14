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
BEGIN_NAMESPACE_PBORIGB

LiteralFactorization::LiteralFactorization(const Polynomial& p){
  Monomial used_variables=p.lead();
  //only vars in the lead can factor out, independently of the order
  BooleSet r(p);
  Monomial::const_iterator it=used_variables.begin();
  const Monomial::const_iterator end=used_variables.end();
  
  
  
  
  
  while(it!=end){
    idx_type v=*it;
    BooleSet s0=r.subset0(v);
    if (s0.emptiness()){
      factors[v]=0;// var(v) is factor
      r=r.change(v);//equivalently subset(1)
      cout<<"found factor0"<<endl;
    } else {
    
    
      if (s0.length()*2==r.length()){
        BooleSet s1=r.subset1(v);
            if (s1==s0){
                factors[v]=1;//var(v)+1 is factor
                r=s1;//==s0
                cout<<"found factor1"<<endl; 
            }
      }
    }

    it++;
  }
  rest=r;
}
deg_type common_literal_factors_deg(const LiteralFactorization& a, const LiteralFactorization& b){
  deg_type res=0;
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
  if (a.rest==b.rest){
    res+=a.rest.lmDeg();
  }
  return res;
}


bool LiteralFactorization::trivial() const{
  return ((this->factors.size()==0) || ((this->factors.size()==1) && this->rest.isOne()));
}
bool LiteralFactorization::occursAsLeadOfFactor(idx_type v) const{
  if (factors.count(v)>0)
    return true;
  else{
    if (rest.lmDeg()==1){
      BooleMonomial m=rest.lead();
      return ((*(m.begin()))==v);
    }
  }
  return false;
  //return ((factors.count(v)>0)||((rest.lmDeg()==1)&& ((*(rest.lead().begin()))==v)));
}

LiteralFactorization::var_pair_type LiteralFactorization::constr_two_var_factor(int i, int j){
    return var_pair_type(std::min(i,j),std::max(i,j));
}
END_NAMESPACE_PBORIGB
