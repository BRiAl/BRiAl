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
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_BooleVariable_h_
#define polybori_BooleVariable_h_

// include basic definitions
#include <polybori/pbori_defs.h>

// get BoolePolynomial's definition
#include <polybori/BooleEnv.h>
#include <polybori/BoolePolynomial.h>
#include <polybori/BooleSet.h>

BEGIN_NAMESPACE_PBORI

/** @class BooleVariable
 * @brief This class is just a wrapper for using variables from @c cudd's
 * decicion diagram. 
 *
 * @note @c BooleVariable is actually a specialization of @c BoolePolynomial
 * with a special constructor.
 *
 **/
class BooleVariable:
  public CAuxTypes {

  /// Generic access to current type
  typedef BooleVariable self;

 public:

  /// Type of sets of Boolean variables
  typedef BooleSet set_type;

  /// Type for Boolean polynomial rings (without ordering)
  typedef BoolePolyRing ring_type;

  /// Constructor idx-th variable of active ring
  /**
  explicit BooleVariable(idx_type idx = 0):
    m_poly( BooleEnv::ring().variable(idx) ) {}
  **/
  /// Construct idx-th variable of a given ring
  BooleVariable(idx_type idx, const ring_type& ring):
    m_poly( ring.variableDiagram(idx) ) {}

  /// Construct first variable of a given ring
  BooleVariable(const ring_type& ring):
    m_poly( ring.variableDiagram(0) ) {}

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

  // Nonequality check
  bool operator!= (const self& other) const{
        return m_poly!=other.m_poly;
  }

  /// Hash value of the variable
  hash_type stableHash() const{  return m_poly.stableHash(); }

  /// Get unique hash value (valid only per runtime)
  hash_type hash() const {  return m_poly.hash(); }

  /// Convert to Boolean set
  set_type set() const { return m_poly.set(); }

  /// Access ring, where this belongs to
  const ring_type& ring() const { return m_poly.ring(); } 

private:
  friend class BoolePolyRing;
  BooleVariable(const BoolePolynomial& poly):m_poly(poly){}

  BoolePolynomial m_poly;
};


/// Division 
inline BoolePolynomial
operator/(const BooleVariable& lhs, const BooleVariable& rhs) {
  return BoolePolynomial(BooleConstant(lhs == rhs), lhs.ring());
}

END_NAMESPACE_PBORI

#endif // of polybori_BooleVariable_h_
