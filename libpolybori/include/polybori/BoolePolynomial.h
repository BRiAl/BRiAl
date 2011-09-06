// -*- c++ -*-
//*****************************************************************************
/** @file BoolePolynomial.h
 *
 * @author Alexander Dreyer
 * @date 2006-03-10
 *
 * This file carries the definition of class @c BoolePolynomial, which can be
 * used to access the boolean polynomials with respect to the polynomial ring,
 * which was active on initialization time.
 * 
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_BoolePolynomial_h_
#define polybori_BoolePolynomial_h_

// include standard definitions
#include <vector>

// get standard map functionality
#include <map>

// get standard algorithmic functionalites
#include <algorithm>

#include <polybori/BoolePolyRing.h>

// include definition of sets of Boolean variables

#include <polybori/routines/pbori_func.h>
#include <polybori/common/tags.h>
#include <polybori/BooleSet.h>

#include <polybori/iterators/CTermIter.h>
#include <polybori/iterators/CGenericIter.h>
#include <polybori/iterators/CBidirectTermIter.h>

#include <polybori/BooleConstant.h>

BEGIN_NAMESPACE_PBORI


// forward declarations
class LexOrder;
class DegLexOrder;
class DegRevLexAscOrder;
class BlockDegLexOrder;
class BlockDegRevLexAscOrder;

class BooleMonomial;
class BooleVariable;
class BooleExponent;


template <class IteratorType, class MonomType>
class CIndirectIter;

template <class IteratorType, class MonomType>
class COrderedIter;


//template<class, class, class, class> class CGenericIter;
template<class, class, class, class> class CDelayedTermIter;

template<class OrderType, class NavigatorType, class MonomType>
class CGenericIter;

template<class NavigatorType, class ExpType>
class CExpIter;


/** @class BoolePolynomial
 * @brief This class wraps the underlying decicion diagram type and defines the
 * necessary operations.
 *
 **/
class BoolePolynomial;
BoolePolynomial 
operator+(const BoolePolynomial& lhs, const BoolePolynomial& rhs);

