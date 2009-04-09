// -*- c++ -*-
//*****************************************************************************
/** @file BlockDegRevLexAscOrder.h
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
 * Revision 1.10  2009/04/09 13:55:14  dreyer
 * Fix: Compatibility: gcc4.3, python 2.6, c++0x
 *
 * Revision 1.9  2008/09/21 22:21:02  dreyer
 * Change: deg_type replaces size_type for deg(), etc.
 *
 * Revision 1.8  2007/11/06 15:03:32  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.7  2007/10/11 09:48:39  dreyer
 * Fix: codes and tags of block_dp_asc corrected
 *
 * Revision 1.6  2007/10/11 09:17:21  bricken
 * + corrected baseorder code
 *
 * Revision 1.5  2007/07/31 07:43:49  dreyer
 * ADD: getBaseOrderCode(), lieInSameBlock(...), isSingleton(), isPair()...
 *
 * Revision 1.4  2007/04/30 15:20:30  dreyer
 * CHANGE: Switching from CTermIter to iterators based on CTermStack
 *
 * Revision 1.3  2007/04/23 15:32:17  dreyer
 * CHANGE: clean-up (reuse stuff from deg-orderings for block-orderings)
 *
 * Revision 1.2  2007/04/19 09:52:07  dreyer
 * FIX: block dp_asc index comparison
 *
 * Revision 1.1  2007/04/18 15:37:28  dreyer
 * ADD: dp_asc now active
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

// include base order definitions
#include "COrderBase.h"

#ifndef BlockDegRevLexAscOrder_h_
#define BlockDegRevLexAscOrder_h_

BEGIN_NAMESPACE_PBORI

/** @class BlockDegRevLexAscOrder
 * @brief This class defines ordering related functions.
 *
 *
 **/
class BlockDegRevLexAscOrder:
  public COrderBase {

 public:
  //-------------------------------------------------------------------------
  // types definitions
  //-------------------------------------------------------------------------

  /// generic access to current type
  typedef BlockDegRevLexAscOrder self;

  /// @name define generic property markers
  //@{
  typedef valid_tag blockorder_property;
  typedef invalid_tag descending_property;
  typedef block_dp_asc_tag order_tag;
  //@}

  /// Define binary predicate for index comparision
  typedef std::greater<idx_type> idx_comparer_type;

  /// Get order code
  enum { order_code = CTypes::block_dp_asc, baseorder_code = CTypes::dp_asc };

  /// Default Constructor
  BlockDegRevLexAscOrder(): base(), m_indices() {
    m_indices.push_back(0); 
    m_indices.push_back(CTypes::max_index()); 
  };

  /// Copy Constructor
  BlockDegRevLexAscOrder(const self& rhs): base(rhs), m_indices(rhs.m_indices) {};

  /// Destructor
  ~BlockDegRevLexAscOrder() {};

  /// Comparison of indices corresponding to variables
  comp_type compare(idx_type, idx_type) const;

  /// Comparison of monomials
  comp_type compare(const monom_type&, const monom_type&) const;

  /// Comparison of exponent vectors
  comp_type compare(const exp_type&, const exp_type&) const;

  /// Get leading term
  monom_type lead(const poly_type&) const;

  /// Get leading term (using upper bound)
  monom_type lead(const poly_type& poly, deg_type) const;

  /// Get leading exponent
  exp_type leadExp(const poly_type&) const;

  /// Get leading exponent (using an upper bound)
  exp_type leadExp(const poly_type&, deg_type) const;

  /// Initialize iterator corresponding to leading term
  indirect_iterator leadIteratorBegin(const poly_type&) const;
  indirect_iterator leadIteratorEnd() const;
  indirect_exp_iterator leadExpIteratorBegin(const poly_type&) const;
  indirect_exp_iterator leadExpIteratorEnd() const;

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

#endif // BlockDegRevLexAscOrder_h_
