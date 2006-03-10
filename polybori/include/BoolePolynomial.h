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
 * Revision 1.1  2006/03/10 15:14:24  dreyer
 * + Initial Version
 *
 * @endverbatim
**/
//*****************************************************************************

#ifndef BoolePolynomial_h_
#define BoolePolynomial_h_

// include basic definitions
#include "pbori_defs.h"

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
  typedef BoolePolyRing self;

  /// @name Adopt global type definitions
  //@{
  typedef CTypes::dd_type dd_type;
  typedef CTypes::ddvector_type ddvector_type;
  typedef CTypes::size_type size_type;
  typedef CTypes::idx_type idx_type;
  //@}

  /// @todo A more sophisticated treatment for monomials is needed.

  /// Fix type for treatment of monomials
  typedef self monom_type; 

  /// @todo A more sophisticated treatment for leading terms is needed.

  /// Fix type of leading terms
  typedef self lead_reference;

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
  self operator+(const self&);
  self& operator*=(const monom_type&);
  self operator*(const monom_type&);
  //@}

  /// Get leading term
  lead_reference lead() const;

  /// Compute spoly of two polynomials
  self spoly(const self&) const;

  /// Total degree of the polynomial
  size_type totalDegree() const;

  /// Degree of the leading term
  size_type deg() const;

  /// Number of nodes in the decision diagram
  size_type nNodes() const;

  /// Number of variables of the polynomial
  size_type nVariables() const;

  /// Set of variables of the polynomial
  ddvector_type variables() const;

protected:
  /// @name Access to internal decision diagramm structure
  //@{
  dd_type& diagram();
  const dd_type& diagram() const;
  //@}

private:
  /// The actual decision diagramm
  dd_type m_dd;
};


END_NAMESPACE_PBORI

#endif // of BoolePolynomial_h_
