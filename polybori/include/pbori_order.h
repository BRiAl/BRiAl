// -*- c++ -*-
//*****************************************************************************
/** @file pbori_order.h
 *
 * @author Alexander Dreyer
 * @date 2006-05-23
 *
 * 
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
 * Revision 1.6  2007/03/19 16:49:39  dreyer
 * CHANGE: ordered iterators made more generic
 *
 * Revision 1.5  2006/10/05 12:51:32  dreyer
 * CHANGE: Made lex-based comparisions more generic.
 *
 * Revision 1.4  2006/10/02 09:28:38  dreyer
 * ADD BoolePolyRing::changeOrdering and infrastructure
 *
 * Revision 1.3  2006/09/13 09:05:44  dreyer
 * ADD: dp_asc/DegRevLexAscOrder
 * ADD: BoolePolynomial::endOfNavigation()
 * CHANGE: BoolePolynomial: removed biDegBegin(), biDegEnd(), which can be
 *   generated more generically using navigation() and endOfNavigation().
 *
 * Revision 1.2  2006/09/05 08:48:33  dreyer
 * ADD: BoolePolyRing::is(Total)DegreeOrder()
 *
 * Revision 1.1  2006/05/23 15:26:25  dreyer
 * CHANGE BoolePolyRing  can handle different orderings (only lex yet)
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

#include "OrderedManager.h"

// get all available orderings
#include "LexOrder.h"
#include "DegLexOrder.h"
#include "DegRevLexAscOrder.h"


#ifndef pbori_order_h_
#define pbori_order_h_

BEGIN_NAMESPACE_PBORI

template <class ConstructorType>
CTypes::manager_ptr
get_ordered_manager(ConstructorType& nvars, CTypes::ordercode_type order) {

  if(order == CTypes::lp)
    return CTypes::manager_ptr(new OrderedManager<CTypes::manager_base,
    LexOrder>(nvars) );
  else if(order == CTypes::dlex)
    return CTypes::manager_ptr(new OrderedManager<CTypes::manager_base,
    DegLexOrder>(nvars) );
  else if(order == CTypes::dp_asc)
    return CTypes::manager_ptr(new OrderedManager<CTypes::manager_base,
    DegRevLexAscOrder>(nvars) );
  else                        // default is lex order
    return CTypes::manager_ptr(new OrderedManager<CTypes::manager_base,
    LexOrder>(nvars) );
}

/// @class lex_compare_predicate
/// @brief defines lexicographic comparison functor
template <class LhsType, class RhsType, class BinaryPredicate>
class lex_compare_predicate:
  public std::binary_function<LhsType, RhsType, bool> {
public:

  /// Constructor
  lex_compare_predicate(const BinaryPredicate& comp):
    m_comp(comp) {}

  /// Functor operation
  bool operator()(const LhsType& lhs, const RhsType& rhs) const {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), 
                                        rhs.begin(), rhs.end(), m_comp);
  }

private:
  BinaryPredicate m_comp;
};

END_NAMESPACE_PBORI

#endif
