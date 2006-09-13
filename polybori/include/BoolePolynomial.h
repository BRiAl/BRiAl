// -*- c++ -*-
//*****************************************************************************
/** @file 
 *
 * @author Alexander Dreyer
 * @date 2006-03-10
 *
 * This file carries the definition of class @c BoolePolynomial, which can be
 * used to access the boolean polynomials with respect to the polynomial ring,
 * which was active on initialization time.
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
 * Revision 1.43  2006/09/13 09:05:43  dreyer
 * ADD: dp_asc/DegRevLexAscOrder
 * ADD: BoolePolynomial::endOfNavigation()
 * CHANGE: BoolePolynomial: removed biDegBegin(), biDegEnd(), which can be
 *   generated more generically using navigation() and endOfNavigation().
 *
 * Revision 1.42  2006/09/12 14:56:55  dreyer
 * ADD bidirectional term iterator template
 *
 * Revision 1.41  2006/09/08 16:15:27  dreyer
 * ADD: Added ordering-dependent term iteration
 *
 * Revision 1.40  2006/09/04 11:33:57  dreyer
 * CHANGE: lm*() use leadFirst()
 *
 * Revision 1.39  2006/09/01 10:35:25  dreyer
 * ADD: Multiplication poly * poly, poly * exponent
 *
 * Revision 1.38  2006/08/29 10:37:56  dreyer
 * CHANGE: non-const version of diagram() now internalDiagram()
 *
 * Revision 1.37  2006/08/29 09:02:36  dreyer
 * ADD: leadExp()
 *
 * Revision 1.36  2006/08/28 07:25:07  dreyer
 * CHANGE: BooleExponent nomenclatur
 *
 * Revision 1.35  2006/08/24 14:47:49  dreyer
 * ADD: BooleExponent integrated, FIX: multiples (for indices < first)
 *
 * Revision 1.34  2006/07/17 15:32:08  dreyer
 * ADD: BoolePolynomial::hasConstantPart, BooleMonomial::isOne, isZero
 *
 * Revision 1.33  2006/07/06 16:01:29  dreyer
 * CHANGE: Functionals ins pbori_func.h made more consistent
 *
 * Revision 1.32  2006/05/24 08:01:01  dreyer
 * ADD operator % and %=
 *
 * Revision 1.31  2006/05/24 06:38:14  bricken
 * + corrected type for usedVariables
 *
 * Revision 1.30  2006/05/23 09:40:39  dreyer
 * ADD operators - and -=
 *
 * Revision 1.29  2006/05/03 10:37:03  bricken
 * + work on pseudo parallel reductions
 *
 * Revision 1.28  2006/04/25 09:30:42  dreyer
 * FIX end of CTermIterm for constants, CHANGE consistent functional names
 *
 * Revision 1.27  2006/04/24 14:45:35  dreyer
 * FIX CTermIter; ADD BoolePolynomial uses CTermIter
 *
 * Revision 1.26  2006/04/24 10:23:22  dreyer
 * ADD BoolePolynomial::begin() and end()
 * FIX type reference in CCuddNavigator
 *
 * Revision 1.25  2006/04/20 16:59:47  dreyer
 * routine
 *
 * Revision 1.24  2006/04/20 08:31:21  dreyer
 * ADD BooleMonomial::mulples(...)
 *
 * Revision 1.23  2006/04/19 15:55:53  dreyer
 * ADD BooleMonomial, BoolePolynomial::fetchTerms() and ::terms()
 *
 * Revision 1.22  2006/04/14 15:45:30  bricken
 * + removed semicolons
 *
 * Revision 1.21  2006/04/12 10:33:01  bricken
 * *bricken: elimination length
 *
 * Revision 1.20  2006/04/06 14:10:58  dreyer
 * ADD hash_type and CCuddNavigator::hash()
 *
 * Revision 1.19  2006/04/06 13:54:58  dreyer
 * ADD BoolePolynomial::length()
 *
 * Revision 1.18  2006/04/05 15:26:03  dreyer
 * CHANGE: File access of BoolePolynomial::prettyPrint moved to CDDInterface
 *
 * Revision 1.17  2006/04/05 14:56:38  dreyer
 * ADD prettyPrint functions for dot-output to stdout or file
 *
 * Revision 1.16  2006/04/04 15:31:06  dreyer
 * ADD: BoolePolynomial::navigator() and corresp. class CCuddNavigator
 *
 * Revision 1.15  2006/04/04 13:01:28  bricken
 * + copyDiagram
 *
 * Revision 1.14  2006/04/04 12:07:37  dreyer
 * ADD BoolePolynomial::reducibleby(), and firstBegin(), firstEnd()
 *
 * Revision 1.13  2006/04/04 11:59:54  bricken
 * + hash function
 *
 * Revision 1.12  2006/04/04 11:21:22  dreyer
 * ADD lmDivisors() added
 *
 * Revision 1.11  2006/04/04 07:36:35  dreyer
 * ADD: tests isZero(), isOne() and poly == bool, bool == poly
 *
 * Revision 1.10  2006/03/30 11:57:11  dreyer
 * CHANGE: Made use of 0/1 constants and the sets {}, {{}} consistent
 *
 * Revision 1.9  2006/03/27 15:02:43  dreyer
 * ADD: BoolePolynomial::operator/=(const self&) and spoly
 *
 * Revision 1.8  2006/03/27 13:47:58  dreyer
 * ADD operator + and *, CHANGE BoolePolyRing::variable(i) generation
 *
 * Revision 1.7  2006/03/24 15:02:44  dreyer
 * ADD: Reference to manager_type can also be used for CDDManager<> -nterface
 * ADD: lead(), (n)usedVariables(), lmDeg() implemented in BoolePolynomial
 *
 * Revision 1.6  2006/03/23 17:15:04  dreyer
 * ADD: lead() and lmdeg() functionality to BoolePolynomial,
 * BoolePolyRing(const manager_type &); leading term exampl.
 *
 * Revision 1.5  2006/03/20 14:51:00  dreyer
 * CHANGE: Use CDDInterface temple specializations instead of raw dd_type
 *
 * Revision 1.4  2006/03/20 12:11:57  dreyer
 * CHANGE: Revised *deg*() functions.
 *
 * Revision 1.3  2006/03/16 17:09:12  dreyer
 * ADD BoolePolynial functionality started
 *
 * Revision 1.2  2006/03/13 12:27:24  dreyer
 * CHANGE: consistent function names
 *
 * Revision 1.1  2006/03/10 15:14:24  dreyer
 * + Initial Version
 *
 * @endverbatim
**/
//*****************************************************************************

