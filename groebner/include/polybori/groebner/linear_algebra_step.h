// -*- c++ -*-
//*****************************************************************************
/** @file linear_algebra_step.h 
 *
 * @author Michael Brickenstein
 * @date 2011-07-01
 *
 * This file includes the definition of linear algebra step-related functions.
 *
 * @par Copyright:
 *   (c) 2006-2011 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_linear_algebra_step_h_
#define polybori_groebner_linear_algebra_step_h_

// include basic definitions
#include "groebner_defs.h"
#include "add_up.h"
#include "nf.h"
#include "draw_matrix.h"

#include "GroebnerStrategy.h"
#include "MatrixMonomialOrderTables.h"
#include "PolyMonomialPairComparerLess.h"

#ifdef HAVE_NTL
#include <NTL/GF2.h>
#include <NTL/mat_GF2.h>
NTL_CLIENT
#endif
#ifdef HAVE_M4RI
const int M4RI_MAXKAY = 16;
#endif

#include <vector>
#include <utility>
#include <sstream>

BEGIN_NAMESPACE_PBORIGB

inline int
select_largest_degree(const ReductionStrategy& strat, const Monomial& m){
    MonomialSet ms=strat.leadingTerms.divisorsOf(m);
    if (ms.isZero())
      return -1;
    else {
      //Monomial min=*(std::min_element(ms.begin(),ms.end(), LessWeightedLengthInStrat(strat)));
      Exponent min=*(std::min_element(ms.expBegin(),ms.expEnd(), LargerDegreeComparer()));
      return strat.exp2Index.find(min)->second;
    }
}


#if  defined(HAVE_NTL) || defined(HAVE_M4RI)

typedef Exponent::idx_map_type from_term_map_type;



inline void
fix_point_iterate(const GroebnerStrategy& strat,std::vector<Polynomial> extendable_system, std::vector<Polynomial>& res1,MonomialSet& res_terms,MonomialSet& leads_from_strat){

    BoolePolyRing current_ring(res_terms.ring());
    leads_from_strat=MonomialSet(current_ring);
    res_terms=MonomialSet(current_ring);

    int i;
        for(i=0;i<extendable_system.size();i++){
            Polynomial p=extendable_system[i];
	    assert(p.ring().id() == current_ring.id());

            if UNLIKELY(p.isZero()) continue;
            
            p=cheap_reductions(strat.generators, p);
            
            // Polynomial p=mod_mon_set(p_orig.diagram(),strat.generators.monomials);
            // if (strat.generators.optLL){
            //     Polynomial p_bak2=p;
            //     p=ll_red_nf(p,strat.generators.llReductor);
            //     if (p!=p_bak2) p=mod_mon_set(p.diagram(),strat.generators.monomials);
            // }
            MonomialSet new_terms=p.diagram().diff(res_terms);
            MonomialSet::const_iterator it=new_terms.begin();
            MonomialSet::const_iterator end=new_terms.end();
            while(it!=end){
                Monomial m=*it;

                int index=select_largest_degree(strat.generators, m);
                if LIKELY(index>=0){

                        Monomial m2=m/strat.generators[index].lead;
                        Polynomial p2=m2*strat.generators[index].p;
                        extendable_system.push_back(p2);
			assert(current_ring.id() ==  strat.generators[index].lead.ring().id());
			assert(current_ring.id() ==  strat.generators[index].p.ring().id());
			assert(current_ring.id() ==  m.ring().id());
			assert(current_ring.id() ==  m2.ring().id());
			assert(current_ring.id() ==  p2.ring().id());
                }
                ++it;
            }
            res_terms=res_terms.unite(new_terms);
            res1.push_back(p);
        }
    leads_from_strat=res_terms.diff(mod_mon_set(res_terms,strat.generators.minimalLeadingTerms));
}

inline void
fill_matrix(mzd_t* mat,std::vector<Polynomial> polys, from_term_map_type from_term_map){
    int i;
    for(i=0;i<polys.size();i++){
        Polynomial::exp_iterator it=polys[i].expBegin();//not order dependend
        Polynomial::exp_iterator end=polys[i].expEnd();
        while(it!=end){
            #ifndef HAVE_M4RI
            mat[i][from_term_map[*it]]=1;
            #else
            from_term_map_type::const_iterator from_it=from_term_map.find(*it);
            assert(from_it!=from_term_map.end());
            mzd_write_bit(mat,i,from_it->second,1);
            #endif
            it++;
        }
    }
}

inline void
translate_back(std::vector<Polynomial>& polys, MonomialSet leads_from_strat,mzd_t* mat,const std::vector<int>& ring_order2lex, const std::vector<Exponent>& terms_as_exp,const std::vector<Exponent>& terms_as_exp_lex,int rank){
    int cols=mat->ncols;
    int rows=mat->nrows;
    
    int i;
    for(i=0;i<rank;i++){
        int j;
        std::vector<int> p_t_i;
    
        bool from_strat=false;
        for(j=0;j<cols;j++){
            #ifndef HAVE_M4RI
            if (mat[i][j]==1){
            #else
            if UNLIKELY(mzd_read_bit(mat,i,j)==1){
            #endif
                if (p_t_i.size()==0){
                    if (leads_from_strat.owns(terms_as_exp[j])) {
                        from_strat=true;break;
                    }
                }
                p_t_i.push_back(ring_order2lex[j]);
            }
        }
        if (!(from_strat)){
            std::vector<Exponent> p_t(p_t_i.size());
            std::sort(p_t_i.begin(),p_t_i.end(),std::less<int>());            
            for(j=0;j<p_t_i.size();j++){
                p_t[j]=terms_as_exp_lex[p_t_i[j]];
            }
            polys.push_back(add_up_lex_sorted_exponents(leads_from_strat.ring(),
							p_t,0,p_t.size()));
            assert(!(polys[polys.size()-1].isZero()));
        }
    }
}


inline void
linalg_step(std::vector<Polynomial>& polys, MonomialSet terms,MonomialSet leads_from_strat, bool log, bool optDrawMatrices=false, const char* matrixPrefix="mat"){
    if UNLIKELY(polys.size()==0) return;
 
    static int round=0;

    int rows=polys.size();
    int cols=terms.size();
    if UNLIKELY(log){
        std::cout<<"ROWS:"<<rows<<"COLUMNS:"<<cols<<std::endl;
    }
    #ifndef HAVE_M4RI
    mat_GF2 mat(INIT_SIZE,rows,cols);
    #else
    mzd_t* mat=mzd_init(rows,cols);
    #endif
    MatrixMonomialOrderTables  tabs(terms);

    fill_matrix(mat,polys,tabs.from_term_map);

    polys.clear();
    #ifndef HAVE_M4RI
    int rank=gauss(mat);
    #else
    if UNLIKELY(optDrawMatrices){
         ++round;
	 std::ostringstream matname;
	 matname << matrixPrefix << round << ".png";
         draw_matrix(mat, matname.str().c_str());
     }
    int rank=mzd_echelonize_m4ri(mat, TRUE, 0);//optimal_k_for_gauss(mat->nrows,mat->ncols,strat));
    #endif
    if UNLIKELY(log){
        std::cout<<"finished gauss"<<std::endl;
    }
    translate_back(polys, leads_from_strat, mat,tabs.ring_order2lex, tabs.terms_as_exp,tabs.terms_as_exp_lex,rank);
    
    #ifdef HAVE_M4RI
    mzd_free(mat);
    #endif
}

inline void
printPackedMatrixMB(mzd_t* mat){
    int i,j;
    for(i=0;i<mat->nrows;i++){
        for(j=0;j<mat->ncols;j++){
            std::cout<<(int)mzd_read_bit(mat,i,j);
        }
        std::cout<<std::endl;
    }
}

inline mzd_t*
transposePackedMB(mzd_t* mat){
    return mzd_transpose(NULL,mat);
    /*mzd_t* res=mzd_init(mat->ncols,mat->nrows);
    int i,j;
    for(i=0;i<mat->nrows;i++){
        for(j=0;j<mat->ncols;j++){
            mzd_write_bit(res,j,i,mzd_read_bit(mat,i,j));
        }
    }
    return res;*/
}

