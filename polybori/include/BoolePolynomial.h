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

// include basic definitions
#include "CDDInterface.h"

BEGIN_NAMESPACE_PBORI

/** @class BoolePolynomial
 * @brief This class wraps the underlying decicion diagram type and defines the
 * necessary operations.
 *
 **/
class BoolePolynomial {

public:
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
  typedef self monom_type; 

  //-------------------------------------------------------------------------
  // constructors and destructor
  //-------------------------------------------------------------------------

  /// Default constructor
  BoolePolynomial();

  /// Construct polynomial from a constant value 0 or 1
  BoolePolynomial(bool_type);

  /// Construct polynomial from decision diagram
  BoolePolynomial(const dd_type&);

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
  self& operator*=(const monom_type&);
  self& operator/=(const monom_type&);
  //@}

  /// @name Logical operations
  //@{
  bool_type operator==(const self&) const;
  bool_type operator!=(const self&) const;
  bool_type operator==(bool_type) const;
  bool_type operator!=(bool_type) const;
  bool_type isZero() const;
  bool_type isOne() const;
  bool_type reducibleBy(const self&) const;
  //@}

  /// Get leading term
  monom_type lead() const;

  /// Get all divisors of the leading term
  monom_type lmDivisors() const;
  
  /// hash value of the leading term
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

  /// Print current polynomial to cout
  /// @todo Cudd provides only cout functionality, iostream needed.
  ostream_type& print(ostream_type&) const;

  /// Pretty print to stdout
  void prettyPrint() const;

  /// Pretty print to filename
  void prettyPrint(const char* filename) const;

  /// Start of leading term
  first_iterator firstBegin() const;

  /// Finish of leading term 
  first_iterator firstEnd() const;

  /// Navigate through structure
  navigator navigation() const;
 
  /// gives a copy of the diagram
  dd_type copyDiagram(){
    return diagram();
  }
  int eliminationLength() const;
#ifndef PBORI_DEVELOPER
protected:
#endif

  /// @name Access to internal decision diagramm structure
  //@{
  dd_type& diagram();
  const dd_type& diagram() const;
  //@}

private:
  /// The actual decision diagramm
  dd_type m_dd;
};


/// Addition operation 
BoolePolynomial 
operator+(const BoolePolynomial&, const BoolePolynomial&);

/// Multiplication with monomial
BoolePolynomial
operator*(const BoolePolynomial&, const BoolePolynomial::monom_type&);


/// Division by monomial (skipping remainder)
BoolePolynomial
operator/(const BoolePolynomial&, const BoolePolynomial::monom_type&);

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
