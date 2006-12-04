// -*- c++ -*-
//*****************************************************************************
/** @file 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file implements the class BoolePolymials, where carries the definition
 * of polynomials over Booleans.
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
 * Revision 1.80  2006/12/04 17:08:19  dreyer
 * CHANGE: multiplication in new style
 *
 * Revision 1.79  2006/11/27 16:25:14  dreyer
 * CHANGE: CDegreeCache, now inherited from standard cache; dlex-lead cached
 *
 * Revision 1.78  2006/11/21 16:06:40  dreyer
 * CHANGE: inlining added
 *
 * Revision 1.77  2006/11/21 12:33:34  dreyer
 * ADD: BooleSet::ownsOne, BooleSet(idx, navi, navi); divisorsof
 *
 * Revision 1.76  2006/11/21 09:52:05  dreyer
 * CHANGE: some simple functions in BoolePolynomial inlined
 * ADD: caching of ternary operations
 * ADD: commandline switch PBORI_FAST_MULTIPLICATION (dense multiplication)
 *
 * Revision 1.75  2006/11/20 14:56:46  dreyer
 * CHANGE CCacheType names, operator*=, CDDInterface node Constructor
 *
 * Revision 1.74  2006/10/24 14:21:56  dreyer
 * ADD: variable_name functional
 *
 * Revision 1.73  2006/10/23 16:05:55  dreyer
 * ADD: BoolePolyRing::set/get(Ring)VariableName()
 *
 * Revision 1.72  2006/10/05 07:33:58  dreyer
 * ADD: BoolePolynomial::genericExpBegin()/End()
 *
 * Revision 1.71  2006/10/04 15:46:49  dreyer
 * ADD: divisorsOf(exp_type), orderedExpBegin/End;
 * CHANGE: Polynomials printing respects ordering
 *
 * Revision 1.70  2006/10/04 13:09:56  dreyer
 * ADD: added compile-time optimied iterators and genericBegin/genericEnd
 *
 * Revision 1.69  2006/10/02 07:41:28  bricken
 * + adjustments to deg order
 *
 * Revision 1.68  2006/09/21 16:09:59  dreyer
 * ADD: caching mechanism for BoolePolynomial::deg()
 *
 * Revision 1.67  2006/09/20 07:06:40  dreyer
 * ADD BoolePolynomial/CDDInterface::isConstant(), used it in deg()
 *
 * Revision 1.66  2006/09/14 10:57:26  dreyer
 * ADD: usedVariablesExp()
 *
 * Revision 1.65  2006/09/13 15:07:05  dreyer
 * ADD: lead(sugar) and infrastructure
 *
 * Revision 1.64  2006/09/13 09:05:44  dreyer
 * ADD: dp_asc/DegRevLexAscOrder
 * ADD: BoolePolynomial::endOfNavigation()
 * CHANGE: BoolePolynomial: removed biDegBegin(), biDegEnd(), which can be
 *   generated more generically using navigation() and endOfNavigation().
 *
 * Revision 1.63  2006/09/08 16:15:28  dreyer
 * ADD: Added ordering-dependent term iteration
 *
 * Revision 1.62  2006/09/05 14:59:07  dreyer
 * CHANGE Code cleanup
 *
 * Revision 1.61  2006/09/05 14:39:39  dreyer
 * CHANGE: enum sp now called dp_asc, BoolePolynomial::reducbileBy() generic
 *
 * Revision 1.60  2006/09/04 11:33:57  dreyer
 * CHANGE: lm*() use leadFirst()
 *
 * Revision 1.59  2006/09/01 10:35:26  dreyer
 * ADD: Multiplication poly * poly, poly * exponent
 *
 * Revision 1.58  2006/08/29 12:09:27  dreyer
 * using CDDOperations for generic dd functions
 *
 * Revision 1.57  2006/08/29 10:37:56  dreyer
 * CHANGE: non-const version of diagram() now internalDiagram()
 *
 * Revision 1.56  2006/08/29 09:02:36  dreyer
 * ADD: leadExp()
 *
 * Revision 1.55  2006/08/24 14:47:50  dreyer
 * ADD: BooleExponent integrated, FIX: multiples (for indices < first)
 *
 * Revision 1.54  2006/08/23 14:24:54  dreyer
 * ADD: BooleSet::usedVariables and infrastructure
 *
 * Revision 1.53  2006/08/22 16:06:22  dreyer
 * + Added highlevel division
 *
 * Revision 1.52  2006/08/01 11:16:01  dreyer
 * CHANGE: Bug fixed, uninitialized *min_element() (valgrind)
 *
 * Revision 1.51  2006/07/31 11:48:53  dreyer
 * ADD: lowlevel implementation for multiples and lmDivisors
 *
 * Revision 1.50  2006/07/18 08:47:11  bricken
 * tuning/optimization
 *
 * Revision 1.49  2006/07/17 15:32:08  dreyer
 * ADD: BoolePolynomial::hasConstantPart, BooleMonomial::isOne, isZero
 *
 * Revision 1.48  2006/07/06 16:01:30  dreyer
 * CHANGE: Functionals ins pbori_func.h made more consistent
 *
 * Revision 1.47  2006/07/04 14:11:03  dreyer
 * ADD: Generic and handy treatment of string literals
 *
 * Revision 1.46  2006/06/07 11:54:26  dreyer
 * ADD variantes for usedVariables
 *
 * Revision 1.45  2006/06/06 10:56:59  dreyer
 * CHANGE usedVariables() more efficient now.
 *
 * Revision 1.44  2006/05/24 13:29:25  dreyer
 * FIX usedVariables corrected
 *
 * Revision 1.43  2006/05/24 11:57:51  dreyer
 * CHANGE alternative modulus operation
 *
 * Revision 1.42  2006/05/24 08:01:01  dreyer
 * ADD operator % and %=
 *
 * Revision 1.41  2006/05/24 06:38:22  bricken
 * + corrected type for usedVariables
 *
 * Revision 1.40  2006/05/23 15:26:25  dreyer
 * CHANGE BoolePolyRing  can handle different orderings (only lex yet)
 *
 * Revision 1.39  2006/05/05 11:28:13  bricken
 * *bricken: improving crit, new examples
 *
 * Revision 1.38  2006/05/03 10:37:03  bricken
 * + work on pseudo parallel reductions
 *
 * Revision 1.37  2006/04/25 09:30:42  dreyer
 * FIX end of CTermIterm for constants, CHANGE consistent functional names
 *
 * Revision 1.36  2006/04/24 16:58:53  bricken
 * + inverted comparison operator
 *
 * Revision 1.35  2006/04/24 15:00:48  dreyer
 * CHANGE BoolePolynomial::eliminationLength() uses lmDeg() now
 *
 * Revision 1.34  2006/04/24 14:45:36  dreyer
 * FIX CTermIter; ADD BoolePolynomial uses CTermIter
 *
 * Revision 1.33  2006/04/24 10:23:22  dreyer
 * ADD BoolePolynomial::begin() and end()
 * FIX type reference in CCuddNavigator
 *
 * Revision 1.32  2006/04/21 16:17:11  dreyer
 * ADD template class CTermIter<>
 *
 * Revision 1.31  2006/04/21 13:13:30  dreyer
 * ADD PBoRiOutITer for more generic manipulations
 *
 * Revision 1.30  2006/04/20 13:30:11  bricken
 * *bricken: improved readibility
 *
 * Revision 1.29  2006/04/20 08:31:21  dreyer
 * ADD BooleMonomial::mulples(...)
 *
 * Revision 1.28  2006/04/19 15:55:53  dreyer
 * ADD BooleMonomial, BoolePolynomial::fetchTerms() and ::terms()
 *
 * Revision 1.27  2006/04/13 08:41:34  dreyer
 * CHANGE change() used by BoolePolynomial backward (for efficiency)
 *
 * Revision 1.26  2006/04/13 07:53:19  dreyer
 * CHANGE BoolePolynomial::print() and deg() produces more useful results
 *
 * Revision 1.25  2006/04/12 16:23:54  dreyer
 * ADD template class CIDXPath<>
 *
 * Revision 1.24  2006/04/12 10:33:01  bricken
 * *bricken: elimination length
 *
 * Revision 1.23  2006/04/10 14:38:39  dreyer
 * FIX operator*= works for nontrivial lhs
 *
 * Revision 1.22  2006/04/06 14:10:58  dreyer
 * ADD hash_type and CCuddNavigator::hash()
 *
 * Revision 1.21  2006/04/06 13:54:58  dreyer
 * ADD BoolePolynomial::length()
 *
 * Revision 1.20  2006/04/05 15:26:04  dreyer
 * CHANGE: File access of BoolePolynomial::prettyPrint moved to CDDInterface
 *
 * Revision 1.19  2006/04/05 14:56:38  dreyer
 * ADD prettyPrint functions for dot-output to stdout or file
 *
 * Revision 1.18  2006/04/04 15:31:06  dreyer
 * ADD: BoolePolynomial::navigator() and corresp. class CCuddNavigator
 *
 * Revision 1.17  2006/04/04 12:07:37  dreyer
 * ADD BoolePolynomial::reducibleby(), and firstBegin(), firstEnd()
 *
 * Revision 1.16  2006/04/04 12:00:01  bricken
 * + hash function
 *
 * Revision 1.15  2006/04/04 11:21:22  dreyer
 * ADD lmDivisors() added
 *
 * Revision 1.14  2006/04/04 07:36:35  dreyer
 * ADD: tests isZero(), isOne() and poly == bool, bool == poly
 *
 * Revision 1.13  2006/03/30 11:57:11  dreyer
 * CHANGE: Made use of 0/1 constants and the sets {}, {{}} consistent
 *
 * Revision 1.12  2006/03/30 08:59:42  dreyer
 * FIX: CCuddFirstIter works for empty and zero polynomials now
 *
 * Revision 1.11  2006/03/29 16:26:46  dreyer
 * ADD: Class CCuddFirstIter used for BoolePolynomial::lead()
 *
 * Revision 1.10  2006/03/27 15:02:43  dreyer
 * ADD: BoolePolynomial::operator/=(const self&) and spoly
 *
 * Revision 1.9  2006/03/27 13:47:58  dreyer
 * ADD operator + and *, CHANGE BoolePolyRing::variable(i) generation
 *
 * Revision 1.8  2006/03/24 16:15:15  dreyer
 * CHANGE: (n)usedVariables() now uses Cudd-internal commands
 * ADD: CDDInterface<> support() and nSupport() (for above)
 *
 * Revision 1.7  2006/03/24 15:02:44  dreyer
 * ADD: Reference to manager_type can also be used for CDDManager<> -nterface
 * ADD: lead(), (n)usedVariables(), lmDeg() implemented in BoolePolynomial
 *
 * Revision 1.6  2006/03/23 17:15:04  dreyer
 * ADD: lead() and lmdeg() functionality to BoolePolynomial,
 * BoolePolyRing(const manager_type &); leading term exampl.
 *
 * Revision 1.5  2006/03/22 08:06:59  dreyer
 * ADD: Template specializations CDDInterface<ZDD>, CDDManager<Cudd>; 
 * ring uses shared_ptr now
 *
 * Revision 1.4  2006/03/20 14:51:01  dreyer
 * CHANGE: Use CDDInterface temple specializations instead of raw dd_type
 *
 * Revision 1.3  2006/03/20 12:11:57  dreyer
 * CHANGE: Revised *deg*() functions.
 *
 * Revision 1.2  2006/03/17 16:53:37  dreyer
 * ADD added nNodes(), operator*= to BoolePolynomial
 *
 * Revision 1.1  2006/03/16 17:09:13  dreyer
 * ADD BoolePolynial functionality started
 *
 * @endverbatim
**/
//*****************************************************************************

