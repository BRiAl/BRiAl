// 
//  FGLMStrategy.h
//  PolyBoRi
//  
//  Created by Michael Brickenstein on 2008-11-13.
//  Copyright 2008 The PolyBoRi Team
// 

#ifndef fglm_header
#define fglm_header

#include <polybori/BoolePolyRing.h>
#include "groebner_alg.h"

extern "C" {
#include <m4ri/m4ri.h>
}

BEGIN_NAMESPACE_PBORIGB

typedef std::vector<Polynomial> poly_vec;

class FGLMStrategy{


    /// Copy constructor (forbitten, cannot be copied, used smart pointer or
    /// reference, if you need to return by value)
    FGLMStrategy(const FGLMStrategy& rhs);

    /// Assignment (forbitten, see above)
    FGLMStrategy& operator=(const FGLMStrategy& rhs);

public:
    typedef BoolePolyRing ring_with_ordering_type;

    FGLMStrategy(const ring_with_ordering_type& from_ring, const ring_with_ordering_type& to_ring,  const PolynomialVector& gb);
 
    PolynomialVector main();
    void analyzeGB(const ReductionStrategy& gb);
    void setupMultiplicationTables();
    void setupStandardMonomialsFromTables();
    void writeRowToVariableDivisors(mzd_t* row, Monomial lm);
    void testMultiplicationTables();
    void transposeMultiplicationTables();
    void writeTailToRow(MonomialSet tail, mzd_t* row);
    Polynomial rowToPoly(mzd_t* row);
    //allocates a window, free it with mzd_free_window
    void findVectorInMultTables(mzd_t* dst, Monomial m);
    mzd_t* multiplicationTableForVariable(const Variable& v){
        return multiplicationTables[ring2Index[v.index()]];
    }
    ~FGLMStrategy(){
        for(std::size_t i=0;i<multiplicationTables.size();i++){
            mzd_free(multiplicationTables[i]);
        }
    }
    typedef std::vector<idx_type> IndexVector;


private:
    bool prot;
    Monomial vars;
    size_t varietySize;
    typedef std::vector<Monomial> MonomialVector;
    typedef std::vector<mzd_t*> MatrixVector;
    typedef std::vector<Variable> VariableVector;
    
    
    MonomialSet standardMonomialsFrom;
    MonomialVector standardMonomialsFromVector;
    MonomialSet leadingTermsFrom;
    MonomialSet varsSet;
    VariableVector varsVector;
    
    ReductionStrategy gbFrom; //reduced gb
    MonomialSet edgesUnitedVerticesFrom;
    ring_with_ordering_type from;
    ring_with_ordering_type to;
    
    //indices in multiplicationTables correspond to reverse standard BooleSet iteration of standardMonomialsFrom
    
    IndexVector ring2Index;
    IndexVector index2Ring;
    IndexVector rowStartingWithIndex;
    MonomialSet::size_type nVariables;
    bool transposed;
    std::vector<IndexVector> tableXRowYIsMonomialFromWithIndex;
    lm2Index_map_type standardMonomialsFrom2Index;
    lm2Index_map_type monomial2MultiplicationMatrix;
    lm2Index_map_type monomial2MultiplicationMatrixRowIndex;
    MatrixVector multiplicationTables;
    
    bool canAddThisElementLaterToGB(Polynomial p);
    PolynomialVector addTheseLater;
    IndexVector rowIsStandardMonomialToWithIndex;
    Exponent::idx_map_type standardExponentsFrom2Index;
    Polynomial reducedNormalFormInFromRing(Polynomial f);
    IndexVector rowVectorIsLinearCombinationOfRows(mzd_t* mat,   mzd_t* v);
    };
END_NAMESPACE_PBORIGB
#endif    

