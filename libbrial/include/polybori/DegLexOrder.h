// -*- c++ -*-
//*****************************************************************************
/** @file DegLexOrder.h
 *
 * @author Alexander Dreyer
 * @date 2006-05-18
 *
 * Defining Degree-lexicographical ordering.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_DegLexOrder_h_
#define polybori_DegLexOrder_h_

// include basic definitions
#include <polybori/pbori_defs.h>

// include base order definitions
#include <polybori/orderings/COrderingFacade.h>
#include <polybori/orderings/COrderingTags.h>

BEGIN_NAMESPACE_PBORI

/** @class DegLexOrder
 * @brief This class defines ordering related functions.
 *
 *
 **/
class DegLexOrder:
  public COrderingFacade<DegLexOrder, dlex_tag> {

  /// generic access to current type
  typedef DegLexOrder self;

public:

  /// Define binary predicate for index comparision
  typedef std::less<idx_type> idx_comparer_type;

  /// Default Constructor
  DegLexOrder(): base() {};

  /// Copy Constructor
  DegLexOrder(const self& rhs): base(rhs) {};

  /// Destructor
  ~DegLexOrder() {};

  /// Comparison of indices corresponding to variables
  comp_type compare(idx_type, idx_type) const;

  /// Comparison of monomials
  comp_type compare(const monom_type&, const monom_type&) const;

  /// Comparison of exponent vectors
  comp_type compare(const exp_type&, const exp_type&) const;

  /// Get leading term
  monom_type lead(const poly_type&) const;

  /// Get leading term (using upper bound of the polynomial degree)
  /// @note returns @c lead of the sub-polynomial of degree @c bound
  monom_type lead(const poly_type& poly, deg_type bound) const;

  /// Get leading exponent
  exp_type leadExp(const poly_type&) const;

  /// Get leading exponent (using an upper bound of the polynomial degree)
  /// @note See note of the bounded @c lead
  exp_type leadExp(const poly_type&, deg_type) const;
};


END_NAMESPACE_PBORI

#endif // polybori_DegLexOrder_h_
