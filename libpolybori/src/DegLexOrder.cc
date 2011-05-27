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
 *   (c) 2006 by The PolyBoRi Team
**/
//*****************************************************************************

// include  definitions


#include <polybori/DegLexOrder.h>

#include <polybori/routines/pbori_algo.h>
#include <polybori/iterators/PBoRiOutIter.h>


// get internal routines
#include <polybori/routines/pbori_routines.h>


#include <polybori/cache/CDegreeCache.h>

#include <polybori/BooleSet.h>

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

  PBORI_TRACE_FUNC( "DegLexOrder::lead(const poly_type&) const)" );
  return  self::lead(poly, poly.deg());
}

// Extraction of leading term
///@todo: more accurate explanation (comments)
DegLexOrder::monom_type 
DegLexOrder::lead(const poly_type& poly, deg_type bound) const {

  PBORI_TRACE_FUNC( "DegLexOrder::lead(const poly_type&, deg_type) const)" );

  CacheManager<CCacheTypes::dlex_lead> cache_mgr(poly.ring());
  CBoundedDegreeCache<set_type> deg_mgr(poly.ring());

  poly_type::navigator navi(poly.navigation());
  deg_type deg(dd_cached_degree(deg_mgr, navi, bound));

  return monom( dd_recursive_degree_lead(cache_mgr, deg_mgr, navi, set_type(poly.ring()), deg,
                                         descending_property()) );
}


// maybe common template here
// Extraction of leading exponent
DegLexOrder::exp_type 
DegLexOrder::leadExp(const poly_type& poly) const {

  PBORI_TRACE_FUNC( "DegLexOrder::leadexp(const poly_type&) const)" );
  return self::leadExp(poly, poly.deg());
}

// maybe common template here
// Extraction of leading exponent
DegLexOrder::exp_type 
DegLexOrder::leadExp(const poly_type& poly, deg_type bound) const {

  PBORI_TRACE_FUNC( "DegLexOrder::leadexp(const poly_type&, deg_type) const)");

  CacheManager<CCacheTypes::dlex_lead> cache_mgr(poly.ring());
  CBoundedDegreeCache<set_type> deg_mgr(poly.ring());

  poly_type::navigator navi(poly.navigation());
  deg_type deg(dd_cached_degree(deg_mgr, navi, bound));

  exp_type result;
  result.reserve(std::max(deg, 0));

  return dd_recursive_degree_leadexp(cache_mgr, deg_mgr, navi, result, deg,
                                     descending_property());
}

END_NAMESPACE_PBORI
