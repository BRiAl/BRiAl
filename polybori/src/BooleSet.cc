// -*- c++ -*-
//*****************************************************************************
/** @file BooleSet.cc
 *
 * @author Alexander Dreyer
 * @date 2006-04-19
 *
 * This file implements the class BooleSet, where carries the definition
 * of sets of Boolean variables.
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
 * Revision 1.52  2008/03/03 14:25:20  dreyer
 * Change: switched to new syntax
 *
 * Revision 1.51  2008/03/03 12:44:33  dreyer
 * Change: More inlining, and safer constructors
 *
 * Revision 1.50  2008/02/28 17:05:47  dreyer
 * Fix: treating constants (0, 1) accordingly
 *
 * Revision 1.49  2008/01/17 16:05:01  dreyer
 * Fix: reverted previous version (performance problem)
 *
 * Revision 1.47  2008/01/17 15:18:41  dreyer
 * CHANGE: removed several calls of BooleEnv::*
 *
 * Revision 1.46  2008/01/16 17:10:19  dreyer
 * CHANGE: term-iterators use correct manager now
 *
 * Revision 1.45  2008/01/11 16:58:58  dreyer
 * CHANGE: Experimenting with iterators and correct rings
 *
 * Revision 1.44  2007/12/17 16:12:03  dreyer
 * CHANGE: reviewed and optimized merge frim sf.net
 *
 * Revision 1.43  2007/12/14 11:50:31  dreyer
 * Fix: merged from bugfix at sf.net
 *
 * Revision 1.42  2007/12/13 15:53:50  dreyer
 * CHANGE: Ordering in BoolePolyRing again; BooleEnv manages active ring
 *
 * Revision 1.41  2007/12/11 14:21:08  dreyer
 * ADD: count terms containing given index
 *
 * Revision 1.40  2007/11/30 09:33:20  dreyer
 * CHANGE: more dd-like stableHash()
 *
 * Revision 1.39  2007/11/29 16:28:33  dreyer
 * ADD: fast hash(), where applicable; + stableHashes() anywhere
 *
 * Revision 1.38  2007/11/19 14:13:26  dreyer
 * Fix: consistend naming of cartesianProduct
 *
 * Revision 1.37  2007/11/06 15:03:38  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.36  2007/07/30 15:19:39  dreyer
 * CHANGE: CCuddNavigator does not convert to DdNode* impicitely any more
 *
 * Revision 1.35  2007/07/06 14:04:22  dreyer
 * ADD: newly written C++_interface for Cudd
 *
 * Revision 1.34  2007/05/25 12:35:32  dreyer
 * ADD: BooleSet::owns(const exp_type&) const
 *
 * Revision 1.33  2007/05/21 16:07:55  dreyer
 * CHANGE: temporarily deactivated some things with curious side-effects
 *
 * Revision 1.32  2007/05/20 09:44:40  dreyer
 * ADD: BooleSet(true/false)
 *
 * Revision 1.31  2007/05/03 16:04:46  dreyer
 * CHANGE: new-style CTermIter integrated
 *
 * Revision 1.30  2006/12/07 13:54:33  dreyer
 * CHANGE: fine-tuning
 *
 * Revision 1.29  2006/12/07 08:22:53  dreyer
 * ADD/CHANGE: Lowlevel variant of existAbstract
 *
 * Revision 1.28  2006/11/24 14:49:01  dreyer
 * CHANGE: divisorsOf (less recursions/cache-lookups)
 *
 * Revision 1.27  2006/11/22 15:46:22  dreyer
 * ADD: CacheManager replacing CCacheManagement for external use
 * CHANGE: CacheManager used, where necessary
 * CHANGE: multiplesOf, uses cached recursion
 *
 * Revision 1.26  2006/11/22 10:10:23  dreyer
 * ADD: dd_first_divisors_of
 *
 * Revision 1.25  2006/11/21 16:06:40  dreyer
 * CHANGE: inlining added
 *
 * Revision 1.24  2006/11/21 15:42:15  dreyer
 * ADD: Construct Booleset from navigator
 *
 * Revision 1.23  2006/11/21 12:33:34  dreyer
 * ADD: BooleSet::ownsOne, BooleSet(idx, navi, navi); divisorsof
 *
 * Revision 1.22  2006/11/20 16:18:07  dreyer
 * ADD: BooleSet new node-constructor, also in dd_multiply_recursively
 *
 * Revision 1.21  2006/10/04 15:46:50  dreyer
 * ADD: divisorsOf(exp_type), orderedExpBegin/End;
 * CHANGE: Polynomials printing respects ordering
 *
 * Revision 1.20  2006/09/14 10:57:26  dreyer
 * ADD: usedVariablesExp()
 *
 * Revision 1.19  2006/09/08 10:22:59  dreyer
 * FIX: Gcc 4 ist more pedantic
 *
 * Revision 1.18  2006/09/05 14:21:01  bricken
 * +multiplesOf, tailVariables to Exponent
 *
 * Revision 1.17  2006/08/31 08:44:16  bricken
 * + variant using subset for divisorsOf
 *
 * Revision 1.16  2006/08/29 12:09:27  dreyer
 * using CDDOperations for generic dd functions
 *
 * Revision 1.15  2006/08/29 08:02:08  dreyer
 * ADD: BooleSet::expBegin() and expEnd()
 *
 * Revision 1.14  2006/08/23 14:24:54  dreyer
 * ADD: BooleSet::usedVariables and infrastructure
 *
 * Revision 1.13  2006/08/22 16:06:23  dreyer
 * + Added highlevel division
 *
 * Revision 1.12  2006/08/18 19:47:15  dreyer
 * change finished low-level variant of minimal_elements also for lnegth()==2
 *
 * Revision 1.11  2006/08/17 15:35:30  dreyer
 * ADD: extended and activated low-level version of dd_minimal_elements
 *
 * Revision 1.10  2006/08/15 14:17:29  dreyer
 * ADD minimalElements(), hasTermOfVariables()
 *
 * Revision 1.9  2006/08/09 12:52:32  dreyer
 * CHANGE/ADD: added lowlevel implementation of BooleSet::divisorsOf()
 *
 * Revision 1.8  2006/08/03 15:20:20  dreyer
 * ADD: BooleSet::divisorsOf and BooleSet::hasCommonVariables
 *
 * Revision 1.7  2006/06/07 08:37:50  dreyer
 * ADD CCuddLastIter and BooleSet::lastLexicographicalTerm()
 *
 * Revision 1.6  2006/04/25 09:30:42  dreyer
 * FIX end of CTermIterm for constants, CHANGE consistent functional names
 *
 * Revision 1.5  2006/04/21 09:31:44  dreyer
 * CHANGE reducing redundancy
 *
 * Revision 1.4  2006/04/21 07:11:50  dreyer
 * ADD operator=
 *
 * Revision 1.3  2006/04/21 06:35:45  dreyer
 * CHANGE Default constructor starts with empty set
 *
 * Revision 1.2  2006/04/20 17:31:05  dreyer
 * FIX removed casting operator, which caused ambigeous overloads
 *
 * Revision 1.1  2006/04/20 16:59:47  dreyer
 * routine
 *
 * @endverbatim
**/
//*****************************************************************************