#include <list>
#include <set>
#include <iterator>
#include <algorithm>
#include <numeric>

#include "pbori_algo.h"
#include "CIdxPath.h"

#define PBORI_USE_CCUDDFIRSTITER

// load header file
# include "BoolePolynomial.h"
# include "BooleMonomial.h"
# include "BooleExponent.h"

// get polynomial riung definition
# include "BoolePolyRing.h"

// get error types
# include "PBoRiError.h"

// get 
# include "CIdxPath.h"

// get transformation algorithms
# include "pbori_algo.h"

// get functionals
# include "pbori_func.h"

// include definition of output iterator over monomials
//# include "OutMonomIter.h"
# include "PBoRiOutIter.h"

# include "OrderedManager.h"

# include "COrderedIter.h"

// include definition of generic string literals
# include "CStringLiteral.h"

// include definition of generic print operation
# include "CPrintOperation.h"

// get internal routines
# include "pbori_routines.h"
# include "CDDOperations.h"
# include "CDegreeCache.h"

#include "CDelayedTermIter.h"
#include "CGenericIter.h"



BEGIN_NAMESPACE_PBORI

//-------------------------------------------------------------------------
// Constructors and destructor
//------------------------------------------------------------------------

// Default constructor
BoolePolynomial::BoolePolynomial():
  m_dd( BoolePolyRing::ringZero() ) {

  PBORI_TRACE_FUNC( "BoolePolynomial()" );
}

