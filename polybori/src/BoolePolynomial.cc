// -*- c++ -*-
//*****************************************************************************
/** @file 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file implements the class BoolePolymials, where carries the definition
 * of polynomials over Booleans.
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
 * Revision 1.1  2006/03/16 17:09:13  dreyer
 * ADD BoolePolynial functionality started
 *
 * @endverbatim
**/
//*****************************************************************************


// load header file
# include "BoolePolynomial.h"

// get polynomial riung definition
# include "BoolePolyRing.h"

// get error types
# include "PBoRiError.h"

BEGIN_NAMESPACE_PBORI

//-------------------------------------------------------------------------
// Constructors and destructor
//------------------------------------------------------------------------

// Default constructor
BoolePolynomial::BoolePolynomial():
  m_dd()  {

  PBORI_TRACE_FUNC( "BoolePolynomial()" );
}

// Constructor polynomial from existing decision diagram
BoolePolynomial::BoolePolynomial(const dd_type& rhs):
  m_dd(rhs)  {

  PBORI_TRACE_FUNC( "BoolePolynomial(const dd_type&)" );
}

// Copy constructor
BoolePolynomial::BoolePolynomial(const self& rhs) :
  m_dd(rhs.m_dd) {
  
  PBORI_TRACE_FUNC( "BoolePolynomial(const self&)" );
}

// Destructor
BoolePolynomial::~BoolePolynomial() {

  PBORI_TRACE_FUNC( "~BoolePolynomial()" );

}

//-------------------------------------------------------------------------
// operators and member functions
//------------------------------------------------------------------------


// Addition
BoolePolynomial&
BoolePolynomial::operator+=(const self& rhs) {

  PBORI_TRACE_FUNC( "BoolePolynomial::operator+=(const self&)" );

  // dd_type tmp = m_dd.Union( rhs.m_dd);
  m_dd = m_dd.Union( rhs.m_dd ).Diff( m_dd.Intersect(rhs.m_dd) );

  return *this;
}

// Multiplication
BoolePolynomial&
BoolePolynomial::operator*=(const monom_type& rhs) {

  PBORI_TRACE_FUNC( "BoolePolynomial::operator*=(const monom_type&)" );

  PBORI_NOT_IMPLEMENTED;

  return *this;
}

// Leading term
BoolePolynomial::monom_type
BoolePolynomial::lead() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::lead() const" );

  PBORI_NOT_IMPLEMENTED;

  return monom_type();
}

// Total degree of the polynomial
BoolePolynomial::size_type
BoolePolynomial::totalDegree() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::totalDegree() const" );

  PBORI_NOT_IMPLEMENTED;

  return 0;
}

// Degree of the leading term
BoolePolynomial::size_type
BoolePolynomial::deg() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::deg() const" );

  PBORI_NOT_IMPLEMENTED;

  return 0;
}

// Number of nodes in the decision diagram
BoolePolynomial::size_type
BoolePolynomial::nNodes() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::nNodes() const" );

  PBORI_NOT_IMPLEMENTED;

  return 0;
}

// Number of variables of the polynomial
BoolePolynomial::size_type
BoolePolynomial::nUsedVariables() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::nUsedVariables() const" );

  PBORI_NOT_IMPLEMENTED;

  return 0;
}

// Set of variables of the polynomial
BoolePolynomial::ddvector_type
BoolePolynomial::usedVariables() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::usedVariables() const" );

  PBORI_NOT_IMPLEMENTED;

  return ddvector_type(0, NULL, NULL);
}

// Access to internal decision diagramm structure
BoolePolynomial::dd_type&
BoolePolynomial::diagram() {

  PBORI_TRACE_FUNC( "BoolePolynomial::diagram()" );

  return m_dd;
}

// Access to internal decision diagramm structure
const BoolePolynomial::dd_type&
BoolePolynomial::diagram() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::diagram() const" );

  return m_dd;
}
// Print current polynomial to cout
BoolePolynomial::ostream_type&
BoolePolynomial::print(ostream_type& os) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::print() const" );

  ///  @todo: add std::cout capability for cudd's ZDD type

  os << '(';
  m_dd.print(BoolePolyRing::nRingVars());
  m_dd.PrintMinterm();
  os << ')';
  return os;
}

// Stream output for Boolean polynomials
BoolePolynomial::ostream_type&
operator<<(BoolePolynomial::ostream_type& os, const BoolePolynomial& source) {

  return source.print(os);
}

END_NAMESPACE_PBORI
