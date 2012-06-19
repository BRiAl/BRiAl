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
 *   (c) 2006 by The PolyBoRi Team
**/
//*****************************************************************************


// load header files
#include <polybori/BooleMonomial.h>
#include <polybori/BooleVariable.h>
#include <polybori/BooleExponent.h>

// get ring definitions
#include <polybori/BooleEnv.h>

#include <polybori/orderings/COrderingBase.h>

BEGIN_NAMESPACE_PBORI

//-------------------------------------------------------------------------
// Constructors and destructor
//------------------------------------------------------------------------


// Construct from Boolean variable
// not inlined to avoid dependency loop!
// (both depend on poly_type)
BooleMonomial::BooleMonomial(const var_type& rhs) :
  m_poly(rhs) {
  
  PBORI_TRACE_FUNC( "BooleMonomial(const var_type&)" );
}

// Reducibility test
BooleMonomial::bool_type
BooleMonomial::reducibleBy(const var_type& rhs) const {

  PBORI_TRACE_FUNC( "BooleMonomial::reducibleBy(const var_type&) const" );

  return m_poly.firstReducibleBy(rhs);
}


//  Substitute variable with index idx by its complement
BooleMonomial
BooleMonomial::change(idx_type idx) const {

  PBORI_TRACE_FUNC( "BooleMonomial::change(idx_type) const " );

  return set_type(diagram().change(idx));
}

//  Multiplication with assignment
BooleMonomial&
BooleMonomial::operator*=(const self& rhs) {

  PBORI_TRACE_FUNC( "BooleMonomial::operator*=(const self&)" );

  m_poly = m_poly.internalDiagram().unateProduct(rhs.m_poly.diagram());

  return *this;
}

//  Division with assignment
// Note: inlining seems to be inefficient, when throwing errors
BooleMonomial&
BooleMonomial::operator/=(const self& rhs) {

  PBORI_TRACE_FUNC( "BooleMonomial::operator*=(const self&)" );

  m_poly /= rhs;//.m_poly;
  if PBORI_UNLIKELY(m_poly.isZero())
    throw PBoRiError(CTypes::monomial_zero);

  return *this;
}

//  Multiplication with assignment (with variable)
BooleMonomial&
BooleMonomial::operator*=(const var_type& rhs) {

  PBORI_TRACE_FUNC( "BooleMonomial::operator*=(const var_type&)" );

  if (!reducibleBy(rhs))
    m_poly = internalDiagram().change(rhs.index());

  return *this;
}

// Division with assignment (with variable)
// Note: inlining seems to be inefficient, when throwing errors
BooleMonomial&
BooleMonomial::operator/=(const var_type& rhs) {

  PBORI_TRACE_FUNC( "BooleMonomial::operator/=(const var_type&)" );

  m_poly = internalDiagram().subset1(rhs.index());
  if PBORI_UNLIKELY(m_poly.isZero())
    throw PBoRiError(CTypes::monomial_zero);

  return *this;
}

// Comparision
BooleMonomial::comp_type
BooleMonomial::compare(const self& rhs) const {

  PBORI_TRACE_FUNC( "BooleMonomial::compare(const self& rhs) const" );

  return ring().ordering().compare(*this, rhs);
}

// Degree of the lcm
BooleMonomial::deg_type
BooleMonomial::LCMDeg(const self& rhs) const {

  PBORI_TRACE_FUNC( "BooleMonomial::LCMDeg(const self&) const" );

  if ( m_poly.isZero() || ( (const BoolePolynomial&)rhs).isZero() )
    return 0;

  if ( (*this == rhs) || ( (const BoolePolynomial&)rhs).isOne() )
    return deg();
  
  if ( m_poly.isOne() )
    return rhs.deg();

  deg_type result = 0;
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
      *this = change(*start);
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
    *this = change(*start);
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


// Get exponent vector
BooleMonomial::exp_type
BooleMonomial::exp() const {

  PBORI_TRACE_FUNC( "BooleMonomial::exp() const" );

  return  exp_type().get(*this);
}

// Get first variable
BooleMonomial::var_type
BooleMonomial::firstVariable() const {

  PBORI_TRACE_FUNC( "BooleMonomial::firstVariable() const" );
  if PBORI_UNLIKELY(m_poly.isConstant())
    throw PBoRiError(CTypes::out_of_bounds);
  return var_type(firstIndex(), ring());
}
END_NAMESPACE_PBORI
