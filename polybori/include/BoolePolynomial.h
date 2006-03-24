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
  typedef CTypes::ostream_type ostream_type;
  //@}

  /// @todo A more sophisticated treatment for monomials is needed.

  /// Fix type for treatment of monomials
  typedef self monom_type; 

  //-------------------------------------------------------------------------
  // constructors and destructor
  //-------------------------------------------------------------------------

  /// Default constructor
  BoolePolynomial();

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
  //@}

  /// Get leading term
  monom_type lead() const;

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

  /// Print current polynomial to cout
  /// @todo Cudd provides only cout functionality, iostream needed.
  ostream_type& print(ostream_type&) const;

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


///  Addition operation 
BoolePolynomial 
operator+(const BoolePolynomial&, const BoolePolynomial&);

///  Multiplication with monomial
BoolePolynomial
operator*(const BoolePolynomial&, const BoolePolynomial::monom_type&);

/// Compute spoly of two polynomials
BoolePolynomial 
spoly(const BoolePolynomial&, const BoolePolynomial&);


/// Stream output operator
BoolePolynomial::ostream_type& 
operator<<(BoolePolynomial::ostream_type&, const BoolePolynomial&);

END_NAMESPACE_PBORI

#endif // of BoolePolynomial_h_
