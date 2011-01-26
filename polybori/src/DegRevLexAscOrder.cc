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
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.18  2008/09/21 22:21:03  dreyer
 * Change: size_type replaces size_type for deg(), etc.
 *
 * Revision 1.17  2008/01/16 17:10:19  dreyer
 * CHANGE: term-iterators use correct manager now
 *
 * Revision 1.16  2008/01/11 16:58:58  dreyer
 * CHANGE: Experimenting with iterators and correct rings
 *
 * Revision 1.15  2007/11/06 15:03:39  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.14  2007/04/30 15:20:32  dreyer
 * CHANGE: Switching from CTermIter to iterators based on CTermStack
 *
 * Revision 1.13  2007/04/13 13:55:54  dreyer
 * CHANGE: using CTermStack for implementing ordered_(exp_)iterator
 *
 * Revision 1.12  2007/03/21 08:55:10  dreyer
 * ADD: first version of block_dlex running
 *
 * Revision 1.11  2007/03/19 16:49:39  dreyer
 * CHANGE: ordered iterators made more generic
 *
 * Revision 1.10  2006/12/04 12:48:17  dreyer
 * CHANGE: cached and recursive lead() and leadexp() refined, generalized
 *
 * Revision 1.9  2006/11/30 19:42:47  dreyer
 * CHANGE: lead(bound) now uses cached and recursive variant
 *
 * Revision 1.8  2006/11/29 13:40:03  dreyer
 * CHANGE: leadexp() made recursive and cached
 *
 * Revision 1.7  2006/11/28 09:32:58  dreyer
 * CHANGE: lead() (for dlex, dp_asc) is recursive and cached now
 *
 * Revision 1.6  2006/10/05 12:51:32  dreyer
 * CHANGE: Made lex-based comparisions more generic.
 *
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
  return self::lead(poly, poly.deg());
}

// maybe common template here
// Extraction of leading exponent
DegRevLexAscOrder::exp_type 
DegRevLexAscOrder::leadExp(const poly_type& poly) const {

  PBORI_TRACE_FUNC( "DegRevLexAscOrder::leadexp(const poly_type&) const)" );
  return self::leadExp(poly, poly.deg());
}

// Extraction of leading exponent
DegRevLexAscOrder::exp_type 
DegRevLexAscOrder::leadExp(const poly_type& poly, deg_type bound) const {

  PBORI_TRACE_FUNC( "DegRevLexAscOrder::leadexp(const poly_type&, deg_type) const)" );

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

  return monom( dd_recursive_degree_lead(cache_mgr, deg_mgr, navi, set_type(), deg,
                                         descending_property()) );

}

END_NAMESPACE_PBORI
