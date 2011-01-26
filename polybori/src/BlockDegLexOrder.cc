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

#include "pbori_algo.h"
#include "PBoRiOutIter.h"
#include "CIdxPath.h"


#include "CDelayedTermIter.h"

// get internal routines
#include "pbori_routines.h"

#include "CDegLexIter.h"


#include "CDegreeCache.h"

#include "BooleSet.h"

BEGIN_NAMESPACE_PBORI



// Comparison of monomials
BlockDegLexOrder::comp_type
BlockDegLexOrder::compare(const monom_type& lhs, const monom_type& rhs) const {
  
  PBORI_TRACE_FUNC( 
    "BlockDegLexOrder::compare(const monom_type&, const monom_type&) const)" );

  return block_dlex_compare(lhs.begin(), lhs.end(),
                            rhs.begin(), rhs.end(),
                            blockBegin(), blockEnd(),
                            idx_comparer_type() );
}

// Comparison of monomials
BlockDegLexOrder::comp_type
BlockDegLexOrder::compare(const exp_type& lhs, const exp_type& rhs) const {
  
  PBORI_TRACE_FUNC( 
    "BlockDegLexOrder::compare(const exp_type&, const exp_type&) const)" );

  return block_dlex_compare(lhs.begin(), lhs.end(),
                            rhs.begin(), rhs.end(),
                            blockBegin(), blockEnd(),
                            idx_comparer_type() );

}

// Comparison of monomials
BlockDegLexOrder::comp_type
BlockDegLexOrder::compare(idx_type lhs, idx_type rhs) const {
  
  PBORI_TRACE_FUNC( 
    "BlockDegLexOrder::compare(monom_type, monom_type) const)" );

  return generic_compare_3way(lhs, rhs, idx_comparer_type());
}

// Extraction of leading term
BlockDegLexOrder::monom_type 
BlockDegLexOrder::lead(const poly_type& poly) const {

  PBORI_TRACE_FUNC( "BlockDegLexOrder::lead(const poly_type&) const)" );

  if UNLIKELY(poly.isZero())
    throw PBoRiGenericError<CTypes::illegal_on_zero>();

  CBlockDegreeCache<set_type> blockDegCache(poly.ring());

  CacheManager<CCacheTypes::block_dlex_lead> cache_mgr(poly.ring());


  return monom(  dd_block_degree_lead(cache_mgr, blockDegCache, 
                                      poly.navigation(), 
                                      m_indices.begin(),
                                      BooleSet(), 
                                      descending_property()) );

}

// Extraction of leading term
BlockDegLexOrder::monom_type 
BlockDegLexOrder::lead(const poly_type& poly, deg_type bound) const {

  PBORI_TRACE_FUNC( "BlockDegLexOrder::lead(const poly_type&, deg_type) const)" );

  if UNLIKELY(poly.isZero())
    throw PBoRiGenericError<CTypes::illegal_on_zero>();

  return lead(poly);
}


// maybe common template here
// Extraction of leading exponent
BlockDegLexOrder::exp_type 
BlockDegLexOrder::leadExp(const poly_type& poly) const {

  PBORI_TRACE_FUNC( "BlockDegLexOrder::leadexp(const poly_type&) const)" );

  if UNLIKELY(poly.isZero())
    throw PBoRiGenericError<CTypes::illegal_on_zero>();

  return exp_type(lead(poly).exp());
}

// maybe common template here
// Extraction of leading exponent
BlockDegLexOrder::exp_type 
BlockDegLexOrder::leadExp(const poly_type& poly, deg_type bound) const {

  PBORI_TRACE_FUNC( "BlockDegLexOrder::leadexp(const poly_type&, deg_type) const)");

  if UNLIKELY(poly.isZero())
    throw PBoRiGenericError<CTypes::illegal_on_zero>();

  return leadExp(poly);
}

END_NAMESPACE_PBORI
