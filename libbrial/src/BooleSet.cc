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
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************


// load header files
# include <polybori/BooleSet.h>
# include <polybori/BooleMonomial.h>
# include <polybori/BooleExponent.h>
# include <polybori/BoolePolyRing.h>
# include <polybori/iterators/PBoRiOutIter.h>
# include <iterator>
# include <polybori/routines/pbori_algo.h>


#include <polybori/routines/pbori_algo_int.h>


// get internal routines
# include <polybori/routines/pbori_routines.h>
# include <polybori/diagram/CDDOperations.h>
# include <polybori/cache/CCacheManagement.h>

// include polybori iterator
#include <polybori/iterators/CGenericIter.h>
#include <polybori/LexOrder.h>

#include <polybori/literals/CStringLiteral.h>

BEGIN_NAMESPACE_PBORI

// Add given monomial to sets
BooleSet
BooleSet::add(const term_type& rhs) const {

  PBORI_TRACE_FUNC( "BooleSet::add(const term_type&) const" );
  return unite(rhs.set());
}

// Check whether rhs is included in *this
BooleSet::bool_type
BooleSet::owns(const term_type& rhs) const {

  PBORI_TRACE_FUNC( "BooleSet::owns(const term_type&) const" );
  return !(intersect(rhs.set()).isZero());
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
  return const_iterator(navigation(), ring());
}

// Finish of iteration over monomials
BooleSet::const_iterator 
BooleSet::end() const {

  PBORI_TRACE_FUNC( "BooleSet::end() const" );
  return const_iterator(navigator(), ring());
}
// Start of iteration over monomials
BooleSet::const_reverse_iterator 
BooleSet::rbegin() const {

  PBORI_TRACE_FUNC( "BooleSet::rbegin() const" );
  return const_reverse_iterator(navigation(), ring());
}

// Finish of iteration over monomials
BooleSet::const_reverse_iterator 
BooleSet::rend() const {

  PBORI_TRACE_FUNC( "BooleSet::rend() const" );
  return const_reverse_iterator(navigator(), ring());
}

// Start of iteration over exponent vectors
BooleSet::exp_iterator 
BooleSet::expBegin() const {

  PBORI_TRACE_FUNC( "BooleSet::exp_begin() const" );
  return exp_iterator(base::navigation(), base::ring());
}

// Finish of iteration over monomials
BooleSet::exp_iterator 
BooleSet::expEnd() const {

  PBORI_TRACE_FUNC( "BooleSet::exp_end() const" );
  return exp_iterator(navigator(), ring());
}

// Start of iteration over exponent vectors
BooleSet::reverse_exp_iterator 
BooleSet::rExpBegin() const {

  PBORI_TRACE_FUNC( "BooleSet::rExpBegin() const" );
  return reverse_exp_iterator(base::navigation(), base::ring());
}

// Finish of iteration over monomials
BooleSet::reverse_exp_iterator 
BooleSet::rExpEnd() const {

  PBORI_TRACE_FUNC( "BooleSet::rExpEnd() const" );
  return reverse_exp_iterator(navigator(), ring());
}

// Get last term (wrt. lexicographical order)
BooleSet::term_type
BooleSet::lastLexicographicalTerm() const {

  PBORI_TRACE_FUNC( "BooleSet::lastTerm() const" );

  if PBORI_UNLIKELY(isZero())
    throw PBoRiError(CTypes::illegal_on_zero);

  return dd_last_lexicographical_term(*this, type_tag<term_type>());
}

BooleSet
BooleSet::firstDivisorsOf(const self& rhs) const {

  PBORI_TRACE_FUNC( "BooleSet::divisorsOf(const term_type&) const" );

  typedef CCacheManagement<ring_type, CCacheTypes::divisorsof> cache_type;
  return dd_first_divisors_of( cache_type(ring()), 
                               navigation(), rhs.navigation(), 
                               self(ring()) );
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

  typedef CCacheManagement<ring_type, CCacheTypes::multiplesof> cache_type;

  return 
    dd_first_multiples_of( cache_type(ring()), navigation(),
                           rhs.diagram().navigation(), self(ring()) );
}

// check whether the intersection with divisors of rhs is non-empty
BooleSet::bool_type
BooleSet::hasTermOfVariables(const term_type& rhs) const {

  PBORI_TRACE_FUNC( "BooleSet::EmptyDivisorsOf(const term_type&) const" );

#ifdef PBORI_HASTERMOFVARIABLES_HIGHLEVEL

  bool_type result =!divisorsOf(rhs).isZero();

#else

  bool_type result = dd_owns_term_of_indices(navigation(),
                                             rhs.begin(), rhs.end());

  PBORI_ASSERT(result == !divisorsOf(rhs).isZero());

#endif
  
  return result;
}





BooleSet
BooleSet::minimalElements() const { 

  typedef CacheManager<CCacheTypes::minimal_elements> cache_mgr_type;
  typedef CacheManager<CCacheTypes::mod_mon_set> modmon_mgr_type;

  return dd_minimal_elements(cache_mgr_type(ring()),
                             modmon_mgr_type(ring()),
                             base::navigation(), self(ring()));
}


// Division by given term
BooleSet
BooleSet::divide(const term_type& rhs) const {

  typedef CCacheManagement<ring_type, CCacheTypes::divide>
    cache_mgr_type;

  return dd_divide_recursively(cache_mgr_type(ring()), 
                               navigation(), rhs.set().navigation(),
                               self(ring()));
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
  return usedVariables().exp();
}

// Computes existential abstraction wrt to the variables in the first
// lexicographical term of rhs
BooleSet
BooleSet::existAbstract(const term_type& rhs) const {

  PBORI_TRACE_FUNC( "BooleSet::existAbstract(const term_type&) const" );

  typedef CCacheManagement<ring_type, CCacheTypes::exist_abstract> cache_mgr_type;
  return 
    dd_existential_abstraction(cache_mgr_type(ring()), 
                               rhs.diagram().navigation(), base::navigation(),
                               self(ring()));
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

  if( base::isZero() )
    os << "{}";
  else {
    os << "{{";
    dd_print_terms(begin(), end(), 
                   variable_name<ring_type>(ring()), 
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

// Test whether, all divisors of degree -1 of term rhs are contained in this
BooleSet::bool_type 
BooleSet::containsDivisorsOfDecDeg(const term_type& rhs) const { 
  return dd_contains_divs_of_dec_deg(navigation(), rhs.begin(), rhs.end());
}

// Test for term corresponding to exponent rhs
BooleSet::bool_type 
BooleSet::containsDivisorsOfDecDeg(const exp_type& rhs) const { 
  return dd_contains_divs_of_dec_deg(navigation(), rhs.begin(), rhs.end());
}

END_NAMESPACE_PBORI
