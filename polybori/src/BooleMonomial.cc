// -*- c++ -*-
//*****************************************************************************
/** @file 
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
 * Revision 1.1  2006/04/19 15:55:53  dreyer
 * ADD BooleMonomial, BoolePolynomial::fetchTerms() and ::terms()
 *
 * @endverbatim
**/
//*****************************************************************************


// load header files
# include "BooleMonomial.h"
# include "BooleVariable.h"

// get ring definitions
# include "BoolePolyRing.h"

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

  m_poly.diagram().changeAssign(idx);

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

  m_poly.diagram().unateProductAssign(rhs.m_poly.diagram());

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

  m_poly.diagram().subset1Assign(rhs.index());

  return *this;
}
// Comparision
BooleMonomial::comp_type
BooleMonomial::compare(const self& rhs) const {

  PBORI_TRACE_FUNC( "BooleMonomial::compare(const self& rhs) const" );


  /// @todo Up to now, this is for lexicographic order only.
  if (*this == rhs)
    return CTypes::equality;

  const_iterator start(begin()), finish(end()),
    rhs_start(rhs.begin()), rhs_finish(rhs.end());

  while ( (start != finish) && (rhs_start != rhs_finish) && 
          (*start == *rhs_start) ) {
    ++start; ++rhs_start;
  }

  if (start == finish)
    return CTypes::less_than;

  if (rhs_start == rhs_finish)
    return CTypes::greater_than;

  return (*start < *rhs_start?  CTypes::greater_than : CTypes::less_than);
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

END_NAMESPACE_PBORI
