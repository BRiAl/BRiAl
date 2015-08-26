// -*- c++ -*-
//*****************************************************************************
/** @file: BooleMonomial.h 
 *
 * @author Alexander Dreyer
 * @date 2006-04-19
 *
 * This file carries the definition of class @c BooleMonomial, which can be used
 * to access the boolean monomials of the currently active Boolean polynomial
 * ring. 
 * 
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_BooleMonomial_h_
#define polybori_BooleMonomial_h_

// include basic definitions
#include <polybori/pbori_defs.h>

// get definition of BoolePolynomial and BooleVariable
#include <polybori/BoolePolynomial.h>
#include <polybori/BooleVariable.h>
// get standard map functionality
#include <map>

// get variable iterator
#include <polybori/iterators/CVariableIter.h>

// get variable iterator
#include <polybori/except/PBoRiError.h>

BEGIN_NAMESPACE_PBORI

class BooleVariable;
class BooleExponent;
template <class DDType, class MonomType> class CDDOperations;

/** @class BooleMonomial
 * @brief This class is just a wrapper for using variables from @c cudd's
 * decicion diagram. 
 *
 * @note @c BooleMonomial is actually a specialization of @c BoolePolynomial
 * with a special constructor.
 *
 **/
class BooleMonomial:
  public CAuxTypes {

  /// Generic access to current type
  typedef BooleMonomial self;

public:
  template <class, class> friend class CDDOperations;
  friend class COrderingBase;
  template <class> friend class CTermGeneratorBase;
  template <class, class> friend class CTermGeneratorBase__;

  /// Type of Boolean polynomials
  typedef BoolePolynomial poly_type;

  /// @name Adopt global type definitions
  //@{
  typedef poly_type::dd_type dd_type;
  typedef poly_type::integer_type integer_type;
  typedef poly_type::ostream_type ostream_type;
  //@}

  /// Type of Boolean variables
  typedef poly_type::var_type var_type;

  /// Type of Boolean constants
  typedef poly_type::constant_type constant_type;

  /// Type of sets of Boolean variables
  typedef poly_type::set_type set_type;

  /// Type of exponent vector
  typedef poly_type::exp_type exp_type;

  /// Type for Boolean polynomial rings (without ordering)
  typedef poly_type::ring_type ring_type;

  /// Access to iterator over indices
  typedef poly_type::first_iterator const_iterator;

  /// Access to iterator over variables
  typedef CVariableIter<const_iterator, var_type> variable_iterator;

  /// Type for index maps
  //  typedef generate_index_map<self>::type idx_map_type;

  typedef std::map<self, idx_type, symmetric_composition<
    std::less<poly_type::navigator>, 
    navigates<poly_type> > > idx_map_type;

  /// The property whether the equality check is easy is inherited from dd_type
  typedef dd_type::easy_equality_property easy_equality_property;

  /// Copy constructor
  BooleMonomial(const self& rhs):  
    m_poly(rhs.m_poly) {}

  /// Construct from Boolean variable
  BooleMonomial(const var_type& rhs);  // not inlined to avoid dependency loop
                                       // (both depend on poly_type)

  /// Construct from exponent vector
  BooleMonomial(const exp_type& rhs, const ring_type& ring): 
    m_poly(rhs, ring) { }

  /// Construct from given ring
  BooleMonomial(const ring_type& ring): 
    m_poly(ring.one()) {}

  /// Destructor
  ~BooleMonomial() {}

  /// Casting operator
  operator const BoolePolynomial&() const { return m_poly; }

  /// Get exponent vector
  exp_type exp() const;


  /// Start iteration over indices
  const_iterator begin() const { return m_poly.firstBegin(); }

  /// Finish iteration over indices
  const_iterator end() const { return m_poly.firstEnd(); }

  /// Start iteration over variables
  variable_iterator variableBegin() const { 
    return variable_iterator(begin(), ring()); 
  }

  /// Finish iteration over variables
  variable_iterator variableEnd() const { 
    return variable_iterator(end(), ring()); 
  }

  /// Degree of the monomial
  deg_type deg() const {
    return std::distance(m_poly.firstBegin(),m_poly.firstEnd());
  }

  /// Size of the exponents
  size_type size() const { return (size_type)deg(); }  // always nonnegative

  /// Divisors of the monomial
  set_type divisors() const { return m_poly.leadDivisors(); }

  /// multiples of the monomial wrt. given monomial
  set_type multiples(const self&) const; 

  /// Hash value of the monomial
  hash_type stableHash() const {
    return stable_first_hash_range(m_poly.navigation());
  }

  /// Get unique hash value (valid only per runtime)
  hash_type hash() const {  return m_poly.hash(); }

  /// Substitute variable with index idx by its complement
  self change(idx_type) const;


  /// @name Arithmetical operations
  //@{
  self& operator*=(const self&);
  self& operator/=(const self&);
  self& operator*=(const var_type&);
  self& operator/=(const var_type&);
  //@}

  /// @name Logical operations
  //@{
  bool_type operator==(const self& rhs) const { return m_poly == rhs.m_poly; }
  bool_type operator!=(const self& rhs) const { return m_poly != rhs.m_poly; }
  bool_type operator==(constant_type rhs) const { return m_poly == rhs; }
  bool_type operator!=(constant_type rhs) const { return m_poly != rhs; }
  bool_type isOne() const { return m_poly.isOne(); }
  bool_type isConstant() const { return m_poly.isConstant(); }
  //@}

  /// Test for reducibility
  bool_type reducibleBy(const self& rhs) const { 
    return m_poly.firstReducibleBy(rhs); }
  bool_type reducibleBy(const var_type& rhs) const;

  /// Compare with rhs monomial and return comparision code
  comp_type compare(const self&) const;

  /// Degree of the least common multiple
  deg_type LCMDeg(const self&) const;

  /// Compute the least common multiple and assign
  self& LCMAssign(const self&);

  /// Compute the least common multiple
  self LCM(const self&) const;

  /// Compute the greatest common divisor and assign
  self& GCDAssign(const self&);

  /// Compute the greatest common divisor
  self GCD(const self&) const;
 
  /// Read-only access to internal decision diagramm structure
  const dd_type& diagram() const { return m_poly.diagram(); }

  /// Get corresponding subset of of the powerset over all variables
  set_type set() const { return m_poly.set(); }

  /// Removes the first variables from monomial
  self& popFirst() { 
    PBORI_ASSERT(!m_poly.isConstant());
    return *this = set_type( dd_type(m_poly.ring(),
                                     m_poly.navigation().thenBranch()) ); 
  }

  /// Get first variable in monomial
  var_type firstVariable() const;

  /// Get first index in monomial
  /// @note return out-of-range integer for polynomial one 
  idx_type firstIndex() const {
    return *m_poly.navigation();
  }

  /// Access ring, where this belongs to
  const ring_type& ring() const { return m_poly.ring(); } 

protected:
  /// Access to internal decision diagramm structure
  dd_type& internalDiagram() { return m_poly.internalDiagram(); }

  /// Construct from decision diagram
  BooleMonomial(const set_type& rhs): m_poly(rhs.diagram()) {
    PBORI_ASSERT(!m_poly.isZero());
  }

private:
  BoolePolynomial m_poly;
};

