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
**/
//*****************************************************************************

// include  definitions
#include "LexOrder.h"

#include "pbori_algo.h"
# include <iterators/PBoRiOutIter.h>
#include "CIdxPath.h"
#include <errors/PBoRiError.h>

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

template <class CacheType, class NaviType, class DDType>
DDType
dd_lex_lead(const CacheType& cache, NaviType navi, const DDType& init) {
  if (navi.isConstant())
    return cache.generate(navi);

  NaviType cached = cache.find(navi);
  if (cached.isValid())
    return cache.generate(cached);

  DDType result = dd_lex_lead(cache, navi.thenBranch(), init).change(*navi);

  cache.insert(navi, result.navigation());
  return result;
}

// Extraction of leading term
LexOrder::monom_type 
LexOrder::lead(const poly_type& poly) const {


  PBORI_TRACE_FUNC( "LexOrder::lead(const poly_type& poly) const" );
  CacheManager<CCacheTypes::lex_lead> cache(poly.ring());

  return monom(dd_lex_lead(cache, poly.navigation(), set_type()));

#if 0
  monom_type leadterm(poly.ring());
   
  // store indices in list
  CIdxPath<idx_type> indices(poly.lexLeadDeg());
  
  // iterator, which uses changeAssign to insert variable
  // wrt. given indices to a monomial
  PBoRiOutIter<monom_type, idx_type, change_assign<monom_type> >  
    outiter(leadterm) ;
  
  // insert backward (for efficiency reasons)
  reversed_inter_copy(poly.firstBegin(), poly.firstEnd(), indices, outiter);

  dd_lex_lead(poly.firstBegin(), poly.firstEnd(), leadterm);
  return leadterm;

#endif
}

// maybe common template here
// Extraction of leading exponent
LexOrder::exp_type 
LexOrder::leadExp(const poly_type& poly) const {

  PBORI_TRACE_FUNC( "LexOrder::leadExp(const poly_type& poly) const" );

  exp_type leadterm;

  if (!poly.isConstant()) {

    leadterm.reserve(poly.lexLeadDeg());
    // wrt. given indices to a monomial
    PBoRiOutIter<exp_type, idx_type, inserts<exp_type> >  
      outiter(leadterm) ;
    
    std::copy(poly.firstBegin(), poly.firstEnd(), outiter);
  } 
  return leadterm;
}

END_NAMESPACE_PBORI
