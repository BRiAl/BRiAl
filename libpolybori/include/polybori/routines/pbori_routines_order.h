// -*- c++ -*-
//*****************************************************************************
/** @file pbori_routines_order.h
 *
 * @author Alexander Dreyer
 * @date 2006-08-23
 *
 * This file includes files, which defines ordering-related function templates.
 *
 * @par Copyright:
 *   (c) 2006 by The PolyBoRi Team
 *
 **/
//*****************************************************************************

#ifndef polybori_routines_pbori_routines_order_h_
#define polybori_routines_pbori_routines_order_h_

// include basic definitions
#include <polybori/pbori_defs.h>
#include "pbori_algo.h"
#include <polybori/common/traits.h>


BEGIN_NAMESPACE_PBORI

template <class FirstIterator, class SecondIterator, class BinaryPredicate>
CTypes::comp_type
lex_compare_3way(FirstIterator start, FirstIterator finish, 
              SecondIterator rhs_start, SecondIterator rhs_finish, 
              BinaryPredicate idx_comp) {

   while ( (start != finish) && (rhs_start != rhs_finish) &&
           (*start == *rhs_start) ) {
     ++start; ++rhs_start;
   }

   if (start == finish) {
     if (rhs_start == rhs_finish)
       return CTypes::equality;

     return CTypes::less_than;
   }
   
   if (rhs_start == rhs_finish)
     return CTypes::greater_than;

   return (idx_comp(*start, *rhs_start)? 
           CTypes::greater_than: CTypes::less_than);
}


/// defines lexicographic comparison
template <class LhsType, class RhsType, class BinaryPredicate>
CTypes::comp_type
lex_compare(const LhsType& lhs, const RhsType& rhs, 
            BinaryPredicate idx_comp, valid_tag has_easy_equality_test) {

  if (lhs == rhs)
    return CTypes::equality;

  return lex_compare_3way(lhs.begin(), lhs.end(), 
                                      rhs.begin(), rhs.end(), idx_comp);
  //typedef lex_compare_predicate<LhsType, RhsType, BinaryPredicate> comp_type;
  //return generic_compare_3way(lhs, rhs, comp_type(idx_comp));
}


/// defines lexicographic comparison
template <class LhsType, class RhsType, class BinaryPredicate>
CTypes::comp_type
lex_compare(const LhsType& lhs, const RhsType& rhs, 
            BinaryPredicate idx_comp, invalid_tag has_no_easy_equality_test) {

  return lex_compare_3way(lhs.begin(), lhs.end(), 
                          rhs.begin(), rhs.end(), idx_comp);
}

/// defines lexicographic comparison
template <class LhsType, class RhsType, class BinaryPredicate>
CTypes::comp_type
lex_compare(const LhsType& lhs, const RhsType& rhs, BinaryPredicate idx_comp) {

  typedef typename pbori_binary_traits<LhsType, RhsType>::easy_equality_property
    equality_property;

  return lex_compare(lhs, rhs, idx_comp, equality_property());
}

/// defines degree-lexicographic comparison
template<class LhsType, class RhsType, class BinaryPredicate>
CTypes::comp_type
deg_lex_compare(const LhsType& lhs, const RhsType& rhs, 
                BinaryPredicate idx_comp) {

  typedef typename LhsType::size_type size_type;
  CTypes::comp_type result = generic_compare_3way( lhs.size(), rhs.size(), 
                                                   std::greater<size_type>() );

  return (result == CTypes::equality? lex_compare(lhs, rhs, idx_comp): result);
}


template <class OrderType, class Iterator>
class generic_iteration;

template<class DummyType>
class dummy_data_type {
public:
  dummy_data_type(const DummyType&) {}
  dummy_data_type(DummyType&) {}
  dummy_data_type() {}
};

//////////////////////
template <class StackType, class Iterator>
void dummy_append(StackType& stack, Iterator start, Iterator finish) {

  while (start != finish) {
    stack.push(*start);
    ++start;
  }
}

template<class NaviType, class DescendingProperty = valid_tag>
class bounded_restricted_term {
public:
  typedef NaviType navigator;
  typedef DescendingProperty descending_property;
  typedef bounded_restricted_term<navigator, descending_property> self;
  typedef std::vector<navigator> stack_type;
  typedef unsigned size_type;
  typedef unsigned idx_type;

  bounded_restricted_term (): 
    m_stack(), m_max_idx(0), m_upperbound(0), m_next() {}

  is_same_type<descending_property, valid_tag> descendingVariables;

  bounded_restricted_term (navigator navi, size_type upperbound, 
                           idx_type max_idx): 
    m_stack(), m_upperbound(upperbound), m_max_idx(max_idx), m_next(navi)  {

    m_stack.reserve(upperbound);

    followThen();

    while (!is_path_end() && !empty() )
      increment();
  }

  size_type operator*() const {
    return m_stack.size();
  }

  const navigator& next() const {
    return m_next;
  }

