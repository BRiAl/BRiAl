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

// include basic definitions
#include "pbori_defs.h"

// include base order definitions
#include "COrderingFacade.h"
#include "COrderingTags.h"

#ifndef LexOrder_h_
#define LexOrder_h_

BEGIN_NAMESPACE_PBORI

/** @class LexOrder
 * @brief This class defines ordering related functions.
 *
 *
 **/
class LexOrder:
  public COrderingFacade<LexOrder>, public COrderingTags<lex_tag> {

  /// generic access to current type
  typedef LexOrder self;

 public:

  /// @name define generic property markers
  //@{
  typedef valid_tag lex_property; 
  typedef valid_tag ordered_property;
  typedef valid_tag symmetry_property;
  typedef valid_tag descending_property;
  //@}

  /// Get order code
  enum { order_code = CTypes::lp, baseorder_code = order_code };

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

  /// Get leading term (using upper bound)
  monom_type lead(const poly_type& poly, size_type) const {
    if UNLIKELY(poly.isZero())
      throw PBoRiGenericError<CTypes::illegal_on_zero>();
    return lead(poly);
  }

  /// Get leading exponent
  exp_type leadExp(const poly_type&) const;

  /// Get leading exponent (using upper bound)
  exp_type leadExp(const poly_type& poly, size_type) const {
    if UNLIKELY(poly.isZero())
      throw PBoRiGenericError<CTypes::illegal_on_zero>();
    return leadExp(poly);
  }
};


END_NAMESPACE_PBORI

#endif // LexOrder_h_
