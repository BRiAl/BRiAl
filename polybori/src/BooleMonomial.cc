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
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.22  2008/09/21 22:21:03  dreyer
 * Change: deg_type replaces size_type for deg(), etc.
 *
 * Revision 1.21  2008/07/13 22:49:36  dreyer
 * Fix: Doxygen clean-up
 *
 * Revision 1.20  2008/03/11 10:04:12  dreyer
 * Fix: Exceptions: Division by zero, Polynomial(0).lead(), and ITE
 *
 * Revision 1.19  2008/03/05 16:23:37  dreyer
 * CHANGE: BooleMonomial::variableBegin()|End(); monom/monom = 0 throws
 *
 * Revision 1.18  2008/03/03 13:52:12  dreyer
 * Change: using more safe Variable(idx, ring)
 *
 * Revision 1.17  2008/03/03 12:44:32  dreyer
 * Change: More inlining, and safer constructors
 *
 * Revision 1.16  2007/12/13 15:53:49  dreyer
 * CHANGE: Ordering in BoolePolyRing again; BooleEnv manages active ring
 *
 * Revision 1.15  2007/12/07 17:06:19  dreyer
 * CHANGE: First try: ring and order separated
 *
 * Revision 1.14  2007/11/06 15:03:38  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.13  2007/10/09 15:07:27  dreyer
 * ADD: mapping;   CHANGE: shared python modules to pyroot
 *
 * Revision 1.12  2007/04/13 13:55:53  dreyer
 * CHANGE: using CTermStack for implementing ordered_(exp_)iterator
 *
 * Revision 1.11  2006/11/21 16:06:40  dreyer
 * CHANGE: inlining added
 *
 * Revision 1.10  2006/08/29 12:09:27  dreyer
 * using CDDOperations for generic dd functions
 *
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
#include "BooleEnv.h"

#include "COrderingBase.h"

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

  return m_poly.reducibleBy(rhs);
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
  if UNLIKELY(m_poly.isZero())
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
  if UNLIKELY(m_poly.isZero())
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
  return var_type(firstIndex(), ring());
}
END_NAMESPACE_PBORI
