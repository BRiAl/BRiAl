// -*- c++ -*-
//*****************************************************************************
/** @file BooleMonomial.cc
 *
 * @author Alexander Dreyer
 * @date 2006-04-19
 *
 * This file implements the class BoolePolymials, where carries the definition
 * of monomials over Booleans.
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
 * Revision 1.9  2006/08/29 10:37:56  dreyer
 * CHANGE: non-const version of diagram() now internalDiagram()
 *
 * Revision 1.8  2006/08/24 14:47:50  dreyer
 * ADD: BooleExponent integrated, FIX: multiples (for indices < first)
 *
 * Revision 1.7  2006/07/31 11:48:53  dreyer
 * ADD: lowlevel implementation for multiples and lmDivisors
 *
 * Revision 1.6  2006/07/21 08:07:27  bricken
 * + work on redTail
 *
 * Revision 1.5  2006/07/14 09:02:49  dreyer
 * ADD: greater_variable()
 *
 * Revision 1.4  2006/05/23 15:26:25  dreyer
 * CHANGE BoolePolyRing  can handle different orderings (only lex yet)
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


// load header files
# include "BooleMonomial.h"
# include "BooleVariable.h"
# include "BooleExponent.h"

// get ring definitions
# include "BoolePolyRing.h"
# include "OrderedManager.h"

BEGIN_NAMESPACE_PBORI

//-------------------------------------------------------------------------
// Constructors and destructor
//------------------------------------------------------------------------

// Default constructor
BooleMonomial::BooleMonomial():
  m_poly( BoolePolyRing::ringOne() )  {

  PBORI_TRACE_FUNC( "BooleMonomial()" );

}

// Copy constructor
BooleMonomial::BooleMonomial(const self& rhs) :
  m_poly(rhs.m_poly) {
  
  PBORI_TRACE_FUNC( "BooleMonomial(const self&)" );
}

//  Construct from Boolean variable
BooleMonomial::BooleMonomial(const var_type& rhs) :
  m_poly(rhs) {
  
  PBORI_TRACE_FUNC( "BooleMonomial(const var_type&)" );
}

//  Construct from Exponent vector
BooleMonomial::BooleMonomial(const exp_type& rhs) :
  m_poly(rhs) {
  
  PBORI_TRACE_FUNC( "BooleMonomial(const exp_type&)" );

}

//  Construct from Boolean constant
BooleMonomial::BooleMonomial(bool_type val) :
  m_poly(val) {
  
  PBORI_TRACE_FUNC( "BooleMonomial(bool_type)" );
}

// Destructor
BooleMonomial::~BooleMonomial() {

  PBORI_TRACE_FUNC( "~BooleMonomial()" );
}


// Reducibility test
BooleMonomial::bool_type
BooleMonomial::reducibleBy(const var_type& rhs) const {

  PBORI_TRACE_FUNC( "BooleMonomial::reducibleBy(const var_type&) const" );

  return m_poly.reducibleBy(rhs);
}


// Casting operator
BooleMonomial::operator const BoolePolynomial&() const {

  PBORI_TRACE_FUNC( "BooleMonomial::operator const BoolePolynomial&() const" );

  return m_poly;
}

//  Substitute variable with index idx by its complement and assign
BooleMonomial&
BooleMonomial::changeAssign(idx_type idx) {

  PBORI_TRACE_FUNC( "BooleMonomial::changeAssign(idx_type)" );

  m_poly.internalDiagram().changeAssign(idx);

  return *this;
}

//  Substitute variable with index idx by its complement
BooleMonomial
BooleMonomial::change(idx_type idx) const {

  PBORI_TRACE_FUNC( "BooleMonomial::change(idx_type) const " );

  return self(*this).changeAssign(idx);
}

//  Multiplication with assignment
BooleMonomial&
BooleMonomial::operator*=(const self& rhs) {

  PBORI_TRACE_FUNC( "BooleMonomial::operator*=(const self&)" );

  m_poly.internalDiagram().unateProductAssign(rhs.m_poly.diagram());

  return *this;
}

//  Division with assignment
BooleMonomial&
BooleMonomial::operator/=(const self& rhs) {

  PBORI_TRACE_FUNC( "BooleMonomial::operator*=(const self&)" );

  m_poly /= rhs;//.m_poly;

  return *this;
}

//  Multiplication with assignment (with variable)
BooleMonomial&
BooleMonomial::operator*=(const var_type& rhs) {

  PBORI_TRACE_FUNC( "BooleMonomial::operator*=(const var_type&)" );

  if (!reducibleBy(rhs))
    changeAssign(rhs.index());

  return *this;
}

// Division with assignment (with variable)
BooleMonomial&
BooleMonomial::operator/=(const var_type& rhs) {

  PBORI_TRACE_FUNC( "BooleMonomial::operator/=(const var_type&)" );

  m_poly.internalDiagram().subset1Assign(rhs.index());

  return *this;
}
// Comparision
BooleMonomial::comp_type
BooleMonomial::compare(const self& rhs) const {

  PBORI_TRACE_FUNC( "BooleMonomial::compare(const self& rhs) const" );

  return BoolePolyRing::activeManager().compare(*this, rhs);

//   /// @todo Up to now, this is for lexicographic order only.
//   if (*this == rhs)
//     return CTypes::equality;

//   const_iterator start(begin()), finish(end()),
//     rhs_start(rhs.begin()), rhs_finish(rhs.end());

//   while ( (start != finish) && (rhs_start != rhs_finish) && 
//           (*start == *rhs_start) ) {
//     ++start; ++rhs_start;
//   }

//   if (start == finish)
//     return CTypes::less_than;

//   if (rhs_start == rhs_finish)
//     return CTypes::greater_than;

//   return (*start < *rhs_start?  CTypes::greater_than : CTypes::less_than);
}

// Degree of the lcm
BooleMonomial::size_type
BooleMonomial::LCMDeg(const self& rhs) const {

  PBORI_TRACE_FUNC( "BooleMonomial::LCMDeg(const self&) const" );

  if ( m_poly.isZero() || ( (const BoolePolynomial&)rhs).isZero() )
    return 0;

  if ( (*this == rhs) || ( (const BoolePolynomial&)rhs).isOne() )
    return deg();
  
  if ( m_poly.isOne() )
    return rhs.deg();

  size_type result = 0;
  const_iterator start(begin()), finish(end()),
    rhs_start(rhs.begin()), rhs_finish(rhs.end());

  while ( (start != finish) && (rhs_start != rhs_finish) ) {

    idx_type last_idx(*start);

    if (last_idx <= *rhs_start) 
      ++start;

    if (last_idx >= *rhs_start) 
      ++rhs_start;

    ++result;
  }

  while (start != finish) {
    ++start;
    ++result;
  }

  while (rhs_start != rhs_finish) {
    ++rhs_start;
    ++result;
  }

  return result;
}

// gcd
BooleMonomial
BooleMonomial::GCD(const self& rhs) const {

  PBORI_TRACE_FUNC( "BooleMonomial::GCD(const self&) const" );
  return self(*this).GCDAssign(rhs);
}

// gcd with assignment
BooleMonomial&
BooleMonomial::GCDAssign(const self& rhs) {

  PBORI_TRACE_FUNC( "BooleMonomial::GCDAssign(const self&)" );

  if ((*this == rhs) || m_poly.isZero() || m_poly.isOne() )
    return *this;

  if ( ( (const BoolePolynomial&)rhs).isOne() || 
       ( (const BoolePolynomial&)rhs).isZero() )
    return (*this = rhs);

  const_iterator start(begin()), finish(end()),
    rhs_start(rhs.begin()), rhs_finish(rhs.end());

  while ( (start != finish) && (rhs_start != rhs_finish) ) {

    // delete variable not on rhs
    if (*start < *rhs_start) {
      changeAssign(*start);
      ++start;
    }
    else {
      // keep common variables
      if (*start == *rhs_start) 
        ++start;

      // proceed with next rhs
      ++rhs_start;
    }
  }

  // delete remaining variables in *this
  while (start != finish) {
    changeAssign(*start);
    ++start;
  }

  return *this;
}

// lcm
BooleMonomial
BooleMonomial::LCM(const self& rhs) const {

  PBORI_TRACE_FUNC( "BooleMonomial::GCD(const self&) const" );
  return self(*this).LCMAssign(rhs);
}

// lcm with assignment
BooleMonomial&
BooleMonomial::LCMAssign(const self& rhs) {

  PBORI_TRACE_FUNC( "BooleMonomial::GCDAssign(const self&)" );

  return (*this *= rhs);
}

// Multiples wrt. given monom
BooleMonomial::set_type
BooleMonomial::multiples(const self& monom) const {

  PBORI_TRACE_FUNC( "BooleMonomial::multiples(const self&) const" );


#ifdef PBORI_MULTIPLES_HIGHLEVEL

  return monom.divisors().unateProduct(m_poly.diagram());
#else // PBORI_MULTIPLES_LOWLEVEL

  std::vector<idx_type> indices(monom.deg());
  std::copy(monom.begin(), monom.end(), indices.begin());

  return set_type(diagram().firstMultiples(indices));
#endif
}

/// @function greater_variable
/// @brief  
BooleMonomial::bool_type
greater_variable(BooleMonomial::idx_type lhs, BooleMonomial::idx_type rhs){

  PBORI_TRACE_FUNC( "greater_variable(idx_type, idx_type)" );

  return
    (BoolePolyRing::activeManager().compare(lhs, rhs)==CTypes::greater_than);
}

// Get exponent vector
BooleMonomial::exp_type
BooleMonomial::exp() const {

  PBORI_TRACE_FUNC( "BooleMonomial::exp() const" );

  return  exp_type().get(*this);
}

END_NAMESPACE_PBORI
