 // -*- c++ -*-
//*****************************************************************************
/** @file 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file contains elementary tests
 *
 * @par Copyright:
 *   (c) 2007 by
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
 * Revision 1.8  2007/02/20 09:41:06  dreyer
 * CHANGE: now running prototype for dlex-block iteration
 *
 * Revision 1.7  2007/02/19 17:21:51  dreyer
 * CHANGE: routine check-in
 *
 * Revision 1.6  2007/02/16 16:14:22  dreyer
 * CHANGE: routine check-in
 *
 * Revision 1.5  2007/02/15 17:03:12  dreyer
 * + Routine check-in
 *
 * Revision 1.4  2007/02/14 13:56:10  dreyer
 * CHANGE: finished prototype for block iterator
 *
 * Revision 1.3  2007/02/09 17:07:00  dreyer
 * CHANGE: experimenting once more
 *
 * Revision 1.2  2007/02/07 11:01:51  dreyer
 * CHANGE: routine
 *
 * Revision 1.1  2007/01/29 17:03:44  dreyer
 * ADD: toy sandbox for block orderings
 * @endverbatim
**/
//*****************************************************************************


// load standard iostream capapilities
#include <iostream>
#include <algorithm>

// load polybori header file
# include "polybori.h"

#include "CDegreeCache.h"

USING_NAMESPACE_PBORI


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
    assert(!empty());

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
    assert(!empty());

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
    assert(!m_next.isConstant());
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



struct block_degree: public CCacheTypes::binary_cache_tag { };
struct block_dlex_lead: public CCacheTypes::unary_cache_tag { };

template <class TagType =  block_degree,
          class DDType = typename CTypes::dd_type,
          class ManagerType = typename CTypes::manager_base>
class CBlockDegreeCache:
  public CCacheManagement<TagType, 2, ManagerType> {

public:
  /// @name Define generic access to data types
  //@{
  typedef ManagerType manager_type;
  typedef DDType dd_type;
  typedef TagType tag_type;
  typedef CCacheManagement<tag_type, 2, manager_type> base;
  typedef CBlockDegreeCache<tag_type, dd_type, manager_type> self;
  //@}

  /// @name Adopt type definitions
  //@{
  typedef typename base::node_type input_node_type;
  typedef typename dd_type::idx_type idx_type;
  typedef typename dd_type::size_type size_type;
  typedef typename dd_type::navigator navi_type;
  typedef CIndexHandle<navi_type> node_type;
  //@}

  /// Constructor
  CBlockDegreeCache(const manager_type& mgr): base(mgr) {}

  /// Copy Constructor
  CBlockDegreeCache(const self& rhs): base(rhs) {}

  /// Destructor
  ~CBlockDegreeCache() {}

  /// Find cached degree wrt. given navigator
  node_type find(input_node_type navi, idx_type idx) const{ 
    return node_type(base::find(navi, node_type(idx))); }

  /// Store cached degree wrt. given navigator
  void insert(input_node_type navi, idx_type idx, size_type deg) const {
    base::insert(navi, node_type(idx), node_type(deg));
  }
};


