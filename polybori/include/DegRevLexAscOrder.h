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
 *   (c) 2006 by The PolyBoRi Team
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

// include base order definitions
#include "COrderBase.h"

#ifndef DegRevLexAscOrder_h_
#define DegRevLexAscOrder_h_

BEGIN_NAMESPACE_PBORI

/** @class DegRevLexAscOrder
 * @brief This class defines ordering related functions.
 *
 *
 **/
class DegRevLexAscOrder:
  public COrderBase {

 public:
  //-------------------------------------------------------------------------
  // types definitions
  //-------------------------------------------------------------------------

  /// generic access to current type
  typedef DegRevLexAscOrder self;

  /// @name define generic property markers
  //@{
  typedef valid_tag symmetry_property;
  typedef valid_tag degorder_property;
  typedef valid_tag totaldegorder_property;
  typedef valid_tag ascending_property;
  typedef valid_tag degrevlexorder_property;
  typedef dp_asc_tag order_tag;
  //@}

  /// Get order code
  enum { order_code = CTypes::dp_asc, baseorder_code = order_code };

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

  /// Get leading term (using upper bound)
  monom_type lead(const poly_type& poly, size_type) const;

  /// Get leading exponent
  exp_type leadExp(const poly_type&) const;

  /// Get leading exponent (using an upper bound)
  exp_type leadExp(const poly_type&, size_type) const;

  /// Initialize iterator corresponding to leading term
  indirect_iterator leadIteratorBegin(const poly_type&) const;
  indirect_iterator leadIteratorEnd() const;
  indirect_exp_iterator leadExpIteratorBegin(const poly_type&) const;
  indirect_exp_iterator leadExpIteratorEnd() const;
};


END_NAMESPACE_PBORI

#endif // DegRevLexAscOrder_h_
