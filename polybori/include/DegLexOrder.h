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
 *   (c) 2006 by
 *   Dep. of Mathematics, Kaiserslautern University of Technology and @n
 *   Fraunhofer Institute for Industrial Mathematics (ITWM)
 *   D-67663 Kaiserslautern, Germany
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.6  2006/10/04 12:22:32  dreyer
 * ADD: getOrderCode()
 *
 * Revision 1.5  2006/09/13 15:07:04  dreyer
 * ADD: lead(sugar) and infrastructure
 *
 * Revision 1.4  2006/09/08 14:31:39  dreyer
 * ADD: COrderedIter and infrastructure for order-dependent iterator
 *
 * Revision 1.3  2006/09/05 11:10:44  dreyer
 * ADD: BoolePolyRing::Compare(...), fixed assertion in groebner
 *
 * Revision 1.2  2006/09/05 08:48:32  dreyer
 * ADD: BoolePolyRing::is(Total)DegreeOrder()
 *
 * Revision 1.1  2006/09/04 15:58:43  dreyer
 * ADD: DegLexOrder and preparations
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

// include basic definitions for orderings
#include "order_tags.h"

// include base order definitions
#include "COrderBase.h"

#ifndef DegLexOrder_h_
#define DegLexOrder_h_

BEGIN_NAMESPACE_PBORI

/** @class DegLexOrder
 * @brief This class defines ordering related functions.
 *
 *
 **/
class DegLexOrder:
  public COrderBase {

 public:
  //-------------------------------------------------------------------------
  // types definitions
  //-------------------------------------------------------------------------

  /// generic access to current type
  typedef DegLexOrder self;

  /// @name define generic property markers
  //@{
  typedef valid_tag symmetry_property;
  typedef valid_tag degorder_property;
  typedef valid_tag totaldegorder_property;
  typedef valid_tag descending_property;
  typedef dlex_tag order_tag;
  //@}

  /// Get order code
  enum { order_code = CTypes::dlex };

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

  /// Initialize iterator corresponding to leading term
  iterator leadIterator(const poly_type&) const;

  /// Find next term (after iter) in polynomial according to current order
  iterator incrementIterator(iterator iter, const poly_type&) const;
};


END_NAMESPACE_PBORI

#endif // DegLexOrder_h_