class BoolePolynomial:
  public CAuxTypes{

public:

  /// Let BooleMonomial access protected and private members
  friend class BooleMonomial;

  //-------------------------------------------------------------------------
  // types definitions
  //-------------------------------------------------------------------------

  /// Generic access to current type
  typedef BoolePolynomial self;

  /// @name Adopt global type definitions
  //@{
  typedef BooleSet dd_type;
  typedef CTypes::ostream_type ostream_type;
  //@}

  /// Iterator type for iterating over indices of the leading term
  typedef dd_type::first_iterator first_iterator;

  /// Iterator-like type for navigating through diagram structure
  typedef dd_type::navigator navigator;

  /// @todo A more sophisticated treatment for monomials is needed.

  /// Fix type for treatment of monomials
  typedef BooleMonomial monom_type; 

  /// Fix type for treatment of monomials
  typedef BooleVariable var_type; 

  /// Fix type for treatment of exponent vectors
  typedef BooleExponent exp_type; 

  /// Type for wrapping integer and bool values
  typedef BooleConstant constant_type;

  /// Type for Boolean polynomial rings (without ordering)
  typedef BoolePolyRing ring_type;

  /// Type for result of polynomial comparisons
  typedef CTypes::comp_type comp_type;

  /// Incrementation functional type
  typedef 
  binary_composition< std::plus<size_type>, 
                      project_ith<1>, integral_constant<size_type, 1> > 
  increment_type;

  /// Decrementation functional type
  typedef 
  binary_composition< std::minus<size_type>, 
                      project_ith<1>, integral_constant<size_type, 1> > 
  decrement_type;



  /// Iterator type for iterating over all exponents in ordering order
  //  typedef COrderedIter<exp_type> ordered_exp_iterator;
  typedef COrderedIter<navigator, exp_type> ordered_exp_iterator;

  /// Iterator type for iterating over all monomials in ordering order
  //  typedef COrderedIter<monom_type> ordered_iterator;
  typedef COrderedIter<navigator, monom_type> ordered_iterator;

  /// @name Generic iterators for various orderings
  //@{
  typedef CGenericIter<LexOrder, navigator, monom_type> lex_iterator;
  ////  typedef CGenericIter<LexOrder, navigator, monom_type> lex_iterator;
  typedef CGenericIter<DegLexOrder, navigator, monom_type> dlex_iterator;
  typedef CGenericIter<DegRevLexAscOrder, navigator, monom_type> 
  dp_asc_iterator;

  typedef CGenericIter<BlockDegLexOrder,  navigator, monom_type> 
  block_dlex_iterator;
  typedef CGenericIter<BlockDegRevLexAscOrder,  navigator, monom_type> 
  block_dp_asc_iterator;

  typedef CGenericIter<LexOrder, navigator, exp_type> lex_exp_iterator;
  typedef CGenericIter<DegLexOrder,  navigator, exp_type> dlex_exp_iterator;
  typedef CGenericIter<DegRevLexAscOrder,  navigator, exp_type> 
  dp_asc_exp_iterator;
  typedef CGenericIter<BlockDegLexOrder, navigator, exp_type> 
  block_dlex_exp_iterator;
  typedef CGenericIter<BlockDegRevLexAscOrder, navigator, exp_type> 
  block_dp_asc_exp_iterator;
  //@}

  /// Iterator type for iterating over all monomials
  typedef lex_iterator const_iterator;

  /// Iterator type for iterating all exponent vectors 
  typedef CExpIter<navigator, exp_type> exp_iterator;

  /// Iterator type for iterating all monomials (dereferencing to degree)
  typedef CGenericIter<LexOrder, navigator, deg_type> deg_iterator;

  /// Type for lists of terms
  typedef std::vector<monom_type> termlist_type;

  /// The property whether the equality check is easy is inherited from dd_type
  typedef dd_type::easy_equality_property easy_equality_property;

  /// Type for sets of Boolean variables
  typedef BooleSet set_type;

  /// Type for index maps
  typedef std::map<self, idx_type, symmetric_composition<
    std::less<navigator>, navigates<self> > > idx_map_type;
  typedef std::map<self, std::vector<self>, symmetric_composition<
    std::less<navigator>, navigates<self> > > poly_vec_map_type;

  //-------------------------------------------------------------------------
  // constructors and destructor
  //-------------------------------------------------------------------------

  /// Default constructor
  //  BoolePolynomial();

  /// Construct polynomial from a constant value 0 or 1
  //  explicit BoolePolynomial(constant_type);

  /// Construct zero polynomial
  BoolePolynomial(const ring_type& ring):
    m_dd(ring.zero() )  { }

  /// Construct polynomial in given @c ring  from a constant value 0 or 1
  BoolePolynomial(constant_type isOne, const ring_type& ring):
    m_dd(isOne? ring.one(): ring.zero() )  { }

  /// Construct polynomial from decision diagram
  BoolePolynomial(const dd_type& rhs): m_dd(rhs) {}

    /// Construct polynomial from a subset of the powerset over all variables
                 //  BoolePolynomial(const set_type& rhs): m_dd(rhs.diagram()) {}

  /// Construct polynomial from exponent vector
  BoolePolynomial(const exp_type&, const ring_type&); 

  /// Construct polynomial from navigator
  BoolePolynomial(const navigator& rhs, const ring_type& ring):
    m_dd(ring, rhs)  {
    PBORI_ASSERT(rhs.isValid());
  }

  /// Destructor
  ~BoolePolynomial() {}

  //-------------------------------------------------------------------------
  // operators and member functions
  //-------------------------------------------------------------------------

  //  self& operator=(const self& rhs) { 
  //  return m_dd = rhs.m_dd;
  // }

  self& operator=(constant_type rhs) { 
    return (*this) = self(rhs, ring());
  }
  /// @name Arithmetical operations
  //@{
  const self& operator-() const { return *this; }
  self& operator+=(const self&);
  self& operator+=(constant_type rhs) {

    //return *this = (self(*this) + (rhs).generate(*this));
    if (rhs) (*this) = (*this + ring().one());
     return *this;
  }
  template <class RHSType>
  self& operator-=(const RHSType& rhs) { return operator+=(rhs); }
  self& operator*=(const monom_type&);
  self& operator*=(const exp_type&);
  self& operator*=(const self&);
  self& operator*=(constant_type rhs) {
    if (!rhs) *this = ring().zero();
    return *this;
  }
  self& operator/=(const var_type&);
  self& operator/=(const monom_type&);
  self& operator/=(const exp_type&);
  self& operator/=(const self& rhs);
  self& operator/=(constant_type rhs);
  self& operator%=(const var_type&);
  self& operator%=(const monom_type&);
  self& operator%=(const self& rhs) { 
    return (*this) -= (self(rhs) *= (self(*this) /= rhs)); 
  }
  self& operator%=(constant_type rhs) { return (*this) /= (!rhs); }
  //@}

  /// @name Logical operations
  //@{
  bool_type operator==(const self& rhs) const { return (m_dd == rhs.m_dd); }
  bool_type operator!=(const self& rhs) const { return (m_dd != rhs.m_dd); }
  bool_type operator==(constant_type rhs) const { 
    return ( rhs? isOne(): isZero() );
  }
  bool_type operator!=(constant_type rhs) const {
    //return ( rhs? (!(isOne())): (!(isZero())) );
      return (!(*this==rhs));
  }
  //@}

  /// Check whether polynomial is constant zero
  bool_type isZero() const { return m_dd.isZero(); }

  /// Check whether polynomial is constant one 
  bool_type isOne() const { return m_dd.isOne(); }

  /// Check whether polynomial is zero or one
  bool_type isConstant() const { return m_dd.isConstant(); }

  /// Check whether polynomial has term one  
  bool_type hasConstantPart() const { return m_dd.ownsOne(); }

  /// Tests whether polynomial can be reduced by right-hand side
  bool_type firstReducibleBy(const self&) const;

  /// Get leading term
  monom_type lead() const;

  /// Get leading term w.r.t. lexicographical order
  monom_type lexLead() const;

  /// Get leading term (using upper bound of the polynomial degree)
  /** @note Implementation note: for degree orderings (dlex, dp_asc)
   *  returns the lead of the sub-polynomial of degree 'bound', 
   *  falls back to @c lead for all other orderings (lp, block_*) */
  monom_type boundedLead(deg_type bound) const;

  /// Get leading term
  exp_type leadExp() const;

  /// Get leading term (using upper bound of the polynomial degree)
  /// @note See implementation notes of @c boundedLead
  exp_type boundedLeadExp(deg_type bound) const;

  /// Get all divisors of the leading term
  set_type leadDivisors() const { return leadFirst().firstDivisors(); };
  
  /// Get unique hash value (may change from run to run)
  hash_type hash() const { return m_dd.hash(); }

  /// Get hash value, which is reproducible
  hash_type stableHash() const { return m_dd.stableHash(); } 

  /// Hash value of the leading term
  hash_type leadStableHash() const;
  
  /// Maximal degree of the polynomial
  deg_type deg() const;

  /// Degree of the leading term
  deg_type leadDeg() const;

  /// Degree of the leading term w.r.t. lexicographical ordering
  deg_type lexLeadDeg() const;

  /// Total maximal degree of the polynomial
  deg_type totalDeg() const;

  /// Total degree of the leading term
  deg_type leadTotalDeg() const;

  /// Get part of given degree
  self gradedPart(deg_type deg) const;

  /// Number of nodes in the decision diagram
  size_type nNodes() const;

  /// Number of variables of the polynomial
  size_type nUsedVariables() const;

  /// Set of variables of the polynomial
  monom_type usedVariables() const;

  /// Exponent vector of all of variables of the polynomial
  exp_type usedVariablesExp() const;

  /// Returns number of terms
  size_type length() const;

  /// Print current polynomial to output stream
  ostream_type& print(ostream_type&) const;

  /// Start of iteration over monomials
  const_iterator begin() const;

  /// Finish of iteration over monomials
  const_iterator end() const;

  /// Start of iteration over exponent vectors
  exp_iterator expBegin() const;

  /// Finish of iteration over exponent vectors
  exp_iterator expEnd() const;

  /// Start of first term
  first_iterator firstBegin() const;

  /// Finish of first term 
  first_iterator firstEnd() const;

  /// Get of first lexicographic term 
  monom_type firstTerm() const;

  /// Start of degrees
  deg_iterator degBegin() const;

  /// Finish of degrees
  deg_iterator degEnd() const;

  /// Start of ordering respecting iterator
  ordered_iterator orderedBegin() const; 

  /// Finish of ordering respecting iterator
  ordered_iterator orderedEnd() const;

   /// Start of ordering respecting exponent iterator
  ordered_exp_iterator orderedExpBegin() const; 

  /// Finish of ordering respecting exponent iterator
  ordered_exp_iterator orderedExpEnd() const;

  /// @name Compile-time access to generic iterators
  //@{
  lex_iterator genericBegin(lex_tag) const;
  lex_iterator genericEnd(lex_tag) const;
  dlex_iterator genericBegin(dlex_tag) const;
  dlex_iterator genericEnd(dlex_tag) const;
  dp_asc_iterator genericBegin(dp_asc_tag) const;
  dp_asc_iterator genericEnd(dp_asc_tag) const;
  block_dlex_iterator genericBegin(block_dlex_tag) const;
  block_dlex_iterator genericEnd(block_dlex_tag) const;
  block_dp_asc_iterator genericBegin(block_dp_asc_tag) const;
  block_dp_asc_iterator genericEnd(block_dp_asc_tag) const;


  lex_exp_iterator genericExpBegin(lex_tag) const;
  lex_exp_iterator genericExpEnd(lex_tag) const;
  dlex_exp_iterator genericExpBegin(dlex_tag) const;
  dlex_exp_iterator genericExpEnd(dlex_tag) const;
  dp_asc_exp_iterator genericExpBegin(dp_asc_tag) const;
  dp_asc_exp_iterator genericExpEnd(dp_asc_tag) const;
  block_dlex_exp_iterator genericExpBegin(block_dlex_tag) const;
  block_dlex_exp_iterator genericExpEnd(block_dlex_tag) const;
  block_dp_asc_exp_iterator genericExpBegin(block_dp_asc_tag) const;
  block_dp_asc_exp_iterator genericExpEnd(block_dp_asc_tag) const;
  //@}

  /// Navigate through structure
  navigator navigation() const { return m_dd.navigation(); }
 
  /// End of navigation marker
  navigator endOfNavigation() const { return navigator(); }
  
  /// gives a copy of the diagram
  dd_type copyDiagram(){   return diagram();  }

  /// Casting operator to Boolean set
  operator set_type() const { return set(); };

  size_type eliminationLength() const;
  size_type eliminationLengthWithDegBound(deg_type garantied_deg_bound) const;
  /// Get list of all terms
  void fetchTerms(termlist_type&) const;

  /// Return of all terms
  termlist_type terms() const;

  /// Read-only access to internal decision diagramm structure
  const dd_type& diagram() const { return m_dd; }

  /// Get corresponding subset of of the powerset over all variables
  set_type set() const { return m_dd; }

  /// Test, whether we have one term only
  bool_type isSingleton() const { return dd_is_singleton(navigation()); }

  /// Test, whether we have one or two terms only
  bool_type isSingletonOrPair() const { 
    return dd_is_singleton_or_pair(navigation()); 
  }

  /// Test, whether we have two terms only
  bool_type isPair() const { return dd_is_pair(navigation()); }

  /// Access ring, where this belongs to
  const ring_type& ring() const {  return m_dd.ring(); } 

  /// Compare with right-hand side and return comparision code
  comp_type compare(const self&) const;

  /// Check whether all variables are in one variable block
  bool_type inSingleBlock() const;

protected:
  /// Access to internal decision diagramm structure
  dd_type& internalDiagram() { return m_dd; }

  /// Generate a polynomial, whose first term is the leading term
  self leadFirst() const;

  /// Get all divisors of the first term
  set_type firstDivisors() const;

private:
  /// The actual decision diagramm
  dd_type m_dd;
};