/// Multiplication of monomials
inline BooleMonomial
operator*(const BooleMonomial& lhs, const BooleMonomial& rhs) {
  return BooleMonomial(lhs) *= rhs;
}
/// Multiplication of monomials
inline BooleMonomial
operator*(const BooleMonomial& lhs, const BooleVariable& rhs) {
  return BooleMonomial(lhs) *= rhs;
}
/// Multiplication of monomials
inline BoolePolynomial
operator*(const BooleMonomial& lhs, BooleConstant rhs) {
  return BoolePolynomial(lhs) *= rhs;
}

/// Multiplication of monomials
inline BoolePolynomial
operator*(BooleConstant lhs, const BooleMonomial& rhs) {
  return rhs * lhs;
}

/// Division of monomials
inline BooleMonomial
operator/(const BooleMonomial& lhs, const BooleMonomial& rhs) {
  return BooleMonomial(lhs) /= rhs;
}

/// Division of monomials
inline BooleMonomial
operator/(const BooleMonomial& lhs, const BooleVariable& rhs) {
  return  lhs / BooleMonomial(rhs);
}

/// Less than comparision
inline BooleMonomial::bool_type
operator<(const BooleMonomial& lhs, const BooleMonomial& rhs) {

  return (lhs.compare(rhs) == CTypes::less_than);
}

/// Greater than comparision
inline BooleMonomial::bool_type
operator>(const BooleMonomial& lhs, const BooleMonomial& rhs) {

  return (lhs.compare(rhs) == CTypes::greater_than);
}

/// Less or equal than comparision
inline BooleMonomial::bool_type
operator<=(const BooleMonomial& lhs, const BooleMonomial& rhs) {

  return (lhs.compare(rhs) <= CTypes::less_or_equal_max);
}

/// Greater or equal than comparision
inline BooleMonomial::bool_type
operator>=(const BooleMonomial& lhs, const BooleMonomial& rhs) {

  return (lhs.compare(rhs) >= CTypes::greater_or_equal_min);
}


/// Compute the greatest common divisor of two monomials
inline BooleMonomial
GCD(const BooleMonomial& lhs, const BooleMonomial& rhs ){

  return lhs.GCD(rhs);
}

/// Compute the greatest common divisor of two monomials
inline BooleMonomial
LCM(const BooleMonomial& lhs, const BooleMonomial& rhs ){

  return lhs.LCM(rhs);
}

// Anyone need this?
/// @fn greater_variable(BooleMonomial::idx_type lhs, BooleMonomial::idx_type rhs);
/// @brief Checks whether BooleVariable(lhs) > BooleVariable(rhs) 
// BooleMonomial::bool_type
// greater_variable(BooleMonomial::idx_type lhs, BooleMonomial::idx_type rhs);


/// Multiplication of variables by a 0 or 1
inline BoolePolynomial
operator*(const BooleVariable& lhs, const BooleConstant& rhs){

  return BooleMonomial(lhs) * rhs;
}

/// Multiplication of 0 or 1  by a Variable
inline BoolePolynomial
operator*(const BooleConstant& lhs, const BooleVariable& rhs){

  return rhs * lhs;
}

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

/// Multiplication of a polynomial by a variable
inline BoolePolynomial
operator*(const BoolePolynomial& lhs, 
          const BooleVariable& rhs){

  return BoolePolynomial(lhs) *= BooleMonomial(rhs);
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


END_NAMESPACE_PBORI


#endif // of polybori_BooleMonomial_h_
