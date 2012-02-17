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
 *   (c) 2006 by The PolyBoRi Team
**/
//*****************************************************************************

// include  definitions
#include <polybori/DegRevLexAscOrder.h>

#include <polybori/routines/pbori_algo.h>
#include <polybori/iterators/PBoRiOutIter.h>


// get internal routines
#include <polybori/routines/pbori_routines.h>

#include <polybori/cache/CDegreeCache.h>

BEGIN_NAMESPACE_PBORI



// Comparison of monomials
DegRevLexAscOrder::comp_type
DegRevLexAscOrder::compare(const monom_type& lhs, const monom_type& rhs) const {
  
  PBORI_TRACE_FUNC( 
    "DegRevLexAscOrder::compare(const monom_type&, const monom_type&) const)" );

  return deg_lex_compare(lhs, rhs, idx_comparer_type());
}

// Comparison of monomials
DegRevLexAscOrder::comp_type
DegRevLexAscOrder::compare(const exp_type& lhs, const exp_type& rhs) const {
  
  PBORI_TRACE_FUNC( 
    "DegRevLexAscOrder::compare(const exp_type&, const exp_type&) const)" );

  return deg_lex_compare(lhs, rhs, idx_comparer_type());

}

// Comparison of monomials
DegRevLexAscOrder::comp_type
DegRevLexAscOrder::compare(idx_type lhs, idx_type rhs) const {
  
  PBORI_TRACE_FUNC( 
    "DegRevLexAscOrder::compare(monom_type, monom_type) const)" );

  return generic_compare_3way(lhs, rhs, idx_comparer_type());
}

// Extraction of leading term
DegRevLexAscOrder::monom_type 
DegRevLexAscOrder::lead(const poly_type& poly) const {

  PBORI_TRACE_FUNC( "DegRevLexAscOrder::lead(const poly_type&) const)" );
  return self::lead(poly, poly.deg());
}

// maybe common template here
// Extraction of leading exponent
DegRevLexAscOrder::exp_type 
DegRevLexAscOrder::leadExp(const poly_type& poly) const {

  PBORI_TRACE_FUNC( "DegRevLexAscOrder::leadexp(const poly_type&) const)" );
//   if PBORI_UNLIKELY(poly.isZero())
//     throw PBoRiGenericError<CTypes::illegal_on_zero>();

  return self::leadExp(poly, poly.deg());
}

// Extraction of leading exponent
DegRevLexAscOrder::exp_type 
DegRevLexAscOrder::leadExp(const poly_type& poly, deg_type bound) const {

  PBORI_TRACE_FUNC( "DegRevLexAscOrder::leadexp(const poly_type&, deg_type) const)" );

//    if PBORI_UNLIKELY(poly.isZero())
//      throw PBoRiGenericError<CTypes::illegal_on_zero>();

  CacheManager<CCacheTypes::dp_asc_lead> 
    cache_mgr(poly.ring());
  CBoundedDegreeCache<set_type> deg_mgr(poly.ring());

  poly_type::navigator navi(poly.navigation());
  deg_type deg(dd_cached_degree(deg_mgr, navi, bound));

  exp_type result;
  result.reserve(std::max(deg, 0));

  return dd_recursive_degree_leadexp(cache_mgr, deg_mgr, navi, result, deg,
                                     descending_property());
}

// Extraction of leading term
DegRevLexAscOrder::monom_type 
DegRevLexAscOrder::lead(const poly_type& poly, deg_type bound) const {

  PBORI_TRACE_FUNC( "DegRevLexAscOrder::lead(const poly_type&, deg_type) const)" );

  CacheManager<CCacheTypes::dp_asc_lead> 
    cache_mgr(poly.ring());
  CBoundedDegreeCache<set_type> deg_mgr(poly.ring());

  poly_type::navigator navi(poly.navigation());
  deg_type deg(dd_cached_degree(deg_mgr, navi, bound));

  return monom( dd_recursive_degree_lead(cache_mgr, deg_mgr, navi, set_type(poly.ring()), deg,
                                         descending_property()) );

}

END_NAMESPACE_PBORI