// Construct polynomial from a constant value 0 or 1
BoolePolynomial::BoolePolynomial(bool_type isOne):
  m_dd(isOne? BoolePolyRing::ringOne() : BoolePolyRing::ringZero() )  {

  PBORI_TRACE_FUNC( "BoolePolynomial(bool_type)" );
}

// Constructor polynomial from existing decision diagram
BoolePolynomial::BoolePolynomial(const dd_type& rhs):
  m_dd(rhs)  {

  PBORI_TRACE_FUNC( "BoolePolynomial(const dd_type&)" );
}

// Constructor polynomial from exponent vector
BoolePolynomial::BoolePolynomial(const exp_type& rhs):
  m_dd( BoolePolyRing::ringOne() )  {

  PBORI_TRACE_FUNC( "BoolePolynomial(const exp_type&)" );

  exp_type::const_reverse_iterator start(rhs.rbegin()), finish(rhs.rend());

  while(start != finish) {
    m_dd.changeAssign(*start);
    ++start;
  }
}

// Copy constructor
BoolePolynomial::BoolePolynomial(const self& rhs) :
  m_dd(rhs.m_dd) {
  
  PBORI_TRACE_FUNC( "BoolePolynomial(const self&)" );
}

//-------------------------------------------------------------------------
// operators and member functions
//------------------------------------------------------------------------


