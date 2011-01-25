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
#include "DegRevLexAscOrder.h"

#include "pbori_algo.h"
#include "PBoRiOutIter.h"
#include "CIdxPath.h"


#include "CDelayedTermIter.h"

// get internal routines
#include "pbori_routines.h"

#include "CDegLexIter.h"

#include "CDegreeCache.h"

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

  CacheManager<CCacheTypes::dp_asc_lead> 
    cache_mgr(poly.ring());
  CDegreeCache<set_type> deg_mgr(poly.ring());

  return monom( dd_recursive_degree_lead(cache_mgr, deg_mgr, poly.navigation(), 
                                         set_type(), descending_property() ) );
}

// maybe common template here
// Extraction of leading exponent
DegRevLexAscOrder::exp_type 
DegRevLexAscOrder::leadExp(const poly_type& poly) const {

  PBORI_TRACE_FUNC( "DegRevLexAscOrder::leadexp(const poly_type&) const)" );

  exp_type result;
  size_type deg(poly.deg());
  result.reserve(deg);

  CacheManager<CCacheTypes::dp_asc_lead> 
    cache_mgr(poly.ring());
  CDegreeCache<set_type> deg_mgr(poly.ring());

  return dd_recursive_degree_leadexp (cache_mgr, deg_mgr, poly.navigation(), result, deg,
                                      descending_property());
}

// Extraction of leading exponent
DegRevLexAscOrder::exp_type 
DegRevLexAscOrder::leadExp(const poly_type& poly, size_type bound) const {

  PBORI_TRACE_FUNC( "DegRevLexAscOrder::leadexp(const poly_type&, size_type) const)" );

  CacheManager<CCacheTypes::dp_asc_lead> 
    cache_mgr(poly.ring());
  CDegreeCache<set_type> deg_mgr(poly.ring());

  poly_type::navigator navi(poly.navigation());
  size_type deg(dd_cached_degree(deg_mgr, navi, bound));

  exp_type result;
  result.reserve(deg);

  return dd_recursive_degree_leadexp(cache_mgr, deg_mgr, navi, result, deg,
                                     descending_property());
}

// Extraction of leading term
DegRevLexAscOrder::monom_type 
DegRevLexAscOrder::lead(const poly_type& poly, size_type bound) const {

  PBORI_TRACE_FUNC( "DegRevLexAscOrder::lead(const poly_type&, size_type) const)" );

  CacheManager<CCacheTypes::dp_asc_lead> 
    cache_mgr(poly.ring());
  CDegreeCache<set_type> deg_mgr(poly.ring());

  poly_type::navigator navi(poly.navigation());
  size_type deg(dd_cached_degree(deg_mgr, navi, bound));

  return monom( dd_recursive_degree_lead(cache_mgr, deg_mgr, navi, set_type(), deg,
                                         descending_property()) );

}

END_NAMESPACE_PBORI
