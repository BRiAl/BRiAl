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

  CBlockDegreeCache<> blockDegCache(poly.diagram().manager());

  CCacheManagement<CCacheTypes::block_dlex_lead> cache_mgr(poly.diagram().manager());


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



// Initialize iterator corresponding to leading term
BlockDegLexOrder::indirect_iterator
BlockDegLexOrder::leadIteratorBegin(const poly_type& poly) const {

  PBORI_TRACE_FUNC( "BlockDegLexOrder::leadIteratorBegin(const poly_type& poly) const" );

//   typedef CGenericCore<self, navigator, monom_type>
//     iterator_core; 
//   typedef CAbstractIterCore<navigator, monom_type> base_core;
//   typedef PBORI_SHARED_PTR(base_core) core_pointer;

//   return indirect_iterator(core_pointer(new iterator_core(poly)));

  return CGenericOrderedIter<self, navigator,
    monom_type>(poly.navigation(), poly.diagram().managerCore()); 
}

BlockDegLexOrder::indirect_iterator
BlockDegLexOrder::leadIteratorEnd() const {

  PBORI_TRACE_FUNC( "BlockDegLexOrder::leadIteratorEnd() const" );
//   typedef CGenericCore<self, navigator, monom_type>
//     iterator_core; 
//   typedef CAbstractIterCore<navigator, monom_type> base_core;
//   typedef PBORI_SHARED_PTR(base_core) core_pointer;

//   return indirect_iterator(core_pointer(new iterator_core()));

  return CGenericOrderedIter<self, navigator, monom_type>();
}

// Initialize iterator corresponding to leading term
BlockDegLexOrder::indirect_exp_iterator
BlockDegLexOrder::leadExpIteratorBegin(const poly_type& poly) const {

  PBORI_TRACE_FUNC( "BlockDegLexOrder::leadExpIteratorBegin(const poly_type& poly) const" );

//   typedef CGenericCore<self, navigator, exp_type>  iterator_core; 
//   typedef CAbstractIterCore<navigator, exp_type> base_core;
//   typedef PBORI_SHARED_PTR(base_core) core_pointer;

//   return indirect_exp_iterator(core_pointer(new iterator_core(poly)));

  return CGenericOrderedIter<self, navigator, exp_type>(poly.navigation());
}

BlockDegLexOrder::indirect_exp_iterator
BlockDegLexOrder::leadExpIteratorEnd() const {

  PBORI_TRACE_FUNC( "BlockDegLexOrder::leadExpIteratorEnd() const" );

//   typedef CGenericCore<self, navigator, exp_type>  iterator_core; 
//   typedef CAbstractIterCore<navigator, exp_type> base_core;
//   typedef PBORI_SHARED_PTR(base_core) core_pointer;

//   return indirect_exp_iterator(core_pointer(new iterator_core()));

  return CGenericOrderedIter<self, navigator, exp_type>();
}


END_NAMESPACE_PBORI