// Addition
BoolePolynomial&
BoolePolynomial::operator+=(const self& rhs) {

  PBORI_TRACE_FUNC( "BoolePolynomial::operator+=(const self&)" );

  dd_add_assign<dd_type>()(m_dd, rhs.m_dd);
  return *this;
}

// Multiplication
BoolePolynomial&
BoolePolynomial::operator*=(const monom_type& rhs) {

  PBORI_TRACE_FUNC( "BoolePolynomial::operator*=(const monom_type&)" );

  // store indices in list
  CIdxPath<idx_type> indices(rhs.deg());
  
  // iterator, which generates m_dd *= var(idx) for given index idx
  // on assignment of dereferenced object
  PBoRiOutIter<dd_type, idx_type, times_indexed_var<dd_type> >  
    outiter(m_dd) ;
  
  // insert backward (for efficiency reasons)
  reversed_inter_copy(rhs.begin(), rhs.end(), indices, outiter);

  return *this;
}

// Multiplication
BoolePolynomial&
BoolePolynomial::operator*=(const exp_type& rhs) {

  PBORI_TRACE_FUNC( "BoolePolynomial::operator*=(const exp_type&)" );

  // iterator, which generates m_dd *= var(idx) for given index idx
  // on assignment of dereferenced object
  PBoRiOutIter<dd_type, idx_type, times_indexed_var<dd_type> >  
    outiter(m_dd) ;
  
  // insert backward (for efficiency reasons)
  std::copy(rhs.rbegin(), rhs.rend(), outiter);

  return *this;
}

// Multiplication
BoolePolynomial&
BoolePolynomial::operator*=(const self& rhs) {

  PBORI_TRACE_FUNC( "BoolePolynomial::operator*=(const self&)" );


  //return *this = dd_multiply_recursively(*this, rhs);
  //  self result = dd_multiply_recursively(*this, rhs);
  typedef CommutativeCacheManager<CCacheTypes::multiply_recursive>
    cache_mgr_type;

  self result = dd_multiply_recursively(cache_mgr_type(diagram().manager()), 
                                        navigation(), rhs.navigation(), self());

#if 0

  self result(0);

  exp_iterator start(rhs.expBegin()), finish(rhs.expEnd());

  while (start != finish) {

    exp_iterator myStart(expBegin()), myFinish(expEnd());

    while(myStart != myFinish) {
      result += ( (*myStart).multiply(*start) );
      ++myStart;
    }
    ++start;
  }

#endif

  return (*this = result);
}