// load header files
# include "BooleSet.h"
# include "BooleMonomial.h"
# include "BooleExponent.h"
# include "BoolePolyRing.h"
# include "CIdxPath.h"
# include "PBoRiOutIter.h"
# include <iterator>
# include "pbori_algo.h"


#include "pbori_algo_int.h"


// get internal routines
# include "pbori_routines.h"
# include "CDDOperations.h"
# include "CCacheManagement.h"

// include polybori iterator
#include "CGenericIter.h"


BEGIN_NAMESPACE_PBORI

//-------------------------------------------------------------------------
// Constructors and destructor
//------------------------------------------------------------------------

// Default constructor
BooleSet::BooleSet():
  base( BooleEnv::zero() )  {

  PBORI_TRACE_FUNC( "BooleSet()" );

}

// // Construct new node (using navigator nodes)
// BooleSet::BooleSet(idx_type idx, navigator first, navigator second):
//   base(BooleEnv::manager().manager(), idx, first, second) {
  
//   PBORI_TRACE_FUNC( "BooleSet(idx_type, navigator, navigator)" );
// }

// // Construct new node (using navigator nodes)
// BooleSet::BooleSet(idx_type idx, navigator navi):
//   base(BooleEnv::manager().manager(), idx, navi) {
  
//   PBORI_TRACE_FUNC( "BooleSet(idx_type, navigator)" );
// }

