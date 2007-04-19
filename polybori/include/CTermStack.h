// -*- c++ -*-
//*****************************************************************************
/** @file CTermStack.h
 *
 * @author Alexander Dreyer
 * @date 2006-04-21
 *
 * 
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
 * Revision 1.5  2007/04/19 16:33:08  dreyer
 * CHANGE: code cleaned-up
 *
 * Revision 1.4  2007/04/18 15:37:28  dreyer
 * ADD: dp_asc now active
 *
 * Revision 1.3  2007/04/13 15:18:10  dreyer
 * CHANGE: fine tuning
 *
 * Revision 1.2  2007/04/13 13:55:53  dreyer
 * CHANGE: using CTermStack for implementing ordered_(exp_)iterator
 *
 * Revision 1.1  2007/04/11 16:09:03  dreyer
 * ADD: initial version
 *
 * @endverbatim
**/
//*****************************************************************************

// get standard header
#include <stack>
#include <iterator>

// include basic definitions
#include "pbori_defs.h"

// include polybori functionals
#include "pbori_func.h"

// include polybori properties
#include "pbori_traits.h"

  ///???

#include "pbori_routines.h"

// include boost's indirect iterator facilities
#include <boost/iterator/indirect_iterator.hpp>

#include "BoolePolyRing.h"
#include "CDegreeCache.h"
#include "CBidirectTermIter.h"



#ifndef CTermStack_h_
#define CTermStack_h_

BEGIN_NAMESPACE_PBORI


/** @class CTermStack
 * @brief This class defines an iterator for the monomials in a Boolean
 * polynomial.
 *
 **/


template <class NavigatorType>
class CTermStackBase {

public:
  typedef CTermStackBase<NavigatorType> self;

  /// Get type of navigators
  typedef NavigatorType navigator;
  /// Type for indices
  typedef typename navigator::idx_type idx_type;

  /// Type for lengths
  typedef typename navigator::size_type size_type;
  typedef typename navigator::bool_type bool_type;


  /// Define type for stacking
  typedef std::deque<navigator> stack_type;

  typedef typename stack_type::reference       reference;
  typedef typename stack_type::const_reference const_reference;

  typedef boost::indirect_iterator<typename stack_type::const_iterator,
                                   typename navigator::value_type, 
                                   boost::use_default, 
                                   typename navigator::reference>
  const_iterator;

  typedef boost::indirect_iterator<typename stack_type::const_reverse_iterator,
                                   typename navigator::value_type, 
                                   boost::use_default, 
                                   typename navigator::reference>
  const_reverse_iterator;

protected:
  void clear() { m_stack.clear(); }


  void pop() { m_stack.pop_back(); }

public:
  void push(const navigator& __x) { m_stack.push_back(__x); }
  reference top() { return m_stack.back(); }
  const_reference top() const { return m_stack.back(); }

  bool_type empty() const { return m_stack.empty(); }
  size_type size() const { return m_stack.size(); }

  const_iterator begin() const { 
    if (markedOne())
      return end();
    else
      return m_stack.begin(); 
  }

  const_iterator end() const { return m_stack.end(); }

  const_reverse_iterator rbegin() const {
    if (markedOne())
      return rend();
    else
      return m_stack.rbegin(); 
  }

  const_reverse_iterator rend() const { return m_stack.rend(); }


  /// result type of top()
  typedef typename stack_type::value_type top_type;

  /// Default constructor
  CTermStackBase(): m_stack() { }

  /// Construct from initial navigator
  CTermStackBase(navigator navi):  m_stack() {
    push(navi);
  }

  /// default Copy Constructor


  /// Equality test (assume iterators from same instance)
  bool_type equal(const self& rhs) const {
    if(empty() || rhs.empty())
      return (empty() && rhs.empty());
    else
      return top() == rhs.top(); 
  }

  void incrementThen() {
    assert(!top().isConstant());

    push(top());
    top().incrementThen();
  }


  void decrementNode() {
    assert(!empty());
    pop();
  }

  bool_type isConstant() const {
    assert(!empty());
    return top().isConstant();
  }

  bool_type isTerminated() const {
    assert(!empty());
    return top().isTerminated();
  }

