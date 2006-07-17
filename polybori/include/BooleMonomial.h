// -*- c++ -*-
//*****************************************************************************
/** @file 
 *
 * @author Alexander Dreyer
 * @date 2006-04-19
 *
 * This file carries the definition of class @c BooleMonomial, which can be used
 * to access the boolean monomials of the currently active Boolean polynomial
 * ring. 
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
 * Revision 1.8  2006/07/17 15:32:08  dreyer
 * ADD: BoolePolynomial::hasConstantPart, BooleMonomial::isOne, isZero
 *
 * Revision 1.7  2006/07/14 09:02:49  dreyer
 * ADD: greater_variable()
 *
 * Revision 1.6  2006/05/27 11:04:25  bricken
 * + optimization remark
 *
 * Revision 1.5  2006/04/24 11:41:56  dreyer
 * FIX only necessary types are traits
 *
 * Revision 1.4  2006/04/20 17:31:05  dreyer
 * FIX removed casting operator, which caused ambigeous overloads
 *
 * Revision 1.3  2006/04/20 16:59:47  dreyer
 * routine
 *
 * Revision 1.2  2006/04/20 08:31:21  dreyer
 * ADD BooleMonomial::mulples(...)
 *
 * Revision 1.1  2006/04/19 15:55:53  dreyer
 * ADD BooleMonomial, BoolePolynomial::fetchTerms() and ::terms()
 *
 * @endverbatim
**/
//*****************************************************************************

#ifndef BooleMonomial_h_
#define BooleMonomial_h_

// include basic definitions
#include "pbori_defs.h"

// get definition of BoolePolynomial and BooleVariable
#include "BoolePolynomial.h"


BEGIN_NAMESPACE_PBORI

class BooleVariable;
/** @class BooleMonomial
 * @brief This class is just a wrapper for using variables from @c cudd's
 * decicion diagram. 
 *
 * @note @c BooleMonomial is actually a specialization of @c BoolePolynomial
 * with a special constructor.
 *
 **/
class BooleMonomial {

 public:
  //-------------------------------------------------------------------------
  // types definitions
  //-------------------------------------------------------------------------

  /// @name Adopt global type definitions
  //@{
  typedef CTypes::dd_type dd_type;
  typedef CTypes::size_type size_type;
  typedef CTypes::idx_type idx_type;
  typedef CTypes::hash_type hash_type;
  typedef CTypes::bool_type bool_type;
  typedef CTypes::comp_type comp_type;
  typedef CTypes::integer_type integer_type;
  typedef CTypes::ostream_type ostream_type;
  //@}

  /// Generic access to current type
  typedef BooleMonomial self;

  /// Type of Boolean polynomials
  typedef BoolePolynomial poly_type;

  /// Type of Boolean variables
  typedef BooleVariable var_type;

  /// Type of sets of Boolean variables
  typedef BooleSet set_type;

  /// Access to iterator type of leading terms
  typedef poly_type::first_iterator const_iterator;

  /// Default Constructor (Constructs monomial one of the active ring)
  BooleMonomial();

  /// Copy constructor
  BooleMonomial(const self&);

  /// Construct from Boolean variable
  BooleMonomial(const var_type&);

  /// Construct from Boolean constant
  BooleMonomial(bool_type);

  /// Destructor
  ~BooleMonomial();

  /// Casting operator
  operator const BoolePolynomial&() const;
 
  /// Casting operator
//  operator const dd_type&() const { return m_poly.diagram(); };

  /// Start iteration over indices
  const_iterator begin() const { return m_poly.firstBegin(); }

  /// Finish iteration over indices
  const_iterator end() const { return m_poly.firstEnd(); }

  /// Degree of the monomial
  size_type deg() const {
    ///@todo optimal, if stored, else much too complicated, as it will probably use cache lookups
    return m_poly.nNodes(); }

  /// Divisors of the monomial
  set_type divisors() const { return m_poly.lmDivisors(); }

