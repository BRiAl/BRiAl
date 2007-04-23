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
 * Revision 1.7  2007/04/23 15:32:17  dreyer
 * CHANGE: clean-up (reuse stuff from deg-orderings for block-orderings)
 *
 * Revision 1.6  2007/04/22 19:47:21  dreyer
 * CHANGE: Code cleaned-up
 *
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

//////////////////////////////////////////////////////////
template<class NavigatorType>
struct cached_deg {

  cached_deg(): m_deg_cache(BoolePolyRing::activeManager()) {}

  typename NavigatorType::size_type 
  operator()(NavigatorType navi) const {
    return dd_cached_degree(m_deg_cache, navi);
  }
  CDegreeCache<> m_deg_cache;
};

//////////////////////////////////////////////////////////

template <class NavigatorType>
class cached_block_deg {
public:
  typedef typename NavigatorType::idx_type idx_type;
  typedef cached_block_deg<NavigatorType> self;

  /// Type for block indices
  typedef std::vector<idx_type> block_idx_type;

  /// Type for block iterators
  typedef typename block_idx_type::const_iterator block_iterator;

  cached_block_deg():
    //  m_indices(BoolePolyRing::blockRingBegin()), 
    m_current_block(BoolePolyRing::blockRingBegin()),
    m_deg_cache(BoolePolyRing::activeManager()) { }

  typename NavigatorType::size_type 
  operator()(NavigatorType navi) const {
    return dd_cached_block_degree(m_deg_cache, navi, max());
  }

  idx_type min() const {
    assert(*m_current_block != 0); // assuming first element to be zero
    return *(m_current_block - 1);
  }

  idx_type max() const {
    return *m_current_block;
  }
  self& operator++(){
    assert(max() != CTypes::max_idx);
    ++m_current_block;
    return *this;
  }

  self& operator--(){
    assert(min() != 0);
    --m_current_block;
    return *this;
  }

private:
  //  block_iterator m_indices;
  block_iterator m_current_block;

  CBlockDegreeCache<CCacheTypes::block_degree, CTypes::dd_type, 
                    CTypes::manager_base> m_deg_cache;
};




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

private:
  void pop() { m_stack.pop_back(); }

protected:
  void push(navigator __x) { m_stack.push_back(__x); }

  void clear() { m_stack.clear(); }
  reference top() { return m_stack.back(); }