// // Construct new node (using navigator nodes)
// BooleSet::BooleSet(navigator navi, const ring_type& ring) :
//   base(ring.manager().manager(), navi) {
  
//   PBORI_TRACE_FUNC( "BooleSet(navigator)" );
// }

// Assignment
BooleSet&
BooleSet::operator=(const self& rhs) {

  PBORI_TRACE_FUNC( "BooleSet::operator=(const self&) const" );

  base::operator=(rhs);
  return *this;
}

// Add given monomial to sets and assign
BooleSet&
BooleSet::addAssign(const term_type& rhs) {

  PBORI_TRACE_FUNC( "BooleSet::addAssign(const term_type&) const" );

  uniteAssign(rhs.diagram());
  return *this;
}

// Add given monomial to sets
BooleSet
BooleSet::add(const term_type& rhs) const {

  PBORI_TRACE_FUNC( "BooleSet::addAssign(const term_type&) const" );

  return self(*this).addAssign(rhs);
}

// Check whether rhs is included in *this
BooleSet::bool_type
BooleSet::owns(const term_type& rhs) const {

  PBORI_TRACE_FUNC( "BooleSet::owns(const term_type&) const" );

  return !(intersect(rhs.diagram()).emptiness());
}

// Check whether rhs is included in *this
BooleSet::bool_type
BooleSet::owns(const exp_type& rhs) const {

  PBORI_TRACE_FUNC( "BooleSet::owns(const exp_type&) const" );

  return dd_owns(navigation(), rhs.begin(), rhs.end());
}
// Start of iteration over monomials
BooleSet::const_iterator 
BooleSet::begin() const {

  PBORI_TRACE_FUNC( "BooleSet::begin() const" );
  return const_iterator(navigation(), managerCore());
}

// Finish of iteration over monomials
BooleSet::const_iterator 
BooleSet::end() const {

  PBORI_TRACE_FUNC( "BooleSet::end() const" );
  return const_iterator();
}

// Start of iteration over exponent vectors
BooleSet::exp_iterator 
BooleSet::expBegin() const {

  PBORI_TRACE_FUNC( "BooleSet::exp_begin() const" );
  return exp_iterator(base::navigation(), base::managerCore());
}

// Finish of iteration over monomials
BooleSet::exp_iterator 
BooleSet::expEnd() const {

  PBORI_TRACE_FUNC( "BooleSet::exp_end() const" );
  return exp_iterator();
}


// Get last term (wrt. lexicographical order)
BooleSet::term_type
BooleSet::lastLexicographicalTerm() const {

  PBORI_TRACE_FUNC( "BooleSet::lastTerm() const" );

  return dd_last_lexicographical_term(*this, type_tag<term_type>());
}

BooleSet
BooleSet::firstDivisorsOf(const self& rhs) const {

  PBORI_TRACE_FUNC( "BooleSet::divisorsOf(const term_type&) const" );

  typedef CCacheManagement<CCacheTypes::divisorsof> cache_type;
  return dd_first_divisors_of( cache_type(manager()), 
                               navigation(), rhs.navigation(), 
                               self() );
}

// compute intersection with divisors of rhs
BooleSet
BooleSet::divisorsOf(const term_type& rhs) const {

  PBORI_TRACE_FUNC( "BooleSet::divisorsOf(const term_type&) const" );
  return firstDivisorsOf(rhs.diagram());
}

// compute intersection with divisors of rhs
BooleSet
BooleSet::divisorsOf(const exp_type& rhs) const {

  PBORI_TRACE_FUNC( "BooleSet::divisorsOf(const exp_type&) const" );

  return firstDivisorsOf(term_type(rhs, ring()).diagram());
}

