// 
//  fglm.cc
//  PolyBoRi
//  
//  Created by Michael Brickenstein on 2008-11-13.
//  Copyright 2008 The PolyBoRi Team.
// 

#include "fglm.h"
#include "nf.h"
BEGIN_NAMESPACE_PBORIGB
void FGLMStrategy::setupStandardMonomialsFromTables(){
     ring_with_ordering_type backup_ring=BooleEnv::ring();
     BooleEnv::set(from);
     standardMonomialsFromVector.resize(varietySize);
     MonomialSet::const_iterator it_set=standardMonomialsFrom.begin();
     MonomialSet::const_iterator end_set=standardMonomialsFrom.end();
     //assume only that iteration is descending w.r.t. divisibility
     
     int i=standardMonomialsFrom.size()-1;
     while(it_set!=end_set){
         Monomial m=*it_set;
         standardMonomialsFrom2Index[m]=i;
         standardMonomialsFromVector[i]=m;
         it_set++;
         i--;
     }

     BooleEnv::set(backup_ring);
     
}



void FGLMStrategy::setupMultiplicationTables(){
    ring_with_ordering_type backup_ring=BooleEnv::ring();
    BooleEnv::set(from);
    
    //first we write into rows, later we transpose
    //algorithm here
    int i;
    multiplicationTables.resize(nVariables);
    for(i=0;i<nVariables;i++){
        multiplicationTables[i]=mzd_init(varietySize,varietySize);
    }
    
    //first we setup multiplication for values leading into standardmonomials
    
    for(i=0;i<standardMonomialsFromVector.size();i++){
        Monomial m=standardMonomialsFromVector[i];
        Monomial::const_iterator it=m.begin();
        Monomial::const_iterator end=m.end();
        while(it!=end){
            idx_type ring_var_index=*it;
            idx_type our_var_index=ring2Index[ring_var_index];
            Monomial divided=m/Variable(ring_var_index);
            size_t divided_index=standardMonomialsFrom2Index[divided];
            packedmatrix* mat=multiplicationTables[our_var_index];
            mzd_write_bit(mat, divided_index,i, 1);
        }
    }
    for(i=0;i<gbFrom.size();i++){
        Monomial lm=gbFrom[i].lm;
        MonomialSet tail=gbFrom[i].tail.diagram();
        MonomialSet::const_iterator it=tail.begin();
        MonomialSet::const_iterator end=tail.end();
        packedmatrix* row=mzd_init(1, varietySize);
        while(it!=end){
            idx_type tail_idx=standardMonomialsFrom2Index[*it];
            mzd_write_bit(row,tail_idx,0,1);
            it++;
        }
        
        Monomial::const_iterator it_lm=lm.begin();
        Monomial::const_iterator end_lm=lm.end();
        while(it_lm!=end_lm){
            idx_type ring_var_index=*it_lm;
            idx_type our_var_index=ring2Index[ring_var_index];
            Monomial divided=lm/Variable(ring_var_index);
            packedmatrix* mat=multiplicationTables[our_var_index];
            size_t divided_index=standardMonomialsFrom2Index[divided];
            int j;
            for(j=0;j<varietySize;j++){
                mzd_write_bit(mat, divided_index, j, mzd_read_bit(row,0,j));
            }
            it++;
        }
        //optimize that;
        mzd_free(row);
    }
    //todo: transpose
    for(i=0;i<nVariables;i++){
        //unnecassary many allocations of matrices
        packedmatrix* new_mat=mzd_init(varietySize,varietySize);
        mzd_transpose(new_mat, multiplicationTables[i]);
        mzd_free(multiplicationTables[i]);
        multiplicationTables[i]=new_mat;
    }
    BooleEnv::set(backup_ring);
}
void FGLMStrategy::analyzeGB(const ReductionStrategy& gb){
    ring_with_ordering_type backup_ring=BooleEnv::ring();
    BooleEnv::set(from);
    vars=gb.leadingTerms.usedVariables();
    int i;
    for (i=0;i<gb.size();i++){
        vars=vars * Monomial(gb[i].usedVariables, from);
    }
    size_t nVariables=vars.deg();
    idx_type ring_index;
    idx_type our_index=0;
    Monomial::const_iterator it=vars.begin();
    Monomial::const_iterator end=vars.end();
    while(it!=end){
        ring_index=*it;
        ring2Index[ring_index]=our_index;
        index2Ring[our_index]=ring_index;
        
        our_index++;
        it++;
    }
    
    standardMonomialsFrom=mod_mon_set(vars.divisors(), gb.leadingTerms);
    leadingTermsFrom=gb.leadingTerms;
    varietySize=standardMonomialsFrom.size();
    BooleEnv::set(backup_ring);
}

END_NAMESPACE_PBORIGB
