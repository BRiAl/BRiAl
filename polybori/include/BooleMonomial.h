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
 *   (c) 2006 by The PolyBoRi Team
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.39  2008/03/05 16:23:37  dreyer
 * CHANGE: BooleMonomial::variableBegin()|End(); monom/monom = 0 throws
 *
 * Revision 1.38  2008/03/03 18:07:19  dreyer
 * Fix: missing things in Python-interface
 *
 * Revision 1.37  2008/03/03 12:44:31  dreyer
 * Change: More inlining, and safer constructors
 *
 * Revision 1.36  2008/03/02 23:45:33  dreyer
 * CHANGED: added contructors for given ring
 *
 * Revision 1.35  2008/03/02 23:24:36  dreyer
 * CHANGE: ring elements like polynomials, monomials, and variables have ring()
 *
 * Revision 1.34  2008/03/01 01:11:24  dreyer
 * Fix: working around bug in addition
 *
 * Revision 1.33  2008/02/28 17:05:46  dreyer
 * Fix: treating constants (0, 1) accordingly
 *
 * Revision 1.32  2008/02/27 16:35:12  dreyer
 * Fix: Polynomial(0|1) removed, where possible
 *
 * Revision 1.31  2007/11/30 09:33:19  dreyer
 * CHANGE: more dd-like stableHash()
 *
 * Revision 1.30  2007/11/29 16:28:31  dreyer
 * ADD: fast hash(), where applicable; + stableHashes() anywhere
 *
 * Revision 1.29  2007/11/15 13:08:00  dreyer
 * CHANGE: removed dd_type from PyPolyBoRi => .diagram()->.set()
 *
 * Revision 1.28  2007/11/06 15:03:32  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.27  2007/10/09 15:07:27  dreyer
 * ADD: mapping;   CHANGE: shared python modules to pyroot
 *
 * Revision 1.26  2007/07/30 15:19:38  dreyer
 * CHANGE: CCuddNavigator does not convert to DdNode* impicitely any more
 *
 * Revision 1.25  2007/05/29 10:33:47  dreyer
 * ADD: BooleMonomial::popFirst()
 *
 * Revision 1.24  2007/05/22 11:05:28  dreyer
 * FIX: ambigous overload
 *
 * Revision 1.23  2007/05/04 15:26:27  dreyer
 * CHANGE: Optimized version for monomial term generation
 *
 * Revision 1.22  2006/12/04 12:48:16  dreyer
 * CHANGE: cached and recursive lead() and leadexp() refined, generalized
 *
 * Revision 1.21  2006/11/21 16:06:40  dreyer
 * CHANGE: inlining added
 *
 * Revision 1.20  2006/10/10 16:12:28  bricken
 * + try this deg function
 *
 * Revision 1.19  2006/10/06 12:52:00  dreyer
 * ADD easy_equility_property and used in lex_compare
 *
 * Revision 1.18  2006/09/22 06:19:12  dreyer
 * CHANGE: deactivated the use of pointer in hash()
 *
 * Revision 1.17  2006/09/19 09:46:05  dreyer
 * CHANGE: binary_compose to binary_composition
 * CHANGE: BooleMonomial::idx_map_type
 * ADD: symmetric_composition<>, navigates<>
 *
 * Revision 1.16  2006/09/13 16:00:55  dreyer
 * CHANGE: Experimenting with hash()
 *
 * Revision 1.15  2006/09/04 11:33:57  dreyer
 * CHANGE: lm*() use leadFirst()
 *
 * Revision 1.14  2006/09/04 09:55:05  bricken
 * + avoid bad templates
 *
 * Revision 1.13  2006/08/29 12:09:26  dreyer
 * using CDDOperations for generic dd functions
 *
 * Revision 1.12  2006/08/29 10:37:55  dreyer
 * CHANGE: non-const version of diagram() now internalDiagram()
 *
 * Revision 1.11  2006/08/29 10:19:43  bricken
 * + removed implicit casts exp to mon
 *
 * Revision 1.10  2006/08/24 14:47:49  dreyer
 * ADD: BooleExponent integrated, FIX: multiples (for indices < first)
 *
 * Revision 1.9  2006/08/01 11:12:22  dreyer
 * CHANGE: removed ';' at end of functions (pedantic)
 *
 * Revision 1.8  2006/07/17 15:32:08  dreyer
 * ADD: BoolePolynomial::hasConstantPart, BooleMonomial::isOne, isZero
 *
 * Revision 1.7  2006/07/14 09:02:49  dreyer
 * ADD: greater_variable()
 *
 * Revision 1.6  2006/05/27 11:04:25  bricken
 * + optimization remark
 *
 * Revision 1.5  2006/04/24 11:41:56  dreyer
 * FIX only necessary types are traits
 *
 * Revision 1.4  2006/04/20 17:31:05  dreyer
 * FIX removed casting operator, which caused ambigeous overloads
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