BooleSet BooleSet::multiplesOf(const term_type& rhs) const{

  typedef CCacheManagement<CCacheTypes::multiplesof> cache_type;

  return 
    dd_first_multiples_of( cache_type(base::manager()), navigation(),
                           rhs.diagram().navigation(), self() );
}

// check whether the intersection with divisors of rhs is non-empty
BooleSet::bool_type
BooleSet::hasTermOfVariables(const term_type& rhs) const {

  PBORI_TRACE_FUNC( "BooleSet::EmptyDivisorsOf(const term_type&) const" );

#ifdef PBORI_HASTERMOFVARIABLES_HIGHLEVEL

  bool_type result =!divisorsOf(rhs).emptiness();

#else

  bool_type result = dd_owns_term_of_indices(navigation(),
                                             rhs.begin(), rhs.end());

  assert(result == !divisorsOf(rhs).emptiness());

#endif
  
  return result;
}





BooleSet
BooleSet::minimalElements() const { 

  //  return base::minimalElements(); 
  // base 

  navigator  resultMultiples;
  std::vector<idx_type> indices(0);

  usedIndices(indices);
  dd_operations<navigator> apply(manager().getManager());
  //  std::cerr<< "b4"<<std::endl;
  navigator result= dd_minimal_elements(navigation(),
  resultMultiples, indices.rbegin(), indices.rend(),  
                                        apply);
  // std::cerr<< "aft"<<std::endl;
  self res(result, ring());

  result.decRef();
  resultMultiples.recursiveDecRef(manager().getManager());
  return res;
}


// Division by given term
BooleSet
BooleSet::divide(const term_type& rhs) const {
  return self(base::divideFirst(rhs.diagram()));
}

// Division with assignment by given term
BooleSet& 
BooleSet::divideAssign(const term_type& rhs)  {
  base::divideFirstAssign(rhs.diagram());
  return *this;
}

// Set of variables of the set
BooleSet::term_type
BooleSet::usedVariables() const {

  PBORI_TRACE_FUNC( "BooleSet::usedVariables() const" );


  //  return dd_used_variables(*this, type_tag<term_type>());
  return CDDOperations<self, term_type>().usedVariables(*this);
}

// Exponent vector of variables of the set
BooleSet::exp_type
BooleSet::usedVariablesExp() const {

  PBORI_TRACE_FUNC( "BooleSet::usedVariablesExp() const" );

  exp_type result;
  usedIndices(result);
  return result;
}

// Computes existential abstraction wrt to the variables in the first
// lexicographical term of rhs
BooleSet
BooleSet::existAbstract(const term_type& rhs) const {

  PBORI_TRACE_FUNC( "BooleSet::existAbstract(const term_type&) const" );

  typedef CCacheManagement<CCacheTypes::exist_abstract> cache_mgr_type;
  return 
    dd_existential_abstraction(cache_mgr_type(base::manager()), 
                               rhs.diagram().navigation(), base::navigation(),
                               self());
}


// Print current polynomial to output stream
BooleSet::ostream_type&
BooleSet::print(ostream_type& os) const {

  PBORI_TRACE_FUNC( "BooleSet::print() const" );

  // defining literal variable products
  typedef CStringLiteral<CLiteralCodes::comma> comma_as_separator;

  // defining literal for term separators
  typedef CStringLiteral<CLiteralCodes::between_list_separator>
    sep_literal_type;

  typedef CStringLiteral<CLiteralCodes::empty>  empty_type;

  //  typedef CTypes::manager_type mgr_type;
  typedef CTypes::manager_base mgr_type;

  if( base::emptiness() )
    os << "{}";
  else {
    os << "{{";
    dd_print_terms(begin(), end(), 
                   variable_name<mgr_type>(managerCore()), 
                   sep_literal_type(), comma_as_separator(), 
                   empty_type(), os);
    os << "}}";
  }
  return os;
}

// Count terms containing BooleVariable(idx)
BooleSet::size_type BooleSet::countIndex(idx_type idx) const {
  size_type size(0);
  return count_index(size, idx, *this);
}

// Count terms containing BooleVariable(idx)
double BooleSet::countIndexDouble(idx_type idx) const {
  double size(0);
  return count_index(size, idx, *this);
}

END_NAMESPACE_PBORI