// Division
BoolePolynomial&
BoolePolynomial::operator/=(const monom_type& rhs) {

  PBORI_TRACE_FUNC( "BoolePolynomial::operator/=(const monom_type&)" );

  m_dd.divideFirstAssign(rhs.diagram());

  return *this;
}

// Modulus
BoolePolynomial&
BoolePolynomial::operator%=(const monom_type& rhs) {

  PBORI_TRACE_FUNC( "BoolePolynomial::operator%=(const monom_type&)" );

  m_dd.diffAssign(  rhs.diagram().support() );

  return *this;
}

// tests whether polynomial can be reduced by rhs
BoolePolynomial::bool_type
BoolePolynomial::reducibleBy(const self& rhs) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::reducibleBy(const self&) const" );

  if( rhs.isOne() )
    return true;

  if( isZero() )
    return rhs.isZero();

  return std::includes(firstBegin(), firstEnd(), 
                       rhs.firstBegin(), rhs.firstEnd());

}

// Equality
BoolePolynomial::bool_type
BoolePolynomial::operator==(const self& rhs) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::operator==(const self&) const" );

  return (m_dd == rhs.m_dd);
}

// Nonequality
BoolePolynomial::bool_type
BoolePolynomial::operator!=(const self& rhs) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::operator!=(const self&) const" );

  return (m_dd != rhs.m_dd);
}

// Equality (compare with constant polynomial)
BoolePolynomial::bool_type
BoolePolynomial::operator==(bool_type rhs) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::operator==(bool_type) const" );

  return ( rhs? isOne() : isZero() );
}

// Nonequality (compare with constant polynomial)
BoolePolynomial::bool_type
BoolePolynomial::operator!=(bool_type rhs) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::operator!=(bool_type) const" );

  return ( rhs? isZero() : isOne() );
}

// Leading term
BoolePolynomial::monom_type
BoolePolynomial::lead() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::lead() const" );

  return BoolePolyRing::activeManager().lead(*this);

//   // Note: implementation relying on CCuddFirstIter 
//   monom_type leadterm;

//   if (m_dd.emptiness())
//     leadterm = 0;
//   else {

//     // store indices in list
//     CIdxPath<idx_type> indices(lmDeg());

//     // iterator, which uses changeAssign to insert variable
//     // wrt. given indices to a monomial
//     PBoRiOutIter<monom_type, idx_type, change_assign<monom_type> >  
//       outiter(leadterm) ;
    
//     // insert backward (for efficiency reasons)
//     reversed_inter_copy(firstBegin(), firstEnd(), indices, outiter);
//   }

//   return leadterm;
}
// Leading term (bound)
BoolePolynomial::monom_type
BoolePolynomial::boundedLead(size_type bound) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::lead(size_type) const" );

  return BoolePolyRing::activeManager().lead(*this, bound);
}

// Leading exponent
BoolePolynomial::exp_type
BoolePolynomial::leadExp() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::leadExp() const" );

  return BoolePolyRing::activeManager().leadExp(*this);
}

// Leading exponent (bound)
BoolePolynomial::exp_type
BoolePolynomial::boundedLeadExp(size_type bound) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::leadExp(size_type) const" );

  return BoolePolyRing::activeManager().leadExp(*this, bound);
}


// all dividers
BoolePolynomial::set_type
BoolePolynomial::firstDivisors() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::lmDivisors() const" );

#ifdef PBORI_DIVISORS_HIGHLEVEL

  // Implementation relying on CCuddFirstIter (may be buggy)
  set_type terms;

  if (m_dd.emptiness())
    terms = m_dd;
  else {
    terms = manager_reference(m_dd).blank();

    // store indices in list
    CIdxPath<idx_type> indices(lmDeg());

    // define iterator, which uses appends the divisors wrt. a Boolean
    // variable of given index
    PBoRiOutIter<dd_type, idx_type, append_indexed_divisor<dd_type> >  
      outiter(terms);
    
    // insert backward (for efficiency reasons)
    reversed_inter_copy(firstBegin(), firstEnd(), indices, outiter);
  }

  return terms;
#else

  return m_dd.firstDivisors();
#endif 
}

// hash value of lm
BoolePolynomial::hash_type 
BoolePolynomial::lmHash() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::lmHash() const" );

  if (m_dd.emptiness())
    return 0;
  else {
    self ld1st(leadFirst());
    return generic_sequence_hash<first_iterator, hash_type, 
      pbori_hash_tag>()( ld1st.firstBegin(), ld1st.firstEnd() );
  }
    
}





