// -*- c++ -*-
//*****************************************************************************
/** @file DegRevLexAscOrder.h
 *
 * @author Alexander Dreyer
 * @date 2006-05-18
 *
 * Defining Degree-reverse lexicographical ordering.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_DegRevLexAscOrder_h_
#define polybori_DegRevLexAscOrder_h_

// include basic definitions
#include <polybori/pbori_defs.h>

// include base order definitions
#include <polybori/orderings/COrderingFacade.h>
#include <polybori/orderings/COrderingTags.h>

BEGIN_NAMESPACE_PBORI

/** @class DegRevLexAscOrder
 * @brief This class defines ordering related functions.
 *
 *
 **/
class DegRevLexAscOrder:
  public COrderingFacade<DegRevLexAscOrder, dp_asc_tag> {

  /// generic access to current type
  typedef DegRevLexAscOrder self;

 public:
  /// Define binary predicate for index comparision
  typedef std::greater<idx_type> idx_comparer_type;

  /// Default Constructor
  DegRevLexAscOrder(): base() {};

  /// Copy Constructor
  DegRevLexAscOrder(const self& rhs): base(rhs) {};

  /// Destructor
  ~DegRevLexAscOrder() {};

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

#endif // polybori_DegRevLexAscOrder_h_
