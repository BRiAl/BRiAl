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
 *   (c) 2006 by The PolyBoRi Team
**/
//*****************************************************************************

#include <list>
#include <set>
#include <iterator>
#include <algorithm>
#include <numeric>

#include <polybori/routines/pbori_algo.h>

#define PBORI_USE_CCUDDFIRSTITER

// load header file
#include <polybori/BoolePolynomial.h>
#include <polybori/BooleMonomial.h>
#include <polybori/BooleExponent.h>
#include <polybori/BooleSet.h>

// get polynomial ring definition
#include <polybori/BoolePolyRing.h>
#include <polybori/BooleEnv.h>
// get error types
# include <polybori/except/PBoRiGenericError.h>

// get transformation algorithms
# include <polybori/routines/pbori_algo.h>

// get functionals
# include <polybori/routines/pbori_func.h>

// include definition of output iterator over monomials
//# include "OutMonomIter.h"
# include <polybori/iterators/PBoRiOutIter.h>


# include <polybori/iterators/COrderedIter.h>

// include definition of generic string literals
# include <polybori/literals/CStringLiteral.h>

// get internal routines
# include <polybori/routines/pbori_routines.h>
# include <polybori/diagram/CDDOperations.h>
# include <polybori/cache/CDegreeCache.h>

#include <polybori/iterators/CGenericIter.h>
#include <polybori/iterators/CExpIter.h>

#include <polybori/orderings/pbori_order.h>

BEGIN_NAMESPACE_PBORI

//-------------------------------------------------------------------------
// Constructors and destructor
//------------------------------------------------------------------------


// Constructor polynomial from exponent vector
BoolePolynomial::BoolePolynomial(const exp_type& rhs, const ring_type& ring):
  m_dd( ring.one() )  {

  PBORI_TRACE_FUNC( "BoolePolynomial(const exp_type&)" );

  exp_type::const_reverse_iterator start(rhs.rbegin()), finish(rhs.rend());

  while(start != finish) {
    m_dd = m_dd.change(*start);
    ++start;
  }
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

  typedef CommutativeCacheManager<CCacheTypes::multiply_recursive>
    cache_mgr_type;

  self result = 
    dd_multiply_recursively_monom(cache_mgr_type(ring()), 
                                  rhs.diagram().navigation(),
                                  navigation(),  self(ring()));

  return (*this = result);
}

// Multiplication
BoolePolynomial&
BoolePolynomial::operator*=(const exp_type& rhs) {

  PBORI_TRACE_FUNC( "BoolePolynomial::operator*=(const exp_type&)" );
  typedef CCacheManagement<ring_type, CCacheTypes::no_cache> cache_mgr_type;

  self result = dd_multiply_recursively_exp(cache_mgr_type(ring()),
                                            rhs.begin(), rhs.end(),
                                            navigation(), self(ring()) );

  return (*this = result);;
}

// Multiplication
BoolePolynomial&
BoolePolynomial::operator*=(const self& rhs) {

  PBORI_TRACE_FUNC( "BoolePolynomial::operator*=(const self&)" );


  //return *this = dd_multiply_recursively(*this, rhs);
  //  self result = dd_multiply_recursively(*this, rhs);
  typedef CommutativeCacheManager<CCacheTypes::multiply_recursive>
    cache_mgr_type;

  self result = dd_multiply_recursively(cache_mgr_type(ring()), 
                                        navigation(), rhs.navigation(),
                                        self(ring())); 

  return (*this = result);
}

// Division
BoolePolynomial&
BoolePolynomial::operator/=(const var_type& rhs) {

  PBORI_TRACE_FUNC( "BoolePolynomial::operator/=(const var_type&)" );
  return operator/=(monom_type(rhs));
}
// Division
BoolePolynomial&
BoolePolynomial::operator/=(const monom_type& rhs) {

  PBORI_TRACE_FUNC( "BoolePolynomial::operator/=(const monom_type&)" );
  return (*this = m_dd.divide(rhs));
}

