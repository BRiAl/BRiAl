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


template<class NaviType>
class bounded_restricted_term {
public:
  typedef NaviType navigator;
  typedef bounded_restricted_term<navigator> self;
  typedef std::vector<navigator> stack_type;
  typedef unsigned size_type;
  typedef unsigned idx_type;

  bounded_restricted_term (): 
    m_stack(), m_max_idx(0), m_upperbound(0), m_next() {}

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

  NaviType find_deg(IdxType upperbound) const {
    NaviType navi = m_stack.top(); 
    navi.incrementThen();

    bounded_restricted_term<NaviType> bstart(navi, upperbound, max_idx), bend;

    bstart = std::max_element(bstart, bend);

    dummy_append(m_stack, bstart.begin(), bstart.end());

    return bstart.next();//.isEmpty();

      /// end?
#if 0
    
    IdxType deg = 0, max_deg = 0;
    //empty?
    assert(!m_stack.empty());
//     if (upperbound==0)
//       return 1;
    std::vector<NaviType> max_path;
    max_path.reserve(upperbound);

    bool notFound = true;

    // goto begin of next term

    NaviType current = m_stack.top(); 
    std::cout << "on pa0"<<*current<<std::endl;
    current.incrementThen();//deg++;
      std::cout << "on pa0"<<std::endl;
    while (notFound) {

      // bool notdone = (*current >= min_idx);
            std::cout << "on pa1"<<std::endl;
      while ( (deg < upperbound) && !at_end(current) ) {

        m_stack.push(current);
        deg++;
        current.incrementThen();
      }
      std::cout << "on pa2"<<std::endl;
      if (!on_path(current) ) {
        current = current.elseBranch();
       }
      std::cout << "on pa3"<<*current<<std::endl;
      if (on_path(current) ) {
        if (deg == upperbound) {
          return deg;
        }
      std::cout << "on pa4"<<std::endl;
        if (deg > max_deg) {
          max_deg = deg;
          max_path.clear();
        }
      }
      std::cout << "on pa5"<<std::endl;
      if (!m_stack.empty())
        current = m_stack.top();//!!!!!!!!!

      NaviType next=current;

      bool notdone = (*current >= min_idx);


      std::cout << "on pa6 "<<*current << max_path.size()<<std::endl;
      if(!notdone) {  
//         while(!current.isConstant())
//           current.incrementElse();
              
//         if ((upperbound ==0) && current.isTerminated() )
//           return 1;
        std::cout << "on pa6b "<<upperbound<<*current << max_path.size()<<max_idx<<std::endl;
        dummy_append(m_stack, max_path.rbegin(), max_path.rend());
        return  max_path.size();
      }
        std::cout << "on pa7 "<<*current <<std::endl;
      while(notdone) {

        assert(!m_stack.empty());
        m_stack.pop();
        deg--;
        assert(current.isValid());

        if (max_path.empty() || *current < *max_path.back() ) {
          max_path.push_back(current);
        }
        if (!current.isConstant())
          current.incrementElse();
   
        next = current;

        if (!m_stack.empty())
          current = m_stack.top();

        assert(current.isValid());
        std::cout << "on pa8 "<<*current <<std::endl;
        notdone = at_end(current) && (*current >= min_idx) && !m_stack.empty(); 
      }
        std::cout << "on pa9 "<<*current <<std::endl;
      assert(current.isValid());   
      assert(next.isValid());
      current = next;
      notFound = !( at_end(current) );
    }
    
    dummy_append(m_stack, max_path.rbegin(), max_path.rend());

    return max_path.size();//deg;
#endif
  }
  

