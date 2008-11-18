// 
//  fglm.cc
//  PolyBoRi
//  
//  Created by Michael Brickenstein on 2008-11-13.
//  Copyright 2008 The PolyBoRi Team.
// 
#include <exception>
#include "fglm.h"
#include "nf.h"
#include "interpolate.h"
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
void FGLMStrategy::writeTailToRow(MonomialSet tail, packedmatrix* row){

    MonomialSet::const_iterator it=tail.begin();
    MonomialSet::const_iterator end=tail.end();
            //optimize that;
    while(it!=end){
        idx_type tail_idx=standardMonomialsFrom2Index[*it];
        mzd_write_bit(row,0, tail_idx,1);
        it++;
    }
}
void FGLMStrategy::writeRowToVariableDivisors(packedmatrix* row, Monomial lm){
    Monomial::const_iterator it_lm=lm.begin();
    Monomial::const_iterator end_lm=lm.end();
    bool first=true;
    while(it_lm!=end_lm){
        idx_type ring_var_index=*it_lm;
        idx_type our_var_index=ring2Index[ring_var_index];
        Monomial divided=lm/Variable(ring_var_index);
        if (standardMonomialsFrom.owns(divided)){
            packedmatrix* mat=multiplicationTables[our_var_index];
            size_t divided_index=standardMonomialsFrom2Index[divided];

            if (first){
                monomial2MultiplicationMatrix[lm]=our_var_index;
                monomial2MultiplicationMatrixRowIndex[lm]=divided_index;
                first=false;
            }
            int j;
            if (transposed){
                for(j=0;j<varietySize;j++){
                    mzd_write_bit(mat, j, divided_index, mzd_read_bit(row,0,j));
                }
            } else {
                for(j=0;j<varietySize;j++){
                    mzd_write_bit(mat, divided_index, j, mzd_read_bit(row,0,j));
                }
            }
        }
        it_lm++;
    }
}

