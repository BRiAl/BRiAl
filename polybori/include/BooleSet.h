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
 * Revision 1.29  2007/07/31 07:43:50  dreyer
 * ADD: getBaseOrderCode(), lieInSameBlock(...), isSingleton(), isPair()...
 *
 * Revision 1.28  2007/05/25 12:35:32  dreyer
 * ADD: BooleSet::owns(const exp_type&) const
 *
 * Revision 1.27  2007/05/21 16:07:54  dreyer
 * CHANGE: temporarily deactivated some things with curious side-effects
 *
 * Revision 1.26  2007/05/20 09:44:40  dreyer
 * ADD: BooleSet(true/false)
 *
 * Revision 1.25  2007/05/19 08:43:03  dreyer
 * CHANGE: remove unnecessary casts
 *
 * Revision 1.24  2007/05/18 16:10:27  dreyer
 * CHANGE: term_accumulate optimized more
 *
 * Revision 1.23  2007/05/03 16:04:45  dreyer
 * CHANGE: new-style CTermIter integrated
 *
 * Revision 1.22  2006/12/07 08:22:52  dreyer
 * ADD/CHANGE: Lowlevel variant of existAbstract
 *
 * Revision 1.21  2006/11/24 14:49:00  dreyer
 * CHANGE: divisorsOf (less recursions/cache-lookups)
 *
 * Revision 1.20  2006/11/22 10:10:23  dreyer
 * ADD: dd_first_divisors_of
 *
 * Revision 1.19  2006/11/21 16:06:40  dreyer
 * CHANGE: inlining added
 *
 * Revision 1.18  2006/11/21 15:42:15  dreyer
 * ADD: Construct Booleset from navigator
 *
 * Revision 1.17  2006/11/21 12:33:33  dreyer
 * ADD: BooleSet::ownsOne, BooleSet(idx, navi, navi); divisorsof
 *
 * Revision 1.16  2006/11/20 16:18:07  dreyer
 * ADD: BooleSet new node-constructor, also in dd_multiply_recursively
 *
 * Revision 1.15  2006/10/04 15:46:49  dreyer
 * ADD: divisorsOf(exp_type), orderedExpBegin/End;
 * CHANGE: Polynomials printing respects ordering
 *
 * Revision 1.14  2006/09/14 10:57:25  dreyer
 * ADD: usedVariablesExp()
 *
 * Revision 1.13  2006/09/05 14:21:01  bricken
 * +multiplesOf, tailVariables to Exponent
 *
 * Revision 1.12  2006/08/29 08:02:08  dreyer
 * ADD: BooleSet::expBegin() and expEnd()
 *
 * Revision 1.11  2006/08/23 14:24:53  dreyer
 * ADD: BooleSet::usedVariables and infrastructure
 *
 * Revision 1.10  2006/08/22 16:06:22  dreyer
 * + Added highlevel division
 *
 * Revision 1.9  2006/08/17 15:35:30  dreyer
 * ADD: extended and activated low-level version of dd_minimal_elements
 *
 * Revision 1.8  2006/08/15 14:17:29  dreyer
 * ADD minimalElements(), hasTermOfVariables()
 *
 * Revision 1.7  2006/08/03 15:20:20  dreyer
 * ADD: BooleSet::divisorsOf and BooleSet::hasCommonVariables
 *
 * Revision 1.6  2006/06/07 08:37:50  dreyer
 * ADD CCuddLastIter and BooleSet::lastLexicographicalTerm()
 *
 * Revision 1.5  2006/04/25 09:30:42  dreyer
 * FIX end of CTermIterm for constants, CHANGE consistent functional names
 *
 * Revision 1.4  2006/04/21 09:31:44  dreyer
 * CHANGE reducing redundancy
 *
 * Revision 1.3  2006/04/21 07:11:49  dreyer
 * ADD operator=
 *
 * Revision 1.2  2006/04/20 16:59:47  dreyer
 * routine
 *
 * Revision 1.1  2006/04/20 08:31:21  dreyer
 * ADD BooleMonomial::mulples(...)
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

// include definitions of decision diagram interfaces
#include "CDDInterface.h"

