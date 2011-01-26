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


#include "BlockDegRevLexAscOrder.h"

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
BlockDegRevLexAscOrder::comp_type
BlockDegRevLexAscOrder::compare(const monom_type& lhs, const monom_type& rhs) const {
  
  PBORI_TRACE_FUNC( 
    "BlockDegRevLexAscOrder::compare(const monom_type&, const monom_type&) const)" );

  return block_dlex_compare(lhs.begin(), lhs.end(),
                            rhs.begin(), rhs.end(),
                            blockBegin(), blockEnd(),
                            idx_comparer_type() );
}

// Comparison of monomials
BlockDegRevLexAscOrder::comp_type
BlockDegRevLexAscOrder::compare(const exp_type& lhs, const exp_type& rhs) const {
  
  PBORI_TRACE_FUNC( 
    "BlockDegRevLexAscOrder::compare(const exp_type&, const exp_type&) const)" );

  return block_dlex_compare(lhs.begin(), lhs.end(),
                            rhs.begin(), rhs.end(),
                            blockBegin(), blockEnd(),
                            idx_comparer_type() );

}

// Comparison of monomials
BlockDegRevLexAscOrder::comp_type
BlockDegRevLexAscOrder::compare(idx_type lhs, idx_type rhs) const {
  
  PBORI_TRACE_FUNC( 
    "BlockDegRevLexAscOrder::compare(monom_type, monom_type) const)" );

  return block_deg_lex_idx_compare( lhs, rhs, blockBegin(), blockEnd(),
                                    idx_comparer_type()  );
}

// Extraction of leading term
BlockDegRevLexAscOrder::monom_type 
BlockDegRevLexAscOrder::lead(const poly_type& poly) const {

  PBORI_TRACE_FUNC( "BlockDegRevLexAscOrder::lead(const poly_type&) const)" );

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
BlockDegRevLexAscOrder::monom_type 
BlockDegRevLexAscOrder::lead(const poly_type& poly, deg_type bound) const {

  PBORI_TRACE_FUNC( "BlockDegRevLexAscOrder::lead(const poly_type&, deg_type) const)" );

  if UNLIKELY(poly.isZero())
    throw PBoRiGenericError<CTypes::illegal_on_zero>();

  return lead(poly);
}


// maybe common template here
// Extraction of leading exponent
BlockDegRevLexAscOrder::exp_type 
BlockDegRevLexAscOrder::leadExp(const poly_type& poly) const {

  PBORI_TRACE_FUNC( "BlockDegRevLexAscOrder::leadexp(const poly_type&) const)" );

  if UNLIKELY(poly.isZero())
    throw PBoRiGenericError<CTypes::illegal_on_zero>();

  return exp_type(lead(poly).exp());
}

// maybe common template here
// Extraction of leading exponent
BlockDegRevLexAscOrder::exp_type 
BlockDegRevLexAscOrder::leadExp(const poly_type& poly, deg_type bound) const {

  PBORI_TRACE_FUNC( "BlockDegRevLexAscOrder::leadexp(const poly_type&, deg_type) const)");

  if UNLIKELY(poly.isZero())
    throw PBoRiGenericError<CTypes::illegal_on_zero>();

  return leadExp(poly);
}

END_NAMESPACE_PBORI
