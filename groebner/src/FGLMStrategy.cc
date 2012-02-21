// 
//  FGLMStrategy.cc
//  PolyBoRi
//  
//  Created by Michael Brickenstein on 2008-11-13.
//  Copyright 2008 The PolyBoRi Team.
// 
#include <exception>
#include <polybori/groebner/FGLMStrategy.h>
#include <polybori/groebner/nf.h>
#include <polybori/groebner/add_up.h>
#include <polybori/groebner/interpolate.h>
#include <polybori/groebner/draw_matrix.h>

using namespace std;
BEGIN_NAMESPACE_PBORIGB


#if 0
static void mult_by_combining_rows(mzd_t* dest, mzd_t* A, packedmatrix* B, packedmatrix* acc1, packedmatrix* acc2, FGLMStrategy::IndexVector& row_is_monomial){

    int i,j;
    const int m=A->nrows;
    const int n=A->ncols;
    //const int nblocks=A->width;
    mzd_t* res_row=acc1;
    mzd_t* dest_row=acc2;
    PBORI_ASSERT(acc1->ncols==B->ncols);
    PBORI_ASSERT(acc2->ncols==B->ncols);
    PBORI_ASSERT(dest->nrows==A->nrows);
    PBORI_ASSERT(dest->ncols==B->ncols);
    
    for(i=0;i<m;i++){
        mzd_row_clear_offset(acc1, 0,0);
        mzd_row_clear_offset(acc2, 0,0);
        mzd_row_clear_offset(dest,i,0);
        
        for(j=0;j<n;j+=RADIX){
            if (mzd_read_block(A,i,j)!=0){
                const int j_bak=j;
                for(j=j_bak;(j<j_bak+RADIX) && (j<n);j++){
                    if (mzd_read_bit(A,i,j)==1){
                        int monomial_idx;
                        if ((monomial_idx=row_is_monomial[j])>=0){
                            mzd_write_bit(res_row, 0, monomial_idx, (1+ mzd_read_bit(res_row,0,monomial_idx))%2);
                        } else {
                            mzd_combine(dest_row,0,0,B,j,0,res_row,0,0);
                            std::swap(res_row,dest_row);
                        }
                    }
                }
                j=j_bak;

            }
        }

            //how to do that most efficiently?
            //copy res_row to dest

            //mzd_row_clear_offset(dest_row,0,0);
            //mzd_combine(dest,i,0,res_row,0,0,dest_row,0,0);

        mzd_copy_row(dest,i,res_row,0);
    }
        
       
//if 0
    int i,j;
       const int m=A->nrows;
       const int n=A->ncols;
       mzd_t* res_row=acc1;
       mzd_t* dest_row=acc2;
       PBORI_ASSERT(acc1->ncols==B->ncols);
       PBORI_ASSERT(acc2->ncols==B->ncols);
       PBORI_ASSERT(dest->nrows==A->nrows);
       PBORI_ASSERT(dest->ncols==B->ncols);
       for(i=0;i<m;i++){
           mzd_row_clear_offset(acc1, 0,0);
           mzd_row_clear_offset(acc2, 0,0);
           mzd_row_clear_offset(dest,i,0);
           for(j=0;j<n;j++){
               if (mzd_read_bit(A,i,j)){
                   mzd_combine(dest_row,0,0,B,j,0,res_row,0,0);
                   std::swap(res_row,dest_row);
               }
           }
           //how to do that most efficiently?
           //copy res_row to dest

           //mzd_row_clear_offset(dest_row,0,0);
           //mzd_combine(dest,i,0,res_row,0,0,dest_row,0,0);

           mzd_copy_row(dest,i,res_row,0);
       }

//endif
    
}
#endif


