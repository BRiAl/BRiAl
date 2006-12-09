// -*- c++ -*-
//*****************************************************************************
/** @file pbori_routines_misc.h
 *
 * @author Alexander Dreyer
 * @date 2006-08-23
 *
 * This file includes files, which defines miscellaneous function templates.
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
 * Revision 1.23  2006/12/09 10:46:18  dreyer
 * CHANGE added and used recursively cache variant of /=
 *
 * Revision 1.22  2006/12/07 13:54:32  dreyer
 * CHANGE: fine-tuning
 *
 * Revision 1.21  2006/12/07 08:22:53  dreyer
 * ADD/CHANGE: Lowlevel variant of existAbstract
 *
 * Revision 1.20  2006/12/06 09:20:09  dreyer
 * CHANGE: poly * exp now recursive
 *
 * Revision 1.19  2006/12/05 16:18:46  dreyer
 * CHANGE: specialized multiplication with monomial
 *
 * Revision 1.18  2006/12/05 08:18:49  dreyer
 * CHANGE: nicer source code
 *
 * Revision 1.17  2006/12/04 17:08:19  dreyer
 * CHANGE: multiplication in new style
 *
 * Revision 1.16  2006/12/04 12:48:16  dreyer
 * CHANGE: cached and recursive lead() and leadexp() refined, generalized
 *
 * Revision 1.15  2006/11/30 19:42:44  dreyer
 * CHANGE: lead(bound) now uses cached and recursive variant
 *
 * Revision 1.14  2006/11/29 13:40:03  dreyer
 * CHANGE: leadexp() made recursive and cached
 *
 * Revision 1.13  2006/11/28 09:32:58  dreyer
 * CHANGE: lead() (for dlex, dp_asc) is recursive and cached now
 *
 * Revision 1.12  2006/11/27 16:25:14  dreyer
 * CHANGE: CDegreeCache, now inherited from standard cache; dlex-lead cached
 *
 * Revision 1.11  2006/11/22 15:46:22  dreyer
 * ADD: CacheManager replacing CCacheManagement for external use
 * CHANGE: CacheManager used, where necessary
 * CHANGE: multiplesOf, uses cached recursion
 *
 * Revision 1.10  2006/11/21 09:52:05  dreyer
 * CHANGE: some simple functions in BoolePolynomial inlined
 * ADD: caching of ternary operations
 * ADD: commandline switch PBORI_FAST_MULTIPLICATION (dense multiplication)
 *
 * Revision 1.9  2006/11/20 16:37:03  dreyer
 * FIX: broken assertion
 *
 * Revision 1.8  2006/11/20 16:18:07  dreyer
 * ADD: BooleSet new node-constructor, also in dd_multiply_recursively
 *
 * Revision 1.7  2006/11/20 14:56:46  dreyer
 * CHANGE CCacheType names, operator*=, CDDInterface node Constructor
 *
 * Revision 1.6  2006/10/24 14:21:56  dreyer
 * ADD: variable_name functional
 *
 * Revision 1.5  2006/10/23 16:05:55  dreyer
 * ADD: BoolePolyRing::set/get(Ring)VariableName()
 *
 * Revision 1.4  2006/10/04 15:46:49  dreyer
 * ADD: divisorsOf(exp_type), orderedExpBegin/End;
 * CHANGE: Polynomials printing respects ordering
 *
 * Revision 1.3  2006/09/21 16:09:59  dreyer
 * ADD: caching mechanism for BoolePolynomial::deg()
 *
 * Revision 1.2  2006/09/14 10:57:26  dreyer
 * ADD: usedVariablesExp()
 *
 * Revision 1.1  2006/08/24 14:52:15  dreyer
 * + Initial Version
 *
 * Revision 1.1  2006/08/23 14:24:54  dreyer
 * ADD: BooleSet::usedVariables and infrastructure
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

// temprarily
#include "CIdxVariable.h"

// temprarily
#include "CacheManager.h"

#include "CDDOperations.h"

BEGIN_NAMESPACE_PBORI

template<class Iterator>
typename Iterator::value_type
index_vector_hash(Iterator start, Iterator finish){

  typedef typename Iterator::value_type value_type;

  value_type vars = 0;
  value_type sum = 0;
 
  while (start != finish){
    vars++;
    sum += ((*start)+1) * ((*start)+1);
    ++start;
  }
  return sum * vars;
}

/// Function templates for determining the degree of a decision diagram
/// with the help of cache (e. g. CDegreeCache)
template <class DegreeCacher, class NaviType>
typename NaviType::size_type
dd_cached_degree(const DegreeCacher& cache, NaviType navi) {

  typedef typename NaviType::size_type size_type;

  if (navi.isConstant()) // No need for caching of constant nodes' degrees
    return 0;
 
  // Look whether result was cached before
  typename DegreeCacher::node_type result = cache.find(navi);
  if (result.isValid())
    return *result;

  // Get degree of then branch (contains at least one valid path)...
  size_type deg = dd_cached_degree(cache, navi.thenBranch()) + 1;
 
  // ... combine with degree of else branch
  deg = std::max(deg,  dd_cached_degree(cache, navi.elseBranch()) );

  // Write result to cache
  cache.insert(navi, deg);
 
  return deg;
}

/// Function templates for determining the degree of a decision diagram
/// with the help of cache (e. g. CDegreeCache)
/// Variant *with* given upper bound
/// Assumming that the bound is valid!
template <class DegreeCacher, class NaviType, class SizeType>
typename NaviType::size_type
dd_cached_degree(const DegreeCacher& cache, NaviType navi, SizeType bound) {

  typedef typename NaviType::size_type size_type;

  // No need for caching of constant nodes' degrees
  if (bound == 0 || navi.isConstant())
    return 0;
 
  // Look whether result was cached before
  typename DegreeCacher::node_type result = cache.find(navi);
  if (result.isValid())
    return *result;

  // Get degree of then branch (contains at least one valid path)...
  size_type deg = dd_cached_degree(cache, navi.thenBranch(), bound - 1) + 1;

  // ... combine with degree of else branch
  if (bound > deg)              // if deg <= bound, we are already finished
    deg = std::max(deg,  dd_cached_degree(cache, navi.elseBranch(), bound) );

  // Write result to cache
  cache.insert(navi, deg);
 
  return deg;
}

template <class Iterator, class NameGenerator, 
          class Separator, class EmptySetType, 
          class OStreamType>
void 
dd_print_term(Iterator start, Iterator finish, const NameGenerator& get_name,
              const Separator& sep, const EmptySetType& emptyset, 
              OStreamType& os){

  if (start != finish){
    os << get_name(*start);
    ++start;
  }
  else
    os << emptyset();

  while (start != finish){
    os << sep() << get_name(*start);
    ++start;
  }
}

template <class TermType, class NameGenerator,
          class Separator, class EmptySetType,
          class OStreamType>
void 
dd_print_term(const TermType& term, const NameGenerator& get_name,
              const Separator& sep, const EmptySetType& emptyset, 
              OStreamType& os){
  dd_print_term(term.begin(), term.end(), get_name, sep, emptyset, os);
}


template <class Iterator, class NameGenerator,
          class Separator, class InnerSeparator, 
          class EmptySetType, class OStreamType>
void 
dd_print_terms(Iterator start, Iterator finish, const NameGenerator& get_name,
               const Separator& sep, const InnerSeparator& innersep,
               const EmptySetType& emptyset, OStreamType& os) {

  if (start != finish){
    dd_print_term(*start, get_name, innersep, emptyset, os);
    ++start;
  }

  while (start != finish){
    os << sep(); 
    dd_print_term(*start, get_name, innersep, emptyset, os);
    ++start;
  }

}



template <class CacheType, class NaviType, class PolyType>
PolyType
dd_multiply_recursively(const CacheType& cache_mgr,
                        NaviType firstNavi, NaviType secondNavi, PolyType init){
  // Extract subtypes
  typedef typename PolyType::set_type dd_type;
  typedef typename NaviType::idx_type idx_type;
  typedef NaviType navigator;

  if (firstNavi.isConstant()) {
    if(firstNavi.terminalValue())
      return dd_type(secondNavi);
    else 
      return init;
  }

  if (secondNavi.isConstant()) {
    if(secondNavi.terminalValue())
      return dd_type(firstNavi);
    else 
      return init;
  }
  if (firstNavi == secondNavi)
    return dd_type(firstNavi);
  
  // Look up, whether operation was already used
  navigator cached = cache_mgr.find(firstNavi, secondNavi);

  if (cached.isValid()) {       // Cache lookup sucessful
    return dd_type(cached);
  }
  else {                        // Cache lookup not sucessful
    // Get top variable's index

    if (*secondNavi < *firstNavi)
      std::swap(firstNavi, secondNavi);

    idx_type index = *firstNavi;

    // Get then- and else-branches wrt. current indexed variable
    navigator as0 = firstNavi.elseBranch();
    navigator as1 = firstNavi.thenBranch();

    navigator bs0;
    navigator bs1;

    if (*secondNavi == index) {
      bs0 = secondNavi.elseBranch();
      bs1 = secondNavi.thenBranch();
    }
    else {
      bs0 = secondNavi;
      bs1 = init.navigation();
    }


#ifdef PBORI_FAST_MULTIPLICATION
    if (*firstNavi == *secondNavi) {

      PolyType res00 = dd_multiply_recursively(cache_mgr, as0, bs0, init);

      PolyType res10 = PolyType(dd_type(as1)) + PolyType(dd_type(as0));
      PolyType res01 = PolyType(dd_type(bs0)) + PolyType(dd_type(bs1));

      PolyType res11 = 
        dd_multiply_recursively(cache_mgr,
                                res10.navigation(), res01.navigation(),
                                init) - res00;

      init = dd_type(index, res11.navigation(), res00.navigation());
    } else
#endif
      {
        if (as0 == as1)
          bs1 = init.navigation();
        else if (bs0 == bs1)
          as1 = init.navigation();

        // Do recursion
        init = dd_type(index,  
                         (dd_multiply_recursively(cache_mgr, as0, bs1, init) 
                         + dd_multiply_recursively(cache_mgr, as1, bs1, init)
                         + dd_multiply_recursively(cache_mgr,
                                                   as1, bs0, init)).diagram(),
                         dd_multiply_recursively(cache_mgr, 
                                                 as0, bs0, init).diagram() );
        
      }
    // Insert in cache
    cache_mgr.insert(firstNavi, secondNavi, init.navigation());
  }

  return init;
}


template <class CacheType, class NaviType, class PolyType,
          class MonomTag>
PolyType
dd_multiply_recursively(const CacheType& cache_mgr,
                        NaviType monomNavi, NaviType navi, PolyType init,
                        MonomTag monom_tag ){
  // Extract subtypes
  typedef typename PolyType::set_type dd_type;
  typedef typename NaviType::idx_type idx_type;
  typedef NaviType navigator;

  if (monomNavi.isConstant()) {
    if(monomNavi.terminalValue())
      return dd_type(navi);
    else 
      return init;
  }

  assert(monomNavi.elseBranch().isEmpty());

  if (navi.isConstant()) {
    if(navi.terminalValue())
      return dd_type(monomNavi);
    else 
      return init;
  }
  if (monomNavi == navi)
    return dd_type(monomNavi);
  
  // Look up, whether operation was already used
  navigator cached = cache_mgr.find(monomNavi, navi);

  if (cached.isValid()) {       // Cache lookup sucessful
    return dd_type(cached);
  }

  // Cache lookup not sucessful
  // Get top variables' index

  idx_type index = *navi;
  idx_type monomIndex = *monomNavi;

  if (monomIndex < index) {     // Case: index may occure within monom
    init = dd_multiply_recursively(cache_mgr, monomNavi.thenBranch(), navi,
                                   init, monom_tag).diagram().change(monomIndex);
  }
  else if (monomIndex == index) { // Case: monom and poly start with same index

    // Increment navigators
    navigator monomThen = monomNavi.thenBranch();
    navigator naviThen = navi.thenBranch();
    navigator naviElse = navi.elseBranch();

    if (naviThen != naviElse)
      init = (dd_multiply_recursively(cache_mgr, monomThen, naviThen, init,
                                      monom_tag)
              + dd_multiply_recursively(cache_mgr, monomThen, naviElse, init,
                                        monom_tag)).diagram().change(index);
  }
  else {                        // Case: var(index) not part of monomial
    
    init = 
      dd_type(index,  
              dd_multiply_recursively(cache_mgr, monomNavi, navi.thenBranch(), 
                                      init, monom_tag).diagram(),
              dd_multiply_recursively(cache_mgr, monomNavi, navi.elseBranch(),
                                      init, monom_tag).diagram() );
  }
  
  // Insert in cache
  cache_mgr.insert(monomNavi, navi, init.navigation());

  return init;
}


template <class Iterator, class NaviType, class PolyType>
PolyType
dd_multiply_recursively_exp(Iterator start, Iterator finish,
                            NaviType navi, PolyType init){
  // Extract subtypes
  typedef typename PolyType::set_type dd_type;
  typedef typename NaviType::idx_type idx_type;
  typedef NaviType navigator;

  if (start == finish)
    return dd_type(navi);

  if (navi.isConstant()) {
    if(navi.terminalValue()) {

      std::reverse_iterator<Iterator> rstart(finish), rfinish(start);
      init = dd_type(navi);
      while (rstart != rfinish) {
        init = init.diagram().change(*rstart);
        ++rstart;
      }
    }

    return init;
  }

  // Cache lookup not sucessful
  // Get top variables' index

  idx_type index = *navi;
  idx_type monomIndex = *start;

  if (monomIndex < index) {     // Case: index may occure within monom

    Iterator next(start);
    while( (next != finish) && (*next < index) )
      ++next;

    init = dd_multiply_recursively_exp(next, finish, navi, init);

    std::reverse_iterator<Iterator> rstart(next), rfinish(start);
    while (rstart != rfinish) {
      init = init.diagram().change(*rstart);
      ++rstart;
    }
  }
  else if (monomIndex == index) { // Case: monom and poly start with same index

    // Increment navigators
    ++start;

    navigator naviThen = navi.thenBranch();
    navigator naviElse = navi.elseBranch();

    if (naviThen != naviElse)
      init = (dd_multiply_recursively_exp(start, finish, naviThen, init)
              + dd_multiply_recursively_exp(start, finish, naviElse,
                                            init)).diagram().change(index);
  }
  else {                        // Case: var(index) not part of monomial
    
    init = 
      dd_type(index,  
              dd_multiply_recursively_exp(start, finish, navi.thenBranch(), 
                                      init).diagram(),
              dd_multiply_recursively_exp(start, finish, navi.elseBranch(),
                                      init).diagram() );
  }

  return init;
}

template<class DegCacheMgr, class NaviType, class SizeType>
bool max_degree_on_then(const DegCacheMgr& deg_mgr, NaviType navi,
                        SizeType degree, valid_tag is_descending) {
  navi.incrementThen();
  return ((dd_cached_degree(deg_mgr, navi, degree - 1) + 1) == degree);
}

template<class DegCacheMgr, class NaviType, class SizeType>
bool max_degree_on_then(const DegCacheMgr& deg_mgr, NaviType navi,
                        SizeType degree, invalid_tag non_descending) {
  navi.incrementElse();
  return (dd_cached_degree(deg_mgr, navi, degree) != degree);
}


// with degree bound
template <class CacheType, class DegCacheMgr, class NaviType, 
          class TermType, class SizeType, class DescendingProperty>
TermType
dd_recursive_degree_lead(const CacheType& cache_mgr, const DegCacheMgr&
                         deg_mgr,
                         NaviType navi, TermType init, SizeType degree,
                         DescendingProperty prop) {

  if ((degree == 0) || navi.isConstant())
    return navi;

  // Check cache for previous results
  NaviType cached = cache_mgr.find(navi);
  if (cached.isValid())
    return cached;

  // Go to next branch
  if ( max_degree_on_then(deg_mgr, navi, degree, prop) ) {
    init = dd_recursive_degree_lead(cache_mgr, deg_mgr, navi.thenBranch(), 
                                    init, degree - 1, prop).change(*navi);
  }
  else {
    init = dd_recursive_degree_lead(cache_mgr, deg_mgr, navi.elseBranch(), 
                                    init, degree, prop);
  }

  NaviType resultNavi(init.navigation());
  cache_mgr.insert(navi, resultNavi);
  deg_mgr.insert(resultNavi, degree);

  return init;
}

template <class CacheType, class DegCacheMgr, class NaviType, 
          class TermType, class DescendingProperty>
TermType
dd_recursive_degree_lead(const CacheType& cache_mgr, const DegCacheMgr& deg_mgr,
                         NaviType navi, TermType init, DescendingProperty prop){

  if (navi.isConstant())
    return navi;
  
  return dd_recursive_degree_lead(cache_mgr, deg_mgr, navi, init,
                                  dd_cached_degree(deg_mgr, navi), prop);
}

template <class CacheType, class DegCacheMgr, class NaviType, 
          class TermType, class SizeType, class DescendingProperty>
TermType&
dd_recursive_degree_leadexp(const CacheType& cache_mgr, 
                            const DegCacheMgr& deg_mgr,
                            NaviType navi, TermType& result,  
                            SizeType degree,
                            DescendingProperty prop) {

  if ((degree == 0) || navi.isConstant())
    return result;
 
  // Check cache for previous result
  NaviType cached = cache_mgr.find(navi);
  if (cached.isValid())
    return result = result.multiplyFirst(cached);

  // Prepare next branch
  if ( max_degree_on_then(deg_mgr, navi, degree, prop) ) {
    result.push_back(*navi);
    navi.incrementThen();
    --degree;
  }
  else 
    navi.incrementElse();

  return 
    dd_recursive_degree_leadexp(cache_mgr, deg_mgr, navi, result, degree, prop);
}

template <class CacheType, class DegCacheMgr, class NaviType, 
          class TermType, class DescendingProperty>
TermType&
dd_recursive_degree_leadexp(const CacheType& cache_mgr, 
                            const DegCacheMgr& deg_mgr,
                            NaviType navi, TermType& result,  
                            DescendingProperty prop) {

  if (navi.isConstant())
    return result;

  return dd_recursive_degree_leadexp(cache_mgr, deg_mgr, navi, result, 
                                     dd_cached_degree(deg_mgr, navi), prop);
}

// Existential Abstraction. Given a ZDD F, and a set of variables
// S, remove all occurrences of s in S from any subset in F. This can
// be implemented by cofactoring F with respect to s = 1 and s = 0,
// then forming the union of these results. 


template <class CacheType, class NaviType, class TermType>
TermType
dd_existential_abstraction(const CacheType& cache_mgr, 
                           NaviType varsNavi, NaviType navi, TermType init){

  typedef typename TermType::idx_type idx_type;

  if (navi.isConstant()) 
    return navi;

  idx_type index(*navi);
  while (!varsNavi.isConstant() && ((*varsNavi) < index))
    varsNavi.incrementThen();

  if (varsNavi.isConstant())
    return navi;

  // Check cache for previous result
  NaviType cached = cache_mgr.find(varsNavi, navi);
  if (cached.isValid()) 
    return cached;

  NaviType thenNavi(navi.thenBranch()), elseNavi(navi.elseBranch()); 

  TermType thenResult = 
    dd_existential_abstraction(cache_mgr, varsNavi, thenNavi, init);

  TermType elseResult = 
    dd_existential_abstraction(cache_mgr, varsNavi, elseNavi, init);

  if ((*varsNavi) == index)
    init = thenResult.unite(elseResult);
  else if ( (thenResult.navigation() == thenNavi) && 
            (elseResult.navigation() == elseNavi)  )
    init = navi;
  else
    init = TermType(index, thenResult, elseResult);

  // Insert result to cache
  cache_mgr.insert(varsNavi, navi, init.navigation());

  return init;
}



template <class CacheType, class NaviType, class PolyType>
PolyType
dd_divide_recursively(const CacheType& cache_mgr,
                        NaviType navi,  NaviType monomNavi,PolyType init){
  // Extract subtypes
  typedef typename PolyType::set_type dd_type;
  typedef typename NaviType::idx_type idx_type;
  typedef NaviType navigator;

  if (monomNavi.isConstant()) {
    if(monomNavi.terminalValue())
      return dd_type(navi);
    else 
      return init;
  }

  assert(monomNavi.elseBranch().isEmpty());

  if (navi.isConstant()) 
    return init;

  if (monomNavi == navi)
    return PolyType(1);
  
  // Look up, whether operation was already used
  navigator cached = cache_mgr.find(navi, monomNavi);

  if (cached.isValid()) {       // Cache lookup sucessful
    return dd_type(cached);
  }

  // Cache lookup not sucessful
  // Get top variables' index

  idx_type index = *navi;
  idx_type monomIndex = *monomNavi;

  if (monomIndex < index) {     // Case: var(monomIndex) not part of poly
    //
  }
  else if (monomIndex == index) { // Case: monom and poly start with same index

    // Increment navigators
    navigator monomThen = monomNavi.thenBranch();
    navigator naviThen = navi.thenBranch();

    init = dd_divide_recursively(cache_mgr, naviThen, monomThen, init);
   }
  else {                        // Case: monomIndex may occure within poly
    
    init = 
      dd_type(index,  
              dd_divide_recursively(cache_mgr,  navi.thenBranch(), monomNavi,
                                      init).diagram(),
              dd_divide_recursively(cache_mgr, navi.elseBranch(), monomNavi, 
                                      init).diagram() );
  }
  
  // Insert in cache
  cache_mgr.insert(navi, monomNavi,  init.navigation());

  return init;
}



END_NAMESPACE_PBORI
