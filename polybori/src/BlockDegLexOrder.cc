// -*- c++ -*-
//*****************************************************************************
/** @file BlockDegLexOrder.cc
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


#include "BlockDegLexOrder.h"

#include <routines/pbori_algo.h>
#include <iterators/PBoRiOutIter.h>
#include "CIdxPath.h"


#include <iterators/CDelayedTermIter.h>

// get internal routines
#include <routines/pbori_routines.h>

#include <iterators/CDegLexIter.h>


#include "CDegreeCache.h"

#include "BooleSet.h"

BEGIN_NAMESPACE_PBORI



// Comparison of monomials
BlockDegLexOrder::comp_type
BlockDegLexOrder::compare(const monom_type& lhs, const monom_type& rhs) const {

  return compare_terms(lhs, rhs, idx_comparer_type());
}

// Comparison of monomials
BlockDegLexOrder::comp_type
BlockDegLexOrder::compare(const exp_type& lhs, const exp_type& rhs) const {
  
  return compare_terms(lhs, rhs, idx_comparer_type());
}

// Comparison of monomials
BlockDegLexOrder::comp_type
BlockDegLexOrder::compare(idx_type lhs, idx_type rhs) const {
  
  PBORI_TRACE_FUNC( 
    "BlockDegLexOrder::compare(monom_type, monom_type) const)" );

  return generic_compare_3way(lhs, rhs, idx_comparer_type());
}

END_NAMESPACE_PBORI
