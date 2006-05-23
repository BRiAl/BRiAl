// -*- c++ -*-
//*****************************************************************************
/** @file LexOrder.cc
 *
 * @author Alexander Dreyer
 * @date 2006-05-18
 *
 * Defining lexicographical ordering.
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
 * Revision 1.2  2006/05/23 15:26:25  dreyer
 * CHANGE BoolePolyRing  can handle different orderings (only lex yet)
 *
 * Revision 1.1  2006/05/23 11:40:59  dreyer
 * + Initial Version
 *
 * @endverbatim
**/
//*****************************************************************************

// include  definitions
#include "LexOrder.h"

#include "pbori_algo.h"
# include "PBoRiOutIter.h"
#include "CIdxPath.h"

BEGIN_NAMESPACE_PBORI


// Comparison of monomials
LexOrder::comp_type
LexOrder::compare(const monom_type& lhs, const monom_type& rhs) const {
  
  PBORI_TRACE_FUNC( 
    "LexOrder::compare(const monom_type&, const monom_type&) const)" );


  if (lhs == rhs)
    return CTypes::equality;

  typedef monom_type::const_iterator const_iterator;

  const_iterator start(lhs.begin()), finish(lhs.end()),
    rhs_start(rhs.begin()), rhs_finish(rhs.end());

  while ( (start != finish) && (rhs_start != rhs_finish) && 
          (*start == *rhs_start) ) {
    ++start; ++rhs_start;
  }

  if (start == finish)
    return CTypes::less_than;

  if (rhs_start == rhs_finish)
    return CTypes::greater_than;

  return (*start < *rhs_start?  CTypes::greater_than : CTypes::less_than);
}

// Extraction of leading term
LexOrder::monom_type 
LexOrder::lead(const poly_type& poly) const {

  monom_type leadterm;
   
  if (poly.isZero())
    leadterm = 0;
  else {

    // store indices in list
    CIdxPath<idx_type> indices(poly.lmDeg());

    // iterator, which uses changeAssign to insert variable
    // wrt. given indices to a monomial
    PBoRiOutIter<monom_type, idx_type, change_assign<monom_type> >  
      outiter(leadterm) ;
    
    // insert backward (for efficiency reasons)
    reversed_inter_copy(poly.firstBegin(), poly.firstEnd(), indices, outiter);
  } 
  return leadterm;
}

END_NAMESPACE_PBORI
