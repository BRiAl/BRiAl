/*  Copyright (c) 2005-2007 by The PolyBoRi Team */

#include <polybori.h>
#include <algorithm>
#include <utility>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include "groebner_defs.h"
#ifndef PBORI_GB_CACHE_H
#define PBORI_GB_CACHE_H


BEGIN_NAMESPACE_PBORIGB
template<class idx_type1, class idx_type2> void set_up_translation_vectors(std::vector<idx_type1>& ring_2_0123, std::vector<idx_type2>& back_2_ring, const Exponent& used_variables){
    BooleExponent::const_iterator it=used_variables.begin();
    BooleExponent::const_iterator end=used_variables.end();
    idx_type1 idx_0123=0;
    while(it!=end){
        ring_2_0123[*it]=idx_0123;
        back_2_ring[idx_0123]=*it;
        idx_0123++;
        it++;
    }
}

//template<class idx_type1, class idx_type2> void set_up_translation_vectors(std::vector<idx_type1>& ring_2_0123, std::vector<idx_type>& back_2_ring, const Exponent& used_variables);
Polynomial translate_indices(const Polynomial& p, const std::vector<idx_type>& table);
class CacheManager{
    public:
        typedef Polynomial::poly_vec_map_type impl_type;
        
        typedef std::vector<Polynomial> poly_vec_type;
        typedef boost::shared_ptr<poly_vec_type> res_type;
        typedef Polynomial::poly_vec_map_type::const_iterator impl_iterator_type;
    protected:
        impl_type impl;
    public:
        CacheManager(){
            
        }
        res_type lookup(const Polynomial& p, bool& succ){
          int nv=p.ring().nVariables();
            BooleExponent used_exp=p.usedVariablesExp();
            std::vector<idx_type> back_2_ring(used_exp.size());
            std::vector<idx_type> ring_2_0123(nv);
            
            //PBORI_ASSERT(nv<255);
            set_up_translation_vectors(ring_2_0123,back_2_ring, used_exp);
            Polynomial p_t=translate_indices(p,ring_2_0123);
            impl_iterator_type it=impl.find(p_t);
            //cache_res_type res;
            if (it!=impl.end()){
                succ=true;
                res_type res(new poly_vec_type(it->second));

                
                for(poly_vec_type::size_type i=0;i<res->size();i++){
                    (*res)[i]=translate_indices((*res)[i],back_2_ring);
                }
                return res;
            }
            else{
                succ=false;
                return res_type();
            }
        }
        void insert(const Polynomial& key, const poly_vec_type& value){
            int nv=key.ring().nVariables();
            BooleExponent used_exp=key.usedVariablesExp();
            std::vector<idx_type> back_2_ring(used_exp.size());
            std::vector<idx_type> ring_2_0123(nv);
            set_up_translation_vectors(ring_2_0123,back_2_ring, used_exp);
            Polynomial key_t=translate_indices(key,ring_2_0123);
            poly_vec_type value_t(value);

            for(poly_vec_type::size_type i=0;i<value_t.size();i++){
                value_t[i]=translate_indices(value_t[i], ring_2_0123);
            }
            impl[key_t]=value_t;
        }
    
        
};
END_NAMESPACE_PBORIGB
#endif
