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
extern "C" {
#include <m4ri.h>
}
BEGIN_NAMESPACE_PBORIGB


typedef std::vector<Polynomial> poly_vec;
class FGLMStrategy{
public:
    typedef BooleEnv::ring_type ring_with_ordering_type;
    FGLMStrategy(){}
    FGLMStrategy(const ring_with_ordering_type& from, const ring_with_ordering_type& to,  const PolynomialVector& gb){
        this->from=from;
        this->to=to;
        PolynomialVector::const_iterator it=gb.begin();
        PolynomialVector::const_iterator end=gb.end();
        
        while(it!=end){
            this->gbFrom.addGenerator(*it);
            it++;
        }
        analyzeGB(this->gbFrom);
        //setupStandardMonomialsFromTables();
        //setupMultiplicationTables();
        
    }
    
    void analyzeGB(const ReductionStrategy& gb);
    void setupMultiplicationTables();
    void setupStandardMonomialsFromTables();
    ~FGLMStrategy(){
        int i;
        for(i=0;i<multiplicationTables.size();i++){
            mzd_free(multiplicationTables[i]);
        }
    }
private:
    Monomial vars;
    size_t varietySize;
    MonomialSet standardMonomialsFrom;
    std::vector<Monomial> standardMonomialsFromVector;
    MonomialSet leadingTermsFrom;
    ReductionStrategy gbFrom; //reduced gb
    ring_with_ordering_type from;
    ring_with_ordering_type to;
    //indices in multiplicationTables correspond to reverse standard BooleSet iteration of standardMonomialsFrom
    typedef std::vector<packedmatrix*> MatrixVector;
    typedef std::vector<Variable> VariableVector;
    typedef std::vector<idx_type> IndexVector;
    IndexVector ring2Index;
    IndexVector index2Ring;
    idx_type nVariables;
    lm2Index_map_type standardMonomialsFrom2Index;
    MatrixVector multiplicationTables;
    
    
    };
END_NAMESPACE_PBORIGB
#endif    

