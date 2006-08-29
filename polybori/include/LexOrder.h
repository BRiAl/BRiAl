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

// include basic definitions for orderings
#include "order_tags.h"

// include polynomial definitions
#include "BoolePolynomial.h"

// include monomial definitions
#include "BooleMonomial.h"

// include exponent vector definitions
#include "BooleExponent.h"

#ifndef LexOrder_h_
#define LexOrder_h_

BEGIN_NAMESPACE_PBORI

/** @class LexOrder
 * @brief This class defines ordering related functions.
 *
 *
 **/
class LexOrder {

 public:
  //-------------------------------------------------------------------------
  // types definitions
  //-------------------------------------------------------------------------

  /// generic access to current type
  typedef LexOrder self;

  /// Type of Boolean polynomials
  typedef BoolePolynomial poly_type;

  /// Type of Boolean monomials
  typedef BooleMonomial monom_type;

  /// Type of Boolean monomials
  typedef BooleExponent exp_type;

  /// @name adopt global type definitions
  //@{
  typedef CTypes::bool_type bool_type;
  typedef CTypes::dd_type dd_type;
  typedef CTypes::size_type size_type;
  typedef CTypes::idx_type idx_type;
  typedef CTypes::comp_type comp_type;
  //@}

  /// @name define generic property markers
  //@{
  typedef lex_tag lex_property; 
  typedef ordered_tag ordered_property;
  //@}

  /// Default Constructor
  LexOrder() {};

  /// Copy Constructor
  LexOrder(const self&) {};

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

  /// Get leading exponent
  exp_type leadExp(const poly_type&) const;
};


END_NAMESPACE_PBORI

#endif // LexOrder_h_