/// Addition operation 
inline BoolePolynomial 
operator+(const BoolePolynomial& lhs, const BoolePolynomial& rhs) {

  return BoolePolynomial(lhs) += rhs;
}
/// Addition operation 
inline BoolePolynomial 
operator+(const BoolePolynomial& lhs, BooleConstant rhs) {
  return BoolePolynomial(lhs) +=  (rhs);
  //return BoolePolynomial(lhs) +=  BoolePolynomial(rhs);
}

/// Addition operation 
inline BoolePolynomial 
operator+(BooleConstant lhs, const BoolePolynomial& rhs) {

  return BoolePolynomial(rhs) += (lhs);
}


/// Subtraction operation 
template <class RHSType>
inline BoolePolynomial 
operator-(const BoolePolynomial& lhs, const RHSType& rhs) {

  return BoolePolynomial(lhs) -= rhs;
}
/// Subtraction operation with constant right-hand-side
inline BoolePolynomial 
operator-(const BooleConstant& lhs, const BoolePolynomial& rhs) {

  return -(BoolePolynomial(rhs) -= lhs);
}


/// Multiplication with other left-hand side type
#define PBORI_RHS_MULT(type) inline BoolePolynomial \
operator*(const BoolePolynomial& lhs, const type& rhs) { \
    return BoolePolynomial(lhs) *= rhs; }

