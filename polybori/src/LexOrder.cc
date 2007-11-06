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
 *   (c) 2006 by The PolyBoRi Team
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.16  2007/11/06 15:03:39  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.15  2007/04/30 15:20:32  dreyer
 * CHANGE: Switching from CTermIter to iterators based on CTermStack
 *
 * Revision 1.14  2007/04/13 13:55:54  dreyer
 * CHANGE: using CTermStack for implementing ordered_(exp_)iterator
 *
 * Revision 1.13  2007/03/21 08:55:10  dreyer
 * ADD: first version of block_dlex running
 *
 * Revision 1.12  2007/03/19 16:49:39  dreyer
 * CHANGE: ordered iterators made more generic
 *
 * Revision 1.11  2006/10/05 12:51:32  dreyer
 * CHANGE: Made lex-based comparisions more generic.
 *
 * Revision 1.10  2006/10/04 12:28:05  dreyer
 * ADD: getOrderCode()
 *
 * Revision 1.9  2006/09/13 09:05:44  dreyer
 * ADD: dp_asc/DegRevLexAscOrder
 * ADD: BoolePolynomial::endOfNavigation()
 * CHANGE: BoolePolynomial: removed biDegBegin(), biDegEnd(), which can be
 *   generated more generically using navigation() and endOfNavigation().
 *
 * Revision 1.8  2006/09/12 14:56:56  dreyer
 * ADD bidirectional term iterator template
 *
 * Revision 1.7  2006/09/08 14:31:39  dreyer
 * ADD: COrderedIter and infrastructure for order-dependent iterator
 *
 * Revision 1.6  2006/09/04 15:58:43  dreyer
 * ADD: DegLexOrder and preparations
 *
 * Revision 1.5  2006/08/29 09:02:36  dreyer
 * ADD: leadExp()
 *
 * Revision 1.4  2006/08/24 14:47:50  dreyer
 * ADD: BooleExponent integrated, FIX: multiples (for indices < first)
 *
 * Revision 1.3  2006/07/14 09:02:49  dreyer
 * ADD: greater_variable()
 *
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

// get internal routines
#include "pbori_routines.h"

BEGIN_NAMESPACE_PBORI

// // todo: move to some header
// template<class Order, class FirstIterator, class SecondIterator>
// typename Order::comp_type
// lex_compare(FirstIterator start, FirstIterator finish, 
//             SecondIterator rhs_start, SecondIterator rhs_finish,
//             Order order) {

//   while ( (start != finish) && (rhs_start != rhs_finish) && 
//           (*start == *rhs_start) ) {
//     ++start; ++rhs_start;
//   }

//   if (start == finish)
//     return CTypes::less_than;

//   if (rhs_start == rhs_finish)
//     return CTypes::greater_than;

//   return order.compare(*start, *rhs_start);
// }

// // todo: move to some header
// template<class Order, class LhsType, class RhsType>
// typename Order::comp_type
// lex_compare(const LhsType& lhs, const RhsType& rhs, Order order) {

//   if (lhs == rhs)
//     return CTypes::equality;

//   return lex_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), order);
// }

// Comparison of monomials
LexOrder::comp_type
LexOrder::compare(const monom_type& lhs, const monom_type& rhs) const {
  
  PBORI_TRACE_FUNC( 
    "LexOrder::compare(const monom_type&, const monom_type&) const)" );

  return lex_compare(lhs, rhs, idx_comparer_type());
}

// Comparison of monomials
LexOrder::comp_type
LexOrder::compare(const exp_type& lhs, const exp_type& rhs) const {
  
  PBORI_TRACE_FUNC( 
    "LexOrder::compare(const exp_type&, const exp_type&) const)" );

  return lex_compare(lhs, rhs, idx_comparer_type());
}

// Comparison of monomials
LexOrder::comp_type
LexOrder::compare(idx_type lhs, idx_type rhs) const {
  
  PBORI_TRACE_FUNC( 
    "LexOrder::compare(monom_type, monom_type) const)" );

  return generic_compare_3way(lhs, rhs, idx_comparer_type());
}

// Extraction of leading term
LexOrder::monom_type 
LexOrder::lead(const poly_type& poly) const {


  PBORI_TRACE_FUNC( "LexOrder::lead(const poly_type& poly) const" );

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

// maybe common template here
// Extraction of leading exponent
LexOrder::exp_type 
LexOrder::leadExp(const poly_type& poly) const {

  PBORI_TRACE_FUNC( "LexOrder::leadExp(const poly_type& poly) const" );

  exp_type leadterm;
   
  if (!poly.isZero() && !poly.isOne()) {

    leadterm.reserve(poly.lmDeg());
    // wrt. given indices to a monomial
    PBoRiOutIter<exp_type, idx_type, inserts<exp_type> >  
      outiter(leadterm) ;
    
    std::copy(poly.firstBegin(), poly.firstEnd(), outiter);
  } 
  return leadterm;
}

// Initialize iterator corresponding to leading term
LexOrder::indirect_iterator
LexOrder::leadIteratorBegin(const poly_type& poly) const {

  PBORI_TRACE_FUNC( "LexOrder::leadIteratorBegin(const poly_type& poly) const" );

  //  return generic_iteration<self, iterator>().leadIterator(poly);
//   typedef CWrappedStack<  >
//     iterator_core; 



//   typedef CTermStackBase<navigator, monom_type> base_core;
//   typedef PBORI_SHARED_PTR(base_core) core_pointer;

//   return indirect_iterator(core_pointer(new iterator_core(poly)));


  return CGenericOrderedIter<self, navigator, monom_type>(poly.navigation());

}

// Initialize iterator corresponding to leading term
LexOrder::indirect_exp_iterator
LexOrder::leadExpIteratorBegin(const poly_type& poly) const {

  PBORI_TRACE_FUNC( "LexOrder::leadExpIteratorBegin(const poly_type& poly) const" );

  //  return generic_iteration<self, iterator>().leadIterator(poly);
//   typedef CGenericCore<self, navigator, exp_type>  iterator_core; 
//   typedef CAbstractIterCore<navigator, exp_type> base_core;
//   typedef PBORI_SHARED_PTR(base_core) core_pointer;

//   return indirect_exp_iterator(core_pointer(new iterator_core(poly)));

  return CGenericOrderedIter<self, navigator, exp_type>(poly.navigation());
}

LexOrder::indirect_exp_iterator
LexOrder::leadExpIteratorEnd() const {

  PBORI_TRACE_FUNC( "LexOrder::leadExpIteratorEnd() const" );

//   typedef CGenericCore<self, navigator, exp_type>
//     iterator_core; 
//   typedef CAbstractIterCore<navigator, exp_type> base_core;
//   typedef PBORI_SHARED_PTR(base_core) core_pointer;

//   return indirect_exp_iterator(core_pointer(new iterator_core()));
  return CGenericOrderedIter<self, navigator, exp_type>();

}

LexOrder::indirect_iterator
LexOrder::leadIteratorEnd() const {

  PBORI_TRACE_FUNC( "LexOrder::leadIteratorEnd() const" );

//   typedef CGenericCore<self, navigator, monom_type> iterator_core; 
//   typedef CAbstractIterCore<navigator, monom_type> base_core;
//   typedef PBORI_SHARED_PTR(base_core) core_pointer;

//   return indirect_iterator(core_pointer(new iterator_core()));
  return CGenericOrderedIter<self, navigator, monom_type>();
}


END_NAMESPACE_PBORI