// Division
BoolePolynomial&
BoolePolynomial::operator/=(const exp_type& rhs) {

  PBORI_TRACE_FUNC( "BoolePolynomial::operator/=(const exp_type&)" );
  typedef CCacheManagement<ring_type, CCacheTypes::no_cache> cache_mgr_type;

  return (*this = 
          dd_divide_recursively_exp(cache_mgr_type(ring()), 
                                    navigation(), rhs.begin(),rhs.end(),
                                    self(ring())));
}

// Polynomial Division
BoolePolynomial&
BoolePolynomial::operator/=(const self& rhs) {

  PBORI_TRACE_FUNC( "BoolePolynomial::operator/=(const self&)" );
  if PBORI_UNLIKELY(rhs.isZero()) {
    throw PBoRiGenericError<CTypes::division_by_zero>();
  }
  return operator/=(rhs.firstTerm());
}

BoolePolynomial&
BoolePolynomial::operator/=(constant_type rhs) {

  PBORI_TRACE_FUNC( "BoolePolynomial::operator/=(constant_type)" );
  if PBORI_UNLIKELY(!rhs) {
    throw PBoRiGenericError<CTypes::division_by_zero>();
  }
  return *this;
}


// Modulus
BoolePolynomial&
BoolePolynomial::operator%=(const monom_type& rhs) {

  PBORI_TRACE_FUNC( "BoolePolynomial::operator%=(const monom_type&)" );

  monom_type allvars(ring());
  for(idx_type nvar = ring().nVariables(); nvar != 0;)
    allvars = allvars.change(--nvar);

  m_dd = m_dd.diff(  rhs.multiples(allvars) );

  return *this;
}
// Modulus
BoolePolynomial&
BoolePolynomial::operator%=(const var_type& rhs) {

  PBORI_TRACE_FUNC( "BoolePolynomial::operator%=(const var_type&)" );
  return operator%=(monom_type(rhs));
}

// Leading term
BoolePolynomial::monom_type
BoolePolynomial::lead() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::lead() const" );
  //if PBORI_UNLIKELY(isZero())
  //  throw PBoRiGenericError<CTypes::illegal_on_zero>();
  
  return ring().ordering().lead(*this);
}

// Leading term w.r.t. lex
BoolePolynomial::monom_type
BoolePolynomial::lexLead() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::lexLead() const" );
  //if PBORI_UNLIKELY(isZero())
  //  throw PBoRiGenericError<CTypes::illegal_on_zero>();

  return LexOrder().lead(*this);
}

// Leading term (bound)
BoolePolynomial::monom_type
BoolePolynomial::boundedLead(deg_type bound) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::lead(deg_type) const" );
  //if PBORI_UNLIKELY(isZero()) 
  //  throw PBoRiGenericError<CTypes::illegal_on_zero>();

  return ring().ordering().lead(*this, bound);
}

// Leading exponent
BoolePolynomial::exp_type
BoolePolynomial::leadExp() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::leadExp() const" );
  //if PBORI_UNLIKELY(isZero())
  //  throw PBoRiGenericError<CTypes::illegal_on_zero>();

  return ring().ordering().leadExp(*this);
}

// Leading exponent (bound)
BoolePolynomial::exp_type
BoolePolynomial::boundedLeadExp(deg_type bound) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::leadExp(deg_type) const" );
  //if PBORI_UNLIKELY(isZero())
  //  throw PBoRiGenericError<CTypes::illegal_on_zero>();

  return ring().ordering().leadExp(*this, bound);
}


// all dividers
BoolePolynomial::set_type
BoolePolynomial::firstDivisors() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::firstDivisors() const" );
  return m_dd.firstDivisors();
}

// hash value of lead
BoolePolynomial::hash_type 
BoolePolynomial::leadStableHash() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::leadStableHash() const" );
  self ld1st(leadFirst());
  return stable_first_hash_range(ld1st.navigation());
}

// Maximal degree of the polynomial
BoolePolynomial::deg_type
BoolePolynomial::deg() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::deg() const" );
  
  /// @todo: This is currently just brute force + caching, 
  /// more efficient search may be needed.