PBORI_RHS_MULT(BoolePolynomial)
PBORI_RHS_MULT(BooleMonomial)
PBORI_RHS_MULT(BooleExponent)
PBORI_RHS_MULT(BooleConstant)


#undef PBORI_RHS_MULT

/// Multiplication with other left-hand side type
#define PBORI_LHS_MULT(type)  inline BoolePolynomial \
operator*(const type& lhs, const BoolePolynomial& rhs) { return rhs * lhs; }

PBORI_LHS_MULT(BooleMonomial)
PBORI_LHS_MULT(BooleExponent)
PBORI_LHS_MULT(BooleConstant)

#undef PBORI_LHS_MULT


/// Division by monomial (skipping remainder)
template <class RHSType>
inline BoolePolynomial
operator/(const BoolePolynomial& lhs, const RHSType& rhs){
  return BoolePolynomial(lhs) /= rhs;
}

/// Modulus monomial (division remainder)
template <class RHSType>
inline BoolePolynomial
operator%(const BoolePolynomial& lhs, const RHSType& rhs){

  return BoolePolynomial(lhs) %= rhs;
}

/// Equality check (with constant lhs)
inline BoolePolynomial::bool_type
operator==(BoolePolynomial::bool_type lhs, const BoolePolynomial& rhs) {

  return (rhs == lhs); 
}

/// Nonquality check (with constant lhs)
inline BoolePolynomial::bool_type
operator!=(BoolePolynomial::bool_type lhs, const BoolePolynomial& rhs) {

  return (rhs != lhs); 
}

/// Stream output operator
BoolePolynomial::ostream_type& 
operator<<(BoolePolynomial::ostream_type&, const BoolePolynomial&);

// tests whether polynomial can be reduced by rhs
inline BoolePolynomial::bool_type
BoolePolynomial::firstReducibleBy(const self& rhs) const {

  if( rhs.isOne() )
    return true;

  if( isZero() )
    return rhs.isZero();

  return std::includes(firstBegin(), firstEnd(), 
                       rhs.firstBegin(), rhs.firstEnd());

}


END_NAMESPACE_PBORI

#endif // of polybori_BoolePolynomial_h_