  bool_type isInvalid() const {
    assert(!empty());
    return top().isEmpty();
  }

  void followThen() {
    assert(!empty());
    while(!isConstant())
      incrementThen();
  }

  void markOne() {
    assert(empty());
    push(navigator());
  }

  bool_type markedOne() const {
    if (empty())
      return false;
    else
      return !m_stack.front().isValid();
  }

  void clearOne() {
    pop();
    assert(empty());
  }
  void print() const{
    std::cout <<"(";
    std::copy(begin(), end(), std::ostream_iterator<int>(cout, ", ")); 
    std::cout <<")";
  }
protected:

  void append(const self& rhs) { 
    assert(empty() || rhs.empty() || ((*rhs.begin()) > (*top())) );
    m_stack.insert(m_stack.end(), rhs.m_stack.begin(), rhs.m_stack.end());
  }


private:
  stack_type m_stack;
};




template <class NavigatorType, class Category = std::forward_iterator_tag>
class CTermStack :
  public CTermStackBase<NavigatorType> {

public:
  typedef CTermStackBase<NavigatorType> base;
  typedef typename base::navigator navigator;
  
  typedef typename on_same_type<Category, std::forward_iterator_tag, 
                                project_ith<0>, 
                                handle_else<NavigatorType> >::type
                                else_handler;

  else_handler handleElse;

  using base::incrementThen;
  using base::followThen;

 /// Default constructor
  CTermStack(): base() { }

  /// Construct from initial navigator
  CTermStack(navigator navi): base(navi) { }

  void firstTerm() {
    followThen();
    terminate();
  }

  void lastTerm() {
    followElse();
    terminate();
  }

  void incrementElse() {
    assert(!base::isConstant());
    handleElse(base::top());
    base::top().incrementElse();
  }

  void nextTerm() {

    bool invalid = true;
    while (!base::empty() && invalid) {
      incrementElse();
      if (invalid = base::isInvalid())
        base::decrementNode();
    }
  }

  void previousTerm() {
    previousTerm(Category());
  }
 
  bool isTermEnd() const {
    assert(!base::empty());
    typename base::navigator navi(base::top());
    
    while(!navi.isConstant()) {
      navi.incrementElse();
    }
    return navi.terminalValue();

  };

  void incrementTerm() {
    assert(!base::empty());
    if (base::markedOne()) {
      base::clearOne();
      return;
    }
      
    nextTerm();
    if (!base::empty()) {
      followThen();
      terminate();
    }

  }

   void decrementTerm() {

    if (base::markedOne()) {
      base::clearOne();
    }
      
    previousTerm();
    followElse();
    base::decrementNode();

  }

  void terminate() {
    assert(!base::empty());

    bool isZero = base::isInvalid();
    base::decrementNode();
    if (base::empty() && !isZero)
      base::markOne();
  }


  void followElse() {
    while( !base::isConstant() ) {       // if still in interior of a path
      if(!base::top().elseBranch().isEmpty()) {
        incrementElse();   // go in direction of last term, if possible
      }
      else
        incrementThen();
    } 
  }

protected:
  void append(const CTermStack& rhs) {
    base::append(rhs);
    append(rhs, Category());
  }

private:
  void previousTerm(std::forward_iterator_tag);
  void previousTerm(std::bidirectional_iterator_tag);

  void append(const CTermStack&, std::forward_iterator_tag){};
  void append(const CTermStack& rhs, std::bidirectional_iterator_tag){
     handleElse.append(rhs.handleElse); 
  };
};


template <class NavigatorType, class Category>
inline void CTermStack<NavigatorType, Category>::previousTerm(
  std::forward_iterator_tag) { }

template <class NavigatorType, class Category>
inline void CTermStack<NavigatorType, Category>::previousTerm(
  std::bidirectional_iterator_tag) { 

  if(handleElse.empty()) {
    base::clear();
    return;
  }

  navigator navi = handleElse.top();

  assert(base::top().isValid());

  while(!base::empty() && (*base::top() >= *navi) ) {
    base::decrementNode();
  }

  handleElse.pop();
  base::push(navi);
  incrementThen();
}

