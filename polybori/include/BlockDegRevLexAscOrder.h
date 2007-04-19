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
 *   (c) 2007 by
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
 * Revision 1.2  2007/04/19 09:52:07  dreyer
 * FIX: block dp_asc index comparison
 *
 * Revision 1.1  2007/04/18 15:37:28  dreyer
 * ADD: dp_asc now active
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
  typedef block_dlex_tag order_tag;
  //@}

  /// Define binary predicate for index comparision
  typedef std::greater<idx_type> idx_comparer_type;

  /// Get order code
  enum { order_code = CTypes::block_dlex };

  /// Default Constructor
  BlockDegRevLexAscOrder(): base(), m_indices() {
    m_indices.push_back(CTypes::max_idx); 
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
  monom_type lead(const poly_type& poly, size_type) const;

  /// Get leading exponent
  exp_type leadExp(const poly_type&) const;

  /// Get leading exponent (using an upper bound)
  exp_type leadExp(const poly_type&, size_type) const;

  /// Initialize iterator corresponding to leading term
  iterator leadIterator(const poly_type&) const;
  indirect_iterator leadIteratorBegin(const poly_type&) const;
  indirect_iterator leadIteratorEnd() const;
  indirect_exp_iterator leadExpIteratorBegin(const poly_type&) const;
  indirect_exp_iterator leadExpIteratorEnd() const;


  /// Find next term (after iter) in polynomial according to current order
  iterator incrementIterator(iterator iter, const poly_type&) const;

  /// @name interface for block orderings
  //@{
  block_iterator blockBegin() const { return m_indices.begin(); }
  block_iterator blockEnd() const { return m_indices.end(); }
  void appendBlock(idx_type idx) { 
    m_indices.back() = idx;
    m_indices.push_back(CTypes::max_idx);
  }
  void clearBlocks() { m_indices.clear(); }
  //@}

private:
  block_idx_type m_indices;
};


END_NAMESPACE_PBORI

#endif // BlockDegRevLexAscOrder_h_
