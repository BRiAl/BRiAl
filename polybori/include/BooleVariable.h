// -*- c++ -*-
//*****************************************************************************
/** @file BooleVariable.h 
 *
 * @author Alexander Dreyer
 * @date 2006-03-10
 *
 * This file carries the definition of class @c BooleVariable, which can be used
 * to access the boolean variables of the currently active Boolean polynomial
 * ring. 
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
 * Revision 1.18  2008/03/02 23:24:37  dreyer
 * CHANGE: ring elements like polynomials, monomials, and variables have ring()
 *
 * Revision 1.17  2008/02/28 17:05:47  dreyer
 * Fix: treating constants (0, 1) accordingly
 *
 * Revision 1.16  2008/01/08 12:27:22  dreyer
 * CHANGE: erreous cast int to Variable removed, info about scons -h added
 *
 * Revision 1.15  2007/12/13 15:53:48  dreyer
 * CHANGE: Ordering in BoolePolyRing again; BooleEnv manages active ring
 *
 * Revision 1.14  2007/12/11 10:50:44  dreyer
 * Fix: BooleVariable()*BooleMonomial() now monomial
 *
 * Revision 1.13  2007/11/19 12:49:59  bricken
 * + set
 *
 * Revision 1.12  2007/11/06 15:03:33  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.11  2007/08/27 09:57:43  bricken
 * + equality test const
 *
 * Revision 1.10  2007/07/31 14:26:29  dreyer
 * CHANCE: BooleVariable, now uses persistent variables
 *
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
#include "BooleEnv.h"
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

  /// Type for Boolean polynomial rings (without ordering)
  typedef BooleRing ring_type;

  /// Constructor idx-th variable of active ring
  explicit BooleVariable(idx_type idx = 0):
    m_poly( BooleEnv::persistentVariable(idx) ) {}
  
  /// Type of sets of Boolean variables
  typedef BooleSet set_type;
  
  /// Copy constructor
  BooleVariable(const self& rhs):  
    m_poly(rhs.m_poly) {}

  /// Cast to polynomial type
  operator const BoolePolynomial&() const { return m_poly; }

  /// Get index of the variable
  idx_type index() const { return *m_poly.firstBegin(); }

  /// Equality check
  bool operator== (const self& other) const{
      return m_poly==other.m_poly;
  }
  bool operator!= (const self& other) const{
        return m_poly!=other.m_poly;
  }
  set_type set() const { return m_poly.set(); }

  /// Access ring, where this belongs to
  ring_type ring() const { return m_poly.ring(); } 

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
inline BooleMonomial
operator*(const BooleVariable& lhs, 
          const BooleMonomial& rhs){

  return BooleMonomial(lhs) * rhs;
}

/// Multiplication of variables by a 0 or 1
inline BooleMonomial
operator*(const BooleVariable& lhs, const BooleConstant& rhs){

  return BooleMonomial(lhs) * rhs;
}

/// Multiplication of 0 or 1  by a Variable
inline BooleMonomial
operator*(const BooleConstant& lhs, const BooleVariable& rhs){

  return rhs * lhs;
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
