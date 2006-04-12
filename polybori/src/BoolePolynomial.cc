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
#include <iterator>
#define PBORI_USE_CCUDDFIRSTITER

// load header file
# include "BoolePolynomial.h"

// get polynomial riung definition
# include "BoolePolyRing.h"

// get error types
# include "PBoRiError.h"

// get transformation algorithms
# include "pbori_algo.h"

// get functionals
# include "pbori_func.h"

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

  m_dd = m_dd.unite( rhs.m_dd ).diff( m_dd.intersect(rhs.m_dd) );
  return *this;
}

// Multiplication
BoolePolynomial&
BoolePolynomial::operator*=(const monom_type& rhs) {

  PBORI_TRACE_FUNC( "BoolePolynomial::operator*=(const monom_type&)" );

  monom_type::first_iterator start(rhs.firstBegin()), finish(rhs.firstEnd());

  while (start != finish) {

    // get all terms not containing the variable with index *start
    dd_type tmp( m_dd.subset0(*start) );

    // get the complementary terms
    m_dd.diffAssign(tmp);

    // construct polynomial terms
    *this += tmp.change(*start);

    ++start;
  }
  return *this;
}

// Division
BoolePolynomial&
BoolePolynomial::operator/=(const monom_type& rhs) {

  PBORI_TRACE_FUNC( "BoolePolynomial::operator*=(const monom_type&)" );

  m_dd.divideAssign(rhs.m_dd);
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


  first_iterator start(m_dd.firstBegin()), finish(m_dd.firstEnd()),
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

// Leading term
BoolePolynomial::monom_type
BoolePolynomial::lead() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::lead() const" );

#ifndef PBORI_USE_CCUDDFIRSTITER
  // high level implementation
  
  dd_type nextterm = m_dd;
  
  manager_reference mgr(m_dd);
  size_type nlen = mgr.nVariables();
  
  for(idx_type idx = 0; idx < nlen; ++idx){
    
    nextterm.intersectAssign( mgr.ddVariable(idx) );
    
    if (nextterm !=  mgr.empty())
      leadterm = nextterm;    
    else
      nextterm = leadterm;
  }

#else 
  // More efficient implementation relying on CCuddFirstIter (may be buggy)
  dd_type leadterm;

  if (m_dd.emptiness())
    leadterm = m_dd;
  else {
    leadterm = manager_reference(m_dd).blank();
    dd_type::first_iterator start(m_dd.firstBegin()), finish(m_dd.firstEnd());
    
    while (start != finish){
      leadterm.changeAssign(*start);
      ++start;
    }
  }

#endif

  return leadterm;
}

// all dividers
BoolePolynomial::monom_type
BoolePolynomial::lmDivisors() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::lmDivisors() const" );

  // Implementation relying on CCuddFirstIter (may be buggy)
  dd_type terms;

  if (m_dd.emptiness())
    terms = m_dd;
  else {
    terms = manager_reference(m_dd).blank();
    dd_type::first_iterator start(m_dd.firstBegin()), finish(m_dd.firstEnd());
    
    while (start != finish){

      terms.uniteAssign( terms.change(*start) );
      ++start;
    }
  }

  return terms;
}

// hash value of lm
BoolePolynomial::hash_type 
BoolePolynomial::lmHash() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::lmHash() const" );

  if (m_dd.emptiness())
    return 0;
  else {
  
    dd_type::first_iterator start(m_dd.firstBegin()), finish(m_dd.firstEnd());
    int vars=0;
    int sum=0;
    while (start != finish){
      vars++;
      sum+=((*start)+1)*((*start)+1);
      ++start;
    }
    return sum*vars;
  }
    
}

// Maximal degree of the polynomial
BoolePolynomial::size_type
BoolePolynomial::deg() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::deg() const" );

  /// @todo: This is currently just an upper bound, efficient search needed.

  return nUsedVariables();
}


// Degree of the leading term
BoolePolynomial::size_type
BoolePolynomial::lmDeg() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::deg() const" );

#ifndef PBORI_USE_CCUDDFIRSTITER
  // Equals number of nodes for monomials
  return lead().nNodes();

#else
  dd_type::first_iterator start(m_dd.firstBegin()), finish(m_dd.firstEnd());
  size_type degree = 0;

  while (start != finish){
    ++degree;
    ++start;
  }

  return degree;
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

  return m_dd.support();
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

// Print current polynomial to cout
BoolePolynomial::ostream_type&
BoolePolynomial::print(ostream_type& os) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::print() const" );

  ///  @todo: add std::cout capability for cudd's ZDD type

  os << "-> ";
  m_dd.print(os);

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

// Navigate through diagram structure
BoolePolynomial::navigator 
BoolePolynomial::navigation() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::navigation() const" );
  return m_dd.navigation();
}

// addition operation 
BoolePolynomial 
operator+(const BoolePolynomial& first, const BoolePolynomial& second){

  PBORI_TRACE_FUNC("operator+(const BoolePolynomial&,const BoolePolynomial&)");

  BoolePolynomial result(first);
  return (result += second);
}

// multiplication with monomial
BoolePolynomial
operator*(const BoolePolynomial& poly, const BoolePolynomial::monom_type& monom) {

  PBORI_TRACE_FUNC("operator*(const BoolePolynomial&,const monom_type&)");

  BoolePolynomial result(poly);
  return (result *= monom);

}

// division by monomial (skipping remainder)
BoolePolynomial
operator/(const BoolePolynomial& poly, const BoolePolynomial::monom_type& monom) {

  PBORI_TRACE_FUNC("operator/(const BoolePolynomial&,const monom_type&)");

  BoolePolynomial result(poly);
  return (result /= monom);

}

/// Compute spoly of two polynomials
BoolePolynomial 
spoly(const BoolePolynomial& first, const BoolePolynomial& second){

  BoolePolynomial lead1(first.lead()), lead2(second.lead());

  BoolePolynomial prod = lead1;
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
int BoolePolynomial::eliminationLength() const{
  if (isZero()) return 0;
  BoolePolynomial::navigator navi = navigation();
  std::list<std::list<int>  > allLists;
  
  dd_transform( navi, std::list<int>(),
                std::back_inserter(allLists),
                push_back<std::list<int> >() );
  
  int deg=(*allLists.begin()).size();
  EliminationDegreeAdder<std::list<int> > sum_up(deg);
  for_each(allLists.begin(), allLists.end(), sum_up);
//return sum_up.sum;
  return len_cheated;
}

END_NAMESPACE_PBORI
