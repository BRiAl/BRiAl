// -*- c++ -*-
//*****************************************************************************
/** @file MatrixMonomialOrderTables.cc
 *
 * @author Michael Brickenstein (original), Alexander Dreyer (refactorized)
 * @date 2011-06-30
 *
 * This file includes the implementation of the class @c MatrixMonomialOrderTables.
 *
 * @par Copyright:
 *   (c) 2006-2011 by The PolyBoRi Team
 *
**/
//*****************************************************************************


// include basic definitions

#include <polybori/groebner/MatrixMonomialOrderTables.h>
#include <polybori/groebner/ExpGreater.h>

#include <algorithm>

BEGIN_NAMESPACE_PBORIGB

void
MatrixMonomialOrderTables::setup_order_tables(){ 
    int n=terms.size();
    terms_as_exp.resize(n);
    terms_as_exp_lex.resize(n);
    std::copy(terms.expBegin(),terms.expEnd(),terms_as_exp.begin());
    terms_as_exp_lex=terms_as_exp;
    std::sort(terms_as_exp.begin(),terms_as_exp.end(),
	      ExpGreater(terms.ring()));
    ring_order2lex.resize(n);
    lex_order2ring.resize(n);
    int i;
    for (i=0;i<n;i++){
        from_term_map[terms_as_exp[i]]=i;
        //to_term_map[i]=Monomial(terms_as_exp[i]);
    }
    for (i=0;i<n;i++){
        int ring_pos=from_term_map[terms_as_exp_lex[i]];
        ring_order2lex[ring_pos]=i;
        lex_order2ring[i]=ring_pos;
        //to_term_map[i]=Monomial(terms_as_exp[i]);
    }
}

END_NAMESPACE_PBORIGB
