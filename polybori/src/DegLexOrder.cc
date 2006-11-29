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
 *   (c) 2006 by
 *   Dep. of Mathematics, Kaiserslautern University of Technology and @n
 *   Fraunhofer Institute for Industrial Mathematics (ITWM)
 *   D-67663 Kaiserslautern, Germany
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
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

  CDDOperations<BooleSet, monom_type> op;
  CCacheManagement<CCacheTypes::dlex_lead> cache_mgr(poly.diagram().manager());
  CDegreeCache<> deg_mgr(poly.diagram().manager());

  return  op.getMonomial( dd_recursive_degree_lead(cache_mgr, deg_mgr,
                                                   poly.navigation(), 
                                                   BooleSet(),
                                                   std::less<size_type>()) );
}

// Extraction of leading term
DegLexOrder::monom_type 
DegLexOrder::lead(const poly_type& poly, size_type bound) const {

  typedef CDelayedTermIter<monom_type, 
    change_assign<>, project_ith<2>, poly_type::deg_iterator >
    degree_term_iterator;

  monom_type leadterm;
   
  if (poly.isZero())
    leadterm = 0;
  else if (poly.isOne())
    leadterm = 1;
  else
    leadterm = degree_term_iterator(bounded_max_element(poly.degBegin(), 
                                                        poly.degEnd(),
                                                        bound)).term();

  return leadterm;
}


// maybe common template here
// Extraction of leading exponent
DegLexOrder::exp_type 
DegLexOrder::leadExp(const poly_type& poly) const {

  exp_type result;
  result.reserve(poly.deg());

  CCacheManagement<CCacheTypes::dlex_lead> cache_mgr(poly.diagram().manager());
  CDegreeCache<> deg_mgr(poly.diagram().manager());

  return dd_recursive_degree_leadexp (cache_mgr, deg_mgr,
                                      poly.navigation(), result,
                                      std::less<size_type>());
}

// maybe common template here
// Extraction of leading exponent
DegLexOrder::exp_type 
DegLexOrder::leadExp(const poly_type& poly, size_type bound) const {

  typedef CDelayedTermIter<exp_type, 
    inserts<>, project_ith<1>, poly_type::deg_iterator >
    degree_term_iterator;

  if (!poly.isZero() && !poly.isOne())
    return degree_term_iterator(bounded_max_element(poly.degBegin(), 
                                                    poly.degEnd(), 
                                                    bound)).term();
  else 
    return exp_type();
}


// Initialize iterator corresponding to leading term
DegLexOrder::iterator
DegLexOrder::leadIterator(const poly_type& poly) const {

  PBORI_TRACE_FUNC( "DegLexOrder::leadIterator(const poly_type& poly) const" );

  return generic_iteration<self, iterator>().leadIterator(poly);
}

// Find next term (after iter) in polynomial according to current order
DegLexOrder::iterator
DegLexOrder::incrementIterator(iterator iter, const poly_type& poly) const {

  PBORI_TRACE_FUNC(
    "DegLexOrder::incrementIterator(iterator, const poly_type&) const" );

  return generic_iteration<self, iterator>().incrementIterator(iter, poly);
}

END_NAMESPACE_PBORI