  typename stack_type::const_iterator begin() const {
    return m_stack.begin();
  }

  typename stack_type::const_iterator end() const {
    return m_stack.end();
  }

  self& operator++() {
    PBORI_ASSERT(!empty());

    // if upperbound was already found we're done
    // (should be optimized away for ascending variables)
    if (descendingVariables() && (m_stack.size() == m_upperbound) )
      return *this = self();

    do {
      increment();
    } while (!empty() && !is_path_end());

    return *this;
  }

  void print() const {

    typename stack_type::const_iterator start(m_stack.begin()), 
      finish(m_stack.end());

    std::cout <<'(';
    while (start != finish) {
      std::cout << *(*start) << ", " ;
      ++start;
    }
    std::cout <<')';

  }

  bool operator==(const self& rhs) const {
    if (rhs.empty())
      return empty();
    if (empty())
      return rhs.empty();

    else (m_stack == rhs.m_stack);
  }
  bool operator!=(const self& rhs) const {
    return !(*this == rhs);
  }
protected:

  void followThen() {
    while (within_degree() && !at_end())
      nextThen();
  }

  void increment() {
    PBORI_ASSERT(!empty());

    m_next = top();
    m_next.incrementElse();
    m_stack.pop_back();

    followThen();

  }

  bool empty() const{
    return m_stack.empty();
  }

  navigator top() const{
    return m_stack.back();
  }

  bool is_path_end() {
 
    path_end();
    return  (!m_next.isConstant() && (*m_next >= m_max_idx)) ||
      m_next.terminalValue();
  }

  void path_end()  {
    while (!at_end()) {
      m_next.incrementElse();
    }
  }

  void nextThen() {
    PBORI_ASSERT(!m_next.isConstant());
    m_stack.push_back(m_next);
    m_next.incrementThen();
  }



  bool within_degree() const {
    
    return (*(*this) <  m_upperbound);
  }

  bool at_end() const {
    
    return m_next.isConstant() || (*m_next >= m_max_idx);
  }

private:
  stack_type m_stack;
  idx_type m_max_idx;
  size_type m_upperbound;
  navigator m_next;
};



/// Function templates for determining the degree of a decision diagram
/// with the help of cache (e. g. CDegreeCache)
template <class DegreeCacher, class NaviType, class IdxType>
typename NaviType::deg_type
dd_cached_block_degree(const DegreeCacher& cache, NaviType navi, 
                       IdxType nextBlock) {

  typedef typename NaviType::deg_type deg_type;

  if( navi.isConstant() || (*navi >= nextBlock) ) // end of block reached
    return 0;
 
  // Look whether result was cached before
  typename DegreeCacher::node_type result = cache.find(navi, nextBlock);
  if (result.isValid())
    return *result;
  
  // Get degree of then branch (contains at least one valid path)...
  deg_type deg = dd_cached_block_degree(cache, navi.thenBranch(), nextBlock) + 1;
 
  // ... combine with degree of else branch
  deg = std::max(deg,  dd_cached_block_degree(cache, navi.elseBranch(), nextBlock) );

  // Write result to cache
  cache.insert(navi, nextBlock, deg);
 
  return deg;
}


template<class DegCacheMgr, class NaviType, class IdxType, class SizeType>
bool max_block_degree_on_then(const DegCacheMgr& deg_mgr, NaviType navi,
                              IdxType next_block,
                              SizeType degree, valid_tag is_descending) {
  navi.incrementThen();
  return ((dd_cached_block_degree(deg_mgr, navi, next_block/*,degree - 1*/) + 1) == degree);
}

template<class DegCacheMgr, class NaviType, class IdxType, class SizeType>
bool max_block_degree_on_then(const DegCacheMgr& deg_mgr, NaviType navi,
                              IdxType next_block,
                              SizeType degree, invalid_tag non_descending) {
  navi.incrementElse();
  return (dd_cached_block_degree(deg_mgr, navi, next_block/*, degree*/) != degree);
}


// with degree bound
template <class CacheType, class DegCacheMgr, class NaviType, 
  class TermType, class Iterator, class SizeType, class DescendingProperty>
TermType
dd_block_degree_lead(const CacheType& cache_mgr, 
                     const DegCacheMgr& deg_mgr,
                     NaviType navi, Iterator block_iter,
                     TermType init, SizeType degree,
                     DescendingProperty prop) {

  if (navi.isConstant())
    return cache_mgr.generate(navi);

  while( (*navi >= *block_iter) && (*block_iter != CUDD_MAXINDEX))  {
    ++block_iter;
    degree = dd_cached_block_degree(deg_mgr, navi, *block_iter);
  }


  // Check cache for previous results - Wrong in general, bounds may have changed!
//   NaviType cached = cache_mgr.find(navi);
//   if (cached.isValid())
//     return cache_mgr.generate(cached);

  // Go to next branch
    if ( max_block_degree_on_then(deg_mgr, navi, *block_iter, degree, prop) ) {
    init = dd_block_degree_lead(cache_mgr, deg_mgr, navi.thenBranch(),
                                block_iter,
                                init, degree - 1, prop).change(*navi);
  }
  else {
    init = dd_block_degree_lead(cache_mgr, deg_mgr, navi.elseBranch(),
                                block_iter,
                                init, degree, prop);
  }

  NaviType resultNavi(init.navigation());
  //  cache_mgr.insert(navi, resultNavi);
  deg_mgr.insert(resultNavi, *block_iter, degree);

  return init;
}