/// Function templates for determining the degree of a decision diagram
/// with the help of cache (e. g. CDegreeCache)
template <class DegreeCacher, class NaviType, class IdxType>
typename NaviType::size_type
dd_cached_block_degree(const DegreeCacher& cache, NaviType navi, 
                       IdxType nextBlock) {

  typedef typename NaviType::size_type size_type;

  if( navi.isConstant() || (*navi >= nextBlock) ) // end of block reached
    return 0;
 
  // Look whether result was cached before
  typename DegreeCacher::node_type result = cache.find(navi, nextBlock);
  if (result.isValid())
    return *result;
  
  // Get degree of then branch (contains at least one valid path)...
  size_type deg = dd_cached_block_degree(cache, navi.thenBranch(), nextBlock) + 1;
 
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
    return navi;

  while( (*navi >= *block_iter) && (*block_iter != CUDD_MAXINDEX))  {
    ++block_iter;
    degree = dd_cached_block_degree(deg_mgr, navi, *block_iter);
  }


  // Check cache for previous results
  NaviType cached = cache_mgr.find(navi);
  if (cached.isValid())
    return cached;

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
  cache_mgr.insert(navi, resultNavi);
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
    return navi;
  
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
    unsigned lhsdeg = 0, rhsdeg = 0;
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


template <class Iterator>
void dummy_print2(Iterator start, Iterator finish) {
  std::cout << "[";
  while (start != finish) {
    std::cout << **start <<std::endl;
    ++start;
  }
  std::cout << "]"<<std::endl;;
}
template <class StackType, class Iterator>

void dummy_append(StackType& stack, Iterator start, Iterator finish) {

  while (start != finish) {
    stack.push(*start);
    ++start;
  }
}

template <class StackType, class NaviType, class IdxType>
class deg_next_term {
public:

  deg_next_term(StackType& thestack, IdxType mini, IdxType maxi):
    m_stack(thestack), min_idx(mini), max_idx(maxi)/*, m_deg_cache(deg_cache)*/ {

    assert(mini < maxi);
  }



  //CBlockDegreeCache<> m_deg_cache;

  bool at_end(const NaviType& navi) const {
    
    return navi.isConstant()  || (*navi >= max_idx);
  }

  bool on_path(const NaviType& navi) const {

    return (navi.isConstant() && navi.terminalValue()) ||
      (!navi.isConstant()&&(*navi >= max_idx));
  }


  NaviType operator()() {

    assert(!m_stack.empty());
    unsigned deg = m_stack.size();
    NaviType current;

    do {
      assert(!m_stack.empty());
      current = m_stack.top();
      m_stack.pop();

      current.incrementElse();

      while (!current.isConstant() && *current < max_idx) {
    
        m_stack.push(current);
        current.incrementThen();
      }
    } while ( !m_stack.empty() && (*m_stack.top() >= min_idx) &&
              (current.isEmpty() || (m_stack.size() != deg)) );

     if (m_stack.size() == deg)
      return current;

     if (m_stack.empty())
      return current;

    bounded_restricted_term<NaviType>
      bstart(m_stack.top().thenBranch(),  deg - m_stack.size() - 1, max_idx),
      bend;
    bstart = std::max_element(bstart, bend);
    dummy_append(m_stack, bstart.begin(), bstart.end());

    return bstart.next();
  }


protected:
  StackType& m_stack;
  IdxType min_idx, max_idx;
};

template <class DelayedIterator>
class CBlockIterator:
  public DelayedIterator {

public:

  typedef DelayedIterator base;
  typedef CBlockIterator<base> self;
  typedef typename base::stack_type stack_type;
  typedef typename base::navigator navigator;
  typedef unsigned size_type;
  typedef unsigned idx_type;

  CBlockIterator(navigator navi, unsigned* indices, 
                 const CBlockDegreeCache<>& deg_cache):
    base(),  m_indices(indices), m_deg_cache(deg_cache), 
    m_current_block(indices) {
    findTerminal(navi);   
  }

  size_type currentBlockDegree(const navigator& navi) const {
    return dd_cached_block_degree(m_deg_cache, navi, *m_current_block);
  }
  
  void incrementBlock(navigator& navi) {
    incrementBlock(navi, currentBlockDegree(navi));
  }

  void incrementBlock(navigator& navi, unsigned deg) {

    while(deg > 0) {
      --deg;
      if ( currentBlockDegree(navi.thenBranch()) == deg){
        assert(!navi.isConstant());
        base::m_stack.push(navi);
        navi.incrementThen(); 
      }
      else {
        ++deg;
        navi.incrementElse();
        assert(!navi.isConstant());
        base::m_stack.push(navi);
      }
    }
  }


  idx_type blockMin() const {
    return ( m_current_block == m_indices? 0: *(m_current_block - 1) );
  }

  idx_type blockMax() const {
    return *m_current_block;
  }

  self & operator++() {

    // the zero term
    if (base::empty())
      return *this;

    navigator current = base::m_stack.top(); 
  
    // the term one
    if (!current.isValid()) {
      base::clear();
      return *this;
    }

    while (*current < blockMin())
      --m_current_block;
    ++m_current_block;


    do {
      --m_current_block;
           
      deg_next_term<stack_type, navigator, unsigned>
        nextop(base::m_stack,  blockMin(), blockMax());
  
      current = nextop();

    } while (!base::empty() && current.isEmpty());
 
    findTerminal(current);

    if (base::empty() && current.terminalValue()) {
      base::clear();
      base::m_stack.push(navigator());
    }
 
    return *this;
  }

  bool atBlockEnd(navigator navi) const {
    return navi.isConstant() || (*navi >= blockMax());
  }

  // template <class IdxIterator>
  void findTerminal(navigator navi) {
    if (!navi.isConstant() ) 
      incrementBlock(navi);

    while (!navi.isConstant()  ) {
      assert (blockMax() != CUDD_MAXINDEX);
      ++m_current_block;

      incrementBlock(navi);
    }
  }

  void print() const {
      std::cout << ":";
      std::cout.flush();
    stack_type thestack( base::m_stack);
    while (!thestack.empty()){
      if (thestack.top().isValid())
        std::cout << *(thestack.top()) << ", ";
      else 
        std::cout << "one";
      std::cout.flush();
      thestack.pop();
    }
  }


  // std::stack<navigator> base::m_stack;

  unsigned* m_indices;
  unsigned* m_current_block;

  const CBlockDegreeCache<>& m_deg_cache;
};




int
main(){

  std::cout << "Experimenting with block orderings..." << std::endl;   

  try {
    BoolePolyRing the_ring(10);

    BooleMonomial x0 = BooleVariable(0);
    BooleMonomial x1 = BooleVariable(1);
    BooleMonomial x2 = BooleVariable(2);
    BooleMonomial x3 = BooleVariable(3);
    BooleMonomial x4 = BooleVariable(4);
    BooleMonomial x5 = BooleVariable(5);
    BooleMonomial x6 = BooleVariable(6);
    BooleMonomial x7 = BooleVariable(7);
    BooleMonomial x8 = BooleVariable(8);
    BooleMonomial x9 = BooleVariable(9);


    BoolePolynomial poly = 
      x1*x2*x6*x9  + x1*x2*x7*x9+ x1*x2*x7+ x1*x2*x8*x9 
      +   x1*x3*x6*x9  + x1*x3*x7*x9+ x1*x3*x7+ x1*x3*x8*x9 + x1*x5+x1 +x2+1; 

      // x1+ x2*x3+ x1*x2 +  x1*x2*x6  + x1*x2*x7+ x3*x4*x5;
    std::cout << "Polynom: "<< poly <<std::endl;

    CBlockDegreeCache<> blockDegCache(poly.diagram().manager());

    std::cout <<"Block degree wrt. 2: " << 
      dd_cached_block_degree(blockDegCache, poly.navigation(), 2) << std::endl;

    CCacheManagement<block_dlex_lead> cache_mgr(poly.diagram().manager());

    BoolePolynomial::navigator navi(poly.navigation());

    unsigned next_block[3] = {4, 6, CUDD_MAXINDEX};



    std::cout << dd_block_degree_lead(cache_mgr, blockDegCache, navi, 
                                      next_block,
                                      BooleSet(), 
                                      valid_tag());


    BooleMonomial monom1 = x1*x2*x6, monom2 = x1*x2*x7;

    std::cout << monom1<< " < " << monom2 << "? "<<
      block_dlex_compare(monom1.begin(), monom1.end(),
                         monom2.begin(), monom2.end(),
                         next_block, next_block+3, std::less<unsigned>())
              << std::endl;

    std::cout << "block iter..."<<std::endl;

    typedef unsigned size_type;
    // Incrementation functional type
    typedef 
      binary_composition< std::plus<size_type>, 
      project_ith<1>, integral_constant<size_type, 1> > 
      increment_type;
      
    // Decrementation functional type
    typedef 
      binary_composition< std::minus<size_type>, 
      project_ith<1>, integral_constant<size_type, 1> > 
      decrement_type;
    typedef BoolePolynomial::navigator navigator;
    // Iterator type for iterating all monomials (dereferencing to degree)
    typedef CTermIter<size_type, navigator, 
      increment_type, decrement_type,
      integral_constant<size_type, 0> >
      deg_iterator;

    typedef BooleMonomial monom_type;
    // Iterator type, which extends deg_iterator with function term()
    typedef CDelayedTermIter<monom_type, 
      change_assign<>, project_ith<2>, 
      deg_iterator> delayed_iterator;


    
    CBlockIterator<delayed_iterator> biter(poly.navigation(), next_block,
    blockDegCache);
    /**/
    delayed_iterator bstop;
    while (biter != bstop) {
      biter.print();
      std::cout << " "<<  biter.term() <<std::endl;
      ++biter;
    }
    /**/


    bounded_restricted_term<navigator> bounded_iter(poly.navigation(), 2, 4);
    bounded_restricted_term<navigator> bounded_end;
    /*
    std::cout << "bounded max"<<std::endl;
    bounded_restricted_term<navigator> bounded_max =
      std::max_element(bounded_iter, bounded_end);

    std::cout << "* " <<  *bounded_max<<std::endl;
    std::cout << "next " <<  *bounded_max.next()<<" " <<
      bounded_max.next().isTerminated()<<std::endl;
    bounded_max.print();   std::cout <<std::endl;std::cout.flush();
    std::cout <<std::endl<< "bounded iter"<<std::endl;

    while ( bounded_iter !=  bounded_end ){
      std::cout << "* " <<  *bounded_iter<<std::endl;
      std::cout << "next " <<  *bounded_iter.next()<< 
        "  "<<bounded_iter.next().isTerminated()<<std::endl; 
      bounded_iter.print();   std::cout <<std::endl;std::cout.flush();
      ++bounded_iter;
    }

      std::cout << "next " <<  *bounded_iter.next()<< "  "<<bounded_iter.next().isTerminated()<<std::endl; 
      bounded_iter.print();

    */
    /*  std::cout << "Experimenting with negations..."<<std::endl;
    navi = poly.navigation();
    std::cout << navi.operator->()<<std::endl;
    std::cout <<  Cudd_Not(navi.operator->())<<std::endl;
    std::cout <<  Cudd_Regular(navi.operator->())<<std::endl;
    std::cout <<  Cudd_Regular( Cudd_Not(navi.operator->()))<<std::endl;
    std::cout <<  Cudd_Complement(navi.operator->())<<std::endl;
    */
    std::cout << "Finished."<<std::endl;
  }
  catch (PBoRiError& err) {
    std::cout << "  Caught error # "<< err.code() <<std::endl;   
    std::cout << "    which says: "<< err.text() <<std::endl;  
 }

  return 0;
}
