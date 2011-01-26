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
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

// include base order definitions
#include "CBlockOrderingFacade.h"
#include "COrderingTags.h"

#ifndef BlockDegLexOrder_h_
#define BlockDegLexOrder_h_

BEGIN_NAMESPACE_PBORI

/** @class BlockDegLexOrder
 * @brief This class defines ordering related functions.
 *
 *
 **/
class BlockDegLexOrder:
  public CBlockOrderingFacade<BlockDegLexOrder>, 
  public COrderingTags<block_dlex_tag> { 

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
  BlockDegLexOrder(): base() {};

  /// Copy Constructor
  BlockDegLexOrder(const self& rhs): base(rhs) {};

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
  monom_type lead(const poly_type& poly, deg_type) const;

  /// Get leading exponent
  exp_type leadExp(const poly_type&) const;

  /// Get leading exponent (using an upper bound)
  exp_type leadExp(const poly_type&, deg_type) const;
};


END_NAMESPACE_PBORI

#endif // BlockDegLexOrder_h_