template <class NavigatorType, class DescendingProperty>
class CProxTermStack;

template <class NavigatorType>
class CProxTermStack<NavigatorType, valid_tag>:
  public CTermStack<NavigatorType, std::forward_iterator_tag> {

public:
  typedef CTermStack<NavigatorType, std::forward_iterator_tag> base;

  typedef std::greater<typename base::size_type> size_comparer;


  CProxTermStack(): base() {}
  CProxTermStack(NavigatorType navi): base(navi) {}

  integral_constant<bool, false> takeLast;

  template <class StackType>
  void proximateTerm(StackType& rhs) { rhs.nextTerm(); }

  void incrementBranch() { base::incrementThen(); }

  template <class FuncType, class SizeType>
  bool maxOnThen(FuncType getDeg, SizeType deg) const {
    return (getDeg(base::top().thenBranch()) + 1 == deg);
  }

};

template <class NavigatorType>
class CProxTermStack<NavigatorType, invalid_tag>:
  public CTermStack<NavigatorType, std::bidirectional_iterator_tag> {

public:
  typedef CTermStack<NavigatorType, std::bidirectional_iterator_tag> base;

  typedef std::greater_equal<typename base::size_type> size_comparer;


  CProxTermStack(): base() {}
  CProxTermStack(NavigatorType navi): base(navi) {}

  integral_constant<bool, true> takeLast;

  template <class StackType>
  void proximateTerm(StackType& rhs) { rhs.previousTerm(); }

  void incrementBranch() {

    // if still in interior of a path
    if(!base::top().elseBranch().isEmpty()) {
      base::incrementElse();   // go in direction of last term, if possible
    }
    else
      base::incrementThen();
  }

  template <class FuncType, class SizeType>
  bool maxOnThen(FuncType getDeg, SizeType deg) const {
    return !(getDeg(base::top().elseBranch())  ==  deg);
  }
};

template<class NavigatorType>
struct cached_deg {

  cached_deg(): m_deg_cache(BoolePolyRing::activeManager()) {}

  typename NavigatorType::size_type 
  operator()(const NavigatorType& navi) const {
    return dd_cached_degree(m_deg_cache, navi);
  }
  CDegreeCache<> m_deg_cache;
};


template <class NavigatorType, class DescendingProperty = valid_tag>
class CDegTermStack:
  public CProxTermStack<NavigatorType, DescendingProperty> {

public:
  typedef CProxTermStack<NavigatorType, DescendingProperty> base;

  typedef NavigatorType navigator;
  typedef typename navigator::size_type size_type;
  typedef DescendingProperty descending_property;
  typedef CDegTermStack<navigator, descending_property> self;
  typedef BoolePolynomial poly_type;
  typedef typename poly_type::dd_type dd_type;



  CDegTermStack(): base(), m_start() {}
  CDegTermStack(navigator navi): base(navi), m_start(navi) {}

  cached_deg<navigator> cachedDeg;

  void firstTerm() {

    assert(!base::empty());
    
    size_type deg = cachedDeg(base::top());

    while (deg > 0) {

      if ( base::maxOnThen(cachedDeg, deg) ) {
        --deg;
        base::incrementThen();
      }
      else
        base::incrementElse();
        
    }

    base::terminate();    
  }

  void incrementTerm() {
    assert(!base::empty());
    if (base::markedOne()) {
      base::clearOne();
      return;
    }


    size_type upperbound = base::size();
    degTerm();

    if(base::empty())
      findTerm(upperbound);

    if(!base::empty())
      base::terminate();
  }


  void degTerm() {
    size_type size = base::size() + 1;

    assert(!base::isConstant());

    base::incrementThen();
  
    do {
      base::decrementNode();
      base::proximateTerm(*this);
      if( base::empty() )
        return;
  
      while(!base::isConstant() && (base::size() < size) )
        base::incrementBranch();
  
    } while  (!base::empty() && 
              !(base::isTermEnd() && (base::size() == size) )  );

  }

 
  void decrementTerm() {}


  void findTerm(size_type upperbound){

    typename base::size_comparer comp;

    base::push(m_start);
    self tmp;
    
    while (!base::empty() && (base::takeLast()|| (tmp.size() != upperbound)) ) {
      
      while (!base::isConstant() && (base::size() < upperbound) )
        base::incrementThen();
      
      if (base::isTermEnd()) {
        if (comp(base::size(), tmp.size()))
          tmp = *this;
        base::decrementNode();
      }
      
      base::nextTerm();
    }
    *this = tmp;
  }


private:
  navigator m_start;
};

