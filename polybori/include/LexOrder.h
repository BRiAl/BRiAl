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
 * Revision 1.13  2006/10/06 12:52:01  dreyer
 * ADD easy_equility_property and used in lex_compare
 *
 * Revision 1.12  2006/10/05 12:51:32  dreyer
 * CHANGE: Made lex-based comparisions more generic.
 *
 * Revision 1.11  2006/10/04 12:22:31  dreyer
 * ADD: getOrderCode()
 *
 * Revision 1.10  2006/09/13 15:07:04  dreyer
 * ADD: lead(sugar) and infrastructure
 *
 * Revision 1.9  2006/09/08 14:31:39  dreyer
 * ADD: COrderedIter and infrastructure for order-dependent iterator
 *
 * Revision 1.8  2006/09/05 11:10:44  dreyer
 * ADD: BoolePolyRing::Compare(...), fixed assertion in groebner
 *
 * Revision 1.7  2006/09/05 08:48:32  dreyer
 * ADD: BoolePolyRing::is(Total)DegreeOrder()
 *
 * Revision 1.6  2006/09/01 11:02:48  dreyer
 * ADD: OrderedManager::isSymmetric()
 *
 * Revision 1.5  2006/08/29 09:02:36  dreyer
 * ADD: leadExp()
 *
 * Revision 1.4  2006/08/24 14:47:50  dreyer
 * ADD: BooleExponent integrated, FIX: multiples (for indices < first)
 *
 * Revision 1.3  2006/07/20 08:55:49  dreyer
 * ADD isOrdered() and  isLexicographical()
 *
 * Revision 1.2  2006/07/14 09:02:49  dreyer
 * ADD: greater_variable()
 *
 * Revision 1.1  2006/05/23 12:01:58  dreyer
 * + Initial Version
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

// include base order definitions
#include "COrderBase.h"

#ifndef LexOrder_h_
#define LexOrder_h_

BEGIN_NAMESPACE_PBORI

/** @class LexOrder
 * @brief This class defines ordering related functions.
 *
 *
 **/
class LexOrder:
  public COrderBase {

 public:
  //-------------------------------------------------------------------------
  // types definitions
  //-------------------------------------------------------------------------

  /// generic access to current type
  typedef LexOrder self;

  /// @name define generic property markers
  //@{
  typedef valid_tag lex_property; 
  typedef valid_tag ordered_property;
  typedef valid_tag symmetry_property;
  typedef valid_tag descending_property;
  typedef lex_tag order_tag;
  //@}

  /// Get order code
  enum { order_code = CTypes::lp  };

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
  monom_type lead(const poly_type& poly, size_type) const { return lead(poly); }

  /// Get leading exponent
  exp_type leadExp(const poly_type&) const;

  /// Get leading exponent (using upper bound)
  exp_type leadExp(const poly_type& poly, size_type) const {
    return leadExp(poly); }

  /// Initialize iterator corresponding to leading term
  iterator leadIterator(const poly_type&) const;

  /// Find next term (after iter) in polynomial according to current order
  iterator incrementIterator(iterator iter, const poly_type&) const;
};


END_NAMESPACE_PBORI

#endif // LexOrder_h_
