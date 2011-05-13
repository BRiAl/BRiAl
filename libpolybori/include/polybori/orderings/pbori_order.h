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
 *   (c) 2006 by The PolyBoRi Team
**/
//*****************************************************************************

#ifndef polybori_orderings_pbori_order_h_
#define polybori_orderings_pbori_order_h_

// include basic definitions
#include <polybori/pbori_defs.h>

// Get infrastructure for dynamic orderings
#include "COrderingBase.h"

// get all available orderings
#include <polybori/LexOrder.h>
#include <polybori/DegLexOrder.h>
#include <polybori/DegRevLexAscOrder.h>
#include <polybori/BlockDegLexOrder.h>
#include <polybori/BlockDegRevLexAscOrder.h>

BEGIN_NAMESPACE_PBORI

inline boost::shared_ptr<COrderingBase>
get_ordering(CTypes::ordercode_type order) {
  typedef boost::shared_ptr<COrderingBase> order_ptr;

  if(order == CTypes::lp)
    return order_ptr(new LexOrder);
  else if(order == CTypes::dlex)
    return order_ptr(new DegLexOrder);
  else if(order == CTypes::dp_asc)
    return order_ptr(new DegRevLexAscOrder);
  else if(order == CTypes::block_dlex)
    return order_ptr(new BlockDegLexOrder);
  else if(order == CTypes::block_dp_asc)
     return order_ptr(new BlockDegRevLexAscOrder);

  // default is lex order
  return order_ptr(new LexOrder);
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
