// -*- c++ -*-
//*****************************************************************************
/** @file CDynamicOrder.h
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

#include "CDynamicOrderBase.h"

#include "COrderProperties.h"
#include "CVariableNames.h"

#include "CGenericIter.h"
#include "COrderedIter.h"

#include "BoolePolynomial.h"
#include "BooleMonomial.h"
#include "BooleExponent.h"

#include <vector>

#ifndef CDynamicOrder_h_
#define CDynamicOrder_h_


BEGIN_NAMESPACE_PBORI


/** @class CDynamicOrder
 * @brief This class initialize the interface for orderings of
 * CDynamicOrderBase for a given OrderType.
 *
 **/

template <class OrderType>
class CDynamicOrder:
  public CDynamicOrderBase { 

public:


  /// Variable ordering definiton functional type
  typedef OrderType order_type;

  /// Variable manager interface (base type)
  typedef CDynamicOrderBase base;

  /// Type of *this
  typedef CDynamicOrder<order_type> self;

  /// Type defining order related properties
  typedef COrderProperties<order_type> properties_type;

  /// @name adopt global type definitions
  //@{
  typedef typename base::bool_type bool_type;
  typedef typename base::size_type size_type;
  typedef typename base::idx_type idx_type;
  typedef typename base::comp_type comp_type;
  typedef typename base::monom_type monom_type;
  typedef typename base::poly_type poly_type;
  typedef typename base::exp_type exp_type;
  typedef typename base::ordered_iterator ordered_iterator;
  typedef typename base::ordered_exp_iterator ordered_exp_iterator;
  typedef typename base::ordercode_type ordercode_type;
  typedef typename base::block_iterator block_iterator;
  //@}

  /// Construct new decision diagramm manager
  CDynamicOrder( const order_type& theOrder = order_type() ): 
    ordering(theOrder) { }
  
  /// Construct new decision diagramm manager
  CDynamicOrder(const self& rhs): 
    ordering(rhs.ordering) { }

  // Destructor
  ~CDynamicOrder() { }

  /// Comparison of indices
  comp_type compare(idx_type lhs, idx_type rhs) const {
    return ordering.compare(lhs, rhs);
  }

  /// Comparison of monomials
  comp_type compare(const monom_type& lhs, const monom_type& rhs) const {
    return ordering.compare(lhs, rhs);
  }

  comp_type compare(const exp_type& lhs, const exp_type& rhs) const {
    return ordering.compare(lhs, rhs);
  }

  /// Get leading term
  monom_type lead(const poly_type& rhs) const {

    return ordering.lead(rhs);
  }
  /// Get leading term (using upper bound)
  monom_type lead(const poly_type& rhs, size_type bound) const {

    return ordering.lead(rhs, bound);
  }

  /// Get leading exponent
  exp_type leadExp(const poly_type& rhs) const {

    return ordering.leadExp(rhs);
  }

  /// Get leading exponent (using upper bound)
  exp_type leadExp(const poly_type& rhs, size_type bound) const {

    return ordering.leadExp(rhs, bound);
  }

  /// Generates polynomial with leading term first (other terms may be skipped)
  poly_type leadFirst(const poly_type& poly) const {

    if(orderedStandardIteration())
      return poly;
    else 
      return lead(poly);
  }

  /// Check whether ring is lexicographical 
  bool_type isLexicographical() const {
    return properties_type().isLexicographical();
  }

  /// Test whether iterators respect order
  bool_type orderedStandardIteration() const {
    return properties_type().orderedStandardIteration();
  }

  /// Test whether variable pertubation do not change the order
  bool_type isSymmetric() const {
    return properties_type().isSymmetric();
  }

  /// Test whether we deal with a degree-ordering
  bool_type isDegreeOrder() const {
    return properties_type().isDegreeOrder();
  }

  /// Test whether we deal with a degree-ordering
  bool_type isBlockOrder() const {
    return properties_type().isBlockOrder();
  }

  /// Test whether we deal with a total degree-ordering
  bool_type isTotalDegreeOrder() const {
    return properties_type().isTotalDegreeOrder();
  }

  /// Test whether ordering is deg-rev-lex ordering
  bool_type isDegreeReverseLexicograpical() const {
    return properties_type().isDegreeReverseLexicograpical();
  }

  /// Test whether variables are in ascending order
  bool_type ascendingVariables() const {
    return properties_type().ascendingVariables();
  }

  /// Test whether variables are in descending order
  bool_type descendingVariables() const {
    return properties_type().descendingVariables();
  }

  /// Initialize iterator corresponding to leading term
  ordered_iterator leadIteratorBegin(const poly_type& poly) const {
    return ordering.leadIteratorBegin(poly);
  }

  /// End marker for iterator corresponding to leading term
  ordered_iterator leadIteratorEnd() const {
    return ordering.leadIteratorEnd();
  }
   /// Initialize exponent iterator corresponding to leading term
  ordered_exp_iterator leadExpIteratorBegin(const poly_type& poly) const {
    return ordering.leadExpIteratorBegin(poly);
  }

  /// End marker for exponent iterator corresponding to leading term
  ordered_exp_iterator leadExpIteratorEnd() const {
    return ordering.leadExpIteratorEnd();
  }

  /// Get numerical code for ordering
  ordercode_type getOrderCode() const {
    return order_type::order_code;
  }

  /// Get numerical code for base ordering (the same for non-block orderings)
  ordercode_type getBaseOrderCode() const {
    return order_type::baseorder_code;
  }

  /// @name interface for block orderings
  //@{
  block_iterator blockBegin() const { return ordering.blockBegin(); }
  block_iterator blockEnd() const { return ordering.blockEnd();  }
  void appendBlock(idx_type idx) { ordering.appendBlock(idx); }
  void clearBlocks() { ordering.clearBlocks();  }
  //@}

  /// Check, whether two indices are in the same block 
  /// (true for nonblock orderings)
  bool_type lieInSameBlock(idx_type first, idx_type second) const {
    return inSameBlockInternal(first, second, 
                               typename properties_type::blockorder_property());
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
    if (second < first)
      std::swap(first, second);
    
    block_iterator upper(blockBegin());
    while (first >= *upper)    // Note: convention, last element is max_idx
      ++upper;
    return (second < *upper);
  }

  order_type ordering;
};

END_NAMESPACE_PBORI

#endif
