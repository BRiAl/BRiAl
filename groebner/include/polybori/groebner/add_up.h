// -*- c++ -*-
//*****************************************************************************
/** @file add_up.h 
 *
 * @author Michael Brickenstein
 * @date 2011-07-01
 *
 * This file includes the definition of specialized adding routines.
 *
 * @par Copyright:
 *   (c) by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_add_up_h_
#define polybori_groebner_add_up_h_

// include basic definitions
#include "groebner_defs.h"
#include "LexOrderGreaterComparer.h"
BEGIN_NAMESPACE_PBORIGB

inline MonomialSet
add_up_lex_sorted_monomials(const BoolePolyRing& ring,
					       std::vector<Monomial>& vec, int start, int end){
    PBORI_ASSERT(end<=vec.size());
    PBORI_ASSERT(start>=0);
    int d=end-start;
    PBORI_ASSERT(d>=0);
    if (d<=2){
        switch(d){
            case 0:return MonomialSet(ring);
            case 1:return vec[start].diagram();
            case 2: 
              return (vec[start]+vec[start+1]).diagram();
        }

    
    }
    
    //more than two monomial, lex sorted, so if first is  constant, all are constant
    if (vec[start].isOne()) return Polynomial(end-start, vec[start].ring()).diagram();
    PBORI_ASSERT (!(vec[start].isOne()));
    idx_type idx=*vec[start].begin();
    int limes=end;
    vec[start].popFirst();
    for(limes=start+1;limes<end;limes++){
        if (vec[limes].isOne()||(*vec[limes].begin()!=idx)){
            PBORI_ASSERT((vec[limes].isOne())||(*vec[limes].begin()>idx));
            break;
        } else 
           vec[limes].popFirst();
            //vec[limes].changeAssign(idx);
    }
    
    return MonomialSet(idx,add_up_lex_sorted_monomials(ring, vec,start,limes),add_up_lex_sorted_monomials(ring,vec,limes,end));
}

inline MonomialSet
add_up_lex_sorted_exponents(const BoolePolyRing& ring,
                            std::vector<Exponent>& vec, int start, int end){
    PBORI_ASSERT(end<=vec.size());
    PBORI_ASSERT(start>=0);
    int d=end-start;
    PBORI_ASSERT(d>=0);
    if (d<=2){
        switch(d){
            case 0:return MonomialSet(ring);
        case 1:return Monomial(vec[start], ring).diagram();
            case 2: 
              Polynomial res=Monomial(vec[start], ring) + 
                Monomial(vec[start+1],ring);
              return MonomialSet(res.diagram());
        }

    
    }
    
    //more than two monomial, lex sorted, so if first is  constant, all are constant
    if (vec[start].deg()==0) return Polynomial(end-start, ring).diagram();
    PBORI_ASSERT (!(vec[start].deg()==0));
    idx_type idx=*vec[start].begin();
    int limes=end;
    vec[start].popFirst();
    for(limes=start+1;limes<end;limes++){
        if (PBORI_UNLIKELY((vec[limes].deg()==0)||(*vec[limes].begin()!=idx))){
            PBORI_ASSERT((vec[limes].deg()==0)||(*vec[limes].begin()>idx));
            break;
        } else 
           vec[limes].popFirst();
            //vec[limes].changeAssign(idx);
    }
    
    return MonomialSet(idx, add_up_lex_sorted_exponents(ring, vec,start,limes),
                       add_up_lex_sorted_exponents(ring, vec,limes,end));
}

/// @note This function is deactivated, because it always uses the active manager!
/// @todo activate and make save, when used
#if 0
inline MonomialSet add_up_lex_sorted_monomial_navs(const BoolePolyRing& ring,
  std::vector<Monomial::const_iterator>& vec, int start, int end){
    PBORI_ASSERT(end<=vec.size());
    PBORI_ASSERT(start>=0);
    int d=end-start;
    PBORI_ASSERT(d>=0);
    if (d<=2){
        switch(d){
            case 0:return MonomialSet(const BoolePolyRing& ring,);
            case 1:return MonomialSet(vec[start]);
            case 2: 
              Polynomial res=Polynomial(vec[start])+Polynomial(vec[start+1]);
              return MonomialSet(res.diagram());
        }

    
    }
    
    //more than two monomial, lex sorted, so if first is  constant, all are constant
    if (vec[start].isConstant()) return Polynomial(end-start).diagram();
    PBORI_ASSERT (!(vec[start].isConstant()));
    idx_type idx=*vec[start];
    int limes=end;
    vec[start]++;
    for(limes=start+1;limes<end;limes++){
        if (vec[limes].isConstant()||(*vec[limes]!=idx)){
            PBORI_ASSERT((vec[limes].isTerminated())||(*vec[limes]>idx));
            break;
        } else 
           vec[limes]++;
            //vec[limes].changeAssign(idx);
    }
    
    return MonomialSet(idx,add_up_lex_sorted_monomial_navs(vec,start,limes),add_up_lex_sorted_monomial_navs(vec,limes,end));
}
#endif

inline Polynomial
add_up_exponents(const std::vector<Exponent>& vec,
			    const Polynomial& init){
    //return add_up_generic(vec);
    std::vector<Exponent> vec_sorted=vec;
    std::sort(vec_sorted.begin(),vec_sorted.end(),LexOrderGreaterComparer());
    
   
    return add_up_lex_sorted_exponents(init.ring(),
				       vec_sorted,0,vec_sorted.size());
}


inline Polynomial
unite_polynomials(const std::vector<Polynomial>& res_vec, int
start, int end, Polynomial init){
    //we assume the polynomials to be pairwise different
    int s=end-start;
    if PBORI_UNLIKELY(s==0) return init;
    if (s==1) return res_vec[start];
    int h=s/2;
    return Polynomial(unite_polynomials(res_vec,start,start+h,
             init).diagram().unite(unite_polynomials(res_vec,start+h,end, 
                                                     init).diagram()));
    //return add_up_monomials(res_vec,start,start+h)+add_up_monomials(res_vec,start+h,end);
}

inline Polynomial
unite_polynomials(const std::vector<Polynomial>& res_vec,
                                    Polynomial init){
    //we assume the polynomials to be pairwise different
    int s=res_vec.size();
    if PBORI_UNLIKELY(s==0) return init;
    if (s==1) return res_vec[0];
    int h=s/2;
    
    return Polynomial(unite_polynomials(res_vec,0,h, init).diagram().unite(unite_polynomials(res_vec,h,s,init).diagram()));
}

// inline Polynomial add_up_polynomials(const std::vector<Polynomial>& res_vec, int
// start, int end, Polynomial init){
//     //we assume the polynomials to be pairwise different
//     int s=end-start;
//     if (s==0) return init;
//     if (s==1) return res_vec[start];
//     int h=s/2;
//     return add_up_polynomials(res_vec,start,start+h,
//              init)+add_up_polynomials(res_vec,start+h,end, 
//                                                      init);
//     //return add_up_monomials(res_vec,start,start+h)+add_up_monomials(res_vec,start+h,end);
// }
// static Polynomial add_up_polynomials(const std::vector<Polynomial>& res_vec,
//                                     Polynomial init){
//     //we assume the polynomials to be pairwise different
//     int s=res_vec.size();
//     if (s==0) return init;
//     if (s==1) return res_vec[0];
//     int h=s/2;
//     
//     return add_up_polynomials(res_vec,0,h, init)+add_up_polynomials(res_vec,h,s,init);
// }



template <class T>
inline Polynomial
add_up_generic(const std::vector<T>& res_vec, int
start, int end, Polynomial init){
    //we assume the polynomials to be pairwise different
    int s=end-start;
    if (s==0) return init;
    if (s==1) return Polynomial(res_vec[start]);
    int h=s/2;
    return add_up_generic(res_vec,start,start+h,init) +
      add_up_generic(res_vec,start+h,end, init);
}

template <class T>
inline Polynomial
add_up_generic(const std::vector<T>& res_vec,
                                             Polynomial init){
    //we assume the polynomials to be pairwise different
    int s=res_vec.size();
    if (s==0) return init;
    if (s==1) return (Polynomial) res_vec[0];
    int h=s/2;
    
    return add_up_generic(res_vec,0,h, init) + 
      add_up_generic(res_vec,h,s, init);
}

inline Polynomial
add_up_monomials(const std::vector<Monomial>& vec,
			    const Polynomial& init){
  return add_up_generic(vec, init);

}

inline Polynomial
add_up_polynomials(const std::vector<Polynomial>& vec,
			      const Polynomial& init){
  return add_up_generic(vec, init);

}


END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_add_up_h_ */
