// -*- c++ -*-
//*****************************************************************************
/** @file COrderingFacade.h
 *
 * @author Alexander Dreyer
 * @date 2006-05-23
 *
 * Defines generic classes for ordering management for polynomial rings.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_orderings_COrderingFacade_h_
#define polybori_orderings_COrderingFacade_h_

// include basic definitions
#include <polybori/pbori_defs.h>

#include <polybori/BoolePolynomial.h>
#include <polybori/BooleMonomial.h>
#include <polybori/BooleExponent.h>

#include "COrderingBase.h"
#include "COrderingTags.h"
#include <polybori/iterators/COrderedIter.h>
// include ordering tags
#include <polybori/common/tags.h>
#include "order_traits.h"
// include polybori functionals
#include <polybori/routines/pbori_func.h>

BEGIN_NAMESPACE_PBORI

/** @class COrderingFacade
 * @brief This class initialize the interface for orderings of
 * CDynamicOrderBase for a given OrderType. OrderType must inherit from
 * COrderingFacade<OrderType>.
 */
template <class OrderType, class OrderTag>
class COrderingFacade:
  public COrderingBase, 
  public COrderingTags<OrderTag>, public order_traits<OrderTag> { 

  /// Name type of *this
  typedef COrderingFacade self;

  /// Actual base type
  typedef COrderingBase base_type;

public:
  /// *this is to be used as base for @c OrderType only
  typedef self base;

  /// Variable ordering definiton functional type
  typedef OrderType order_type;

  /// Tag for for leading monomial cache
  typedef CCacheTypes::lead_tag<OrderTag> order_lead_tag;
  typedef COrderingTags<OrderTag> ordering_tags;
  /// Construct new decision diagramm manager
  COrderingFacade(): 
    base_type() { }
  
  /// Construct new decision diagramm manager
  COrderingFacade(const self& rhs): 
    base_type(rhs) { }

  /// Destructor
  ~COrderingFacade() { }


  /// Generates polynomial with leading term first (other terms may be skipped)
  poly_type leadFirst(const poly_type& poly) const {

    if(orderedStandardIteration())
      return poly;
    else 
      return lead(poly);
  }

  /// Check whether ring is lexicographical 
  bool_type isLexicographical() const {
    return is_valid<typename ordering_tags::lex_property>::result;
  }

  /// Test whether iterators respect order
  bool_type orderedStandardIteration() const {
    return is_valid<typename ordering_tags::ordered_property>::result;
  }

  /// Test whether variable pertubation do not change the order
  bool_type isSymmetric() const {
    return is_valid<typename ordering_tags::symmetry_property>::result;
  }

  /// Test whether we deal with a degree-ordering
  bool_type isDegreeOrder() const {
    return is_valid<typename ordering_tags::degorder_property>::result;
  }

  /// Test whether we deal with a degree-ordering
  bool_type isBlockOrder() const {
    return is_valid<typename ordering_tags::blockorder_property>::result;
  }

  /// Test whether we deal with a total degree-ordering
  bool_type isTotalDegreeOrder() const {
    return is_valid<typename ordering_tags::totaldegorder_property>::result;
  }

  /// Test whether ordering is deg-rev-lex ordering
  bool_type isDegreeReverseLexicographical() const {
    return is_valid<typename ordering_tags::degrevlexorder_property>::result;
  }

  /// Test whether variables are in ascending order
  bool_type ascendingVariables() const {
    return is_valid<typename ordering_tags::ascending_property>::result;
  }

  /// Test whether variables are in descending order
  bool_type descendingVariables() const {
    return is_valid<typename ordering_tags::descending_property>::result;
  }

  /// Get numerical code for ordering
  ordercode_type getOrderCode() const {
    return order_traits<OrderTag>::order_code;
  }

  /// Get numerical code for base ordering (the same for non-block orderings)
  ordercode_type getBaseOrderCode() const {
    return  order_traits<OrderTag>::baseorder_code;
  }

  /// Check, whether two indices are in the same block 
  /// (true for nonblock orderings)
  bool_type lieInSameBlock(idx_type first, idx_type second) const {
    return inSameBlockInternal(first, second, 
                               typename ordering_tags::blockorder_property());
  }


  /// Generic procedure to get index, where last block starts
  idx_type lastBlockStart() const {
    if (isBlockOrder()) {
      return *(blockEnd() - 2);
    }
    else if (isLexicographical()) {
      return CTypes::max_idx;
    }
    return 0;
  }

  // Initialize iterator corresponding to leading term
  ordered_iterator
  leadIteratorBegin(const poly_type& poly) const {
    return CGenericOrderedIter<order_type, navigator,
      monom_type>(poly.navigation(), poly.ring());
  }

  ordered_iterator
  leadIteratorEnd(const poly_type& poly) const {
    return CGenericOrderedIter<order_type, navigator, monom_type>(navigator(), poly.ring());
  }

  // Initialize iterator corresponding to leading term
  ordered_exp_iterator
  leadExpIteratorBegin(const poly_type& poly) const {
    return CGenericOrderedIter<order_type, navigator, exp_type>(poly.navigation(), poly.ring()); 
  }

  ordered_exp_iterator
  leadExpIteratorEnd(const poly_type& poly) const {
    return CGenericOrderedIter<order_type, navigator, exp_type>(navigator(), poly.ring());
  }

protected:

  /// trivial case for non-block orderings
  bool_type inSameBlockInternal(idx_type, idx_type,
                                invalid_tag) const { // not a block order 
    return true;
  }
  
  /// complicated case for block orderings  
  bool_type inSameBlockInternal(idx_type first, idx_type second, 
                                valid_tag) const { // is block order 
    // todo: throw here if first,second >=CTypes::max_idx
    if(PBORI_UNLIKELY(first > CTypes::max_idx || second > CTypes::max_idx || 
                first < 0 || second < 0))
      throw std::runtime_error("Variable index out of range.");

    if (second < first)
      std::swap(first, second);
    
    block_iterator upper(blockBegin());
    while (first >= *upper)    // Note: convention, last element is max_idx
      ++upper;
    return (second < *upper);
  }

};

END_NAMESPACE_PBORI

#endif