inline mzd_t*
pbori_transpose(mzd_t* mat) {

  if UNLIKELY(mat->nrows == 0)
    return mzd_init(mat->ncols, 0);

  if UNLIKELY(mat->ncols == 0)
    return mzd_init(0, mat->nrows);

  return mzd_transpose(NULL,mat);
}

inline void 
linalg_step_modified(std::vector < Polynomial > &polys, MonomialSet terms, MonomialSet leads_from_strat, bool log, bool optDrawMatrices, const char* matrixPrefix)
{
    BoolePolyRing current_ring(terms.ring());
    assert(current_ring.id() ==  leads_from_strat.ring().id());
#ifndef NDEBUG
    std::vector < Polynomial >::const_iterator start(polys.begin()),
      finish(polys.end());

    while (start != finish) {
      assert(current_ring.id() == start->ring().id());
      ++start;
    }
#endif

    int unmodified_rows=polys.size();
    int unmodified_cols=terms.size();

    if UNLIKELY(((long long) unmodified_cols)*((long long) unmodified_rows)>20000000000ll){
      PBoRiError error(CTypes::matrix_size_exceeded);
      throw error;
    }
    static int round=0;
    round++;
    const int russian_k=16;
    MonomialSet     terms_unique(current_ring);
    std::vector < Monomial > terms_unique_vec;
    MonomialSet     terms_step1(current_ring);
    int             i;
    std::vector < std::pair < Polynomial, Monomial > >polys_lm;
    for (i = 0; i < polys.size(); i++) {
        if LIKELY(!(polys[i].isZero()))
		   polys_lm.push_back(std::pair < Polynomial, Monomial > (polys[i], polys[i].lead()));
    }
std::  sort(polys_lm.begin(), polys_lm.end(), PolyMonomialPairComparerLess());
    polys.clear();
    
    //special cases
    if UNLIKELY(polys_lm.size() == 0)
        return;
    Monomial        last(current_ring);
    if UNLIKELY(polys_lm[0].second.deg() == 0) {
        assert(polys_lm[0].first.isOne());
        polys.resize(1, current_ring);
        polys[0] = 1;

        return;
    }
    
    std::vector < Polynomial > polys_triangular;
    std::vector < Polynomial > polys_rest;

    {
      std::vector < std::pair < Polynomial, Monomial > >::iterator it = polys_lm.begin();
      std::vector < std::pair < Polynomial, Monomial > >::iterator end = polys_lm.end();

        while (it != end) {
            if LIKELY(it->second != last) {
                last = it->second;
                polys_triangular.push_back(it->first);
		

        assert(std::   find(terms_unique_vec.begin(), terms_unique_vec.end(), it->second) == terms_unique_vec.end());

                terms_unique_vec.push_back(it->second);
                terms_step1=terms_step1.unite(it->first.diagram());
            } else
                polys_rest.push_back(it->first);
            it++;
        }
    }
    polys.clear();
    std::reverse(polys_triangular.begin(), polys_triangular.end());
    terms_unique = add_up_generic(terms_unique_vec, current_ring.zero());
    assert(terms_step1.diff(terms).isZero());
    assert(polys_triangular.size()!=0);
    from_term_map_type eliminated2row_number;
    int remaining_cols;
    mzd_t* mat_step1;
    std::vector<int> compactified_columns2old_columns;
    int rows_step1;
    std::vector<int> row_start;
    //std::vector<Polynomial> result;
    MatrixMonomialOrderTables step1(terms_step1);
    //std::vector<Exponent> terms_as_exp_step1;
    {
        int rows=polys_triangular.size();
        int cols=terms_step1.size();
        rows_step1=rows;
        if UNLIKELY(log){
            std::cout<<"STEP1: ROWS:"<<rows<<"COLUMNS:"<<cols<<std::endl;
        }

        mat_step1=mzd_init(rows,cols);
        //vector<Exponent> terms_as_exp_lex_step1;
        //vector<int> ring_order2lex_step1;
        //vector<int> lex_order2ring_step1;
  //      from_term_map_type from_term_map_step1;
//setup_order_tables(terms_as_exp_step1,terms_as_exp_lex_step1,ring_order2lex_step1,lex_order2ring_step1,from_term_map_step1, terms_step1);
        fill_matrix(mat_step1,polys_triangular,step1.from_term_map);

        polys_triangular.clear();
        
        if UNLIKELY(optDrawMatrices) {
	    std::ostringstream matname;
	    matname << matrixPrefix << round << "_step1.png";
	    draw_matrix(mat_step1, matname.str().c_str());
        }
        //optimize: call back subst directly
        mzd_top_echelonize_m4ri
            (mat_step1,0);

        if UNLIKELY(log){
            std::cout<<"finished gauss"<<std::endl;
        }
        int rank=mat_step1->nrows;

        //sort rows
        int pivot_row=0;
        row_start.resize(rows);
        assert(cols>=rows);
        remaining_cols=cols-rows;
        compactified_columns2old_columns.resize(remaining_cols);
        for(i=0;i<cols;i++){
            int j;
            for(j=pivot_row;j<rows;j++){
                if UNLIKELY(mzd_read_bit(mat_step1,j,i)==1){
                    if (j!=pivot_row)
                        mzd_row_swap(mat_step1,j,pivot_row);
                    
                    eliminated2row_number[step1.terms_as_exp[i]]=pivot_row;
                    row_start[pivot_row]=i;
                    pivot_row++;
                    
                    break;
                }
            }
            if UNLIKELY(j==rows){
                assert(i>=pivot_row);
                compactified_columns2old_columns[i-pivot_row]=i;
            }
            
        }
        if UNLIKELY(log){
            std::cout<<"finished sort"<<std::endl;
        }
        assert(pivot_row==rows);

        translate_back(polys, leads_from_strat, mat_step1,step1.ring_order2lex, step1.terms_as_exp,step1.terms_as_exp_lex,rank);
        
        if UNLIKELY(log){
            std::cout<<"finished translate"<<std::endl;
        }

        //delete columns
        mzd_t* transposed_step1 = pbori_transpose(mat_step1);
        if UNLIKELY(log){
            std::cout<<"finished transpose"<<std::endl;
        }
        mzd_free(mat_step1);

        for(i=0;i<remaining_cols;i++){
            int source=compactified_columns2old_columns[i];
            assert(i<=source);
            assert(source<=transposed_step1->nrows);
            if LIKELY(i!=source) mzd_row_swap(transposed_step1,source,i);
            
        }
        if UNLIKELY(log){
            std::cout<<"finished permute"<<std::endl;
        }

        //cols, rows arguments are swapped, as matrix is transposed
        mzd_t* sub_step1=mzd_submatrix(NULL,transposed_step1,0,0,remaining_cols,rows);

        if UNLIKELY(log){
            std::cout<<"finished submat"<<std::endl;
        }
        mzd_free(transposed_step1);
        mat_step1 = pbori_transpose(sub_step1);
        if UNLIKELY(log){
            std::cout<<"finished transpose"<<std::endl;
        }
        mzd_free(sub_step1);
    }
    MonomialSet terms_step2=terms.diff(terms_unique);
    const int rows_step2=polys_rest.size();
    const int cols_step2=terms_step2.size();
    mzd_t* mat_step2=mzd_init(rows_step2,cols_step2);
    mzd_t* mat_step2_factor=mzd_init(rows_step2,mat_step1->nrows);
    MatrixMonomialOrderTables step2(terms_step2);
    // vector<Exponent> step2.terms_as_exp;
    //    vector<Exponent> step2.terms_as_exp_lex;
    //    vector<int> step2.ring_order2lex;
    //    vector<int> step2.lex_order2ring;
    //    from_term_map_type step2.from_term_map;
    // setup_order_tables(step2.terms_as_exp,step2.terms_as_exp_lex,step2.ring_order2lex,step2.lex_order2ring,step2.from_term_map, terms_step2);
    
    
    for(i=0;i<polys_rest.size();i++){
        Polynomial p_r=polys_rest[i];
        Polynomial p_t=p_r.diagram().intersect(terms_step2);
        Polynomial p_u=p_r.diagram().diff(p_t.diagram());
        Polynomial::exp_iterator it(p_u.expBegin()), end(p_u.expEnd());
        
        while(it!=end){
            Exponent e=*it; 
                from_term_map_type::const_iterator from_it=eliminated2row_number.find(e);
                assert(step1.terms_as_exp[row_start[from_it->second]]==e);
                assert(from_it!=eliminated2row_number.end());
                int index=from_it->second;//...translate e->line number;
                mzd_write_bit(mat_step2_factor,i,index,1);
            it++;
        }
        it=p_t.expBegin();
        end=p_t.expEnd();
        while(it!=end){
            Exponent e=*it;
                from_term_map_type::const_iterator from_it=step2.from_term_map.find(e);
                assert(from_it!=step2.from_term_map.end());
                int index=from_it->second;
                mzd_write_bit(mat_step2,i,index,1);
            it++;       
        }
    }
    
    if UNLIKELY(log){
        std::cout<<"iterate over rest polys"<<std::endl;
    }
    
    std::vector<int> remaining_col2new_col(remaining_cols);
    for(i=0;i<remaining_cols;i++){
        remaining_col2new_col[i]=step2.from_term_map[step1.terms_as_exp[compactified_columns2old_columns[i]]];
    }
    assert(mat_step2_factor->ncols==mat_step1->nrows);
    assert(mat_step1->nrows==terms_unique.size());
    assert(mat_step1->ncols==remaining_cols);

    if UNLIKELY(optDrawMatrices)
    {
      std::ostringstream matname;
      matname << matrixPrefix << round << "_mult_A.png";
      draw_matrix(mat_step2_factor, matname.str().c_str());
      matname.clear();
      matname << mat_step2_factor << round << "_mult_B.png";
      draw_matrix(mat_step1,matname.str().c_str());
    }
    if UNLIKELY(log){
        std::cout<<"start mult"<<std::endl;
    }
    
    mzd_t* eliminated;
    if LIKELY((mat_step2_factor->nrows!=0) && (mat_step1->nrows!=0) && (mat_step2_factor->ncols!=0) && (mat_step1->ncols!=0))   
        eliminated=mzd_mul_m4rm(NULL,mat_step2_factor,mat_step1,0);
    else
        eliminated=mzd_init(mat_step2_factor->nrows,mat_step1->ncols);

    mzd_free(mat_step2_factor);
    if UNLIKELY(log){
        std::cout<<"end mult"<<std::endl;
    }
    mzd_free(mat_step1);
    assert(polys_rest.size()==eliminated->nrows);
    assert(mat_step2->nrows==eliminated->nrows);
    for(i=0;i<polys_rest.size();i++){
        int j;
        assert(remaining_cols==eliminated->ncols);
        for(j=0;j<remaining_cols;j++){
            if UNLIKELY(mzd_read_bit(eliminated,i,j)==1){
                assert(step2.terms_as_exp[remaining_col2new_col[j]]==step1.terms_as_exp[compactified_columns2old_columns[j]]);
                
                if UNLIKELY(mzd_read_bit(mat_step2,i,remaining_col2new_col[j])==1){
                    mzd_write_bit(mat_step2,i,remaining_col2new_col[j],0);
                        } else mzd_write_bit(mat_step2,i,remaining_col2new_col[j],1);
            }
        }
    }

    mzd_free(eliminated);
    
     if UNLIKELY(log){
            std::cout<<"STEP2: ROWS:"<<rows_step2<<"COLUMNS:"<<cols_step2<<std::endl;
        }
    if UNLIKELY(optDrawMatrices)
    {
      std::ostringstream matname;
      matname << matrixPrefix << round << "_step2.png";
      draw_matrix(mat_step2, matname.str().c_str());
    }


    int rank_step2;
    if ((mat_step2->ncols>0) &&( mat_step2->nrows>0)){
        rank_step2=
        mzd_echelonize_m4ri(mat_step2,TRUE,0);
        //mzd_echelonize_pluq(mat_step2,TRUE);
    } else
        rank_step2=0;

        if UNLIKELY(log){
            std::cout<<"finished gauss"<<std::endl;
        }

    translate_back(polys, leads_from_strat, mat_step2,step2.ring_order2lex, step2.terms_as_exp,step2.terms_as_exp_lex,rank_step2);
    mzd_free(mat_step2);
    

}


inline std::vector<Polynomial>
gauss_on_polys(const std::vector<Polynomial>& orig_system){

  if (orig_system.empty())
    return orig_system;

  Polynomial init(0, orig_system[0].ring());
  MonomialSet terms=unite_polynomials(orig_system, init);
  MonomialSet from_strat(init.ring());//no strat
  std::vector<Polynomial> polys(orig_system);
  linalg_step(polys, terms, from_strat, false);
  return polys;
}
#endif

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_linear_algebra_step_h_ */