//////////////////////////////////////////////////////////

template<class NavigatorType>
class cached_block_deg {
public:
  typedef  typename NavigatorType::idx_type idx_type;

  cached_block_deg(idx_type idx):
    m_max_idx(idx), m_deg_cache(BoolePolyRing::activeManager()) { }

  typename NavigatorType::size_type 
  operator()(const NavigatorType& navi) const {
    return dd_cached_block_degree(m_deg_cache, navi, m_max_idx);
  }

private:
  idx_type m_max_idx; 
  CBlockDegreeCache<CCacheTypes::block_degree, CTypes::dd_type, 
                    CTypes::manager_base> m_deg_cache;
};

//////////////////////////////////////////////////////////
template <class NavigatorType, class DescendingProperty = valid_tag>
class CBlockTermStack:
  public CProxTermStack<NavigatorType, DescendingProperty> {

public:
  typedef CProxTermStack<NavigatorType, DescendingProperty> base; 

  typedef NavigatorType navigator;
  typedef typename navigator::size_type size_type;
  typedef DescendingProperty descending_property;
  typedef CBlockTermStack<navigator, descending_property> self;
  typedef BoolePolynomial poly_type;
  typedef typename poly_type::dd_type dd_type;
  typedef poly_type PolyType;


  CBlockTermStack(navigator navi): 
    base(navi), m_indices(BoolePolyRing::blockRingBegin()), 
    m_current_block(BoolePolyRing::blockRingBegin()),
    m_deg_cache(BoolePolyRing::activeManager()), m_navi(navi) {    

  }
  void firstTerm() {
    assert(!base::empty());
    findTerminal();
    base::terminate();
  }

  // Default Constructor
  CBlockTermStack(): base(), m_indices(), m_current_block(),
                  m_deg_cache(PolyType().diagram().manager()),
                  m_navi(){}


  typedef typename PolyType::idx_type idx_type;
   /// Type for block indices
  typedef std::vector<idx_type> block_idx_type;

  typedef typename base::stack_type stack_type;

  /// Type for block iterators
  typedef typename block_idx_type::const_iterator block_iterator;

  size_type currentBlockDegree(const navigator& navi) const {
    return dd_cached_block_degree(m_deg_cache, navi, *m_current_block);
  }
  
  void incrementBlock() {
    size_type deg = currentBlockDegree(base::top());

    while(deg > 0) {
  
      if ( base::maxOnThen(cached_block_deg<navigator>(blockMax()), deg) ){
        --deg;
        assert(!base::isConstant());
        base::incrementThen(); 
      }
      else {
        assert(!base::isConstant());
        base::incrementElse();
        assert(!base::isConstant());
      }
    }
  }


  idx_type blockMin() const {
    return ( m_current_block == m_indices? 0: *(m_current_block - 1) );
  }

  idx_type blockMax() const {
    return *m_current_block;
  }

 void incrementTerm() {

    assert(!base::empty());
    if (base::markedOne()) {
      base::clearOne();
      return;
    }

    navigator current = base::top(); 
  
    while (*current < blockMin())
      --m_current_block;

   
    ++m_current_block;

    do {
      --m_current_block;
    
      degTerm();
      assert(!base::empty()); 
      current = base::top();
      base::pop();

    } while (!base::empty() && current.isEmpty());

    base::push(current);
    findTerminal();

    if(!base::empty())
      base::terminate();
 
  }

   bool isBlockEnd() const {
     assert(!base::empty());
     navigator navi(base::top());
     while( (!navi.isConstant()) && (*navi < blockMax()) ) {
       navi.incrementElse();
     }
     return (navi.isConstant()? navi.terminalValue(): *navi >= blockMax());
   }

  void gotoBlockEnd()  {
     assert(!base::empty());
     navigator navi(base::top());
     while( (!base::isConstant()) && (*base::top() < blockMax()) ) {
       base::incrementElse();
     }
  }

  void findTerminal() {
    assert(base::top().isValid());
    if (!base::isConstant() ) 
      incrementBlock();

    while (!base::isConstant()  ) {
      assert (blockMax() != CUDD_MAXINDEX);
      ++m_current_block;

      incrementBlock();
    }
  }

  void nextTerm() {

    bool invalid = true;
    while (!base::empty() && (*base::top() >= blockMin()) && invalid) {
      assert(!base::isConstant());
      base::incrementElse();
      if (invalid = base::isInvalid())
        base::decrementNode();
    }
  }
  void previousTerm() {

    if( base::handleElse.empty() || (*base::handleElse.top() <  blockMin()) ) {
      while(!base::empty() && (*base::top() >= blockMin()) )
        base::decrementNode();
      return;
    }
    navigator navi =  base::handleElse.top();
    assert(base::top().isValid());

    while(!base::empty() && (*base::top() >= *navi) 
          && (*base::top() >= blockMin()) ) {
      base::decrementNode();
    }

    if (base::empty() || (*base::top() < *navi)) {
      base::handleElse.pop();
      base::push(navi);
    }
      base::incrementThen();
  }

  void degTerm();



  using base::print;

  block_iterator m_indices;
  block_iterator m_current_block;

  navigator m_navi;
  CBlockDegreeCache<CCacheTypes::block_degree, CTypes::dd_type,
                          CTypes::manager_base> m_deg_cache; 

};

