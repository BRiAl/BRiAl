// -*- c++ -*-
//*****************************************************************************
/** @file LexOrder.h
 *
 * @author Alexander Dreyer
 * @date 2006-05-18
 *
 * Defining lexicographical ordering.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_LexOrder_h_
#define polybori_LexOrder_h_

// include basic definitions
#include <polybori/pbori_defs.h>

// include base order definitions
#include <polybori/orderings/COrderingFacade.h>
#include <polybori/orderings/COrderingTags.h>

BEGIN_NAMESPACE_PBORI

/** @class LexOrder
 * @brief This class defines ordering related functions.
 *
 *
 **/
class LexOrder:
  public COrderingFacade<LexOrder, lex_tag> {

  /// generic access to current type
  typedef LexOrder self;

 public:

  /// Define binary predicate for index comparision
  typedef std::less<idx_type> idx_comparer_type;

  /// Default Constructor
  LexOrder(): base() {};

  /// Copy Constructor
  LexOrder(const self& rhs): base(rhs) {};

  /// Destructor
  ~LexOrder() {};

  /// Comparison of indices corresponding to variables
  comp_type compare(idx_type, idx_type) const;

  /// Comparison of monomials
  comp_type compare(const monom_type&, const monom_type&) const;

  /// Comparison of exponent vectors
  comp_type compare(const exp_type&, const exp_type&) const;

  /// Get leading term
  monom_type lead(const poly_type&) const;

  /// Leading monomial with bound (just the ordinary leading monomial)
  /// @note falls back to @c leadExp, ignores second argument
  monom_type lead(const poly_type& poly, deg_type) const {
    return lead(poly);
  }

  /// Get leading exponent
  exp_type leadExp(const poly_type&) const;

  /// Leading exponent with bound (just the ordinary leading monomial)
  /// @note falls back to @c leadExp, ignores second argument
  exp_type leadExp(const poly_type& poly, deg_type) const {
    return leadExp(poly);
  }
};


END_NAMESPACE_PBORI

#endif // polybori_LexOrder_h_
