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
 * Revision 1.10  2006/03/27 15:02:43  dreyer
 * ADD: BoolePolynomial::operator/=(const self&) and spoly
 *
 * Revision 1.9  2006/03/27 13:47:58  dreyer
 * ADD operator + and *, CHANGE BoolePolyRing::variable(i) generation
 *
 * Revision 1.8  2006/03/24 16:15:15  dreyer
 * CHANGE: (n)usedVariables() now uses Cudd-internal commands
 * ADD: CDDInterface<> support() and nSupport() (for above)
 *
 * Revision 1.7  2006/03/24 15:02:44  dreyer
 * ADD: Reference to manager_type can also be used for CDDManager<> -nterface
 * ADD: lead(), (n)usedVariables(), lmDeg() implemented in BoolePolynomial
 *
 * Revision 1.6  2006/03/23 17:15:04  dreyer
 * ADD: lead() and lmdeg() functionality to BoolePolynomial,
 * BoolePolyRing(const manager_type &); leading term exampl.
 *
 * Revision 1.5  2006/03/22 08:06:59  dreyer
 * ADD: Template specializations CDDInterface<ZDD>, CDDManager<Cudd>; 
 * ring uses shared_ptr now
 *
 * Revision 1.4  2006/03/20 14:51:01  dreyer
 * CHANGE: Use CDDInterface temple specializations instead of raw dd_type
 *
 * Revision 1.3  2006/03/20 12:11:57  dreyer
 * CHANGE: Revised *deg*() functions.
 *
 * Revision 1.2  2006/03/17 16:53:37  dreyer
 * ADD added nNodes(), operator*= to BoolePolynomial
 *
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
  m_dd( BoolePolyRing::ringEmpty() ) {

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

  m_dd = m_dd.unite( rhs.m_dd ).diff( m_dd.intersect(rhs.m_dd) );
  return *this;
}

// Multiplication
BoolePolynomial&
BoolePolynomial::operator*=(const monom_type& rhs) {

  PBORI_TRACE_FUNC( "BoolePolynomial::operator*=(const monom_type&)" );

  m_dd.unateProductAssign(rhs.m_dd);
  return *this;
}

// Division
BoolePolynomial&
BoolePolynomial::operator/=(const monom_type& rhs) {

  PBORI_TRACE_FUNC( "BoolePolynomial::operator*=(const monom_type&)" );

  m_dd.divideAssign(rhs.m_dd);
  return *this;
}

// Equality
BoolePolynomial::bool_type
BoolePolynomial::operator==(const self& rhs) {

  PBORI_TRACE_FUNC( "BoolePolynomial::operator==(const self&)" );

  return (m_dd == rhs.m_dd);
}

// Nonequality
BoolePolynomial::bool_type
BoolePolynomial::operator!=(const self& rhs) {

  PBORI_TRACE_FUNC( "BoolePolynomial::operator!=(const self&)" );

  return (m_dd != rhs.m_dd);
}

// Leading term
BoolePolynomial::monom_type
BoolePolynomial::lead() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::lead() const" );

  dd_type leadterm = m_dd;
  dd_type nextterm = m_dd;

  manager_reference mgr(m_dd);
  size_type nlen = mgr.nVariables();

  for(idx_type idx = 0; idx < nlen; ++idx){

    nextterm.intersectAssign( mgr.ddVariable(idx) );

    if (nextterm !=  mgr.empty())
      leadterm = nextterm;    
    else
      nextterm = leadterm;
  }

  return leadterm;
}

// Maximal degree of the polynomial
BoolePolynomial::size_type
BoolePolynomial::deg() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::deg() const" );

  PBORI_NOT_IMPLEMENTED;

  return 0;
}


// Degree of the leading term
BoolePolynomial::size_type
BoolePolynomial::lmDeg() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::deg() const" );

  // Equals number of nodes for monomials
  return lead().nNodes();
}


// Total (weighted) maximal degree of the polynomial
BoolePolynomial::size_type
BoolePolynomial::totalDeg() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::totalDeg() const" );

  // No weighted degrees yet, so map to non-weighted variant
  return deg();
}

// Total (weighted) degree of the leading term
BoolePolynomial::size_type
BoolePolynomial::lmTotalDeg() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::lmTotalDeg() const" );

  // No weighted degrees yet, so map to non-weighted variant
  return lmDeg();
}

// Number of nodes in the decision diagram
BoolePolynomial::size_type
BoolePolynomial::nNodes() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::nNodes() const" );

  return m_dd.nNodes();
}

// Number of variables of the polynomial
BoolePolynomial::size_type
BoolePolynomial::nUsedVariables() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::nUsedVariables() const" );

  return m_dd.nSupport();
}

// Set of variables of the polynomial
BoolePolynomial::monom_type
BoolePolynomial::usedVariables() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::usedVariables() const" );

  return m_dd.support();
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

  os << "-> ";
  m_dd.print(os);

  return os;
}

// addition operation 
BoolePolynomial 
operator+(const BoolePolynomial& first, const BoolePolynomial& second){

  PBORI_TRACE_FUNC("operator+(const BoolePolynomial&,const BoolePolynomial&)");

  BoolePolynomial result(first);
  return (result += second);
}

// multiplication with monomial
BoolePolynomial
operator*(const BoolePolynomial& poly, const BoolePolynomial::monom_type& monom) {

  PBORI_TRACE_FUNC("operator*(const BoolePolynomial&,const monom_type&)");

  BoolePolynomial result(poly);
  return (result *= monom);

}

// division by monomial (skipping remainder)
BoolePolynomial
operator/(const BoolePolynomial& poly, const BoolePolynomial::monom_type& monom) {

  PBORI_TRACE_FUNC("operator/(const BoolePolynomial&,const monom_type&)");

  BoolePolynomial result(poly);
  return (result /= monom);

}

/// Compute spoly of two polynomials
BoolePolynomial 
spoly(const BoolePolynomial& first, const BoolePolynomial& second){

  BoolePolynomial lead1(first.lead()), lead2(second.lead());

  BoolePolynomial prod = lead1;
  prod *= lead2;

  return ( first * (prod / lead1) ) + ( second * (prod / lead2) );
}

// Stream output for Boolean polynomials
BoolePolynomial::ostream_type&
operator<<(BoolePolynomial::ostream_type& os, const BoolePolynomial& source) {

  return source.print(os);
}

END_NAMESPACE_PBORI
