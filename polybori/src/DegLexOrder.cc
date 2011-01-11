// -*- c++ -*-
//*****************************************************************************
/** @file DegLexOrder.cc
 *
 * @author Alexander Dreyer
 * @date 2006-05-18
 *
 * Defining Degree-lexicographical ordering.
 *
 * @par Copyright:
 *   (c) 2006 by The PolyBoRi Team
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.21  2008/09/21 22:21:03  dreyer
 * Change: size_type replaces size_type for deg(), etc.
 *
 * Revision 1.20  2008/01/16 17:10:19  dreyer
 * CHANGE: term-iterators use correct manager now
 *
 * Revision 1.19  2008/01/11 16:58:58  dreyer
 * CHANGE: Experimenting with iterators and correct rings
 *
 * Revision 1.18  2007/11/06 15:03:39  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.17  2007/04/30 15:20:31  dreyer
 * CHANGE: Switching from CTermIter to iterators based on CTermStack
 *
 * Revision 1.16  2007/04/13 13:55:53  dreyer
 * CHANGE: using CTermStack for implementing ordered_(exp_)iterator
 *
 * Revision 1.15  2007/03/21 08:55:10  dreyer
 * ADD: first version of block_dlex running
 *
 * Revision 1.14  2007/03/19 16:49:39  dreyer
 * CHANGE: ordered iterators made more generic
 *
 * Revision 1.13  2006/12/04 12:48:17  dreyer
 * CHANGE: cached and recursive lead() and leadexp() refined, generalized
 *
 * Revision 1.12  2006/11/30 19:42:47  dreyer
 * CHANGE: lead(bound) now uses cached and recursive variant
 *
 * Revision 1.11  2006/11/29 13:40:03  dreyer
 * CHANGE: leadexp() made recursive and cached
 *
 * Revision 1.10  2006/11/28 09:32:58  dreyer
 * CHANGE: lead() (for dlex, dp_asc) is recursive and cached now
 *
 * Revision 1.9  2006/11/27 16:25:14  dreyer
 * CHANGE: CDegreeCache, now inherited from standard cache; dlex-lead cached
 *
 * Revision 1.8  2006/10/05 12:51:32  dreyer
 * CHANGE: Made lex-based comparisions more generic.
 *
 * Revision 1.7  2006/10/04 12:28:05  dreyer
 * ADD: getOrderCode()
 *
 * Revision 1.6  2006/09/13 15:07:05  dreyer
 * ADD: lead(sugar) and infrastructure
 *
 * Revision 1.5  2006/09/13 09:05:44  dreyer
 * ADD: dp_asc/DegRevLexAscOrder
 * ADD: BoolePolynomial::endOfNavigation()
 * CHANGE: BoolePolynomial: removed biDegBegin(), biDegEnd(), which can be
 *   generated more generically using navigation() and endOfNavigation().
 *
 * Revision 1.4  2006/09/12 14:56:55  dreyer
 * ADD bidirectional term iterator template
 *
 * Revision 1.3  2006/09/08 14:31:39  dreyer
 * ADD: COrderedIter and infrastructure for order-dependent iterator
 *
 * Revision 1.2  2006/09/07 16:04:32  dreyer
 * ADD: CDegLexIter.h
 *
 * Revision 1.1  2006/09/04 15:58:43  dreyer
 * ADD: DegLexOrder and preparations
 *
 * @endverbatim
**/
//*****************************************************************************

// include  definitions


#include "DegLexOrder.h"

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
DegLexOrder::comp_type
DegLexOrder::compare(const monom_type& lhs, const monom_type& rhs) const {
  
  PBORI_TRACE_FUNC( 
    "DegLexOrder::compare(const monom_type&, const monom_type&) const)" );

  return deg_lex_compare(lhs, rhs, idx_comparer_type());
}

// Comparison of monomials
DegLexOrder::comp_type
DegLexOrder::compare(const exp_type& lhs, const exp_type& rhs) const {
  
  PBORI_TRACE_FUNC( 
    "DegLexOrder::compare(const exp_type&, const exp_type&) const)" );

  return deg_lex_compare(lhs, rhs, idx_comparer_type());

}

// Comparison of monomials
DegLexOrder::comp_type
DegLexOrder::compare(idx_type lhs, idx_type rhs) const {
  
  PBORI_TRACE_FUNC( 
    "DegLexOrder::compare(monom_type, monom_type) const)" );

  return generic_compare_3way(lhs, rhs, idx_comparer_type());
}

// Extraction of leading term
DegLexOrder::monom_type 
DegLexOrder::lead(const poly_type& poly) const {

  PBORI_TRACE_FUNC( "DegLexOrder::lead(const poly_type&) const)" );

  CacheManager<CCacheTypes::dlex_lead> cache_mgr(poly.ring());
  CDegreeCache<set_type> deg_mgr(poly.ring());

  return monom( dd_recursive_degree_lead(cache_mgr, deg_mgr, poly.navigation(), 
                                         set_type(), descending_property()) );

}

