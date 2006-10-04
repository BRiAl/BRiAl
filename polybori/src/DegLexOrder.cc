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

BEGIN_NAMESPACE_PBORI



// Comparison of monomials
DegLexOrder::comp_type
DegLexOrder::compare(const monom_type& lhs, const monom_type& rhs) const {
  
  PBORI_TRACE_FUNC( 
    "DegLexOrder::compare(const monom_type&, const monom_type&) const)" );

  return deg_lex_compare(lhs, rhs);
}

// Comparison of monomials
DegLexOrder::comp_type
DegLexOrder::compare(const exp_type& lhs, const exp_type& rhs) const {
  
  PBORI_TRACE_FUNC( 
    "DegLexOrder::compare(const exp_type&, const exp_type&) const)" );

  return deg_lex_compare(lhs, rhs);

}

// Comparison of monomials
DegLexOrder::comp_type
DegLexOrder::compare(idx_type lhs, idx_type rhs) const {
  
  PBORI_TRACE_FUNC( 
    "DegLexOrder::compare(monom_type, monom_type) const)" );

  return lex_compare_indices(lhs, rhs);
}

// Extraction of leading term
DegLexOrder::monom_type 
DegLexOrder::lead(const poly_type& poly) const {

  typedef CDelayedTermIter<monom_type, 
    change_assign<>, project_ith<2>, poly_type::deg_iterator >
    degree_term_iterator;

  monom_type leadterm;
   
  if (poly.isZero())
    leadterm = 0;
  else if (poly.isOne())
    leadterm = 1;
  else
    leadterm =  degree_term_iterator(std::max_element( poly.degBegin(), 
                                                       poly.degEnd() )).term();

  return leadterm;
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

  typedef CDelayedTermIter<exp_type, 
    inserts<>, project_ith<1>, poly_type::deg_iterator >
    degree_term_iterator;

  if (!poly.isZero() && !poly.isOne())
    return std::max_element( degree_term_iterator(poly.degBegin()), 
                             degree_term_iterator(poly.degEnd()) ).term();
  else 
    return exp_type();
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
