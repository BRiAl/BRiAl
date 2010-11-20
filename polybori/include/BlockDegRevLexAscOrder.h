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
 *   (c) 2007-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

// include base order definitions
#include "COrderingFacade.h"
#include "COrderingTags.h"

#ifndef BlockDegRevLexAscOrder_h_
#define BlockDegRevLexAscOrder_h_

BEGIN_NAMESPACE_PBORI

/** @class BlockDegRevLexAscOrder
 * @brief This class defines ordering related functions.
 *
 *
 **/
class BlockDegRevLexAscOrder:
  public COrderingFacade<BlockDegRevLexAscOrder>, 
   public COrderingTags<block_dp_asc_tag> {

  /// generic access to current type
  typedef BlockDegRevLexAscOrder self;

 public:

  /// @name define generic property markers
  //@{
  typedef valid_tag blockorder_property;
  typedef valid_tag ascending_property;
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

#endif // BlockDegRevLexAscOrder_h_
