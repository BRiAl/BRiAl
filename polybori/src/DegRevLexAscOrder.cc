// -*- c++ -*-
//*****************************************************************************
/** @file DegRevLexAscOrder.cc
 *
 * @author Alexander Dreyer
 * @date 2006-05-18
 *
 * Defining Degree-reverse lexicographical ordering.
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
 * Revision 1.5  2006/10/03 18:17:21  bricken
 * + removed minus sign again
 *
 * Revision 1.4  2006/10/03 11:36:36  bricken
 * + a minus sign
 *
 * Revision 1.3  2006/10/03 09:55:26  dreyer
 * FIX: monomial comparison broken on dp_asc
 *
 * Revision 1.2  2006/09/13 15:07:05  dreyer
 * ADD: lead(sugar) and infrastructure
 *
 * Revision 1.1  2006/09/13 09:05:44  dreyer
 * ADD: dp_asc/DegRevLexAscOrder
 * ADD: BoolePolynomial::endOfNavigation()
 * CHANGE: BoolePolynomial: removed biDegBegin(), biDegEnd(), which can be
 *   generated more generically using navigation() and endOfNavigation().
 *
 * @endverbatim
**/
//*****************************************************************************

// include  definitions
#include "DegRevLexAscOrder.h"

#include "pbori_algo.h"
#include "PBoRiOutIter.h"
#include "CIdxPath.h"


#include "CDelayedTermIter.h"

// get internal routines
#include "pbori_routines.h"

#include "CDegLexIter.h"

BEGIN_NAMESPACE_PBORI



// Comparison of monomials
DegRevLexAscOrder::comp_type
DegRevLexAscOrder::compare(const monom_type& lhs, const monom_type& rhs) const {
  
  PBORI_TRACE_FUNC( 
    "DegRevLexAscOrder::compare(const monom_type&, const monom_type&) const)" );

  return deg_rev_lex_asc_compare(lhs, rhs);
}

// Comparison of monomials
DegRevLexAscOrder::comp_type
DegRevLexAscOrder::compare(const exp_type& lhs, const exp_type& rhs) const {
  
  PBORI_TRACE_FUNC( 
    "DegRevLexAscOrder::compare(const exp_type&, const exp_type&) const)" );

  return deg_rev_lex_asc_compare(lhs, rhs);

}

// Comparison of monomials
DegRevLexAscOrder::comp_type
DegRevLexAscOrder::compare(idx_type lhs, idx_type rhs) const {
  
  PBORI_TRACE_FUNC( 
    "DegRevLexAscOrder::compare(monom_type, monom_type) const)" );

  return lex_compare_indices(rhs, lhs);
}

// Extraction of leading term
DegRevLexAscOrder::monom_type 
DegRevLexAscOrder::lead(const poly_type& poly) const {

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
                                                       poly.degEnd() ,
                                                 std::less_equal<size_type>() )
                                     ).term();

  return leadterm;
}


// maybe common template here
// Extraction of leading exponent
DegRevLexAscOrder::exp_type 
DegRevLexAscOrder::leadExp(const poly_type& poly) const {

  typedef CDelayedTermIter<exp_type, 
    inserts<>, project_ith<1>, poly_type::deg_iterator >
    degree_term_iterator;

  if (!poly.isZero() && !poly.isOne())
    return degree_term_iterator(std::max_element( poly.degBegin(), 
                                                  poly.degEnd(),
                                                  std::less_equal<size_type>()
                                                  )
                                ).term();
  else 
    return exp_type();
}

// Extraction of leading exponent
DegRevLexAscOrder::exp_type 
DegRevLexAscOrder::leadExp(const poly_type& poly, size_type bound) const {

  typedef CDelayedTermIter<exp_type, 
    inserts<>, project_ith<1>, iterator >
    degree_term_iterator;

  if (!poly.isZero() && !poly.isOne())
    return degree_term_iterator( bounded_max_element(
          reversed_iteration_adaptor<iterator>(iterator(poly.navigation(),
                                                        dummy_iterator())), 
          reversed_iteration_adaptor<iterator>((iterator)poly.endOfNavigation()),
          bound
          ).get()
        ).term();
  else 
    return exp_type();
}


// Extraction of leading term
DegRevLexAscOrder::monom_type 
DegRevLexAscOrder::lead(const poly_type& poly, size_type bound) const {

  typedef CDelayedTermIter<monom_type, 
    change_assign<>, project_ith<2>, iterator >
    degree_term_iterator;

  monom_type leadterm;
   
  if (poly.isZero())
    leadterm = 0;
  else if (poly.isOne())
    leadterm = 1;
  else
    leadterm = 
      degree_term_iterator(
        bounded_max_element(
          reversed_iteration_adaptor<iterator>(iterator(poly.navigation(),
                                                        dummy_iterator())), 
          reversed_iteration_adaptor<iterator>(poly.endOfNavigation()),
          bound
          ).get()
        ).term();

  return leadterm;
}


// Initialize iterator corresponding to leading term
DegRevLexAscOrder::iterator
DegRevLexAscOrder::leadIterator(const poly_type& poly) const {

  PBORI_TRACE_FUNC( "DegRevLexAscOrder::leadIterator(const poly_type&) const" );

  return generic_iteration<self, iterator>().leadIterator(poly);
}

// Find next term (after iter) in polynomial according to current order
DegRevLexAscOrder::iterator
DegRevLexAscOrder::incrementIterator(iterator iter,
                                     const poly_type& poly) const {

  PBORI_TRACE_FUNC(
    "DegRevLexAscOrder::incrementIterator(iterator, const poly_type&) const" );

  return generic_iteration<self, iterator>().incrementIterator(iter, poly);
}

END_NAMESPACE_PBORI