#ifndef BoolePolynomial_h_
#define BoolePolynomial_h_

// include standard definitions
#include <vector>

// include basic definitions and decision diagram interface
#include "CDDInterface.h"

// include definition of sets of Boolean variables
#include "CTermIter.h"
#include "CBidirectTermIter.h"

#include "pbori_func.h"
#include "BooleSet.h"


BEGIN_NAMESPACE_PBORI

class BooleMonomial;
class BooleExponent;
class COrderedIter;

/** @class BoolePolynomial
 * @brief This class wraps the underlying decicion diagram type and defines the
 * necessary operations.
 *
 **/
class BoolePolynomial {

public:

  /// Let BooleMonomial access protected and private members
  friend class BooleMonomial;

  //-------------------------------------------------------------------------
  // types definitions
  //-------------------------------------------------------------------------

  /// Generic access to current type
  typedef BoolePolynomial self;

  /// @name Adopt global type definitions
  //@{
  typedef CTypes::manager_type manager_type;
  typedef CTypes::manager_reference manager_reference;
  typedef CTypes::manager_ptr manager_ptr;
  typedef CTypes::dd_type dd_type;
  typedef CTypes::size_type size_type;
  typedef CTypes::idx_type idx_type;
  typedef CTypes::bool_type bool_type;
  typedef CTypes::ostream_type ostream_type;
  typedef CTypes::hash_type hash_type;
  //@}

  /// Iterator type for iterating over indices of the leading term
  typedef dd_type::first_iterator first_iterator;

  /// Iterator-like type for navigating through diagram structure
  typedef dd_type::navigator navigator;

  /// Type for output of pretty print
  typedef dd_type::pretty_out_type pretty_out_type;

  /// Type for naming file for pretty print
  typedef dd_type::filename_type filename_type;

  /// @todo A more sophisticated treatment for monomials is needed.

