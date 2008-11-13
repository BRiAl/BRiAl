// 
//  fglm.h
//  PolyBoRi
//  
//  Created by Michael Brickenstein on 2008-11-13.
//  Copyright 2008 The PolyBoRi Team
// 

#ifndef fglm_header
#define fglm_header

#include <BooleEnv.h>
#include "groebner_alg.h"

BEGIN_NAMESPACE_PBORIGB

typedef BooleEnv::ring_type ring_with_ordering_type;
typedef std::vector<Polynomial> poly_vec;
class FGLMStrategy{
public:
    FGLMStrategy(ring_with_ordering_type& from, ring_with_ordering_type& to,  const PolynomialVector& gb){
        this->from=from;
        this->to=to;
        PolynomialVector::const_iterator it=gb.begin();
        PolynomialVector::const_iterator end=gb.end();
        while(it!=end){
            this->gb.addGenerator(*it);
            it++;
        }
    }
    
private:
    ReductionStrategy gb; //reduced gb
    ring_with_ordering_type from;
    ring_with_ordering_type to;

    };
#endif    

END_NAMESPACE_PBORIGB