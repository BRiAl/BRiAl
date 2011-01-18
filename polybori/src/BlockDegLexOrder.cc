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
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.9  2008/09/21 22:21:03  dreyer
 * Change: size_type replaces size_type for deg(), etc.
 *
 * Revision 1.8  2008/07/18 22:37:50  dreyer
 * Fix: doxygen clean-up (removed inclusion loop)
 *
 * Revision 1.7  2008/07/13 22:49:36  dreyer
 * Fix: Doxygen clean-up
 *
 * Revision 1.6  2008/01/16 17:10:18  dreyer
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
BlockDegLexOrder::lead(const poly_type& poly, size_type bound) const {

  PBORI_TRACE_FUNC( "BlockDegLexOrder::lead(const poly_type&, size_type) const)" );

  return lead(poly);
}


// maybe common template here
// Extraction of leading exponent
BlockDegLexOrder::exp_type 
BlockDegLexOrder::leadExp(const poly_type& poly) const {

  PBORI_TRACE_FUNC( "BlockDegLexOrder::leadexp(const poly_type&) const)" );

  return exp_type(lead(poly).exp());
}

// maybe common template here
// Extraction of leading exponent
BlockDegLexOrder::exp_type 
BlockDegLexOrder::leadExp(const poly_type& poly, size_type bound) const {

  PBORI_TRACE_FUNC( "BlockDegLexOrder::leadexp(const poly_type&, size_type) const)");
  return leadExp(poly);
}

END_NAMESPACE_PBORI
