// -*- c++ -*-
//*****************************************************************************
/** @file BlockDegRevLexAscOrder.cc
 *
 * @author Alexander Dreyer
 * @date 2006-05-18
 *
 * Defining Degree-lexicographical ordering.
 *
 * @par Copyright:
 *   (c) 2007 by The PolyBoRi Team
**/
//*****************************************************************************

// include  definitions


#include <polybori/BlockDegRevLexAscOrder.h>

#include <polybori/routines/pbori_algo.h>
#include <polybori/iterators/PBoRiOutIter.h>


// get internal routines
#include <polybori/routines/pbori_routines.h>


#include <polybori/cache/CDegreeCache.h>

#include <polybori/BooleSet.h>

BEGIN_NAMESPACE_PBORI



// Comparison of monomials
BlockDegRevLexAscOrder::comp_type
BlockDegRevLexAscOrder::compare(const monom_type& lhs, const monom_type& rhs) const {
  
  return compare_terms(lhs, rhs, idx_comparer_type());
}

// Comparison of monomials
BlockDegRevLexAscOrder::comp_type
BlockDegRevLexAscOrder::compare(const exp_type& lhs, const exp_type& rhs) const {

  return compare_terms(lhs, rhs, idx_comparer_type());
}

// Comparison of monomials
BlockDegRevLexAscOrder::comp_type
BlockDegRevLexAscOrder::compare(idx_type lhs, idx_type rhs) const {
  
  PBORI_TRACE_FUNC( 
    "BlockDegRevLexAscOrder::compare(monom_type, monom_type) const)" );

  return block_deg_lex_idx_compare( lhs, rhs, blockBegin(), blockEnd(),
                                    idx_comparer_type()  );
}

END_NAMESPACE_PBORI