#ifndef BooleMonomial_h_
#define BooleMonomial_h_

// include basic definitions
#include "pbori_defs.h"

// get definition of BoolePolynomial and BooleVariable
#include "BoolePolynomial.h"
#include "BooleVariable.h"
// get standard map functionality
#include <map>

// get variable iterator
#include "CVariableIter.h"

// get variable iterator
#include "PBoRiError.h"

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
class BooleMonomial {

 public:
  template <class, class> friend class CDDOperations;
  friend class COrderBase;
  template <class, class> friend class CTermGeneratorBase;

  //-------------------------------------------------------------------------
  // types definitions
  //-------------------------------------------------------------------------

  /// @name Adopt global type definitions
  //@{
  typedef CTypes::dd_type dd_type;
  typedef CTypes::size_type size_type;
  typedef CTypes::idx_type idx_type;
  typedef CTypes::hash_type hash_type;
  typedef CTypes::bool_type bool_type;
  typedef CTypes::comp_type comp_type;
  typedef CTypes::integer_type integer_type;
  typedef CTypes::ostream_type ostream_type;
  //@}

  /// Generic access to current type
  typedef BooleMonomial self;

  /// Type of Boolean polynomials
  typedef BoolePolynomial poly_type;

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

  /// Default Constructor (Constructs monomial one of the active ring)
  BooleMonomial():
    m_poly( BooleEnv::one() )  { }

  /// Copy constructor
  BooleMonomial(const self& rhs):  
    m_poly(rhs.m_poly) {}

  /// Construct from Boolean variable
  BooleMonomial(const var_type& rhs);  // not inlined to avoid dependency loop
                                       // (both depend on poly_type)

  /// Construct from exponent vector
  BooleMonomial(const exp_type& rhs, const ring_type& ring): 
    m_poly(rhs, ring) { }

  /// Construct from Boolean constant and given ring
  BooleMonomial(constant_type val, const ring_type& ring): 
    m_poly(val, ring) {}

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
  size_type deg() const {
    ///@todo optimal, if stored, else much too complicated, as it will probably use cache lookups
    #if 0
    return m_poly.nNodes(); 
    #else
    return std::distance(m_poly.firstBegin(),m_poly.firstEnd());
    #endif
    }

  /// Size of the exponents
  size_type size() const { return deg(); }

  /// Divisors of the monomial
  set_type divisors() const { return m_poly.lmDivisors(); }

  /// multiples of the monomial wrt. given monomial
  set_type multiples(const self&) const; 

  /// Hash value of the monomial
  hash_type stableHash() const {
    return stable_first_hash_range(m_poly.navigation());
  }

  /// Get unique hash value (valid only per runtime)
  hash_type hash() const {  return m_poly.hash(); }

  /// Substitute variable with index idx by its complement and assign
  self& changeAssign(idx_type);

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
  bool_type isZero() const { return m_poly.isZero(); }
  bool_type isOne() const { return m_poly.isOne(); }
  bool_type isConstant() const { return m_poly.isConstant(); }
  //@}

  /// Test for reducibility
  bool_type reducibleBy(const self& rhs) const { 
    return m_poly.reducibleBy(rhs); }
  bool_type reducibleBy(const var_type& rhs) const;

  /// Compare with rhs monomial and return comparision code
  comp_type compare(const self&) const;

  /// Degree of the least common multiple
  size_type LCMDeg(const self&) const;

  /// Compute theleast common multiple and assign
  self& LCMAssign(const self&);

  /// Compute the greatest common divisor
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
    assert(!m_poly.isConstant());
    return *this = set_type( dd_type(m_poly.diagram().manager(),
                                     m_poly.navigation().thenBranch()) ); 
  }

  /// Get first variable in monomial
  var_type firstVariable() const;

  /// Get first index in monomial
  idx_type firstIndex() const {
    assert(!m_poly.isConstant());
    return *begin();
  }

  /// Access ring, where this belongs to
  ring_type ring() const { return m_poly.ring(); } 

protected:
  /// Access to internal decision diagramm structure
  dd_type& internalDiagram() { return m_poly.internalDiagram(); }

  /// Construct from decision diagram
  //  BooleMonomial(const dd_type& rhs): m_poly(rhs) {}
  /// Construct from decision diagram
  BooleMonomial(const set_type& rhs): m_poly(rhs.diagram()) {}

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

/// @function greater_variable
/// @brief Checks whether BooleVariable(lhs) > BooleVariable(rhs) 
BooleMonomial::bool_type
greater_variable(BooleMonomial::idx_type lhs, BooleMonomial::idx_type rhs);


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
inline BoolePolynomial&
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


#endif // of BooleMonomial_h_
