// -*- c++ -*-
//*****************************************************************************
/** @file tables.h 
 *
 * @author Michael Brickenstein
 * @date 2011-06-30
 *
 * This file some functions for managing precomputed Groebner tables.
 *
 * @par Copyright:
 *   (c) 2006-2011 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_tables_h_
#define polybori_groebner_tables_h_

// include basic definitions
#include "groebner_defs.h"
#include <stdexcept>
#include <polybori/groebner/ZeroFunction.h>
#include <polybori/groebner/SetBitUInt.h>

#define PBORI_HAVE_DLEX4_DATA 1

#ifdef PBORI_HAVE_DLEX4_DATA
#include <polybori/groebner/dlex4data.h>
#endif

#define PBORI_HAVE_LP4_DATA 1

#ifdef PBORI_HAVE_LP4_DATA
#include <polybori/groebner/lp4data.h>
#endif

#define PBORI_HAVE_DP_ASC4_DATA 1

#ifdef PBORI_HAVE_DP_ASC4_DATA
#include <polybori/groebner/dp_asc4data.h>
#endif

BEGIN_NAMESPACE_PBORIGB


template<class value_type, class initializer, class set_bit> 
inline value_type
p2code(Polynomial p, const std::vector<char> & ring_2_0123, int max_vars){
    Polynomial::exp_iterator it_p=p.expBegin();
    Polynomial::exp_iterator end_p=p.expEnd();
    initializer init;
    value_type p_code=init(max_vars);
    PBORI_ASSERT(max_vars<sizeof(unsigned int)*8);
    set_bit bit_setter;
    while(it_p!=end_p){
        Exponent curr_exp=*it_p;
        Exponent::const_iterator it_v=curr_exp.begin();
        Exponent::const_iterator end_v=curr_exp.end();
        unsigned int exp_code=0;
        //exp code is int between 0 and 15
        while(it_v!=end_v){
            //cout<<"table value:"<<(int)ring_2_0123[(*it_v)]<<endl;
            exp_code|=(1<<ring_2_0123[(*it_v)]);
            //cout<<"exp_code:"<<exp_code<<endl;
            it_v++;
        }
        //cout<<"exp_code final:"<<exp_code<<endl;
        //p_code|=(1<<exp_code);
        bit_setter(p_code,exp_code);
        //so p code is 16-bit unsigned int
        //int is fastest
        it_p++;
    }
    return p_code;
}

inline unsigned int
p2code_4(Polynomial p, const std::vector<char> & ring_2_0123){
  return p2code<unsigned int, ZeroFunction, SetBitUInt>(p,ring_2_0123, 4);
}

inline unsigned int
get_table_entry4(const BoolePolyRing& ring, int p_code, int pos){
  switch(ring.ordering().getBaseOrderCode()){
        #ifdef PBORI_HAVE_LP4_DATA
        case COrderEnums::lp:
            return lp4var_data[p_code][pos];
        #endif
        #ifdef  PBORI_HAVE_DLEX4_DATA
        case COrderEnums::dlex:
            return dlex4var_data[p_code][pos];
        #endif
        #ifdef PBORI_HAVE_DP_ASC4_DATA
        case COrderEnums::dp_asc:
            return dp_asc4var_data[p_code][pos];
        #endif
        default:
          throw std::runtime_error("Groebner tables used with forbidden order");
    }
    return 0;
}


inline Monomial
code_2_m_4(const BoolePolyRing& ring, 
           unsigned int code, const std::vector<idx_type>& back_2_ring){

    Monomial result(ring);
    for(int idx = 3; idx >= 0; --idx){
      if ((code & (1<<idx)) != 0){
        result *= ring.variable(back_2_ring[idx]);
      }
    }
    return result;
}


inline Polynomial
code_2_poly_4(const BoolePolyRing& ring,
              unsigned int code, const std::vector<idx_type>& back_2_ring){

  Polynomial result(ring);
  for(int idx = 15; idx >= 0; --idx){
    if ((code & (1<<idx)) != 0){
      result += code_2_m_4(ring, idx, back_2_ring);
    }
  }
  return result;
}

inline bool 
have_ordering_for_tables(const int order_code) {
    #ifdef PBORI_HAVE_DLEX4_DATA
        if (order_code==COrderEnums::dlex)
           return true;
    #endif
    #ifdef PBORI_HAVE_LP4_DATA
        if (order_code==COrderEnums::lp)
           return true;
    #endif
    #ifdef PBORI_HAVE_DP_ASC4_DATA
        if (order_code==COrderEnums::dp_asc)
           return true;
    #endif
    return false;
}

inline bool 
have_ordering_for_tables(const BoolePolyRing& ring){  
  return have_ordering_for_tables(ring.ordering().getOrderCode());
}

inline bool
have_base_ordering_for_tables(const BoolePolyRing& ring){  
  return have_ordering_for_tables(ring.ordering().getBaseOrderCode());
}


END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_tables_h_ */
