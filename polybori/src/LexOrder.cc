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
 * Revision 1.1  2006/05/23 11:40:59  dreyer
 * + Initial Version
 *
 * @endverbatim
**/
//*****************************************************************************

// include  definitions
#include "LexOrder.h"

BEGIN_NAMESPACE_PBORI

// // Default constructor
// LexOrder::LexOrder()  {
  
//   PBORI_TRACE_FUNC( "LexOrder()" );
// }


// // Copy constructor
// LexOrder::LexOrder(const self&) {
  
//   PBORI_TRACE_FUNC( "LexOrder(const self&)" );
// }

// // Destructor
// LexOrder::~LexOrder() {

//   PBORI_TRACE_FUNC( "~LexOrder()" );

// }


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



END_NAMESPACE_PBORI