  /// multiples of the monomial wrt. given monomial
  set_type multiples(const self&) const; 

  /// Hash value of the monomial
  hash_type hash() const { return m_poly.lmHash(); }

  /// Substitute variable with index idx by its complement and assign
  self& changeAssign(idx_type);

  /// Substitute variable with index idx by its complement
  self change(idx_type) const;


  /// @name Arithmetical operations
  //@{
  self& operator*=(const self&);
  self& operator/=(const self&);
  self& operator*=(const var_type&);
  self& operator/=(const var_type&);
  //@}

  /// @name Logical operations
  //@{
  bool_type operator==(const self& rhs) const { return m_poly == rhs.m_poly; }
  bool_type operator!=(const self& rhs) const { return m_poly != rhs.m_poly; }
  bool_type isZero() const { return m_poly.isZero(); }
  bool_type isOne() const { return m_poly.isOne(); }
  //@}

  /// Test for reducibility
  bool_type reducibleBy(const self& rhs) const { 
    return m_poly.reducibleBy(rhs); }
  bool_type reducibleBy(const var_type& rhs) const;

  /// Compare with rhs monomial and return comparision code
  comp_type compare(const self&) const;

  /// Degree of the least common multiple
  size_type LCMDeg(const self&) const;

  /// Compute theleast common multiple and assign
  self& LCMAssign(const self&);

  /// Compute the greatest common divisor
  self LCM(const self&) const;

  /// Compute the greatest common divisor and assign
  self& GCDAssign(const self&);

  /// Compute the greatest common divisor
  self GCD(const self&) const;
 
  /// Read-only access to internal decision diagramm structure
  const dd_type& diagram() const { return m_poly.diagram(); }

#ifndef PBORI_DEVELOPER
protected:
#endif

  /// Access to internal decision diagramm structure
  dd_type& diagram() { return m_poly.diagram(); }

private:
  BoolePolynomial m_poly;
};

/// Multiplication of monomials
template <class RHSType>
inline BooleMonomial
operator*(const BooleMonomial& lhs, const RHSType& rhs) {
  return BooleMonomial(lhs) *= rhs;
}

/// Division of monomials
template <class RHSType>
inline BooleMonomial
operator/(const BooleMonomial& lhs, const RHSType& rhs) {
  return BooleMonomial(lhs) /= rhs;
}


/// Less than comparision
inline BooleMonomial::bool_type
operator<(const BooleMonomial& lhs, const BooleMonomial& rhs) {

  return (lhs.compare(rhs) == CTypes::less_than);
};

/// Greater than comparision
inline BooleMonomial::bool_type
operator>(const BooleMonomial& lhs, const BooleMonomial& rhs) {

  return (lhs.compare(rhs) == CTypes::greater_than);
};

/// Less or equal than comparision
inline BooleMonomial::bool_type
operator<=(const BooleMonomial& lhs, const BooleMonomial& rhs) {

  return (lhs.compare(rhs) <= CTypes::less_or_equal_max);
};

/// Greater or equal than comparision
inline BooleMonomial::bool_type
operator>=(const BooleMonomial& lhs, const BooleMonomial& rhs) {

  return (lhs.compare(rhs) >= CTypes::greater_or_equal_min);
};


/// Compute the greatest common divisor of two monomials
inline BooleMonomial
GCD(const BooleMonomial& lhs, const BooleMonomial& rhs ){

  return lhs.GCD(rhs);
}

/// Compute the greatest common divisor of two monomials
inline BooleMonomial
LCM(const BooleMonomial& lhs, const BooleMonomial& rhs ){

  return lhs.LCM(rhs);
}

/// @function greater_variable
/// @brief Checks whether BooleVariable(lhs) > BooleVariable(rhs) 
BooleMonomial::bool_type
greater_variable(BooleMonomial::idx_type lhs, BooleMonomial::idx_type rhs);

END_NAMESPACE_PBORI

#endif // of BooleMonomial_h_