// Maximal degree of the polynomial
BoolePolynomial::size_type
BoolePolynomial::deg() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::deg() const" );
  
  /// @todo: This is currently just brute force + caching, 
  /// more efficient search may be needed.

#ifndef PBORI_NO_DEGCACHE
  return dd_cached_degree(CDegreeCache<>(m_dd.manager()), navigation());
#else
  return ( isConstant() ? 
           (size_type) 0 :
           *std::max_element(degBegin(), degEnd()) );
#endif           
}


// Degree of the leading term
BoolePolynomial::size_type
BoolePolynomial::lmDeg() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::deg() const" );

#ifndef PBORI_USE_CCUDDFIRSTITER
  // Equals number of nodes for monomials
  return lead().nNodes();

#else
  self ld1st(leadFirst());
  return std::distance(ld1st.firstBegin(), ld1st.firstEnd());
#endif
}


// Total (weighted) maximal degree of the polynomial
BoolePolynomial::size_type
BoolePolynomial::totalDeg() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::totalDeg() const" );

  // No weighted degrees yet, so map to non-weighted variant
  return deg();
}

// Total (weighted) degree of the leading term
BoolePolynomial::size_type
BoolePolynomial::lmTotalDeg() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::lmTotalDeg() const" );

  // No weighted degrees yet, so map to non-weighted variant
  return lmDeg();
}

// Number of nodes in the decision diagram
BoolePolynomial::size_type
BoolePolynomial::nNodes() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::nNodes() const" );

  return m_dd.nNodes();
}

// Number of variables of the polynomial
BoolePolynomial::size_type
BoolePolynomial::nUsedVariables() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::nUsedVariables() const" );

  return m_dd.nSupport();
}

// Set of variables of the polynomial
BoolePolynomial::monom_type
BoolePolynomial::usedVariables() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::usedVariables() const" );

  return CDDOperations<dd_type, monom_type>().usedVariables(diagram());

}
// Exponent vector of variables of the polynomial
BoolePolynomial::exp_type
BoolePolynomial::usedVariablesExp() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::usedVariablesExp() const" );

  exp_type result;
  m_dd.usedIndices(result);
  return result;
}


/// Returns number of terms
BoolePolynomial::size_type
BoolePolynomial::length() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::length() const" );

  return m_dd.length();
}


// // Access to internal decision diagramm structure
// BoolePolynomial::dd_type&
// BoolePolynomial::internalDiagram() {

//   PBORI_TRACE_FUNC( "BoolePolynomial::internalDiagram()" );

//   return m_dd;
// }

// // Access to internal decision diagramm structure
// const BoolePolynomial::dd_type&
// BoolePolynomial::diagram() const {

//   PBORI_TRACE_FUNC( "BoolePolynomial::diagram() const" );

//   return m_dd;
// }




// Print current polynomial to output stream
BoolePolynomial::ostream_type&
BoolePolynomial::print(ostream_type& os) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::print() const" );

  // defining literal variable products
  typedef CStringLiteral<CLiteralCodes::times> times_as_separator;

  // defining literal for term separators
  typedef CStringLiteral<CLiteralCodes::term_separator> sep_literal_type;

  if( isZero() )
    os << 0;
  else
    dd_print_terms(orderedExpBegin(), orderedExpEnd(), 
                   variable_name<manager_type>(BoolePolyRing::activeManager()), 
                   sep_literal_type(), times_as_separator(), 
                   integral_constant<unsigned, 1>(), os);

  os << ' ';

  return os;
}

/// Pretty print to stdout
void BoolePolynomial::prettyPrint() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::prettyPrint() const" );
  m_dd.prettyPrint();
}

/// Pretty print to filename
void BoolePolynomial::prettyPrint(filename_type filename) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::prettyPrint(filename_type) const" );
  bool_type error = m_dd.prettyPrint(filename);

  if (error)
    throw PBoRiError(CTypes::io_error);
}


// Start of leading term
BoolePolynomial::first_iterator 
BoolePolynomial::firstBegin() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::firstBegin() const" );
  return m_dd.firstBegin();
}

// Finish of leading term 
BoolePolynomial::first_iterator 
BoolePolynomial::firstEnd() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::firstEnd() const" );
  return m_dd.firstEnd();
}

