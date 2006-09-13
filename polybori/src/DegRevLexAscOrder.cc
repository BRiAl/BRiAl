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

  return deg_lex_compare(lhs, rhs);
}

// Comparison of monomials
DegRevLexAscOrder::comp_type
DegRevLexAscOrder::compare(const exp_type& lhs, const exp_type& rhs) const {
  
  PBORI_TRACE_FUNC( 
    "DegRevLexAscOrder::compare(const exp_type&, const exp_type&) const)" );

  return deg_lex_compare(lhs, rhs);

}

// Comparison of monomials
DegRevLexAscOrder::comp_type
DegRevLexAscOrder::compare(idx_type lhs, idx_type rhs) const {
  
  PBORI_TRACE_FUNC( 
    "DegRevLexAscOrder::compare(monom_type, monom_type) const)" );

  return lex_compare_indices(lhs, rhs);
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

// Initialize iterator corresponding to leading term
DegRevLexAscOrder::iterator
DegRevLexAscOrder::leadIterator(const poly_type& poly) const {

  PBORI_TRACE_FUNC( "DegRevLexAscOrder::leadIterator(const poly_type&) const" );

  return std::max_element(iterator(poly.navigation()), 
                          iterator(poly.endOfNavigation()),
                          std::less_equal<size_type>() );
}

// Find next term (after iter) in polynomial according to current order
DegRevLexAscOrder::iterator
DegRevLexAscOrder::incrementIterator(iterator iter,
                                     const poly_type& poly) const {

  PBORI_TRACE_FUNC(
    "DegRevLexAscOrder::incrementIterator(iterator, const poly_type&) const" );

  typedef CRestrictedIter<iterator> bounded_iterator;

  iterator m_start(poly.navigation());
  iterator m_finish(poly.endOfNavigation());

  // reversed_iteration_adaptor<iterator> riter(iter), rfinish(m_finish);

  if (iter != m_finish) {

    size_type deg = *iter;
    --iter;
    iter = std::find(reversed_iteration_adaptor<iterator>(iter), 
                     reversed_iteration_adaptor<iterator>(m_finish), deg).get();
      
    if(iter == m_finish) {
      iter = std::max_element( bounded_iterator(m_start, deg),
                               bounded_iterator(m_finish, deg), 
                               std::less_equal<size_type>() );

    }
  }
  return iter;
}

END_NAMESPACE_PBORI
