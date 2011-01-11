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

// include basic definitions
#include "pbori_defs.h"

#include "BoolePolynomial.h"
#include "BooleMonomial.h"
#include "BooleExponent.h"

#include "COrderingBase.h"
#include "COrderedIter.h"
// include ordering tags
#include "pbori_tags.h"

// include polybori functionals
#include "pbori_func.h"

#ifndef COrderingFacade_h_
#define COrderingFacade_h_


BEGIN_NAMESPACE_PBORI

/** @class COrderingFacade
 * @brief This class initialize the interface for orderings of
 * CDynamicOrderBase for a given OrderType. OrderType must inherit from
 * COrderingFacade<OrderType>.
 */
template <class OrderType>
class COrderingFacade:
  public COrderingBase { 
  typedef COrderingFacade self;

  /// Actual base type
  typedef COrderingBase base_type;

public:

  /// *this is to be used as base for @c OrderType only
  typedef self base;

  /// Variable ordering definiton functional type
  typedef OrderType order_type;

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
    return is_valid<typename order_type::lex_property>::result;
  }

  /// Test whether iterators respect order
  bool_type orderedStandardIteration() const {
    return is_valid<typename order_type::ordered_property>::result;
  }

  /// Test whether variable pertubation do not change the order
  bool_type isSymmetric() const {
    return is_valid<typename order_type::symmetry_property>::result;
  }

  /// Test whether we deal with a degree-ordering
  bool_type isDegreeOrder() const {
    return is_valid<typename order_type::degorder_property>::result;
  }

  /// Test whether we deal with a degree-ordering
  bool_type isBlockOrder() const {
    return is_valid<typename order_type::blockorder_property>::result;
  }

  /// Test whether we deal with a total degree-ordering
  bool_type isTotalDegreeOrder() const {
    return is_valid<typename order_type::totaldegorder_property>::result;
  }

  /// Test whether ordering is deg-rev-lex ordering
  bool_type isDegreeReverseLexicograpical() const {
    return is_valid<typename order_type::degrevlexorder_property>::result;
  }

  /// Test whether variables are in ascending order
  bool_type ascendingVariables() const {
    return is_valid<typename order_type::ascending_property>::result;
  }

  /// Test whether variables are in descending order
  bool_type descendingVariables() const {
    return is_valid<typename order_type::descending_property>::result;
  }

  /// Get numerical code for ordering
  ordercode_type getOrderCode() const {
    return order_type::order_code;
  }

  /// Get numerical code for base ordering (the same for non-block orderings)
  ordercode_type getBaseOrderCode() const {
    return order_type::baseorder_code;
  }

  /// Check, whether two indices are in the same block 
  /// (true for nonblock orderings)
  bool_type lieInSameBlock(idx_type first, idx_type second) const {
    return inSameBlockInternal(first, second, 
                               typename order_type::blockorder_property());
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
    if(UNLIKELY(first > CTypes::max_idx || second > CTypes::max_idx))
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