// Start of degrees
BoolePolynomial::deg_iterator 
BoolePolynomial::degBegin() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::degBegin() const" );
  return navigation();
}

// Finish of leading term 
BoolePolynomial::deg_iterator 
BoolePolynomial::degEnd() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::degEnd() const" );
  return deg_iterator();
}

// Start of degrees
BoolePolynomial::ordered_iterator 
BoolePolynomial::orderedBegin() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::orderedBegin() const" );
  return *this;
}

// Finish of leading term 
BoolePolynomial::ordered_iterator 
BoolePolynomial::orderedEnd() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::orderedEnd() const" );
  return ordered_iterator();
}

// Start of degrees
BoolePolynomial::ordered_exp_iterator 
BoolePolynomial::orderedExpBegin() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::orderedExpBegin() const" );
  return *this;
}

// Finish of leading term 
BoolePolynomial::ordered_exp_iterator 
BoolePolynomial::orderedExpEnd() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::orderedExpEnd() const" );
  return ordered_exp_iterator();
}

// Start of iteration over monomials
BoolePolynomial::const_iterator 
BoolePolynomial::begin() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::begin() const" );
  return navigation();
}

// Finish of iteration over monomials
BoolePolynomial::const_iterator 
BoolePolynomial::end() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::end() const" );
  return const_iterator();
}


// Start of iteration over exponent vectors
BoolePolynomial::exp_iterator 
BoolePolynomial::expBegin() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::exp_begin() const" );
  return navigation();
}

// Finish of iteration over monomials
BoolePolynomial::exp_iterator 
BoolePolynomial::expEnd() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::exp_end() const" );
  return exp_iterator();
}
// Navigate through diagram structure
BoolePolynomial::navigator 
BoolePolynomial::navigation() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::navigation() const" );
  return m_dd.navigation();
}
// End marker
BoolePolynomial::navigator 
BoolePolynomial::endOfNavigation() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::nendOfNavigation() const" );
  return navigator();
}


// Start of iteration over monomials in lex ordering
BoolePolynomial::lex_iterator 
BoolePolynomial::genericBegin(lex_tag) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::genericBegin(lex_tag) const" );
  return lex_iterator(*this);
}

// Finish of iteration over monomials in lex ordering
BoolePolynomial::lex_iterator
BoolePolynomial::genericEnd(lex_tag) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::genericEnd(lex_tag) const" );
  return lex_iterator();
}

// Start of iteration over monomials in deg-lex ordering
BoolePolynomial::dlex_iterator 
BoolePolynomial::genericBegin(dlex_tag) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::genericBegin(dlex_tag) const" );
  return dlex_iterator(*this);
}

// Finish of iteration over monomials in deg-lex ordering
BoolePolynomial::dlex_iterator
BoolePolynomial::genericEnd(dlex_tag) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::genericEnd(dlex_tag) const" );
  return dlex_iterator();
}

// Start of iteration over monomials in ascending deg-rev-lex ordering
BoolePolynomial::dp_asc_iterator 
BoolePolynomial::genericBegin(dp_asc_tag) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::genericBegin(sd_asc_tag) const" );
  return dp_asc_iterator(*this);
}

// Finish of iteration over monomials in ascending deg-rev-lex ordering
BoolePolynomial::dp_asc_iterator
BoolePolynomial::genericEnd(dp_asc_tag) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::genericEnd(dp_asc_tag) const" );
  return dp_asc_iterator();
}

// Start of iteration over monomials in lex ordering
BoolePolynomial::lex_exp_iterator 
BoolePolynomial::genericExpBegin(lex_tag) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::genericExpBegin(lex_tag) const" );
  return lex_exp_iterator(*this);
}

// Finish of iteration over monomials in lex ordering
BoolePolynomial::lex_exp_iterator
BoolePolynomial::genericExpEnd(lex_tag) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::genericExpEnd(lex_tag) const" );
  return lex_exp_iterator();
}

// Start of iteration over monomials in deg-lex ordering
BoolePolynomial::dlex_exp_iterator 
BoolePolynomial::genericExpBegin(dlex_tag) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::genericExpBegin(dlex_tag) const" );
  return dlex_exp_iterator(*this);
}

// Finish of iteration over monomials in deg-lex ordering
BoolePolynomial::dlex_exp_iterator
BoolePolynomial::genericExpEnd(dlex_tag) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::genericExpEnd(dlex_tag) const" );
  return dlex_exp_iterator();
}