template <class CacheType, class DegCacheMgr, class NaviType,  class Iterator,
          class TermType, class DescendingProperty>
TermType
dd_block_degree_lead(const CacheType& cache_mgr, const DegCacheMgr& deg_mgr,
                     NaviType navi, Iterator block_iter, TermType init,
                     DescendingProperty prop){ 

  if (navi.isConstant())
    return cache_mgr.generate(navi);
  
  return dd_block_degree_lead(cache_mgr, deg_mgr, navi,block_iter, init,
                              dd_cached_block_degree(deg_mgr, navi,
                              *block_iter), prop);
}


template <class FirstIterator, class SecondIterator, class IdxType, 
          class BinaryPredicate>
CTypes::comp_type
restricted_lex_compare_3way(FirstIterator start, FirstIterator finish, 
                            SecondIterator rhs_start, SecondIterator rhs_finish,
                            IdxType max_index,
                            BinaryPredicate idx_comp) {

  while ( (start != finish) && (*start < max_index) && (rhs_start != rhs_finish)
          && (*rhs_start < max_index) &&  (*start == *rhs_start) ) {
     ++start; ++rhs_start;
   }

  if ( (start == finish) || (*start >= max_index) ) {
    if ( (rhs_start == rhs_finish) || (*rhs_start >= max_index) )
       return CTypes::equality;

     return CTypes::less_than;
   }
   
  if ( (rhs_start == rhs_finish) || (*rhs_start >= max_index) )
     return CTypes::greater_than;

   return (idx_comp(*start, *rhs_start)? 
           CTypes::greater_than: CTypes::less_than);
}




template<class LhsIterator, class RhsIterator, class Iterator,
         class BinaryPredicate>
CTypes::comp_type
block_dlex_compare(LhsIterator lhsStart, LhsIterator lhsFinish,
                   RhsIterator rhsStart, RhsIterator rhsFinish, 
                   Iterator start, Iterator finish,
                   BinaryPredicate idx_comp) {

  // unsigned lhsdeg = 0, rhsdeg = 0;


  CTypes::comp_type result = CTypes::equality;

  while ( (start != finish) && (result == CTypes::equality) ) {
    CTypes::deg_type lhsdeg = 0, rhsdeg = 0;
    LhsIterator oldLhs(lhsStart); // maybe one can save this and do both
    RhsIterator oldRhs(rhsStart); // comparisons at once.

    // maybe one can save 
    while( (lhsStart != lhsFinish)  &&  (*lhsStart <  *start) ) {
      ++lhsStart; ++lhsdeg;
    }
    while( (rhsStart != rhsFinish)  &&  (*rhsStart <  *start) ) {
      ++rhsStart; ++rhsdeg;
    }
    result = generic_compare_3way(lhsdeg, rhsdeg, 
                                  std::greater<unsigned>() );
  
    if (result == CTypes::equality) {
      result = restricted_lex_compare_3way(oldLhs, lhsFinish,
                                           oldRhs, rhsFinish, *start, idx_comp);
    }
  
    ++start;
  }
    
  return result;
}


///@note: no necessary for block_dlex, only for block_dp_asc
template <class IdxType, class Iterator, class BinaryPredicate>
CTypes::comp_type
block_deg_lex_idx_compare(IdxType lhs, IdxType rhs, 
                          Iterator start, Iterator finish,
                          BinaryPredicate idx_comp) {

  if (lhs == rhs)
    return CTypes::equality;

  Iterator lhsend = std::find_if(start, finish, 
                                 std::bind2nd(std::greater<IdxType>(), lhs));


  Iterator rhsend = std::find_if(start, finish, 
                                 std::bind2nd(std::greater<IdxType>(), rhs));

  CTypes::comp_type result = CTypes::equality;

  if PBORI_UNLIKELY( (rhsend == finish) && (lhsend != finish) )
    result = CTypes::less_than;
  else if PBORI_UNLIKELY(lhsend == finish)
    result = CTypes::greater_than;
  else {
    PBORI_ASSERT((lhsend != finish) && (rhsend != finish));
    result = generic_compare_3way( *lhsend, *rhsend, std::less<IdxType>() );
  }

  return ( result == CTypes::equality? 
           generic_compare_3way(lhs, rhs, idx_comp): result );
}

END_NAMESPACE_PBORI

#endif