// Extraction of leading term
///@todo: more accurate explanation (comments)
DegLexOrder::monom_type 
DegLexOrder::lead(const poly_type& poly, size_type bound) const {

  PBORI_TRACE_FUNC( "DegLexOrder::lead(const poly_type&, size_type) const)" );

  CacheManager<CCacheTypes::dlex_lead> cache_mgr(poly.ring());
  CDegreeCache<set_type> deg_mgr(poly.ring());

  poly_type::navigator navi(poly.navigation());
  size_type deg(dd_cached_degree(deg_mgr, navi, bound));

  return monom( dd_recursive_degree_lead(cache_mgr, deg_mgr, navi, set_type(), deg,
                                         descending_property()) );
}


// maybe common template here
// Extraction of leading exponent
DegLexOrder::exp_type 
DegLexOrder::leadExp(const poly_type& poly) const {

  PBORI_TRACE_FUNC( "DegLexOrder::leadexp(const poly_type&) const)" );

  exp_type result;
  size_type deg(poly.deg());
  result.reserve(deg);

  CacheManager<CCacheTypes::dlex_lead> cache_mgr(poly.ring());
  CDegreeCache<set_type> deg_mgr(poly.ring());

  return dd_recursive_degree_leadexp(cache_mgr, deg_mgr, poly.navigation(), result, deg,
                                     descending_property());
}

// maybe common template here
// Extraction of leading exponent
DegLexOrder::exp_type 
DegLexOrder::leadExp(const poly_type& poly, size_type bound) const {

  PBORI_TRACE_FUNC( "DegLexOrder::leadexp(const poly_type&, size_type) const)");

  CacheManager<CCacheTypes::dlex_lead> cache_mgr(poly.ring());
  CDegreeCache<set_type> deg_mgr(poly.ring());

  poly_type::navigator navi(poly.navigation());
  size_type deg(dd_cached_degree(deg_mgr, navi, bound));

  exp_type result;
  result.reserve(deg);

  return dd_recursive_degree_leadexp(cache_mgr, deg_mgr, navi, result, deg,
                                     descending_property());
}


// Initialize iterator corresponding to leading term
DegLexOrder::ordered_iterator
DegLexOrder::leadIteratorBegin(const poly_type& poly) const {

  PBORI_TRACE_FUNC( "DegLexOrder::leadIteratorBegin(const poly_type& poly) const" );

  //  return generic_iteration<self, iterator>().leadIterator(poly);
//   typedef CGenericCore<self, navigator, monom_type>
//     iterator_core; 
//   typedef CAbstractIterCore<navigator, monom_type> base_core;
//   typedef boost::shared_ptr<base_core> core_pointer;

//   return ordered_iterator(core_pointer(new iterator_core(poly)));

  return CGenericOrderedIter<self, navigator,
    monom_type>(poly.navigation(), poly.ring()); 
}

DegLexOrder::ordered_iterator
DegLexOrder::leadIteratorEnd() const {

  PBORI_TRACE_FUNC( "DegLexOrder::leadIteratorEnd() const" );

//   typedef CGenericCore<self, navigator, monom_type>
//     iterator_core; 
//   typedef CAbstractIterCore<navigator, monom_type> base_core;
//   typedef boost::shared_ptr<base_core> core_pointer;
//   return ordered_iterator(core_pointer(new iterator_core()));
  return CGenericOrderedIter<self, navigator, monom_type>();
}
// Initialize iterator corresponding to leading term
DegLexOrder::ordered_exp_iterator
DegLexOrder::leadExpIteratorBegin(const poly_type& poly) const {

  PBORI_TRACE_FUNC( "DegLexOrder::leadExpIteratorBegin(const poly_type& poly) const" );

  //  return generic_iteration<self, iterator>().leadIterator(poly);

//   typedef CGenericCore<self, navigator, exp_type>
//     iterator_core; 
//   typedef CAbstractIterCore<navigator, exp_type> base_core;
//   typedef boost::shared_ptr<base_core> core_pointer;

//   return ordered_exp_iterator(core_pointer(new iterator_core(poly)));
  return CGenericOrderedIter<self, navigator, exp_type>(poly.navigation(), poly.ring());
}

DegLexOrder::ordered_exp_iterator
DegLexOrder::leadExpIteratorEnd() const {

  PBORI_TRACE_FUNC( "DegLexOrder::leadExpIteratorEnd() const" );

//   typedef CGenericCore<self, navigator, exp_type>
//     iterator_core; 
//   typedef CAbstractIterCore<navigator, exp_type> base_core;
//   typedef boost::shared_ptr<base_core> core_pointer;

//   return ordered_exp_iterator(core_pointer(new iterator_core()));
  return CGenericOrderedIter<self, navigator, exp_type>();

}


END_NAMESPACE_PBORI