void FGLMStrategy::setupStandardMonomialsFromTables(){

     standardMonomialsFromVector.resize(varietySize, from);
     MonomialSet::const_iterator it_set=standardMonomialsFrom.begin();
     MonomialSet::const_iterator end_set=standardMonomialsFrom.end();
     //assume only that iteration is descending w.r.t. divisibility
     
     int i=standardMonomialsFrom.size()-1;
     while(it_set!=end_set){
         Monomial m=*it_set;
         standardMonomialsFrom2Index[m]=i;
         standardExponentsFrom2Index[m.exp()]=i;
         standardMonomialsFromVector[i]=m;
         it_set++;
         i--;
     }

}
#if 0
void FGLMStrategy::writeTailToRow(MonomialSet tail, mzd_t* row){

    MonomialSet::const_iterator it=tail.begin();
    MonomialSet::const_iterator end=tail.end();
            //optimize that;
    while(it!=end){
        idx_type tail_idx=standardMonomialsFrom2Index[*it];
        mzd_write_bit(row,0, tail_idx,1);
        it++;
    }
}
#else
void FGLMStrategy::writeTailToRow(MonomialSet tail, mzd_t* row){

    MonomialSet::exp_iterator it=tail.expBegin();
    MonomialSet::exp_iterator end=tail.expEnd();
            //optimize that;
    while(it!=end){
        idx_type tail_idx=standardExponentsFrom2Index[*it];
        mzd_write_bit(row,0, tail_idx,1);
        it++;
    }
}
#endif
void FGLMStrategy::writeRowToVariableDivisors(mzd_t* row, Monomial lm){
    Monomial::const_iterator it_lm=lm.begin();
    Monomial::const_iterator end_lm=lm.end();
    Exponent exp=lm.exp();
    bool first=true;
    while(it_lm!=end_lm){
        idx_type ring_var_index=*it_lm;
        idx_type our_var_index=ring2Index[ring_var_index];
        Exponent divided=exp.removeConst(ring_var_index);
        if (standardMonomialsFrom.owns(divided)){
            mzd_t* mat=multiplicationTables[our_var_index];
            size_t divided_index=standardExponentsFrom2Index[divided];

            if (first){
                monomial2MultiplicationMatrix[lm]=our_var_index;
                monomial2MultiplicationMatrixRowIndex[lm]=divided_index;
                first=false;
            }
            if (transposed){
              for(std::size_t j=0;j<varietySize;j++){
                    mzd_write_bit(mat, j, divided_index, mzd_read_bit(row,0,j));
                }
            } else {
                
                /*mzd_t* window=mzd_init_window(mat,divided_index,0,divided_index+1,varietySize);
                mzd_copy(window,row);
                mzd_free_window(window);*/
                
                mzd_copy_row(mat, divided_index, row,0);
                
                /*for(j=0;j<varietySize;j++){
                    mzd_write_bit(mat, divided_index, j, mzd_read_bit(row,0,j));
                }*/
            }
        }
        it_lm++;
    }
}

void transpose_window_to_row(mzd_t* transposed_vec, mzd_t* window){
    int i;
    const int n=window->nrows;
    for(i=0;i<n;i++){
        mzd_write_bit(transposed_vec,0,i, mzd_read_bit(window,i,0));
    }
}
Polynomial FGLMStrategy::rowToPoly(mzd_t* row){
    MonomialVector vec;
    for(std::size_t i=0;i<varietySize;i++){
        if (mzd_read_bit(row,0,i)==1){
            vec.push_back(standardMonomialsFromVector[i]);
        }
    }
    return add_up_monomials(vec, to);
}