void transpose_window_to_row(packedmatrix* transposed_vec, packedmatrix* window){
    int i;
    const int n=window->nrows;
    for(i=0;i<n;i++){
        mzd_write_bit(transposed_vec,0,i, mzd_read_bit(window,i,0));
    }
}
Polynomial FGLMStrategy::rowToPoly(packedmatrix* row){
    MonomialVector vec;
    int i;
    for(i=0;i<varietySize;i++){
        if (mzd_read_bit(row,0,i)==1){
            vec.push_back(standardMonomialsFromVector[i]);
        }
    }
    return add_up_monomials(vec);
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
    
    //standard monomials
    
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
            //finally treat the "edge" case: m*v->m, where v divides m
            mzd_write_bit(mat,i,i,1);
            it++;
        }
        
    }
    
    //leading monomials from gb: vertices/
    packedmatrix* row=mzd_init(1, varietySize);
    for(i=0;i<gbFrom.size();i++){
        Monomial lm=gbFrom[i].lm;
        MonomialSet tail=gbFrom[i].tail.diagram();
        mzd_row_clear_offset(row,0,0);
        writeTailToRow(tail, row);
        writeRowToVariableDivisors(row,lm);
        
    }
    mzd_free(row);
    //edges
    MonomialSet edges=standardMonomialsFrom.cartesianProduct(varsSet).
        diff(standardMonomialsFrom).diff(leadingTermsFrom);
    Polynomial edges_poly=edges;
    MonomialVector edges_vec(edges.size());
    std::copy(edges_poly.orderedBegin(), edges_poly.orderedEnd(), edges_vec.begin());
    
    //reverse is important, so that divisors and elements in the tail have already been treated
    
    transposeMultiplicationTables();
    
    MonomialVector::reverse_iterator it_edges=edges_vec.rbegin();
    MonomialVector::reverse_iterator end_edges=edges_vec.rend();
    MonomialSet EdgesUnitedVertices=edges.unite(leadingTermsFrom);
    
    packedmatrix* multiplied_row=mzd_init(1,varietySize);
    packedmatrix* reduced_problem_to_row=mzd_init(1,varietySize);
    while(it_edges!=end_edges){
        mzd_row_clear_offset(multiplied_row, 0, 0);
        Monomial m=*it_edges;

        MonomialSet candidates=Polynomial(EdgesUnitedVertices.divisorsOf(m)).gradedPart(m.deg()-1).set();
        
        Monomial reduced_problem_to=*(candidates.begin());

        Monomial v_m=m/reduced_problem_to;

        assert (v_m.deg()==1);
        Variable var=*v_m.variableBegin();
        packedmatrix* mult_table=multiplicationTableForVariable(var);
        
        findVectorInMultTables(reduced_problem_to_row, reduced_problem_to);

        if (!(transposed)){
            
        //standardMonomialsFrom2Index[reduced_problem_to];
        
        //highly inefficient/far to many allocations
        
        //mzd_mul expects second arg to be transposed
        //which is a little bit tricky as we multiply from left
        //packedmatrix* transposed_mult_table=mzd_transpose(NULL, mult_table);
        mzd_mul_naiv(multiplied_row,reduced_problem_to_row, mult_table);
        
        } else {
            //packedmatrix* transposed_vec=mzd_init(1,varietySize);
            //assert (window->nrows==varietySize);
            //assert (window->ncols==1);
            //transpose_window_to_row(transposed_vec, window);
            _mzd_mul_naiv(multiplied_row, reduced_problem_to_row, mult_table);
            //mzd_free(transposed_vec);
        }

        writeRowToVariableDivisors(multiplied_row, m);
        //matrices are transposed, so now we have write to columns
        
        //mzd_free(transposed_mult_table);
        //mzd_free_window(window);
        it_edges++;
    }
    mzd_free(reduced_problem_to_row);
   
    mzd_free(multiplied_row);
    
    
    
    
    
    
    BooleEnv::set(backup_ring);
}
void FGLMStrategy::findVectorInMultTables(packedmatrix* dst, Monomial m){
    packedmatrix* mat=multiplicationTables[monomial2MultiplicationMatrix[m]];
    size_t idx=monomial2MultiplicationMatrixRowIndex[m];
    if (!(transposed))
        mzd_submatrix(dst, mat, idx, 0, idx+1, varietySize);
    else{
        const int n=varietySize;
        int i;
        for(i=0;i<n;i++){
            mzd_write_bit(dst, 0, i, mzd_read_bit(mat,i,idx));
        }
    }
}
void FGLMStrategy::transposeMultiplicationTables(){
    //From now on, we multiply, so here we transpose
    int i;
    //packedmatrix* new_mat=mzd_init(varietySize,varietySize);
    packedmatrix* swap;
    for(i=0;i<multiplicationTables.size();i++){
        //unnecassary many allocations of matrices
        packedmatrix* new_mat=mzd_init(varietySize,varietySize);
        mzd_transpose(new_mat, multiplicationTables[i]);
        
        swap=new_mat;
        new_mat=multiplicationTables[i];
        multiplicationTables[i]=swap;
        mzd_free(new_mat);
        {
            #ifdef DRAW_MATRICES
                char matname[255];
                sprintf(matname,"mult_table%d.png",i);

                drawmatrix(multiplicationTables[i],matname);
            #endif
        }
    }
    //mzd_free(new_mat);
    transposed=(!(transposed));
}
void FGLMStrategy::analyzeGB(const ReductionStrategy& gb){
    ring_with_ordering_type backup_ring=BooleEnv::ring();
    BooleEnv::set(from);
    vars=gb.leadingTerms.usedVariables();
    int i;
    for (i=0;i<gb.size();i++){
        vars=vars * Monomial(gb[i].usedVariables,BooleEnv::ring());
    }
    
    Monomial::variable_iterator it_var=vars.variableBegin();
    Monomial::variable_iterator end_var=vars.variableEnd();
    while (it_var!=end_var){
        varsVector.push_back(*it_var);
        it_var++;
    }
    VariableVector::reverse_iterator it_varvec=varsVector.rbegin();
    VariableVector::reverse_iterator end_varvec=varsVector.rend();
    while(it_varvec!=end_varvec){
        varsSet=varsSet.unite(Monomial(*it_varvec).diagram());
        it_varvec++;
    }


    nVariables=vars.deg();
    ring2Index.resize(BooleEnv::ring().nVariables());
    index2Ring.resize(nVariables);
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
class FGLMNoLinearCombinationException: public std::exception
{
 
};
FGLMStrategy::IndexVector FGLMStrategy::rowVectorIsLinearCombinationOfRows(packedmatrix* mat, packedmatrix* v){
    //returns vector with indices, where the coefficients in this linear combination are 1
    //if no such combination exists, raises Exception
    #ifdef DRAW_MATRICES
    static int round=0;
    round++;
    assert (mat->ncols==varietySize);
    assert (v->ncols==varietySize);
    assert (v->nrows==1);
    #endif
    packedmatrix* row_combined=mzd_stack(NULL, mat, v);
    {        
        #ifdef DRAW_MATRICES
            char matname[255];
            sprintf(matname,"row_combined%d.png",round);

            drawmatrix(row_combined,matname);
        #endif
    }
    packedmatrix* col_combined=mzd_transpose(NULL, row_combined);
    mzd_free(row_combined);
    
    mzd_reduce_m4ri(col_combined,TRUE,0,NULL,NULL);
    {        
        #ifdef DRAW_MATRICES
            char matname[255];
            sprintf(matname,"col_reduced%d.png",round);

            drawmatrix(col_combined,matname);
        #endif
    }
    const int cols=col_combined->ncols;
    const int rows=col_combined->nrows;
    assert (rows>=cols-1);
    const int ngenerators=cols-1;
    const int last_col=cols-1;
    int i;
    IndexVector res;
    
    //first col-1 cols are linear independend -> reduced row echelon form has unimat het
    for(i=rows-1;i>ngenerators-1;i--){
        assert(i>=last_col);
        if (mzd_read_bit(col_combined,i,last_col)==1){
            //no inhomgeneous solution exists
            mzd_free(col_combined);
            FGLMNoLinearCombinationException ex;
            throw ex;
            
        }
    }
    for(i=ngenerators-1;i>=0;i--){
        assert(i<last_col);
        if (mzd_read_bit(col_combined,i, last_col)==1){
            assert(mzd_read_bit(col_combined,i,i)==1);
            res.push_back(i);
        }
    }
    mzd_free(col_combined);
    
    return res;
}
PolynomialVector FGLMStrategy::main(){
    PolynomialVector F;
    const Monomial monomial_one;
    if (leadingTermsFrom.owns(monomial_one)){
        F.push_back(monomial_one);
        return F;
    }
    ring_with_ordering_type bak_ring=BooleEnv::ring();
    //variables are oriented at Tim Wichmanns Diploma thesis
    BooleEnv::set(to);

    int i;
    
    typedef std::set<Monomial> MonomialSetSTL;
    
    MonomialSetSTL C;

    lm2Index_map_type mon2index;

    //initialize with one monomial
    packedmatrix* v=mzd_init(varietySize, varietySize);//write vectors in rows;
    MonomialSet b_set=Polynomial(1).diagram();
    MonomialVector b;
    b.push_back(monomial_one);
    mzd_write_bit(v,0,0,1);
    for(i=0;i<varsVector.size();i++){
        C.insert(varsVector[i]);
    }
    mon2index[monomial_one]=0;
    packedmatrix* v_d=mzd_init(1,varietySize);
    while(!(C.empty())){
        const int d=b.size();
        Monomial m=*(C.begin());
        
        //TODO: multiply
        C.erase(C.begin());
        
        assert(m!=monomial_one);
        Polynomial divisors;
        if ((divisors=Polynomial(b_set.divisorsOf(m)).gradedPart(m.deg()-1)).length()==m.deg()/*varsM=Zm,Ecke oder Standard Monom*/) {
            
            //packedmatrix* v_d=mzd_init(1,varietySize);
            mzd_row_clear_offset(v_d,0,0);
            assert(varietySize>0);
            
            Monomial b_j=*divisors.begin();
            int j=mon2index[b_j];
            Monomial x_i_m=(m/b_j);
            assert (x_i_m.deg()==1);
            Variable x_i=*x_i_m.variableBegin();
            packedmatrix* mult_table=multiplicationTableForVariable(x_i);
            packedmatrix* v_j=mzd_init_window(v,j,0,j+1,varietySize);
            
            assert (v_j->nrows==1);
            assert ( v_j->ncols==varietySize);
            v_d=_mzd_mul_naiv(v_d, v_j, mult_table);
            mzd_free_window(v_j);
            assert (v_d->nrows==1);
            assert (v_d->ncols=varietySize);
            packedmatrix* v_window=mzd_init_window(v,0,0,d,varietySize);
            try
            {    
               
                IndexVector lin_combination=rowVectorIsLinearCombinationOfRows(v_window, v_d);
                MonomialVector p_vec;
                for(i=0;i<lin_combination.size();i++){
                    assert (lin_combination[i]<b.size());
                    p_vec.push_back(b[lin_combination[i]]);
                }
                Polynomial p=add_up_monomials(p_vec)+m;
                F.push_back(p);

                
            }
            catch (FGLMNoLinearCombinationException& e)
            {   

                b_set=b_set.unite(m.diagram());
                b.push_back(m);
                packedmatrix* copy_window=mzd_init_window(v,d,0,d+1,varietySize);
                mzd_copy(copy_window, v_d);
                mzd_free_window(copy_window);
                //packedmatrix* v_new=mzd_stack(NULL, v, v_d);
                //assert(mzd_read_bit(v_new,0,0)==1);
                //mzd_free(v);
                //v=v_new;
                for(i=0;i<varsVector.size();i++){
                    Variable var=varsVector[i];
                    if (!(m.reducibleBy(var))){
                        Monomial m_v=var*m;
                        C.insert(m_v);
                    }
                }
                mon2index[m]=b.size()-1;
            }
            mzd_free_window(v_window);
           
        } 
        
    }
    mzd_free(v_d);
    mzd_free(v);
    BooleEnv::set(bak_ring);
    return F;
}
void FGLMStrategy::testMultiplicationTables(){
    #ifndef NDEBUG
    ring_with_ordering_type backup_ring=BooleEnv::ring();
    BooleEnv::set(from);
    int i;
    int j;
    
    for(i=0;i<varsVector.size();i++){
        Variable v=varsVector[i];
        assert (v.index()>=i);
        for (j=0;j<standardMonomialsFromVector.size(); j++){
            Monomial m=standardMonomialsFromVector[j];
            packedmatrix* table=multiplicationTableForVariable(v);
            int k;
            if (m==v){continue;}
            Polynomial product=reducedNormalFormInFromRing(m*v);

            MonomialSet product_set=product.diagram();
            Polynomial sum;
            for(k=0;k<varietySize;k++){
                Monomial m2=standardMonomialsFromVector[k];
                
                if (mzd_read_bit(table,k,j)==1){
                    sum+=m2;
                }

            }
            if (sum!=product)
                cout<<"v:"<<v<<"\tm:"<<m<<"\tsum:"<<sum<<"\tproduct:"<<product<<endl;
            assert(sum==product);
        }
    }
    BooleEnv::set(backup_ring);
    #endif
}
Polynomial FGLMStrategy::reducedNormalFormInFromRing(Polynomial f){
    ring_with_ordering_type bak_ring=BooleEnv::ring();
    BooleEnv::set(from);
    Polynomial res=gbFrom.reducedNormalForm(f);
    BooleEnv::set(bak_ring);
    return res;
    
}
END_NAMESPACE_PBORIGB
