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
    FGLMStrategy(const ring_with_ordering_type& from_ring, const ring_with_ordering_type& to_ring,  const PolynomialVector& gb)
   :to(to_ring), from(from_ring)
    {
 
        
        PolynomialVector::const_iterator it=gb.begin();
        PolynomialVector::const_iterator end=gb.end();
        
        while(it!=end){
            this->gbFrom.addGenerator(*it);
            it++;
        }
        //assert ((BooleEnv::ring()==from) ||(BooleEnv::ring()==to));
        analyzeGB(this->gbFrom);
        setupStandardMonomialsFromTables();
        setupMultiplicationTables();
        testMultiplicationTables();
        Monomial monomial_one(from_ring);
        if (!(this->gbFrom.leadingTerms.owns(monomial_one))){
            //cout<<standardMonomialsFrom2Index[monomial_one]<<endl;
            assert(standardMonomialsFrom2Index[monomial_one]==0);
        }
    }
    PolynomialVector main();
    void analyzeGB(const ReductionStrategy& gb);
    void setupMultiplicationTables();
    void setupStandardMonomialsFromTables();
    void writeRowToVariableDivisors(packedmatrix* row, Monomial lm);
    void testMultiplicationTables();
    void writeTailToRow(MonomialSet tail, packedmatrix* row);
    Polynomial rowToPoly(packedmatrix* row);
    //allocates a window, free it with mzd_free_window
    packedmatrix* findVectorInMultTables(Monomial m){
        packedmatrix* mat=multiplicationTables[monomial2MultiplicationMatrix[m]];
        size_t idx=monomial2MultiplicationMatrixRowIndex[m];
        return mzd_init_window(mat, idx, 0, idx+1, varietySize);
    }
    packedmatrix* multiplicationTableForVariable(const Variable& v){
        return multiplicationTables[ring2Index[v.index()]];
    }
    ~FGLMStrategy(){
        int i;
        for(i=0;i<multiplicationTables.size();i++){
            mzd_free(multiplicationTables[i]);
        }
    }
private:
    Monomial vars;
    size_t varietySize;
    typedef std::vector<Monomial> MonomialVector;
    typedef std::vector<packedmatrix*> MatrixVector;
    typedef std::vector<Variable> VariableVector;
    typedef std::vector<idx_type> IndexVector;
    
    MonomialSet standardMonomialsFrom;
    MonomialVector standardMonomialsFromVector;
    MonomialSet leadingTermsFrom;
    MonomialSet varsSet;
    VariableVector varsVector;
    
    ReductionStrategy gbFrom; //reduced gb
    
    ring_with_ordering_type from;
    ring_with_ordering_type to;
    
    //indices in multiplicationTables correspond to reverse standard BooleSet iteration of standardMonomialsFrom
    
    IndexVector ring2Index;
    IndexVector index2Ring;
    idx_type nVariables;
    lm2Index_map_type standardMonomialsFrom2Index;
    lm2Index_map_type monomial2MultiplicationMatrix;
    lm2Index_map_type monomial2MultiplicationMatrixRowIndex;
    MatrixVector multiplicationTables;
    Polynomial reducedNormalFormInFromRing(Polynomial f);
    IndexVector rowVectorIsLinearCombinationOfRows(packedmatrix* mat, packedmatrix* v);
    };
END_NAMESPACE_PBORIGB
#endif    

