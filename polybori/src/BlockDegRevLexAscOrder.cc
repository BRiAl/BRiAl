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
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.7  2008/09/21 22:21:03  dreyer
 * Change: size_type replaces size_type for deg(), etc.
 *
 * Revision 1.6  2008/01/16 17:10:19  dreyer
 * CHANGE: term-iterators use correct manager now
 *
 * Revision 1.5  2008/01/11 16:58:57  dreyer
 * CHANGE: Experimenting with iterators and correct rings
 *
 * Revision 1.4  2007/11/06 15:03:37  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.3  2007/04/30 15:20:31  dreyer
 * CHANGE: Switching from CTermIter to iterators based on CTermStack
 *
 * Revision 1.2  2007/04/19 09:52:08  dreyer
 * FIX: block dp_asc index comparison
 *
 * Revision 1.1  2007/04/18 15:37:29  dreyer
 * ADD: dp_asc now active
 *
 * Revision 1.2  2007/04/13 13:55:53  dreyer
 * CHANGE: using CTermStack for implementing ordered_(exp_)iterator
 *
 * Revision 1.1  2007/03/21 08:55:09  dreyer
 * ADD: first version of block_dlex running
 *
 * @endverbatim
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

  return lead(poly);
}


// maybe common template here
// Extraction of leading exponent
BlockDegRevLexAscOrder::exp_type 
BlockDegRevLexAscOrder::leadExp(const poly_type& poly) const {

  PBORI_TRACE_FUNC( "BlockDegRevLexAscOrder::leadexp(const poly_type&) const)" );

  return exp_type(lead(poly).exp());
}

// maybe common template here
// Extraction of leading exponent
BlockDegRevLexAscOrder::exp_type 
BlockDegRevLexAscOrder::leadExp(const poly_type& poly, deg_type bound) const {

  PBORI_TRACE_FUNC( "BlockDegRevLexAscOrder::leadexp(const poly_type&, deg_type) const)");
  return leadExp(poly);
}

END_NAMESPACE_PBORI
