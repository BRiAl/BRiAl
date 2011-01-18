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

// include basic definitions
#include "pbori_defs.h"

// include base order definitions
#include "COrderingFacade.h"
#include "COrderingTags.h"

#ifndef DegLexOrder_h_
#define DegLexOrder_h_

BEGIN_NAMESPACE_PBORI

/** @class DegLexOrder
 * @brief This class defines ordering related functions.
 *
 *
 **/
class DegLexOrder:
  public COrderingFacade<DegLexOrder>, public COrderingTags<dlex_tag> {

  /// generic access to current type
  typedef DegLexOrder self;

public:

  /// @name define generic property markers
  //@{
  typedef valid_tag symmetry_property;
  typedef valid_tag degorder_property;
  typedef valid_tag totaldegorder_property;
  typedef valid_tag descending_property;
  //@}

  /// Define binary predicate for index comparision
  typedef std::less<idx_type> idx_comparer_type;

  /// Get order code
  enum { order_code = CTypes::dlex, baseorder_code = order_code };

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

  /// Get leading term (using upper bound)
  monom_type lead(const poly_type& poly, size_type) const;

  /// Get leading exponent
  exp_type leadExp(const poly_type&) const;

  /// Get leading exponent (using an upper bound)
  exp_type leadExp(const poly_type&, size_type) const;

/*  /// Initialize iterator corresponding to leading term
  ordered_iterator leadIteratorBegin(const poly_type&) const;
  ordered_iterator leadIteratorEnd() const;
  ordered_exp_iterator leadExpIteratorBegin(const poly_type&) const;
  ordered_exp_iterator leadExpIteratorEnd() const;*/
};


END_NAMESPACE_PBORI

#endif // DegLexOrder_h_
