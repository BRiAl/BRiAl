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

#ifndef polybori_BlockDegLexOrder_h_
#define polybori_BlockDegLexOrder_h_

// include basic definitions
#include <polybori/pbori_defs.h>

// include base order definitions
#include <polybori/orderings/CBlockOrderingFacade.h>
#include <polybori/orderings/COrderingTags.h>

BEGIN_NAMESPACE_PBORI

/** @class BlockDegLexOrder
 * @brief This class defines ordering related functions.
 *
 *
 **/
class BlockDegLexOrder:
  public CBlockOrderingFacade<BlockDegLexOrder, block_dlex_tag> { 

  /// generic access to current type
  typedef BlockDegLexOrder self;

public:

  /// Define binary predicate for index comparision
  typedef std::less<idx_type> idx_comparer_type;

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

};


END_NAMESPACE_PBORI

#endif // polybori_BlockDegLexOrder_h_
