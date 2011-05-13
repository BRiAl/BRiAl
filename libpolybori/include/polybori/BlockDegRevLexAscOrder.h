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

#ifndef polybori_BlockDegRevLexAscOrder_h_
#define polybori_BlockDegRevLexAscOrder_h_

// include basic definitions
#include <polybori/pbori_defs.h>

// include base order definitions
#include <polybori/orderings/CBlockOrderingFacade.h>
#include <polybori/orderings/COrderingTags.h>

BEGIN_NAMESPACE_PBORI

/** @class BlockDegRevLexAscOrder
 * @brief This class defines ordering related functions.
 *
 *
 **/
class BlockDegRevLexAscOrder:
  public CBlockOrderingFacade<BlockDegRevLexAscOrder, block_dp_asc_tag> {

  /// generic access to current type
  typedef BlockDegRevLexAscOrder self;

public:
  /// Define binary predicate for index comparision
  typedef std::greater<idx_type> idx_comparer_type;

  /// Default Constructor
  BlockDegRevLexAscOrder(): base() {};

  /// Copy Constructor
  BlockDegRevLexAscOrder(const self& rhs): base(rhs) {};

  /// Destructor
  ~BlockDegRevLexAscOrder() {};

  /// Comparison of indices corresponding to variables
  comp_type compare(idx_type, idx_type) const;

  /// Comparison of monomials
  comp_type compare(const monom_type&, const monom_type&) const;

  /// Comparison of exponent vectors
  comp_type compare(const exp_type&, const exp_type&) const;

};


END_NAMESPACE_PBORI

#endif // polybori_BlockDegRevLexAscOrder_h_
