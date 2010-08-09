// -*- c++ -*-
//*****************************************************************************
/** @file BlockDegLexOrder.h
 *
 * @author Alexander Dreyer
 * @date 2006-05-18
 *
 * Defining Block Degree-lexicographical ordering.
 *
 * @par Copyright:
 *   (c) 2007 by The PolyBoRi Team
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.7  2009/04/09 13:55:14  dreyer
 * Fix: Compatibility: gcc4.3, python 2.6, c++0x
 *
 * Revision 1.6  2008/09/21 22:21:01  dreyer
 * Change: size_type replaces size_type for deg(), etc.
 *
 * Revision 1.5  2007/11/06 15:03:32  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.4  2007/07/31 07:43:49  dreyer
 * ADD: getBaseOrderCode(), lieInSameBlock(...), isSingleton(), isPair()...
 *
 * Revision 1.3  2007/04/30 15:20:30  dreyer
 * CHANGE: Switching from CTermIter to iterators based on CTermStack
 *
 * Revision 1.2  2007/04/23 15:32:17  dreyer
 * CHANGE: clean-up (reuse stuff from deg-orderings for block-orderings)
 *
 * Revision 1.1  2007/03/21 08:55:08  dreyer
 * ADD: first version of block_dlex running
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

// include base order definitions
#include "COrderingFacade.h"

#ifndef BlockDegLexOrder_h_
#define BlockDegLexOrder_h_

BEGIN_NAMESPACE_PBORI

/** @class BlockDegLexOrder
 * @brief This class defines ordering related functions.
 *
 *
 **/
class BlockDegLexOrder:
  public COrderingFacade<BlockDegLexOrder> {

  /// generic access to current type
  typedef BlockDegLexOrder self;

public:

  /// @name define generic property markers
  //@{
  typedef valid_tag blockorder_property;
  typedef valid_tag descending_property;
  typedef block_dlex_tag order_tag;
  //@}

  /// Define binary predicate for index comparision
  typedef std::less<idx_type> idx_comparer_type;

  /// Get order code
  enum { order_code = CTypes::block_dlex, baseorder_code = CTypes::dlex };

  /// Default Constructor
  BlockDegLexOrder(): base(), m_indices() {
    m_indices.push_back(0); 
    m_indices.push_back(CTypes::max_index()); 
  };

  /// Copy Constructor
  BlockDegLexOrder(const self& rhs): base(rhs), m_indices(rhs.m_indices) {};

  /// Destructor
  ~BlockDegLexOrder() {};

  /// Comparison of indices corresponding to variables
  comp_type compare(idx_type, idx_type) const;

  /// Comparison of monomials
  comp_type compare(const monom_type&, const monom_type&) const;

  /// Comparison of exponent vectors
  comp_type compare(const exp_type&, const exp_type&) const;

  /// Get leading term
  monom_type lead(const poly_type&) const;

  /// Get leading term (using upper bound)
  monom_type lead(const poly_type& poly, size_type) const;

  /// Get leading exponent
  exp_type leadExp(const poly_type&) const;

  /// Get leading exponent (using an upper bound)
  exp_type leadExp(const poly_type&, size_type) const;

  /// Initialize iterator corresponding to leading term
  ordered_iterator leadIteratorBegin(const poly_type&) const;
  ordered_iterator leadIteratorEnd() const;
  ordered_exp_iterator leadExpIteratorBegin(const poly_type&) const;
  ordered_exp_iterator leadExpIteratorEnd() const;

  /// @name interface for block orderings
  //@{
  block_iterator blockBegin() const { return m_indices.begin() + 1; }
  block_iterator blockEnd() const { return m_indices.end(); }
  void appendBlock(idx_type idx) { 
    m_indices.back() = idx;
    m_indices.push_back(CTypes::max_index());
  }
  void clearBlocks() { 
    m_indices.clear(); 
    m_indices.push_back(0); 
    m_indices.push_back(CTypes::max_index());
  }
  //@}

private:
  block_idx_type m_indices;
};


END_NAMESPACE_PBORI

#endif // BlockDegLexOrder_h_