#ifndef PBORI_NO_DEGCACHE
  return dd_cached_degree(CDegreeCache<set_type>(ring()), navigation());
#else
  return ( isConstant() ? 
           (deg_type) 0 :
           *std::max_element(degBegin(), degEnd()) );
#endif           
}


// Degree of the leading term
BoolePolynomial::deg_type
BoolePolynomial::leadDeg() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::leadDeg() const" );
  if (PBORI_UNLIKELY(isZero())) return -1;
#ifndef PBORI_USE_CCUDDFIRSTITER
  // Equals number of nodes for monomials
  return lead().nNodes();

#else
  self ld1st(leadFirst());
  return ld1st.lexLeadDeg();
#endif
}


// Degree of the leading term
BoolePolynomial::deg_type
BoolePolynomial::lexLeadDeg() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::lexLeadDeg() const" );
  if (PBORI_UNLIKELY(isZero())) return -1;
  return std::distance(firstBegin(), firstEnd());
}

// Total (weighted) maximal degree of the polynomial
BoolePolynomial::deg_type
BoolePolynomial::totalDeg() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::totalDeg() const" );

  // No weighted degrees yet, so map to non-weighted variant
  return deg();
}

// Total (weighted) degree of the leading term
BoolePolynomial::deg_type
BoolePolynomial::leadTotalDeg() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::leadTotalDeg() const" );

  // No weighted degrees yet, so map to non-weighted variant
  return leadDeg();
}

// Get part of  of given degree
BoolePolynomial
BoolePolynomial::gradedPart(deg_type deg) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::gradedPart(deg_type) const" );
  typedef CDegreeArgumentCache<CCacheTypes::graded_part, set_type> cache_type;
  return dd_graded_part(cache_type(ring()), 
                        navigation(), deg, set_type(ring()));
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

  return usedVariables().size();
}

// Set of variables of the polynomial
BoolePolynomial::monom_type
BoolePolynomial::usedVariables() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::usedVariables() const" );
  monom_type result (CDDOperations<dd_type,
                     monom_type>().usedVariables(diagram()));

  self result2 (result);
  return  CDDOperations<BooleSet, monom_type>().getMonomial(self(result2.navigation(), ring()).set());

}
// Exponent vector of variables of the polynomial
BoolePolynomial::exp_type
BoolePolynomial::usedVariablesExp() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::usedVariablesExp() const" );

  return m_dd.usedVariablesExp();
}


/// Returns number of terms
BoolePolynomial::size_type
BoolePolynomial::length() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::length() const" );

  return m_dd.length();
}


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
  else if( isOne() )
    os << 1;
  else
    dd_print_terms(orderedExpBegin(), orderedExpEnd(), 
                   variable_name<ring_type>(ring()), 
                   sep_literal_type(), times_as_separator(), 
                   integral_constant<unsigned, 1>(), os);

  return os;
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

// Lexicographical leading term
BoolePolynomial::monom_type
BoolePolynomial::firstTerm() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::firstTerm() const" );

  return LexOrder().lead(*this);
}

// Start of degrees
BoolePolynomial::deg_iterator 
BoolePolynomial::degBegin() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::degBegin() const" );
  return deg_iterator(navigation(), ring());
}

// Finish of leading term 
BoolePolynomial::deg_iterator 
BoolePolynomial::degEnd() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::degEnd() const" );
  return deg_iterator(navigator(), ring());
}

// Start of degrees
BoolePolynomial::ordered_iterator 
BoolePolynomial::orderedBegin() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::orderedBegin() const" );
  return ring().ordering().leadIteratorBegin(*this);
}

// Finish of leading term 
BoolePolynomial::ordered_iterator 
BoolePolynomial::orderedEnd() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::orderedEnd() const" );
  return ring().ordering().leadIteratorEnd(*this);
}

// Start of degrees
BoolePolynomial::ordered_exp_iterator 
BoolePolynomial::orderedExpBegin() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::orderedExpBegin() const" );
  return ring().ordering().leadExpIteratorBegin(*this);//*this;
}