  /// Fix type for treatment of monomials
  typedef BooleMonomial monom_type; 

  /// Fix type for treatment of exponent vectors
  typedef BooleExponent exp_type; 

  /// Iterator type for iterating over all monomials in ordering order
  typedef COrderedIter ordered_iterator;

  /// Iterator type for iterating over all monomials
  typedef CTermIter<monom_type, navigator, 
                    change_assign<>, 
                    change_assign<> >
  const_iterator;

  /// Iterator type for iterating all exponent vectors 
  typedef CTermIter<exp_type, navigator, 
                    inserts<>, 
                    removes<>, project_ith<1> >
  exp_iterator;

  /// Incrementation functional type
  typedef 
  binary_compose< std::plus<size_type>, 
                  project_ith<1>, integral_constant<size_type, 1> > 
  increment_type;

  /// Decrementation functional type
  typedef 
  binary_compose< std::minus<size_type>, 
                  project_ith<1>, integral_constant<size_type, 1> > 
  decrement_type;

  /// Iterator type for iterating all monomials (dereferencing to degree)
  typedef CTermIter<size_type, navigator, 
                    increment_type, decrement_type,
                    integral_constant<size_type, 0> >
  deg_iterator;

  /// Bidirectional iterator type (dereferencing to degree)
  typedef CBidirectTermIter<size_type, navigator, 
                            increment_type, decrement_type,
                            integral_constant<size_type, 0> >
  bidirectional_iterator;

  /// Type for lists of terms
  typedef std::vector<monom_type> termlist_type;

  /// Type for sets of Boolean variables
  typedef BooleSet set_type;

  //-------------------------------------------------------------------------
  // constructors and destructor
  //-------------------------------------------------------------------------

  /// Default constructor
  BoolePolynomial();

  /// Construct polynomial from a constant value 0 or 1
  BoolePolynomial(bool_type);

  /// Construct polynomial from decision diagram
  BoolePolynomial(const dd_type&);

  /// Construct polynomial from exponent vector
  BoolePolynomial(const exp_type&);

  /// Copy constructor
  BoolePolynomial(const self&);

  /// Destructor
  ~BoolePolynomial();

  //-------------------------------------------------------------------------
  // operators and member functions
  //-------------------------------------------------------------------------

  /// @name Arithmetical operations
  //@{
  self& operator+=(const self&);
  self& operator-=(const self& rhs) { return operator+=(rhs); }
  self& operator*=(const monom_type&);
  self& operator*=(const exp_type&);
  self& operator*=(const self&);
  self& operator/=(const monom_type&);
  self& operator%=(const monom_type&);
  //@}

  /// @name Logical operations
  //@{
  bool_type operator==(const self&) const;
  bool_type operator!=(const self&) const;
  bool_type operator==(bool_type) const;
  bool_type operator!=(bool_type) const;
  bool_type isZero() const;
  bool_type isOne() const;
  bool_type hasConstantPart() const;
  bool_type reducibleBy(const self&) const;
  //@}

  /// Get leading term
  monom_type lead() const;

  /// Get leading term
  exp_type leadExp() const;

  /// Get all divisors of the leading term
  set_type lmDivisors() const { return leadFirst().firstDivisors(); };
  
  /// Hash value of the leading term
  hash_type lmHash() const;
  
  /// Maximal degree of the polynomial
  size_type deg() const;

  /// Degree of the leading term
  size_type lmDeg() const;

  /// Total maximal degree of the polynomial
  size_type totalDeg() const;

  /// Total degree of the leading term
  size_type lmTotalDeg() const;

  /// Number of nodes in the decision diagram
  size_type nNodes() const;

  /// Number of variables of the polynomial
  size_type nUsedVariables() const;

  /// Set of variables of the polynomial
  monom_type usedVariables() const;

  /// Returns number of terms
  size_type length() const;

  /// Print current polynomial to output stream
  ostream_type& print(ostream_type&) const;

  /// Pretty print to stdout
  void prettyPrint() const;

  /// Pretty print to filename
  void prettyPrint(const char* filename) const;

  /// Start of iteration over monomials
  const_iterator begin() const;

  /// Finish of iteration over monomials
  const_iterator end() const;

  /// Start of iteration over exponent vectors
  exp_iterator expBegin() const;

