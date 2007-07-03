// -*- c++ -*-
//*****************************************************************************
/** @file 
 *
 * @author Alexander Dreyer
 * @date 2006-03-10
 *
 * This file carries the definition of class @c BooleVariable, which can be used
 * to access the boolean variables of the currently active Boolean polynomial
 * ring. 
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
 * Revision 1.9  2007/07/03 07:21:07  dreyer
 * Fix: ambigious overload var*monom
 *
 * Revision 1.8  2007/06/11 14:23:30  bricken
 * + var==var
 *
 * Revision 1.7  2007/05/22 11:05:28  dreyer
 * FIX: ambigous overload
 *
 * Revision 1.6  2007/05/14 08:10:59  dreyer
 * ADD: added poly / poly and poly % poly
 *
 * Revision 1.5  2007/04/17 09:12:18  dreyer
 * FIX: ambigious overload in var * poly
 *
 * Revision 1.4  2006/07/14 09:02:49  dreyer
 * ADD: greater_variable()
 *
 * Revision 1.3  2006/04/19 15:55:53  dreyer
 * ADD BooleMonomial, BoolePolynomial::fetchTerms() and ::terms()
 *
 * Revision 1.2  2006/03/20 09:52:57  dreyer
 * CHANGE: BooleVariable uses composition; variable generated in BoolePolyRing
 *
 * Revision 1.1  2006/03/16 17:09:13  dreyer
 * ADD BoolePolynial functionality started
 *
 * @endverbatim
**/
//*****************************************************************************

#ifndef BooleVariable_h_
#define BooleVariable_h_

// include basic definitions
#include "pbori_defs.h"

// get BoolePolynomial's definition
#include "BoolePolyRing.h"
#include "BooleMonomial.h"

BEGIN_NAMESPACE_PBORI

/** @class BooleVariable
 * @brief This class is just a wrapper for using variables from @c cudd's
 * decicion diagram. 
 *
 * @note @c BooleVariable is actually a specialization of @c BoolePolynomial
 * with a special constructor.
 *
 **/
class BooleVariable {

 public:
  //-------------------------------------------------------------------------
  // types definitions
  //-------------------------------------------------------------------------

  /// @name Adopt global type definitions
  //@{
  typedef CTypes::dd_type dd_type;
  typedef CTypes::size_type size_type;
  typedef CTypes::idx_type idx_type;
  //@}

  /// Generic access to current type
  typedef BooleVariable self;

  /// Constructor idx-th variable of active ring
  BooleVariable(idx_type idx = 0):
    m_poly( BoolePolyRing::ringVariable(idx) ) {}

  /// Copy constructor
  BooleVariable(const self& rhs):  
    m_poly(rhs.m_poly) {}

  /// Cast to polynomial type
  operator const BoolePolynomial&() const { return m_poly; }

  /// Get index of the variable
  idx_type index() const { return *m_poly.firstBegin(); }
bool operator== (const self& other){
      return m_poly==other.m_poly;
  }
private:
  BoolePolynomial m_poly;
};

/**/
/// Multiplication of variables by a polynomial
inline BoolePolynomial
operator*(const BooleVariable& lhs, 
          const BoolePolynomial& rhs){

  return BoolePolynomial(rhs) *= BooleMonomial(lhs);
}

/// Multiplication of variables by a monomial
inline BoolePolynomial
operator*(const BooleVariable& lhs, 
          const BooleMonomial& rhs){

  return BooleMonomial(lhs) * rhs;
}

/// Multiplication of a polynomial by a variable
inline BoolePolynomial
operator*(const BoolePolynomial& lhs, 
          const BooleVariable& rhs){

  return BoolePolynomial(lhs) *= BooleMonomial(rhs);
}

/// Multiplication of a polynomial by a variable with assignment
inline BoolePolynomial&
operator*=(BoolePolynomial& lhs, 
           const BooleVariable& rhs){

  return lhs *= BooleMonomial(rhs);
}

/// Multiplication of monomials by a polynomial
inline BooleMonomial
operator*(const BooleVariable& lhs, 
          const BooleVariable& rhs){

  return BooleMonomial(lhs) *= BooleMonomial(rhs);
}

/// Division of a polynomial by a variable (forcing monomial variant)
inline BoolePolynomial
operator/=(BoolePolynomial& lhs, const BooleVariable& rhs){

  return lhs /= BooleMonomial(rhs);
}

/// Division of a polynomial by a variable (forcing monomial variant)
inline BoolePolynomial
operator/(const BoolePolynomial& lhs, 
          const BooleVariable& rhs){

  return lhs / BooleMonomial(rhs);
}


/// Remainder of division of a polynomial by a variable 
inline BoolePolynomial
operator%(const BoolePolynomial& lhs, 
          const BooleVariable& rhs){

  return lhs % BooleMonomial(rhs);
}

/// Remainder of division of a polynomial by a variable (with assignment)
inline BoolePolynomial&
operator%=(BoolePolynomial& lhs, 
          const BooleVariable& rhs){

  return lhs %= BooleMonomial(rhs);
}

END_NAMESPACE_PBORI

#endif // of BooleVariable_h_
