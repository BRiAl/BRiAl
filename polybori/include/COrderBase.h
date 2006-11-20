// -*- c++ -*-
//*****************************************************************************
/** @file COrderBase.h
 *
 * @author Alexander Dreyer
 * @date 2006-09-05
 *
 * This file defines a base class for orderings.
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
 * Revision 1.8  2006/11/20 13:08:00  dreyer
 * FIX: Destructor now virtual
 *
 * Revision 1.7  2006/10/06 12:52:00  dreyer
 * ADD easy_equility_property and used in lex_compare
 *
 * Revision 1.6  2006/10/04 11:48:28  dreyer
 * ADD: isDegreeReverseLexicograpical()
 *
 * Revision 1.5  2006/09/13 15:07:04  dreyer
 * ADD: lead(sugar) and infrastructure
 *
 * Revision 1.4  2006/09/12 14:56:55  dreyer
 * ADD bidirectional term iterator template
 *
 * Revision 1.3  2006/09/08 14:31:39  dreyer
 * ADD: COrderedIter and infrastructure for order-dependent iterator
 *
 * Revision 1.2  2006/09/05 11:10:44  dreyer
 * ADD: BoolePolyRing::Compare(...), fixed assertion in groebner
 *
 * Revision 1.1  2006/09/05 10:21:39  dreyer
 * + Initial Version
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

// include basic definitions for orderings
#include "pbori_tags.h"

// include polynomial definitions
#include "BoolePolynomial.h"

// include monomial definitions
#include "BooleMonomial.h"

// include exponent vector definitions
#include "BooleExponent.h"

#ifndef COrderBase_h_
#define COrderBase_h_

BEGIN_NAMESPACE_PBORI


/** @class COrderBase
 * @brief This class defines ordering related functions.
 *
 *
 **/
class COrderBase:
  public CTypes::auxtypes_type {

 public:
  //-------------------------------------------------------------------------
  // types definitions
  //-------------------------------------------------------------------------

  /// current type should used as base only
  typedef COrderBase base;

  /// Type of Boolean polynomials
  typedef BoolePolynomial poly_type;

  /// Type of degree iterator
  typedef poly_type::bidirectional_iterator iterator;


  /// Type of degree iterator
  typedef poly_type::size_type size_type;

  /// Type of Boolean monomials
  typedef BooleMonomial monom_type;

  /// Type of Boolean monomials
  typedef BooleExponent exp_type;

  /// @name define generic property markers (default is invalid)
  //@{
  typedef invalid_tag lex_property; 
  typedef invalid_tag ordered_property;
  typedef invalid_tag symmetry_property;
  typedef invalid_tag degorder_property;
  typedef invalid_tag degrevlexorder_property; 
  typedef invalid_tag totaldegorder_property;
  typedef invalid_tag ascending_property;
  typedef invalid_tag descending_property;
  //@}

  /// Default Constructor
  COrderBase() {};

  /// Copy Constructor
  COrderBase(const COrderBase&) {};

  /// Destructor
  virtual ~COrderBase() {};

  /// Comparison of indices corresponding to variables
  virtual comp_type compare(idx_type, idx_type) const = 0;

  /// Comparison of monomials
  virtual comp_type compare(const monom_type&, const monom_type&) const = 0;

  /// Comparison of exponent vectors
  virtual comp_type compare(const exp_type&, const exp_type&) const = 0;

  /// Get leading term
  virtual monom_type lead(const poly_type&) const = 0;

   /// Get leading term (using an upper bound)
  virtual monom_type lead(const poly_type&, size_type) const = 0;

  /// Get leading exponent
  virtual exp_type leadExp(const poly_type&) const = 0;

  /// Get leading exponent (using an upper bound)
  virtual exp_type leadExp(const poly_type&, size_type) const = 0;

  /// Initialize iterator corresponding to leading term
  virtual iterator leadIterator(const poly_type&) const = 0;

  /// Find next term (after iter) in polynomial according to current order
  virtual iterator incrementIterator(iterator iter, const poly_type&) const = 0;
};

END_NAMESPACE_PBORI

#endif