// Start of iteration over monomials in ascending deg-rev-lex ordering
BoolePolynomial::dp_asc_exp_iterator 
BoolePolynomial::genericExpBegin(dp_asc_tag) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::genericExpBegin(sd_asc_tag) const" );
  return dp_asc_exp_iterator(*this);
}

// Finish of iteration over monomials in ascending deg-rev-lex ordering
BoolePolynomial::dp_asc_exp_iterator
BoolePolynomial::genericExpEnd(dp_asc_tag) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::genericExpEnd(dp_asc_tag) const" );
  return dp_asc_exp_iterator();
}

// fetch list of terms
void
BoolePolynomial::fetchTerms(termlist_type& theOutputList) const {

  PBORI_TRACE_FUNC("BoolePolynomial:fetchTerms(const termlist_type&)");

  theOutputList.resize(length());

  std::copy(begin(), end(), theOutputList.begin());
}


// return list of terms
BoolePolynomial::termlist_type
BoolePolynomial::terms() const {

  PBORI_TRACE_FUNC("BoolePolynomial::terms() const" ); 
  termlist_type theList;
  fetchTerms(theList);

  return theList;
}

// generate a polynomial, whose first term is the leading term
BoolePolynomial
BoolePolynomial::leadFirst() const {

  PBORI_TRACE_FUNC("BoolePolynomial::leadFirst() const" ); 

  return BoolePolyRing::activeManager().leadFirst(*this);
}

/// Compute spoly of two polynomials
BoolePolynomial 
spoly(const BoolePolynomial& first, const BoolePolynomial& second){

   BooleMonomial lead1(first.lead()), lead2(second.lead());

   BooleMonomial prod = lead1;
   prod *= lead2;

   return ( first * (prod / lead1) ) + ( second * (prod / lead2) );
}

// Stream output for Boolean polynomials
BoolePolynomial::ostream_type&
operator<<(BoolePolynomial::ostream_type& os, const BoolePolynomial& source) {

  return source.print(os);
}
static int len_cheated;
template <class ExponentVectorType> class EliminationDegreeAdder{
public:
  int min;
  int sum;
  EliminationDegreeAdder(int min){
    this->min=min;
    sum=0;
  }
  void operator() (const ExponentVectorType& ev){
    int s=ev.size();
    sum++;
    if(s>min)
      sum+=s-min;
    len_cheated=sum;
  }
};

template <class SizeType>
class AddEliminationDegree:
public std::binary_function<SizeType, SizeType, SizeType>{
public:
  typedef SizeType size_type;
  typedef std::binary_function<size_type, size_type, size_type> base;

  AddEliminationDegree(size_type min): 
    m_min(min), base() {}

  size_type& operator()(size_type& rhs, size_type lhs) {
    ++rhs;
    if (lhs > m_min)
      rhs += (lhs - m_min);
    return rhs;
  }

private:
  const size_type m_min;
};

BoolePolynomial::size_type
BoolePolynomial::eliminationLength() const{

  if (isZero()) 
    return 0;
  if (BoolePolyRing::isTotalDegreeOrder())
    return this->length();
  size_type deg=this->deg();
  if (deg==this->lmDeg()){
    return this->length();
  }
//   BoolePolynomial::navigator navi = navigation();
//   std::list<std::list<int>  > allLists;
  
//   dd_transform( navi, std::list<int>(),
//                 std::back_inserter(allLists),
//                 push_back<std::list<int> >() );
  
//   int deg=(*allLists.begin()).size();

//   EliminationDegreeAdder<std::list<int> > sum_up(deg);
//   for_each(allLists.begin(), allLists.end(), sum_up);

//   //return sum_up.sum;
//   return len_cheated;

  return std::accumulate( degBegin(), degEnd(), size_type(0), 
                          AddEliminationDegree<size_type>(lmDeg()) );
}

BoolePolynomial::size_type
BoolePolynomial::eliminationLengthWithDegBound(BoolePolynomial::size_type garantied_deg_bound) const{
  assert(garantied_deg_bound>=this->deg());
  if (BoolePolyRing::isTotalDegreeOrder())
    return this->length();
  if (this->lmDeg()==garantied_deg_bound)
    return this->length();
  else 
    return this->eliminationLength();
}

END_NAMESPACE_PBORI
