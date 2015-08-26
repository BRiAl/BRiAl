// -*- c++ -*-
//*****************************************************************************
/** @file BooleSet.h
 *
 * @author Alexander Dreyer
 * @date 2006-04-20
 *
 * This file defines the class BooleSet, which is currently just a typedef.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_BooleSet_h_
#define polybori_BooleSet_h_

// include basic definitions
#include <polybori/pbori_defs.h>

// include polybori functionals
#include <polybori/routines/pbori_func.h>

#include <polybori/diagram/CCuddDDFacade.h>

#include "BoolePolyRing.h"

BEGIN_NAMESPACE_PBORI

/// Forward declaration of monomial type
class BooleMonomial;
class BooleExponent;

template<class OrderType, class NavigatorType, class MonomType>
class CGenericIter;
// temporarily
class LexOrder;

template<class OrderType, class NavigatorType, class MonomType>
class CReverseIter;


#define PBORI_CONST_DDFUNCS(func)   \
  self func(const self& rhs) const { return self(base::func(rhs.diagram())); }

#define PBORI_DDFUNCS(func) \
  self& func(const self& rhs) { base::func(rhs.diagram()); return *this; }

#define PBORI_CONST_DDFUNCS_IDX(func) \
  self func(idx_type idx) const { return self(base::func(idx)); }

#define PBORI_DDFUNCS_IDX(func) \
  self& func(idx_type idx) { base::func(idx); return *this; }


class BooleSet:
  public CCuddDDFacade<BoolePolyRing, BooleSet> {
  /// Generic access to type of *this
  typedef BooleSet self;

public:
  /// Generic access to type of *this
  typedef self dd_type;

  /// Generic access to base type
  typedef CCuddDDFacade<BoolePolyRing, BooleSet> base;

  /// Type of terms
  typedef BooleMonomial term_type;

  /// Fix type for treatment of exponent vectors
  typedef BooleExponent exp_type; 

  /// Type for Boolean polynomial rings (without ordering)
  typedef BoolePolyRing ring_type;

  /// Iterator type for iterating all monomials
  typedef CGenericIter<LexOrder, navigator, term_type> const_iterator;

  /// Iterator type for iterating all exponent vectors 
  typedef CGenericIter<LexOrder, navigator, exp_type> exp_iterator;

  /// Iterator type for iterating all exponent vectors 
  typedef CReverseIter<LexOrder, navigator, exp_type> reverse_exp_iterator;

  /// Iterator type for iterating all monomials
  typedef CReverseIter<LexOrder, navigator, term_type> const_reverse_iterator;

public:
  /// Copy constructor
  BooleSet(const self& rhs): base(rhs) {}

  /// Copy constructor
  BooleSet(const base& rhs): base(rhs) {}

  /// Construct new node
  BooleSet(idx_type idx, const self& first, const self& second):
    base(idx, first, second) { }

  /// Construct new node (using navigator nodes)
  BooleSet(idx_type idx, navigator first, navigator second, 
           const ring_type& ring): 
    base(ring, idx, first, second) { }
 
  /// Construct new node (using nodes)
  BooleSet(const ring_type& ring, node_ptr node): 
    base(ring, node) { }

  /// Construct new node (using navigator nodes)
  BooleSet(const ring_type& ring, navigator navi): 
    base(ring, navi.getNode()) { }

  /// Construct empty set in ring
  BooleSet(const ring_type& ring): 
    base(ring.zero()) { };

  /// Construct new node (using navigator for then and else-branches)
  BooleSet(idx_type idx, const self& rhs):
    base(idx, rhs, rhs) { }

  /// Construct from navigator node
  BooleSet(navigator navi, const ring_type& ring):
    base(ring, navi) { }

  /// Destructor
  ~BooleSet() {}

  /// Start of iteration over terms
  const_iterator begin() const;

  /// Finish of iteration over terms
  const_iterator end() const;

  /// Start of backward iteration over terms
  const_reverse_iterator rbegin() const;

  /// Finish of backward iteration over terms
  const_reverse_iterator rend() const;

  /// Start of backward exponent iteration over terms
  reverse_exp_iterator rExpBegin() const;

  /// Finish of backward iteration over terms
  reverse_exp_iterator rExpEnd() const;

  /// Start of iteration over exponent vectors
  exp_iterator expBegin() const;

  /// Finish of iteration over exponent vectors
  exp_iterator expEnd() const;

  /// Get unique hash value (valid only per runtime)
  hash_type hash() const { 
    return static_cast<hash_type>(reinterpret_cast<std::ptrdiff_t>(getNode()));
  }

  /// Get stable hash value, which is reproducible
  hash_type stableHash() const { 
    return stable_hash_range(navigation());
  }

  /// Set of variables of the whole set
  term_type usedVariables() const;

  /// Exponent vector of variables of the whole set
  exp_type usedVariablesExp() const;

  self change(idx_type idx) const {
    if PBORI_UNLIKELY(size_type(idx) >= ring().nVariables())
      throw PBoRiError(CTypes::out_of_bounds);
    return base::change(idx);
  }


  /// Add given monomial to sets
  self add(const term_type& rhs) const;

  // Check whether rhs is included in *this
  bool_type owns(const term_type& rhs) const;

  /// Check whether rhs is included in *this
  bool_type owns(const exp_type&) const;

  /// Get last term (wrt. lexicographical order)
  term_type lastLexicographicalTerm() const; 

  /// Compute intersection with divisors of rhs
  self divisorsOf(const term_type& rhs) const;
  
   /// Compute intersection with divisors of rhs
  self divisorsOf(const exp_type& rhs) const;

   /// Intersection with divisors of first (lexicographical) term of rhs
  self firstDivisorsOf(const self& rhs) const;

 /// Compute intersection with multiples of rhs
  self multiplesOf(const term_type& rhs) const;
  
  /// Division by given term
  self divide(const term_type& rhs) const;

  /// Check for empty intersection with divisors of rhs, i.e. test whether there
  /// are terms made of the given variables.
  bool_type hasTermOfVariables(const term_type& rhs) const;

  /// Get minimal elements wrt. inclusion
  self minimalElements() const;

  /// Test whether the empty set is included
  bool_type ownsOne() const { return owns_one(navigation()); }

  /// Test, whether we have one term only
  bool_type isSingleton() const { return dd_is_singleton(navigation()); }

  /// Test, whether we have one or two terms only
  bool_type isSingletonOrPair() const { 
    return dd_is_singleton_or_pair(navigation()); 
  }

  /// Test, whether we have two terms only
  bool_type isPair() const { return dd_is_pair(navigation()); }

  /// Compute existential abstraction:
  /// Given a diagram @c F, and a set of variables @c S, remove all occurrences
  /// of each @c s in @c S from any subset in @c F. This can be implemented by 
  /// cofactoring @c F w.r.t. @c s = 1 and @c s = 0, then forming their union
  self existAbstract(const term_type& rhs) const;

  /// Access internal decision diagram
  const self& diagram() const { return *this; }  //  to be removed

  /// Cartesean product
  self cartesianProduct(const self& rhs) const {
    return unateProduct(rhs);
  };

  /// Test containment
  bool_type contains(const self& rhs) const { return rhs.implies(*this); }

  /// Returns number of terms
  size_type size() const { return count(); }

  /// Returns number of terms (deprecated)
  size_type length() const { return size(); }

  /// Returns number of variables in manager
  size_type nVariables() const { return ring().nVariables(); }

  /// Approximation of number of terms
  double sizeDouble() const { return countDouble(); }

  /// Print current set to output stream
  ostream_type& print(ostream_type&) const;

  /// Get corresponding zero element (may be removed in the future)
  self emptyElement() const { return ring().zero(); }

  /// Count terms containing BooleVariable(idx)
  size_type countIndex(idx_type idx) const;

  /// Count many terms containing BooleVariable(idx)
  double countIndexDouble(idx_type idx) const ;

  /// Test whether, all divisors of degree -1 of term rhs are contained in this
  bool_type containsDivisorsOfDecDeg(const term_type& rhs) const;

  /// Test for term corresponding to exponent rhs
  bool_type containsDivisorsOfDecDeg(const exp_type& rhs) const;
};

/// Stream output operator
inline BooleSet::ostream_type& 
operator<<( BooleSet::ostream_type& os, const BooleSet& bset ) {
  return bset.print(os);
}


END_NAMESPACE_PBORI

#endif