// Finish of leading term 
BoolePolynomial::ordered_exp_iterator 
BoolePolynomial::orderedExpEnd() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::orderedExpEnd() const" );
  return ring().ordering().leadExpIteratorEnd(*this);//ordered_exp_iterator();
}

// Start of iteration over monomials
BoolePolynomial::const_iterator 
BoolePolynomial::begin() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::begin() const" );
  return const_iterator(navigation(), ring() );
}

// Finish of iteration over monomials
BoolePolynomial::const_iterator 
BoolePolynomial::end() const {

  PBORI_TRACE_FUNC( "BoolePolynomial::end() const" );
  return const_iterator(navigator(), ring());
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
  return exp_iterator(navigator());
}

// Start of iteration over monomials in lex ordering
BoolePolynomial::lex_iterator 
BoolePolynomial::genericBegin(lex_tag) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::genericBegin(lex_tag) const" );
  return lex_iterator(navigation(), ring());
}

// Finish of iteration over monomials in lex ordering
BoolePolynomial::lex_iterator
BoolePolynomial::genericEnd(lex_tag) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::genericEnd(lex_tag) const" );
  return lex_iterator(navigator(), ring());
}

// Start of iteration over monomials in deg-lex ordering
BoolePolynomial::dlex_iterator 
BoolePolynomial::genericBegin(dlex_tag) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::genericBegin(dlex_tag) const" );
  return dlex_iterator(navigation(), ring());
}

// Finish of iteration over monomials in deg-lex ordering
BoolePolynomial::dlex_iterator
BoolePolynomial::genericEnd(dlex_tag) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::genericEnd(dlex_tag) const" );
  return dlex_iterator(navigator(), ring());
}

// Start of iteration over monomials in ascending deg-rev-lex ordering
BoolePolynomial::dp_asc_iterator 
BoolePolynomial::genericBegin(dp_asc_tag) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::genericBegin(sd_asc_tag) const" );
  return dp_asc_iterator(navigation(), ring());
}

// Finish of iteration over monomials in ascending deg-rev-lex ordering
BoolePolynomial::dp_asc_iterator
BoolePolynomial::genericEnd(dp_asc_tag) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::genericEnd(dp_asc_tag) const" );
  return dp_asc_iterator(navigator(), ring());
}
// Start of iteration over monomials in block deg-lex ordering
BoolePolynomial::block_dlex_iterator 
BoolePolynomial::genericBegin(block_dlex_tag) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::genericBegin(block_dlex_tag) const" );
  return block_dlex_iterator(navigation(), ring());
}

// Finish of iteration over monomials in block deg-lex ordering
BoolePolynomial::block_dlex_iterator
BoolePolynomial::genericEnd(block_dlex_tag) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::genericEnd(block_dlex_tag) const" );
  return block_dlex_iterator(navigator(), ring());
}
BoolePolynomial::block_dp_asc_iterator 
BoolePolynomial::genericBegin(block_dp_asc_tag) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::genericBegin(block_dp_asc_tag) const" );
  return block_dp_asc_iterator(navigation(), ring());
}

BoolePolynomial::block_dp_asc_iterator
BoolePolynomial::genericEnd(block_dp_asc_tag) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::genericEnd(block_dp_asc_tag) const" );
  return block_dp_asc_iterator(navigator(), ring());
}

// Start of iteration over monomials in lex ordering
BoolePolynomial::lex_exp_iterator 
BoolePolynomial::genericExpBegin(lex_tag) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::genericExpBegin(lex_tag) const" );
  return lex_exp_iterator(navigation(), ring());
}

// Finish of iteration over monomials in lex ordering
BoolePolynomial::lex_exp_iterator
BoolePolynomial::genericExpEnd(lex_tag) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::genericExpEnd(lex_tag) const" );
  return lex_exp_iterator(navigator(), ring());
}

// Start of iteration over monomials in deg-lex ordering
BoolePolynomial::dlex_exp_iterator 
BoolePolynomial::genericExpBegin(dlex_tag) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::genericExpBegin(dlex_tag) const" );
  return dlex_exp_iterator(navigation(), ring());
}