  NaviType operator()() {

    assert(!m_stack.empty());
    unsigned deg = m_stack.size();
    unsigned subdeg = 0;
    NaviType current;

    do {
      assert(!m_stack.empty());
      current = m_stack.top();
      m_stack.pop();
      //   previous = current;
     std::cout << "curr "<<*current<<std::endl;

    current.incrementElse();

     std::cout << "curr2 "<<*current<<std::endl;
      subdeg++;

 

      while (!current.isConstant() && *current < max_idx) {
    
        m_stack.push(current);
        current.incrementThen();
        subdeg--;
      }
     std::cout << "curr3 "<<std::endl;
    } while ( !m_stack.empty() && (
                                   ( (*m_stack.top() >= min_idx) &&
                                     (  current.isEmpty() ||  (m_stack.size() != deg) )))) ;
    std::cout << "curr4 "<<std::endl;
    subdeg = deg - m_stack.size();//
    //    std::cout << "A"<<deg<<" "<< m_stack.size()<< " "<<*m_stack.top()<<std::endl;
    if (m_stack.size() == deg)
      return current;

    std::cout << "B"<<subdeg<<current.isTerminated()<<std::endl;
    if (m_stack.empty())
      return current;

    if ((subdeg == 0))
      return NaviType();

    std::cout << "C"<<std::endl;

    bounded_restricted_term<NaviType> bstart(m_stack.top().thenBranch(),
                                             subdeg - 1, max_idx), bend;
    bstart = std::max_element(bstart, bend);


    bstart.print();

    dummy_append(m_stack, bstart.begin(), bstart.end());

    return bstart.next();
#if 0
    NaviType current, next;
    IdxType deg = 0;
    bool notFound = true;
    IdxType last_deg = 0;
    bool LastNotFound = true;

    // goto begin of next term
    while (notFound) {

      assert(!m_stack.empty());
      current = m_stack.top();

      next=m_stack.top();
      bool atend = true;

      //assert(!current.isConstant());

      bool notdone = (*current >= min_idx)&&(LastNotFound);
      std::cout << "LastNotFound "<<LastNotFound<<std::endl;
      if(!notdone) {
      std::cout << "huhu1"<<std::endl;

        if (deg) {
          std::cout << "huhu2"<<*m_stack.top()<<std::endl;
       NaviType  tmp =  find_deg(deg-1);
       std::cout << "huhu2tmp "<<*tmp<< "" <<" "<<*m_stack.top()<<std::endl;
          return !tmp.isEmpty();
          }
        else { 

         std::cout << "deg0 "<<*current<< *current.elseBranch()<<std::endl;
         std::cout << "deg0 "<<*m_stack.top()<<max_idx<<std::endl;
          if (current.elseBranch().isTerminated())
            return false;
        }
          std::cout << "deg0 "<<*current<< *current.elseBranch()<<std::endl;
        return LastNotFound;
      }
      
      while(notdone) {

        next = m_stack.top();
        m_stack.pop();
        deg++;

        std::cout <<"huhu? "<<*next<<std::endl;
        if (m_stack.empty()) {
          assert(!next.isConstant());
          m_stack.push(next);
          notdone = false;
          --deg; //?

          LastNotFound = false;
        }
        else {
          current.incrementElse();
          next = current;
          current = m_stack.top();
          notdone = at_end(current) && (*current >= min_idx); 
        }
      }
      current = next; 
      std::cout <<"current "<<deg <<" "<<*current<<std::endl;
      if (*current < min_idx) {

        return true;
      }


      while (LastNotFound&& (deg > 0) && !at_end(current) ) {

        assert(!current.isConstant());
        m_stack.push(current);
        deg--;
        current.incrementThen();
      }

      notFound = !((deg == 0) && at_end(current) );
      std::cout <<"notFound "<<deg <<" "<<*current<<std::endl;
    }

    return false;
 #endif
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
    navigator next;
    // the term one
    if (!current.isValid()) {
      base::clear();
      return *this;
    }

    while (*current < blockMin())
      --m_current_block;
    
    bool notfound = true;

    while (notfound) {
      std::cout <<"max? "<<blockMax()<< " "<< *base::top()<<std::endl;
      deg_next_term<stack_type, navigator, unsigned>
        nextop(base::m_stack,  blockMin(), blockMax());
      print();
      navigator tmp =  nextop();
      if (!tmp.isValid()) {
        std::cout << "NOT TREATED YET!" <<std::endl;
        return *this;
      }

      notfound = tmp.isEmpty();
      std::cout<< "not found? "<<notfound<<std::endl;
      print();
      if(notfound)  {

        if  (m_current_block == m_indices) {
          assert(!base::empty());
          current = base::m_stack.bottom();

          // owns one?
          while (!current.isConstant())
            current.incrementElse();

          base::clear();
          
          if (current.terminalValue())
            base::m_stack.push(navigator());

          return *this;
        }
        --m_current_block;
      }
      else
        next = tmp;

    }


//     navigator navi(base::top());


//     // go to next block
//     if ( !atBlockEnd(navi) )
//       navi.incrementThen();
//     std::cout <<"atend "<< *navi<<std::endl;
//     while( !atBlockEnd(navi) ){
//       navi.incrementElse();
//       std::cout <<"atend "<< *navi<<std::endl;
//    }
//     std::cout <<"atend "<< *navi<<std::endl;
    findTerminal(next);
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
    std::cout <<"max "<<blockMax()<<std::endl;
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


    
    CBlockIterator<delayed_iterator> biter(poly.navigation(), next_block, blockDegCache);
    /**/
    for(unsigned i = 0; i < 12; ++i) {
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