  /// Finish of iteration over exponent vectors
  exp_iterator expEnd() const;

  /// Start of first term
  first_iterator firstBegin() const;

  /// Finish of first term 
  first_iterator firstEnd() const;

  /// Start of degrees
  deg_iterator degBegin() const;

  /// Finish of degrees
  deg_iterator degEnd() const;

  /// Start of ordering respecting iterator
  ordered_iterator orderedBegin() const; 

  /// Finish of ordering respecting iterator
  ordered_iterator orderedEnd() const;

  /// Navigate through structure
  navigator navigation() const;
 
  /// End of navigation marker
  navigator endOfNavigation() const;
  
  /// gives a copy of the diagram
  dd_type copyDiagram(){   return diagram();  }

  /// Casting operator
  operator const dd_type&() const { return diagram(); };

  size_type eliminationLength() const;
  size_type eliminationLengthWithDegBound(size_type garantied_deg_bound) const;
  /// Get list of all terms
  void fetchTerms(termlist_type&) const;

  /// Return of all terms
  termlist_type terms() const;

  /// Read-only access to internal decision diagramm structure
  const dd_type& diagram() const { return m_dd; }

protected:

  /// Access to internal decision diagramm structure
  dd_type& internalDiagram() { return m_dd; }

  /// Generate a polynomial, whose first term is the leading term
  self leadFirst() const;

  /// Get all divisors of the first term
  set_type firstDivisors() const;

private:
  /// The actual decision diagramm
  dd_type m_dd;
};


/// Addition operation 
inline BoolePolynomial 
operator+(const BoolePolynomial& lhs, const BoolePolynomial& rhs) {

   BoolePolynomial result(lhs);
   result += rhs;

   return result;
}

/// Subtraction operation 
inline BoolePolynomial 
operator-(const BoolePolynomial& lhs, const BoolePolynomial& rhs) {

  return BoolePolynomial(lhs) -= rhs;
}

/// Multiplication with monomial
inline BoolePolynomial
operator*(const BoolePolynomial& lhs, const BoolePolynomial::monom_type& rhs){

  return BoolePolynomial(lhs) *= rhs;
}

/// Multiplication of monomials by a polynomial
inline BoolePolynomial
operator*(const BoolePolynomial::monom_type& lhs, 
          const BoolePolynomial& rhs){

  return BoolePolynomial(rhs) *= lhs;
}

/// Multiplication with monomial
inline BoolePolynomial
operator*(const BoolePolynomial& lhs, const BoolePolynomial::exp_type& rhs){

  return BoolePolynomial(lhs) *= rhs;
}

/// Multiplication of monomials by a polynomial
inline BoolePolynomial
operator*(const BoolePolynomial::exp_type& lhs, 
          const BoolePolynomial& rhs){

  return BoolePolynomial(rhs) *= lhs;
}

/// Multiplication with monomial
inline BoolePolynomial
operator*(const BoolePolynomial& lhs, const BoolePolynomial& rhs){

  return BoolePolynomial(lhs) *= rhs;
}

/// Division by monomial (skipping remainder)
inline BoolePolynomial
operator/(const BoolePolynomial& lhs, const BoolePolynomial::monom_type& rhs){

  return BoolePolynomial(lhs) /= rhs;
}

/// Modulus monomial (division remainder)
inline BoolePolynomial
operator%(const BoolePolynomial& lhs, const BoolePolynomial::monom_type& rhs){

  return BoolePolynomial(lhs) %= rhs;
}

/// Compute spoly of two polynomials
BoolePolynomial 
spoly(const BoolePolynomial&, const BoolePolynomial&);

/// Equality check (with constant lhs)
inline BoolePolynomial::bool_type
operator==(BoolePolynomial::bool_type lhs, const BoolePolynomial& rhs) {

  return (rhs == lhs); 
}

/// Nonquality check (with constant lhs)
inline BoolePolynomial::bool_type
operator!=(BoolePolynomial::bool_type lhs, const BoolePolynomial& rhs) {

  return (rhs != lhs); 
}

/// Stream output operator
BoolePolynomial::ostream_type& 
operator<<(BoolePolynomial::ostream_type&, const BoolePolynomial&);

END_NAMESPACE_PBORI

#endif // of BoolePolynomial_h_