// Finish of iteration over monomials in deg-lex ordering
BoolePolynomial::dlex_exp_iterator
BoolePolynomial::genericExpEnd(dlex_tag) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::genericExpEnd(dlex_tag) const" );
  return dlex_exp_iterator(navigator(), ring());
}

// Start of iteration over monomials in ascending deg-rev-lex ordering
BoolePolynomial::dp_asc_exp_iterator 
BoolePolynomial::genericExpBegin(dp_asc_tag) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::genericExpBegin(sd_asc_tag) const" );
  return dp_asc_exp_iterator(navigation(), ring());
}

// Finish of iteration over monomials in ascending deg-rev-lex ordering
BoolePolynomial::dp_asc_exp_iterator
BoolePolynomial::genericExpEnd(dp_asc_tag) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::genericExpEnd(dp_asc_tag) const" );
  return dp_asc_exp_iterator(navigator(), ring());
}

// Start of iteration over monomials in block deg-lex ordering
BoolePolynomial::block_dlex_exp_iterator 
BoolePolynomial::genericExpBegin(block_dlex_tag) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::genericExpBegin(block_dlex_tag) const" );
  return block_dlex_exp_iterator(navigation(), ring());
}

// Finish of iteration over monomials in block deg-lex ordering
BoolePolynomial::block_dlex_exp_iterator
BoolePolynomial::genericExpEnd(block_dlex_tag) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::genericExpEnd(block_dlex_tag) const" );
  return block_dlex_exp_iterator(navigator(), ring());
}
// Start of iteration over monomials in block deg-lex ordering
BoolePolynomial::block_dp_asc_exp_iterator 
BoolePolynomial::genericExpBegin(block_dp_asc_tag) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::genericExpBegin(block_dp_asc_tag) const" );
  return block_dp_asc_exp_iterator(navigation(), ring());
}

// Finish of iteration over monomials in block deg-lex ordering
BoolePolynomial::block_dp_asc_exp_iterator
BoolePolynomial::genericExpEnd(block_dp_asc_tag) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::genericExpEnd(block_dp_asc_tag) const" );
  return block_dp_asc_exp_iterator(navigator(), ring());
}


// fetch list of terms
void
BoolePolynomial::fetchTerms(termlist_type& theOutputList) const {

  PBORI_TRACE_FUNC("BoolePolynomial:fetchTerms(const termlist_type&)");

  theOutputList.resize(length(), ring());

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

  return ring().ordering().leadFirst(*this);
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
    base(), m_min(min) {}

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
  if (ring().ordering().isTotalDegreeOrder())
    return this->length();
  deg_type deg=this->deg();
  if (deg==this->leadDeg()){
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
                          AddEliminationDegree<size_type>(leadDeg()) );
}

BoolePolynomial::size_type
BoolePolynomial::eliminationLengthWithDegBound(deg_type garantied_deg_bound)
  const {

  PBORI_ASSERT(garantied_deg_bound>=this->deg());

  if (ring().ordering().isTotalDegreeOrder())
    return this->length();
  if (this->leadDeg()==garantied_deg_bound)
    return this->length();
  else 
    return this->eliminationLength();
}


// Comparision
// @todo more sophisticated procedure needed
BoolePolynomial::comp_type
BoolePolynomial::compare(const self& rhs) const {

  PBORI_TRACE_FUNC( "BoolePolynomial::compare(const self& rhs) const" );

  if (operator==(rhs))
    return 0;

  if (isZero())
    return -1;

  if (rhs.isZero())
    return 1;

  monom_type lhs_lead(lead()), rhs_lead(rhs.lead());
  if (lhs_lead == rhs_lead)
    return (*this - lhs_lead).compare(rhs - rhs_lead);

  return (lhs_lead < rhs_lead? -1 : 1);
}


BoolePolynomial::bool_type
BoolePolynomial::inSingleBlock() const{
  if (isConstant()) return true;
  monom_type vars(usedVariables());

  return ring().ordering().lieInSameBlock(*vars.begin(),
                                          *std::max_element(vars.begin(), vars.end()));
}

END_NAMESPACE_PBORI
