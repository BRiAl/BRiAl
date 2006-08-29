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

// include definition of generic string literals
# include "CStringLiteral.h"

// include definition of generic print operation
# include "CPrintOperation.h"

// get internal routines
# include "pbori_routines.h"

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

// Destructor
BoolePolynomial::~BoolePolynomial() {

  PBORI_TRACE_FUNC( "~BoolePolynomial()" );

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


  first_iterator start(firstBegin()), finish(firstEnd()),
    rhs_start(rhs.firstBegin()), rhs_finish(rhs.firstEnd());
    
  bool_type is_reducible = true;
  
  while (is_reducible && (rhs_start != rhs_finish) ) {
    idx_type rhs_idx(*rhs_start); 

    while( (start != finish) && (*start < rhs_idx)  ) 
      ++start;

    is_reducible = (start != finish) && (*start == rhs_idx);
    ++rhs_start;
  }

  return is_reducible;
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

// Check whether polynomial is zero
BoolePolynomial::bool_type
BoolePolynomial::isZero() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::isZero() const" );

  return m_dd.emptiness();
}

// Check whether polynomial is one
BoolePolynomial::bool_type
BoolePolynomial::isOne() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::isOne() const" );

  return m_dd.blankness();
}

// Check whether polynomial own the one term
BoolePolynomial::bool_type
BoolePolynomial::hasConstantPart() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::hasConstantPart() const" );

  navigator navi(navigation());

  while (!navi.isConstant() )
    navi.incrementElse();

  return navi.terminalValue();
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

// Leading exponent
BoolePolynomial::exp_type
BoolePolynomial::leadExp() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::leadExp() const" );

  return BoolePolyRing::activeManager().leadExp(*this);
}

// all dividers
BoolePolynomial::set_type
BoolePolynomial::lmDivisors() const {

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
    return index_vector_hash(firstBegin(), firstEnd());

//     dd_type::first_iterator start(firstBegin()), finish(firstEnd());
//     int vars=0;
//     int sum=0;
//     while (start != finish){
//       vars++;
//       sum+=((*start)+1)*((*start)+1);
//       ++start;
//     }
//     return sum*vars;
  }
    
}

// Maximal degree of the polynomial
BoolePolynomial::size_type
BoolePolynomial::deg() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::deg() const" );

  /// @todo: This is currently just brute force, efficient search needed.

  deg_iterator start(degBegin()), finish(degEnd());

  return (start == finish ? 
          (size_type) 0 :
          *std::max_element(start, finish));
}


// Degree of the leading term
BoolePolynomial::size_type
BoolePolynomial::lmDeg() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::deg() const" );

#ifndef PBORI_USE_CCUDDFIRSTITER
  // Equals number of nodes for monomials
  return lead().nNodes();

#else

  return std::distance(firstBegin(), firstEnd());
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


  return dd_used_variables(diagram(), type_tag<monom_type>());

#if 0

#ifdef PBORI_USEDVARS_BY_SUPPORT

  BooleSet bset(diagram().support());
  return bset.lastLexicographicalTerm();

#else

  // default value is the one monomial
  monom_type result(true);

# ifdef PBORI_USEDVARS_HIGHLEVEL

  // define iterator type for storing used variables (on forward branches)
  typedef CTermIter< std::set<idx_type>, navigator, 
    inserts< std::set<idx_type> >, project_ith<1>, project_ith<1> >
  the_iterator;

  // initialize iteration
  the_iterator start(navigation());

  // collect all indices during iteration
  while( !start.empty() ){ ++start; }

  the_iterator::reference indices(*start);

# elif defined(PBORI_USEDVARS_BY_TRANSFORM) // variant of highlevel

  typedef std::set<idx_type> path_type;
  path_type indices;

  dd_transform( navigation(), dummy_iterator(),
                dummy_iterator(),
                insert_second_to_list<path_type,
                   dummy_iterator,idx_type>(indices),
                project_ith<1, 2>(),  
                project_ith<1>()
                );

# elif defined(PBORI_USEDVARS_BY_IDX) // using internal variant

  // Get indices of used variables
  std::vector<idx_type> indices(nUsedVariables());
  m_dd.usedIndices(indices);

# endif

  // generate monomial from indices
  PBoRiOutIter<monom_type, idx_type, change_assign<monom_type> >  
    outiter(result);
  copy(indices.rbegin(), indices.rend(), outiter);

  return result;
#endif

#endif
}

/// Returns number of terms
BoolePolynomial::size_type
BoolePolynomial::length() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::length() const" );

  return m_dd.length();
}


// Access to internal decision diagramm structure
BoolePolynomial::dd_type&
BoolePolynomial::diagram() {

  PBORI_TRACE_FUNC( "BoolePolynomial::diagram()" );

  return m_dd;
}

// Access to internal decision diagramm structure
const BoolePolynomial::dd_type&
BoolePolynomial::diagram() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::diagram() const" );

  return m_dd;
}


// Print current polynomial to output stream
BoolePolynomial::ostream_type&
BoolePolynomial::print(ostream_type& os) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::print() const" );

  // defining literal variable products
  typedef CStringLiteral<CLiteralCodes::times> times_as_separator;

  // defining path type
  typedef CIdxPath<CIdxVariable<idx_type>, times_as_separator> path_type;

  // defining literal for term separators
  typedef CStringLiteral<CLiteralCodes::term_separator> sep_literal_type;

  if( isZero() )
    os << 0;
  else
    dd_transform( navigation(), path_type(),
                  std::ostream_iterator<path_type>(os),
                  push_back<path_type>(),
                  project_ith<1, 2>(),  
                  CPrintOperation<path_type, sep_literal_type>(os),
                  project_ith<1>());
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
  if (this->lmDeg()==garantied_deg_bound)
    return this->length();
  else 
    return this->eliminationLength();
}

END_NAMESPACE_PBORI