public:
  const_reference top() const { return m_stack.back(); }
  idx_type index() const { return *top(); }
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

  void invalidate() {
    push(BoolePolynomial(false).navigation());
  }

  void restart(navigator navi) {
    assert(empty());
    push(navi);
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




template <class NavigatorType, class Category>
class CTermStack:
  public CTermStackBase<NavigatorType> {

public:
  typedef CTermStackBase<NavigatorType> base;
  typedef typename base::navigator navigator;
  typedef CTermStack<NavigatorType, Category> self;
  typedef self purestack_type;

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
    return checkEnd(base::top());
  };

  bool checkEnd(typename base::navigator navi) const { 
    while(!navi.isConstant()) {
      navi.incrementElse();
    }
    return navi.terminalValue();
  }
  bool checkEnd(const self& rhs) const {
    assert(!rhs.empty());
    return checkEnd(rhs.top());
  }

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


template <class NavigatorType, class BlockProperty, class Category>
class CDegStackBase;

/// @brief for pure degree stacks
template <class NavigatorType, class Category>
class CDegStackBase<NavigatorType, invalid_tag, Category>:
  public CTermStack<NavigatorType, Category> {

public:
  typedef CTermStack<NavigatorType, Category> base;
  typedef NavigatorType navigator;

  CDegStackBase(): base() {}
  CDegStackBase(navigator navi): base(navi), getDeg() {}

  bool atBegin() const { return base::empty(); }
 
  bool atEnd(const typename base::purestack_type& rhs) const {
    return rhs.isConstant();
  }

  bool atEnd() const { return atEnd(*this); }

  void gotoEnd()  {
     assert(!base::empty());
     while(!base::isConstant()) {
       base::incrementElse();
     }
  }

protected:
  cached_deg<navigator> getDeg;
};

/// @brief for block stacks
template <class NavigatorType, class Category>
class CDegStackBase<NavigatorType, valid_tag, Category> :
  public CTermStack<NavigatorType, Category> {

public:
  typedef CTermStack<NavigatorType, Category> base;
  typedef NavigatorType navigator;
  typedef typename base::idx_type idx_type;

  CDegStackBase(): base() {}
  CDegStackBase(navigator navi): base(navi), block() {}

  idx_type getDeg(navigator navi) const { return block(navi); }

  bool atBegin() const { return base::empty() || (*base::top() < block.min()); }

  bool atEnd(const typename base::purestack_type& rhs) const {
    return rhs.isConstant() || (rhs.index() >= block.max());
  }
  bool atEnd() const { 
    return atEnd(*this);
  }


  bool checkEnd(const typename base::purestack_type& rhs) const {
    assert(!rhs.empty());
    return checkEnd(rhs.top());
  }

  bool checkEnd(navigator navi) const {

    while( (!navi.isConstant()) && (*navi < block.max()) ) {
      navi.incrementElse();
    }
    return (navi.isConstant()? navi.terminalValue(): *navi >= block.max());
  }

  void nextTerm() {

    bool invalid = true;
    while (!atBegin() && invalid) {
      assert(!base::isConstant());
      base::incrementElse();
      if (invalid = base::isInvalid())
        base::decrementNode();
    }
  }
  void previousTerm() {

    if( base::handleElse.empty() || (*base::handleElse.top() < block.min()) ) {
      while(!atBegin())
        base::decrementNode();
      return;
    }
    navigator navi =  base::handleElse.top();
    assert(base::top().isValid());

    while(!atBegin() && (*base::top() >= *navi) ) {
      base::decrementNode();
    }

    if (base::empty() || (*base::top() < *navi)) {
      base::handleElse.pop();
      base::push(navi);
    }
      base::incrementThen();
  }

  void gotoEnd()  {
     assert(!base::empty());
     while( (!base::isConstant()) && (*base::top() < block.max()) ) {
       base::incrementElse();
     }
  }

protected:
  cached_block_deg<navigator> block;
};

template <class NavigatorType, class BlockProperty, class DescendingProperty>
class CProxTermStack;

template <class NavigatorType, class BlockProperty>
class CProxTermStack<NavigatorType, valid_tag, BlockProperty>:
  public CDegStackBase<NavigatorType, BlockProperty, 
                       std::forward_iterator_tag> {

public:
  typedef CDegStackBase<NavigatorType, BlockProperty, 
                        std::forward_iterator_tag> base;

  typedef typename base::size_type size_type;
  typedef std::greater<size_type> size_comparer;


  CProxTermStack(): base() {}
  CProxTermStack(NavigatorType navi): base(navi) {}

  integral_constant<bool, false> takeLast;

  void proximateTerm() { base::nextTerm(); }

  void incrementBranch() { base::incrementThen(); }

  bool maxOnThen(size_type deg) const {
    return (base::getDeg(base::top().thenBranch()) + 1 == deg);
  }

};


template <class NavigatorType, class BlockProperty>
class CProxTermStack<NavigatorType, invalid_tag, BlockProperty>:
    public CDegStackBase<NavigatorType, BlockProperty, 
                         std::bidirectional_iterator_tag> {

public:
  typedef CDegStackBase<NavigatorType, BlockProperty, 
                         std::bidirectional_iterator_tag> base;
  typedef typename base::size_type size_type;
  typedef std::greater_equal<size_type> size_comparer;


  CProxTermStack(): base() {}
  CProxTermStack(NavigatorType navi): base(navi) {}

  integral_constant<bool, true> takeLast;

  void proximateTerm() { base::previousTerm(); }

  void incrementBranch() {
    assert(!base::empty() && !base::isConstant());

    // if still in interior of a path
    if(!base::top().elseBranch().isEmpty()) {
      base::incrementElse();   // go in direction of last term, if possible
    }
    else
      base::incrementThen();
  }

  bool maxOnThen(size_type deg) const {
    return !(base::getDeg(base::top().elseBranch())  ==  deg);
  }
};


template <class NavigatorType, class DescendingProperty, 
          class BlockProperty = invalid_tag>
class CDegTermStack:
  public CProxTermStack<NavigatorType, DescendingProperty, BlockProperty> {

public:
  typedef CProxTermStack<NavigatorType, DescendingProperty, BlockProperty> base;
  typedef CDegTermStack<NavigatorType, DescendingProperty, BlockProperty> self;

  typedef typename base::navigator navigator;
  typedef typename navigator::size_type size_type;

  CDegTermStack(): base(), m_start() {}
  CDegTermStack(navigator navi): base(navi), m_start(navi) {}

  void firstTerm() {
    findTerminal();
    base::terminate();    
  }
  void findTerminal() {
    assert(!base::empty());
    
    size_type deg = base::getDeg(base::top());

    while (deg > 0) {

      if ( base::maxOnThen(deg) ) {
        --deg;
        base::incrementThen();
      }
      else
        base::incrementElse();
        
    }
  }

  void incrementTerm() {
    assert(!base::empty());
    if (base::markedOne()) {
      base::clearOne();
      return;
    }


    size_type upperbound = base::size();
    degTerm();

    if(base::empty()) {
      restart();
      findTerm(upperbound);
    }
    if(!base::empty())
      base::terminate();
  }


  void degTerm() {
    size_type size = base::size() + 1;

    assert(!base::isConstant());
    bool doloop;
    do {
      assert(!base::empty());
      base::proximateTerm();

      if (base::atBegin()) 
        return;

        while (!base::atEnd() && (base::size() < size) ) {
          base::incrementBranch();
        }
        base::gotoEnd();

        if (doloop = (base::isInvalid() || (base::size() != size)) )
          base::decrementNode();

    } while (!base::empty() && doloop);

  }

 
  void decrementTerm() {}

  void findTerm(size_type upperbound) {
    assert(!base::empty());

    typename base::purestack_type max_elt, current(base::top());
    base::decrementNode();

    typename base::size_comparer comp;

    while (!current.empty() && 
           (base::takeLast() || (max_elt.size() != upperbound)) ) {
      
      while (!base::atEnd(current) && (current.size() < upperbound) )
        current.incrementThen();
      
      if (base::checkEnd(current)) {
        if (comp(current.size(), max_elt.size()))
          max_elt = current;
        current.decrementNode();
      }
      current.nextTerm();
    }
    base::append(max_elt);

    if(max_elt.empty())
      base::invalidate();
  }

  void restart() { base::restart(m_start); }

private:
  navigator m_start;
};



//////////////////////////////////////////////////////////
template <class NavigatorType, class DescendingProperty>
class CBlockTermStack:
  public CDegTermStack<NavigatorType, DescendingProperty, valid_tag> {

public:
  typedef CDegTermStack<NavigatorType, DescendingProperty, valid_tag> base; 
  typedef CBlockTermStack<NavigatorType, DescendingProperty> self;

  typedef typename base::navigator navigator;
  typedef typename navigator::size_type size_type;
  typedef typename navigator::idx_type idx_type;

  /// Construct stack from navigator
  CBlockTermStack(navigator navi): base(navi) { }

  /// Default Constructor
  CBlockTermStack(): base() {}


  void firstTerm() {
    assert(!base::empty());
    findTerminal();
    base::terminate();
  }



  void blockTerminal() { base::findTerminal(); }

  void incrementTerm() {
    assert(!base::empty());

    if (base::markedOne()) {
      base::clearOne();
      return;
    }

    navigator current = base::top(); 
    while (*current < base::block.min())
      --base::block;

    incrementBlock();
    while ( (base::size() > 1 ) && base::isInvalid()  ) {
      --base::block;
      base::decrementNode();
      incrementBlock();
    }

    findTerminal();

    assert(!base::empty());
    base::terminate();
  }

  void findTerminal() {
    assert(base::top().isValid());

    if (!base::isConstant() ) 
      blockTerminal();

    while (!base::isConstant()  ) {
      ++base::block;
      blockTerminal();
    }
  }

  void incrementBlock();

  using base::print;

};

template <class NavigatorType, class DescendingProperty>
inline void
CBlockTermStack<NavigatorType, DescendingProperty>::incrementBlock() {

  assert(!base::empty());
  size_type size = base::size() + 1;

  if (*base::top() < base::block.min()) {
    base::invalidate();
    return;
  }
   
  base::degTerm();

  if (base::size() == size) return;

  if (base::empty())
    base::restart();
  else {
    assert(*base::top() < base::block.min());
    base::incrementThen();
  }

  while (!base::isConstant() && (*base::top() <  base::block.min()))
    base::incrementElse();

  assert(size > base::size()); 
  
  base::findTerm(size - base::size());
  base::gotoEnd();
}

END_NAMESPACE_PBORI

#endif