// include polybori functionals
#include "pbori_func.h"



#ifndef BooleSet_h_
#define BooleSet_h_

BEGIN_NAMESPACE_PBORI

/// Forward declaration of monomial type
class BooleMonomial;
class BooleExponent;

template<class OrderType, class NavigatorType, class MonomType>
class CGenericIter;
// temporarily
class LexOrder;

//template<class OrderType, class NavigatorType, class MonomType>
//class CGenericIter;

class BooleSet:
  public CTypes::dd_type {

public:
  /// Generic access to type of *this
  typedef BooleSet self;

  /// Generic access to base type
  typedef CTypes::dd_type base;

  /// Generic access to underlying diagram type
  typedef base dd_type;

  /// Type of terms
  typedef BooleMonomial term_type;

  /// Fix type for treatment of exponent vectors
  typedef BooleExponent exp_type; 

  /// Iterator type for iterating all monomials
//   typedef CTermIter<term_type, navigator, 
//                     change_assign<>,
//                     change_assign<> >
//   const_iterator;
  typedef CGenericIter<LexOrder, navigator, term_type> const_iterator;

  /// Iterator type for iterating all exponent vectors 
  typedef CGenericIter<LexOrder, navigator, exp_type> exp_iterator;
//  typedef CTermIter<exp_type, navigator, 
//                     inserts<>, 
//                     removes<>, project_ith<1> >
//   exp_iterator;

  /// Default constructor
  BooleSet();

  /// Copy constructor
  BooleSet(const self& rhs): base(rhs) {}

  /// Copy constructor
  BooleSet(const base& rhs): base(rhs) {}

  /// Construct new node
  BooleSet(idx_type idx, const self& first, const self& second):
    base(idx, first, second) {}

  /// Construct new node (using navigator nodes)
  BooleSet(idx_type idx, navigator, navigator);

  /// Construct new node (using navigator for then and else-branches)
  BooleSet(idx_type idx, navigator);

  /// Construct one or zero set from constant
  //  BooleSet(bool_type);
  /// @todo  temporarily deactivated, as it slow downs procedures like
  /// term_accumulate, needs check, what happens to inlinings etc. in this case

  /// Construct from navigator node
  BooleSet(navigator);

  /// Destructor
  ~BooleSet() {}

  /// Start of iteration over terms
  const_iterator begin() const;

  /// Finish of iteration over terms
  const_iterator end() const;

  /// Start of iteration over exponent vectors
  exp_iterator expBegin() const;

  /// Finish of iteration over exponent vectors
  exp_iterator expEnd() const;

  /// Assignment operator
  self& operator=(const self&);

  /// Use assignment operator of base element
  using base::operator=;

  /// Set of variables of the whole set
  term_type usedVariables() const;

  /// Exponent vector of variables of the whole set
  exp_type usedVariablesExp() const;

  /// Add given monomial to sets and assign
  self& addAssign(const term_type&);

  /// Add given monomial to sets 
  self add(const term_type&) const;

  /// Check whether rhs is included in *this
  bool_type owns(const term_type&) const;

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

  /// Division with assignment by given term
  self& divideAssign(const term_type& rhs);

  /// Check for empty intersection with divisors of rhs
  bool_type hasTermOfVariables(const term_type& rhs) const;

  /// Get minimal elements wrt. inclusion
  self minimalElements() const;// { return base::minimalElements(); };

  /// Test whether the empty set is included
  using base::ownsOne;

  /// Test, whether we have one term only
  bool_type isSingleton() const { return dd_is_singleton(navigation()); }

  /// Test, whether we have one or two terms only
  bool_type isSingletonOrPair() const { 
    return dd_is_singleton_or_pair(navigation()); 
  }

  /// Test, whether we have two terms only
  bool_type isPair() const { return dd_is_pair(navigation()); }

  /// Compute existential abstraction
  self existAbstract(const self& rhs) const;

  /// Access internal decision diagram
  const dd_type& diagram() const { return dynamic_cast<const dd_type&>(*this); }
};


END_NAMESPACE_PBORI

#endif
