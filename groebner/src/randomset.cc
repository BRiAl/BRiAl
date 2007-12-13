//
//  untitled
//
//  Created by  on 2007-12-13.
//  Copyright (c) 2007 The PolyBoRi Team. All rights reserved.
//  PolyBoRi Project
#include "randomset.h"
#include <boost/random/uniform_smallint.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/variate_generator.hpp>
#include <ctime> 
#include <set>
#include "nf.h"
BEGIN_NAMESPACE_PBORIGB
typedef boost::minstd_rand base_generator_type;

MonomialSet random_set_using_generator(const Monomial& variables, unsigned int len, base_generator_type& generator){
    Exponent var_exp=variables.exp();
    typedef boost::uniform_smallint<> distribution_type;
    typedef boost::variate_generator<base_generator_type&, distribution_type> gen_type;
    gen_type bit_gen(generator,distribution_type(0,1));
    std::set<Exponent> exponents;
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
    Polynomial p=add_up_exponents(exponents_vec);
    return p.diagram();
}
MonomialSet random_set(const Monomial& variables, unsigned int len){
    
    base_generator_type generator(static_cast<unsigned int>(std::time(0)));
    return random_set_using_generator(variables,len,generator);
}
END_NAMESPACE_PBORIGB