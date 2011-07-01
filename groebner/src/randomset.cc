//
//  untitled
//
//  Created by  on 2007-12-13.
//  Copyright (c) 2007 The PolyBoRi Team. See LICENSE file.
//  PolyBoRi Project
#include <polybori/groebner/randomset.h>

#include <ctime> 
#include <set>
#include <polybori/groebner/nf.h>
#include <polybori/groebner/add_up.h>
#include <polybori/groebner/ExpLexLess.h>

BEGIN_NAMESPACE_PBORIGB
static base_generator_type generator(static_cast<unsigned int>(std::time(0)));

void set_random_seed(unsigned int seed){
    generator=base_generator_type(seed);
}


MonomialSet random_set_using_generator(const Monomial& variables, unsigned int len, bool_gen_type& bit_gen){
    Exponent var_exp=variables.exp();

    
    std::set<Exponent, ExpLexLess> exponents;
    while(exponents.size()<len){
        Exponent new_exp;
        Exponent::const_iterator it=var_exp.begin();
        Exponent::const_iterator end=var_exp.end();
        while(it!=end){
            if (bit_gen())
                new_exp.push_back(*it);
            ++it;
        }
        exponents.insert(new_exp);
    }
    std::vector<Exponent> exponents_vec(exponents.size());
    std::copy(exponents.begin(),exponents.end(),exponents_vec.begin());
    Polynomial p=add_up_exponents(exponents_vec, variables.ring().zero());
    return p.diagram();
}
MonomialSet random_set(const Monomial& variables, unsigned int len){
    
    
    bool_gen_type bit_gen(generator,distribution_type(0,1));
    return random_set_using_generator(variables,len,bit_gen);
}
END_NAMESPACE_PBORIGB