void FGLMStrategy::setupMultiplicationTables(){
    
    //first we write into rows, later we transpose
    //algorithm here
    multiplicationTables.resize(nVariables);
    tableXRowYIsMonomialFromWithIndex.resize(nVariables);
    for(std::size_t i=0; i < nVariables;i++){
        multiplicationTables[i]=mzd_init(varietySize,varietySize);
        tableXRowYIsMonomialFromWithIndex[i].resize(varietySize);
        for(std::size_t j=0;j<varietySize;j++){
            tableXRowYIsMonomialFromWithIndex[i][j]=-1;
        }
    }
    
    //standard monomials
    
    for(std::size_t i=0;i<standardMonomialsFromVector.size();i++){
        Monomial m=standardMonomialsFromVector[i];
        Monomial::const_iterator it=m.begin();
        Monomial::const_iterator end=m.end();
        while(it!=end){
            idx_type ring_var_index=*it;
            idx_type our_var_index=ring2Index[ring_var_index];
            Monomial divided=m / Variable(ring_var_index, m.ring());
            size_t divided_index=standardMonomialsFrom2Index[divided];
            mzd_t* mat=multiplicationTables[our_var_index];
            mzd_write_bit(mat, divided_index,i, 1);
            tableXRowYIsMonomialFromWithIndex[our_var_index][divided_index]=i;
            //finally treat the "edge" case: m*v->m, where v divides m
            mzd_write_bit(mat,i,i,1);
            tableXRowYIsMonomialFromWithIndex[our_var_index][i]=i;
            it++;
        }

        
    }
    
    //leading monomials from gb: vertices/
    mzd_t* row=mzd_init(1, varietySize);
    ReductionStrategy::const_iterator start(gbFrom.begin()), 
      finish(gbFrom.end());
    while (start != finish){
        Monomial lm = start->lead;
        MonomialSet tail = start->tail.set();
        mzd_row_clear_offset(row,0,0);
        writeTailToRow(tail, row);
        writeRowToVariableDivisors(row,lm);

        ++start;
    }
    mzd_free(row);
    //edges
    MonomialSet edges=standardMonomialsFrom.cartesianProduct(varsSet).
        diff(standardMonomialsFrom).diff(leadingTermsFrom);
    Polynomial edges_poly=edges;
    MonomialVector edges_vec(edges.size(), from);
    std::copy(edges_poly.orderedBegin(), edges_poly.orderedEnd(), edges_vec.begin());
    
    //reverse is important, so that divisors and elements in the tail have already been treated
    
    
    
    MonomialVector::reverse_iterator it_edges=edges_vec.rbegin();
    MonomialVector::reverse_iterator end_edges=edges_vec.rend();
    edgesUnitedVerticesFrom=edges.unite(leadingTermsFrom);
    
    mzd_t* multiplied_row=mzd_init(1,varietySize);
    mzd_t* reduced_problem_to_row=mzd_init(1,varietySize);
    
    mzd_t* acc1=mzd_init(1, varietySize);
    mzd_t* acc2=mzd_init(1, varietySize);
    
    while(it_edges!=end_edges){
        mzd_row_clear_offset(multiplied_row, 0, 0);
        Monomial m=*it_edges;

        MonomialSet candidates=Polynomial(edgesUnitedVerticesFrom.divisorsOf(m)).gradedPart(m.deg()-1).set();
        
        Monomial reduced_problem_to=*(candidates.begin());

        Monomial v_m=m/reduced_problem_to;

        PBORI_ASSERT (v_m.deg()==1);
        Variable var=*v_m.variableBegin();
        mzd_t* mult_table=multiplicationTableForVariable(var);
        
        findVectorInMultTables(reduced_problem_to_row, reduced_problem_to);

        if (!(transposed)){
            
        //standardMonomialsFrom2Index[reduced_problem_to];
        
        //highly inefficient/far to many allocations
        
        //mzd_mul expects second arg to be transposed
        //which is a little bit tricky as we multiply from left
        //mzd_t* transposed_mult_table=mzd_transpose(NULL, mult_table);
        
        //mzd_mul_naiv(multiplied_row,reduced_problem_to_row, mult_table);
        #if 0
        mult_by_combining_rows(multiplied_row, reduced_problem_to_row, 
            mult_table, acc1, acc2,tableXRowYIsMonomialFromWithIndex[ring2Index[var.index()]]);
        #else
        _mzd_mul_va(multiplied_row, reduced_problem_to_row, mult_table, 1);
        #endif
        } else {
            //mzd_t* transposed_vec=mzd_init(1,varietySize);
            //PBORI_ASSERT (window->nrows==varietySize);
            //PBORI_ASSERT (window->ncols==1);
            //transpose_window_to_row(transposed_vec, window);
            _mzd_mul_va(multiplied_row, reduced_problem_to_row, mult_table, FALSE);
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
    
    mzd_free(acc1);
    mzd_free(acc2);
    
    //transposeMultiplicationTables();
    
    {
        #ifdef DRAW_MATRICES
      for(std::size_t i=0;i<multiplicationTables.size();i++){
            char matname[255];
            sprintf(matname,"mult_table%d.png",(int)i);

            draw_matrix(multiplicationTables[i],matname);
        }
        #endif
    }
}
void FGLMStrategy::findVectorInMultTables(mzd_t* dst, Monomial m){
    mzd_t* mat=multiplicationTables[monomial2MultiplicationMatrix[m]];
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
void clear_mat(mzd_t* mat) {
  for(int i = 0; i < mat->nrows; ++i) {
    mzd_row_clear_offset(mat,i,0);
  }
}
void FGLMStrategy::transposeMultiplicationTables(){
    //From now on, we multiply, so here we transpose
    mzd_t* new_mat=mzd_init(varietySize,varietySize);
    mzd_t* swap;
    for(std::size_t i=0;i<multiplicationTables.size();i++){
        //unnecassary many allocations of matrices
        //mzd_t* new_mat=mzd_init(varietySize,varietySize);
        clear_mat(new_mat);
        mzd_transpose(new_mat, multiplicationTables[i]);
        
        swap=new_mat;
        new_mat=multiplicationTables[i];
        multiplicationTables[i]=swap;
        //mzd_free(new_mat);

    }
    mzd_free(new_mat);
    transposed=(!(transposed));
}
void FGLMStrategy::analyzeGB(const ReductionStrategy& gb){

    vars=gb.leadingTerms.usedVariables();
    for (std::size_t i=0;i<gb.size();i++){
        vars=vars * Monomial(gb[i].usedVariables, from);
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


    nVariables=vars.size();
    ring2Index.resize(from.nVariables());
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
}

class FGLMNoLinearCombinationException: public std::exception
{
public:
    size_t firstNonZeroIndex;
    FGLMNoLinearCombinationException(size_t firstNonZeroIndex){
        this->firstNonZeroIndex=firstNonZeroIndex;
    }
};
#if 0
FGLMStrategy::IndexVector FGLMStrategy::rowVectorIsLinearCombinationOfRows(mzd_t* mat, mzd_t* v){
    //returns vector with indices, where the coefficients in this linear combination are 1
    //if no such combination exists, raises Exception
    #ifdef DRAW_MATRICES
    static int round=0;
    round++;
    PBORI_ASSERT (mat->ncols==varietySize);
    PBORI_ASSERT (v->ncols==varietySize);
    PBORI_ASSERT (v->nrows==1);
    #endif
    mzd_t* row_combined=mzd_stack(NULL, mat, v);
    {        
        #ifdef DRAW_MATRICES
            char matname[255];
            sprintf(matname,"row_combined%d.png",round);

            draw_matrix(row_combined,matname);
        #endif
    }
    mzd_t* col_combined=mzd_transpose(NULL, row_combined);
    mzd_free(row_combined);
    
    mzd_echelonize_m4ri(col_combined,TRUE,0,NULL,NULL);
    {        
        #ifdef DRAW_MATRICES
            char matname[255];
            sprintf(matname,"col_reduced%d.png",round);

            draw_matrix(col_combined,matname);
        #endif
    }
    const int cols=col_combined->ncols;
    const int rows=col_combined->nrows;
    PBORI_ASSERT (rows>=cols-1);
    const int ngenerators=cols-1;
    const int last_col=cols-1;
    int i;
    IndexVector res;
    
    //first col-1 cols are linear independend -> reduced row echelon form has unimat het
    for(i=rows-1;i>ngenerators-1;i--){
        PBORI_ASSERT(i>=last_col);
        if (mzd_read_bit(col_combined,i,last_col)==1){
            //no inhomgeneous solution exists
            mzd_free(col_combined);
            FGLMNoLinearCombinationException ex;
            throw ex;
            
        }
    }
    for(i=ngenerators-1;i>=0;i--){
        PBORI_ASSERT(i<last_col);
        if (mzd_read_bit(col_combined,i, last_col)==1){
            PBORI_ASSERT(mzd_read_bit(col_combined,i,i)==1);
            res.push_back(i);
        }
    }
    mzd_free(col_combined);
    
    return res;
}
#else
FGLMStrategy::IndexVector FGLMStrategy::rowVectorIsLinearCombinationOfRows(mzd_t* mat, mzd_t* v){
    const int d=mat->nrows-1;
    mzd_row_clear_offset(mat,d,0);
    PBORI_ASSERT (mat->ncols==2*varietySize);
    
    //mzd_t* copy_v_into=mzd_init_window(mat,d,0,d+1,varietySize);
    //mzd_copy(copy_v_into,v);
    //mzd_free_window(copy_v_into);
    
    mzd_copy_row(mat,d,v,0);
    //mzd_row_clear_offset(mat,d,varietySize);//might be random data at the end as mat is wider


    for(std::size_t i=0;i<varietySize;i++){
        if (mzd_read_bit(mat,d,i)==1){
            bool succ=false;
            int row_idx=rowStartingWithIndex[i];
            if (row_idx>=0){
                succ=true;
                int standard_idx;
                if ((standard_idx=rowIsStandardMonomialToWithIndex[row_idx])>=0){
                    mzd_write_bit(mat,d,i,0);
                    const int standard_idx_with_offset=varietySize+standard_idx;
                    mzd_write_bit(mat,d,standard_idx_with_offset,(1+mzd_read_bit(mat,d,standard_idx_with_offset))%2);
                } else {
                    mzd_row_add_offset(mat, d, row_idx, i);
                }
                
            }

            if (!(succ)){
                FGLMNoLinearCombinationException ex(i);
                throw ex;
                
            }
        }
    }
    IndexVector res;
    for(int i=0;i<d;i++){
        if (mzd_read_bit(mat,d,i+varietySize)==1){
            res.push_back(i);
        }
    }
    return res;
}
#endif
PolynomialVector FGLMStrategy::main(){
    PolynomialVector F;
    const Monomial monomial_one(to);

    if (leadingTermsFrom.owns(monomial_one)){
        F.push_back(monomial_one);
        return F;
    }
    //variables are oriented at Tim Wichmanns Diploma thesis

    mzd_t* acc1=mzd_init(1, varietySize);
    mzd_t* acc2=mzd_init(1, varietySize);

    
    typedef std::set<Monomial> MonomialSetSTL;
    
    MonomialSetSTL C;

    lm2Index_map_type mon2index;
    Exponent::idx_map_type exp2index;
    //initialize with one monomial
    mzd_t* v=mzd_init(varietySize, varietySize);//write vectors in rows;
    mzd_t* w=mzd_init(varietySize+1, varietySize*2);
    IndexVector w_start_indices;
    rowStartingWithIndex.resize(varietySize);
    rowIsStandardMonomialToWithIndex.resize(varietySize);
    for(std::size_t i=0;i<rowStartingWithIndex.size();i++){
        rowStartingWithIndex[i]=-1;
        rowIsStandardMonomialToWithIndex[i]=-1;
    }
    MonomialSet b_set=monomial_one.set();
    MonomialVector b;
    b.push_back(monomial_one);
    mzd_write_bit(v,0,0,1);
    mzd_write_bit(w,0,0,1);
    mzd_write_bit(w,0,varietySize+0,1);
    w_start_indices.push_back(0);
    rowStartingWithIndex[0]=0;
    rowIsStandardMonomialToWithIndex[0]=0;

    VariableVector varsVectorTo;
    varsVectorTo.reserve(varsVector.size());
    for(std::size_t i=0;i<varsVector.size();i++){
      varsVectorTo.push_back(to.coerce(varsVector[i]));
      C.insert(varsVectorTo.back());
    }

    mon2index[monomial_one]=0;
    exp2index[monomial_one.exp()]=0;
    mzd_t* v_d=mzd_init(1,varietySize);
    
    while(!(C.empty())){
        const int d=b.size();
        Monomial m=*(C.begin());

        C.erase(C.begin());
        
        PBORI_ASSERT(m!=monomial_one);
        Polynomial divisors(to);
        PBORI_ASSERT(b_set.containsDivisorsOfDecDeg(m)==(Polynomial(b_set.divisorsOf(m)).gradedPart(m.deg()-1).length()==m.deg()));
        if (b_set.containsDivisorsOfDecDeg(m)/*varsM=Zm,Ecke oder Standard Monom*/) {
            Polynomial divisors=Polynomial(b_set.divisorsOf(m)).gradedPart(m.deg()-1);
            mzd_row_clear_offset(v_d,0,0);
            PBORI_ASSERT(varietySize>0);
            bool is_standard_monomial_from=false;
            
            if (edgesUnitedVerticesFrom.owns(m)){
                findVectorInMultTables(v_d, m);
            } else {
                if (standardMonomialsFrom.owns(m)){
                    mzd_write_bit(v_d,0, standardMonomialsFrom2Index[m],1);
                    is_standard_monomial_from=true;
                } else{
                    Exponent b_j=*divisors.expBegin();
                    int j=exp2index[b_j];
                    Exponent x_i_m=(m.exp()-b_j);
                    PBORI_ASSERT (x_i_m.deg()==1);
                    //Variable x_i=*x_i_m.variableBegin();
                    idx_type our_x_i_index=ring2Index[*x_i_m.begin()];
                    mzd_t* mult_table=multiplicationTables[our_x_i_index];//multiplicationTableForVariable(x_i);
                    mzd_t* v_j=mzd_init_window(v,j,0,j+1,varietySize);

                    PBORI_ASSERT (v_j->nrows==1);
                    PBORI_ASSERT ( v_j->ncols==varietySize);
                    if (transposed)
                        v_d=_mzd_mul_va(v_d, v_j, mult_table, FALSE);
                    else{
                        #if 0
                        mult_by_combining_rows(v_d, v_j, mult_table, acc1, acc2, tableXRowYIsMonomialFromWithIndex[our_x_i_index]);
                        #else
                        _mzd_mul_va(v_d, v_j, mult_table, 1);
                        #endif
                    }
                    mzd_free_window(v_j);
                }
                
            }
            
            PBORI_ASSERT (v_d->nrows==1);
            PBORI_ASSERT (v_d->ncols==varietySize);
            mzd_t* w_window=mzd_init_window(w,0,0,d+1,2*varietySize);
            //mzd_t* w_row_window=mzd_init_window(w,d,0,d+1,varietySize);
            try
            {    
                
                
                IndexVector lin_combination=rowVectorIsLinearCombinationOfRows(w_window,  v_d);
                MonomialVector p_vec;
                for(std::size_t i=0;i<lin_combination.size();i++){
                    PBORI_ASSERT (lin_combination[i]<b.size());
                    p_vec.push_back(b[lin_combination[i]]);
                }
                Polynomial p=add_up_monomials(p_vec, to.zero())+m;
                F.push_back(p);

                
            }
            catch (FGLMNoLinearCombinationException& e)
            {   

                b_set=b_set.unite(m.diagram());
                b.push_back(m);
                
                rowStartingWithIndex[e.firstNonZeroIndex]=d;
                mzd_write_bit(w,d,varietySize+d,1);
                if (is_standard_monomial_from){
                    std::size_t from_idx=standardMonomialsFrom2Index[m];
                    if (e.firstNonZeroIndex==from_idx){
                        //we assume, the row is untached
                        rowIsStandardMonomialToWithIndex[d]=d;
                        //might swap rows and use a vector of bools
                    } else {
                        const int reduced_with_this_row=rowStartingWithIndex[from_idx];
                        PBORI_ASSERT(reduced_with_this_row>=0);
                        mzd_row_clear_offset(w, reduced_with_this_row,0);
                        mzd_write_bit(w, reduced_with_this_row, from_idx,1);
                        mzd_write_bit(w, reduced_with_this_row, varietySize+d,1);
                        rowIsStandardMonomialToWithIndex[reduced_with_this_row]=d;
                        //still generate the same vector space
                    }
                }
                /*mzd_t* copy_window=mzd_init_window(v,d,0,d+1,varietySize);
                mzd_copy(copy_window, v_d);
                mzd_free_window(copy_window);*/
                mzd_copy_row(v,d,v_d,0);
                
                idx_type m_begin=*m.begin();
                for(std::size_t i=0;(i<varsVectorTo.size())&&(index2Ring[i]<m_begin);i++){
                  Variable var=varsVectorTo[i];
                    if (!(m.reducibleBy(var))){
                      Monomial m_v= var*m;
                        C.insert(m_v);
                    }
                }
                mon2index[m]=b.size()-1;
                exp2index[m.exp()]=b.size()-1;
            }

            mzd_free_window(w_window);
        } 
        
    }
    mzd_free(w);
    mzd_free(v_d);
    mzd_free(v);

    for(std::size_t i=0;i<addTheseLater.size();i++){
        F.push_back(to.coerce(addTheseLater[i]));
    }
    
    mzd_free(acc1);
    mzd_free(acc2);

#ifndef PBORI_NDEBUG
    for (std::size_t idx = 0; idx < F.size(); ++idx) {
      PBORI_ASSERT(to.id() == F[idx].ring().id());
    }
#endif
    
    return F;
}

void FGLMStrategy::testMultiplicationTables(){
    #ifndef PBORI_NDEBUG

    for(std::size_t i=0;i<varsVector.size();i++){
        Variable v=varsVector[i];
        PBORI_ASSERT (v.index()>=i);
        for (std::size_t j=0;j<standardMonomialsFromVector.size(); j++){
            Monomial m=standardMonomialsFromVector[j];
            mzd_t* table=multiplicationTableForVariable(v);
            if (m==v){continue;}
            
            Polynomial product=reducedNormalFormInFromRing(m*v);

            MonomialSet product_set=product.diagram();
            Polynomial sum(0, product.ring());
            for(std::size_t k=0;k<varietySize;k++){
                Monomial m2=standardMonomialsFromVector[k];
                
                if (mzd_read_bit(table,j,k)==1){
                    sum+=m2;
                }

            }
            if (sum!=product)
                cout<<"v:"<<v<<"\tm:"<<m<<"\tsum:"<<sum<<"\tproduct:"<<product<<endl;
            PBORI_ASSERT(sum==product);
        }
    }
    #endif
}
Polynomial FGLMStrategy::reducedNormalFormInFromRing(Polynomial f){
    Polynomial res=gbFrom.reducedNormalForm(f);
    return res;
}

bool FGLMStrategy::canAddThisElementLaterToGB(Polynomial p){
    Monomial lm_from=from.ordering().lead(p);
    size_t length=p.length();
    if ((lm_from.deg()==1)&& ((length==1)||((length==2) && (p.hasConstantPart())))){
        return true;
    }/*
    if (lm_from.deg()==1){
        Monomial lm_to=to.ordering().lead(p);
        if (lm_from==lm_to){
            return true;
        }
    }*/
    return false;
}
FGLMStrategy::FGLMStrategy(const ring_with_ordering_type& from_ring, const ring_with_ordering_type& to_ring,  const PolynomialVector& gb)
  : vars(from_ring), 

   standardMonomialsFrom(from_ring),
   leadingTermsFrom(from_ring),
   varsSet(from_ring),
    gbFrom(from_ring), edgesUnitedVerticesFrom(from_ring), from(from_ring), to(to_ring){

    prot=false;
    transposed=false;

    PolynomialVector::const_iterator it=gb.begin();
    PolynomialVector::const_iterator end=gb.end();
    
    while(it!=end){
        Polynomial gen=*it;
        if (canAddThisElementLaterToGB(gen)){
            addTheseLater.push_back(gen);
        } else{
            this->gbFrom.addGenerator(gen);
        }
        it++;
    }
    
    Monomial monomial_one(from_ring);
    if (prot)
        cout<<"analyzing gb..."<<endl;
    analyzeGB(this->gbFrom);
    if (!(this->gbFrom.leadingTerms.owns(monomial_one))){
        //cout<<standardMonomialsFrom2Index[monomial_one]<<endl;
        
        if (prot){
            cout<<"varietySize:"<<varietySize<<endl;
            cout<<"standard monomials tables..."<<endl;
        }
        setupStandardMonomialsFromTables();
        if (prot)
            cout<<"multiplication tables..."<<endl;
        setupMultiplicationTables();

#ifndef PBORI_NDEBUG
        if (prot)
            cout<<"test multiplication table..."<<endl;
        testMultiplicationTables();
#endif
        PBORI_ASSERT(standardMonomialsFrom2Index[monomial_one]==0);
    }
    if (prot)
        cout<<"initialization finished"<<endl;
}


END_NAMESPACE_PBORIGB