template <class NavigatorType, class DescendingProperty>
inline void
CBlockTermStack<NavigatorType, DescendingProperty>::degTerm() {

  idx_type min_idx(blockMin()), max_idx(blockMax());

  assert(!base::empty());
    unsigned deg = base::size();
    unsigned size = deg +1;
    typedef navigator  NaviType;
    NaviType current(m_navi);

    if (*base::top() < min_idx) {
      base::push(BoolePolynomial(false).navigation());
      return;
    }
   
    bool notfound = true;; 
    size_type prev;
    bool blockended;
    bool isFound;


    do {
      assert(!base::empty());
      base::proximateTerm(*this);

      if ( !base::empty() && (notfound = (*base::top() >= min_idx)) ) {
        while (!base::isConstant() && (base::size() <= deg) && 
               (*base::top() < max_idx)) {
          base::incrementBranch();
        }
        gotoBlockEnd();
        current = base::top();
        base::decrementNode();
      }
    } while ( !base::empty() && notfound &&
              !(!current.isEmpty() && (base::size() == deg) ) );

    if (base::size() == deg) {
      base::push(current);
      return;
    }

    if (base::empty())
      base::push(m_navi);
    else {
      base::incrementThen();
    }

     while (!base::isConstant() && (*base::top() < min_idx))
       base::incrementElse();

     if (*base::top()  < min_idx) { 
       base::pop();
       base::push( BoolePolynomial(false).navigation());
       return;
     }

     assert(deg >= base::size()); 

     base tmp, max_elt(base::top());

     typename base::size_comparer comp;    
       

     unsigned upperbound(deg - base::size()+1);

     while (!max_elt.empty() && 
            (base::takeLast() || (tmp.size() != upperbound)) ) {
      
       while (!max_elt.isConstant() && (max_elt.size() < upperbound) 
              && (*max_elt.top() < max_idx) )
        max_elt.incrementThen();

       navigator navi(max_elt.top());
       while(!navi.isConstant() && (*navi < max_idx)) {
         navi.incrementElse();
       }

      if (navi.isTerminated() || 
          (!navi.isConstant () &&(*navi >= max_idx)) ) {
        if (comp(max_elt.size(), tmp.size())) {
          tmp = max_elt;
          tmp.decrementNode();
          tmp.push(navi);
        }
        max_elt.decrementNode();
      }
      max_elt.nextTerm();
     }

     base::decrementNode();

     base::append(tmp);
     if(tmp.empty())
       base::push(BoolePolynomial(false).navigation());

}

END_NAMESPACE_PBORI

#endif
