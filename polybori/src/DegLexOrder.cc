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
    leadterm =  std::max_element( degree_term_iterator(poly.degBegin()), 
                                  degree_term_iterator(poly.degEnd()) ).term();

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

END_NAMESPACE_PBORI
