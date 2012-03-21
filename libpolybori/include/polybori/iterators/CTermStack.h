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
 *   (c) 2007-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_iterators_CTermStack_h_
#define polybori_iterators_CTermStack_h_

// get standard header
#include <stack>
#include <iterator>
#include <utility> // for std::pair

// include basic definitions
#include <polybori/pbori_defs.h>

// include polybori functionals
#include <polybori/routines/pbori_func.h>

// include polybori properties
#include <polybori/common/traits.h>

#include <polybori/routines/pbori_routines.h>
 
// include boost's indirect iterator facilities
#include <boost/iterator/indirect_iterator.hpp>

#include <polybori/BoolePolyRing.h>
#include <polybori/BooleEnv.h>
#include <polybori/cache/CDegreeCache.h>
#include "CBidirectTermIter.h"
  
#include <polybori/BooleSet.h>

BEGIN_NAMESPACE_PBORI

//////////////////////////////////////////////////////////
template<class NavigatorType>
struct cached_deg {
  typedef CDegreeCache<BooleSet> cache_type;
  typedef typename cache_type::manager_type manager_type;
  cached_deg(const manager_type & mgr): m_deg_cache(mgr) {}

  typename NavigatorType::deg_type
  operator()(NavigatorType navi) const {
    return dd_cached_degree(m_deg_cache, navi);
  }
  cache_type m_deg_cache;
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
  typedef CBlockDegreeCache<BooleEnv::dd_type>
  cache_type;
  typedef typename cache_type::manager_type manager_type;

  cached_block_deg(const manager_type& mgr):
    m_current_block(block_begin(mgr)),
    m_deg_cache(mgr) { }

  typename NavigatorType::size_type
  operator()(NavigatorType navi) const {
    return dd_cached_block_degree(m_deg_cache, navi, max());
  }

  idx_type min() const {
    PBORI_ASSERT(*m_current_block != 0); // assuming first element to be zero
    return *(m_current_block - 1);
  }

  idx_type max() const {
    return *m_current_block;
  }
  self& operator++(){
    PBORI_ASSERT(max() != CTypes::max_idx);
    ++m_current_block;
    return *this;
  }

  self& operator--(){
    PBORI_ASSERT(min() != 0);
    --m_current_block;
    return *this;
  }

private:
  //  block_iterator m_indices;
  block_iterator m_current_block;

  cache_type m_deg_cache;
};




/** @class CTermStackBase
 * @brief This class defines an iterator for the monomials in a Boolean
 * polynomial.
 *
 **/


template <class NavigatorType, class BaseType = internal_tag>
class CTermStackBase:
  public BaseType {

public:

  template <class, class> friend class CTermStackBase;

  typedef CTermStackBase<NavigatorType, BaseType> self;

  /// Get type of navigators
  typedef NavigatorType navigator;
  /// Type for indices
  typedef typename navigator::idx_type idx_type;

  /// Type for lengths
  typedef typename navigator::size_type size_type;
  typedef typename navigator::deg_type deg_type;
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

  typedef typename stack_type::const_iterator stack_iterator;

  typedef typename stack_type::const_reverse_iterator stack_reverse_iterator;

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


public:
  bool_type empty() const { return m_stack.empty(); }
  const_reference top() const { 
    PBORI_ASSERT(!empty());
    return m_stack.back();
  }
  idx_type index() const { return *top(); }
  size_type size() const { return m_stack.size(); }

  const_iterator begin() const { return stackBegin(); }
  const_iterator end() const { return stackEnd(); }

  const_reverse_iterator rbegin() const { return stackRBegin(); }

  const_reverse_iterator rend() const { return stackREnd(); }

  /// Get navigator of stack start
  navigator navigation() const {
    PBORI_ASSERT(m_stack.begin() != m_stack.end());
    return *m_stack.begin();
  }

  /// result type of top()
  typedef typename stack_type::value_type top_type;

  /// Default constructor
  CTermStackBase(): BaseType(), m_stack() { }

  /// Construct from initial navigator
  CTermStackBase(navigator navi): BaseType(), m_stack() {
    if (navi.isValid())
      push(navi);
  }

  /// Copy constructor
  CTermStackBase(const CTermStackBase& rhs):
    BaseType(rhs), m_stack(rhs.m_stack) { }

  /// Equality test (assume iterators from same instance)
  bool_type equal(const self& rhs) const {

    if(empty() || rhs.empty())
      return (empty() && rhs.empty());
    else
      return (m_stack == rhs.m_stack);
  }

  void incrementThen() {
    PBORI_ASSERT(!top().isConstant());

    push(top());
    m_stack.back().incrementThen();
  }
  void incrementElse() {
    PBORI_ASSERT(!isConstant());
    m_stack.back().incrementElse();
  }

  void decrementNode() {
    PBORI_ASSERT(!empty());
    pop();
  }

  bool_type isConstant() const {
    PBORI_ASSERT(!empty());
    return top().isConstant();
  }

  bool_type isTerminated() const {
    PBORI_ASSERT(!empty());
    return top().isTerminated();
  }

  bool_type isInvalid() const {
    PBORI_ASSERT(!empty());
    return top().isEmpty();
  }

  void followThen() {
    PBORI_ASSERT(!empty());
    while(!isConstant())
      incrementThen();
  }

  void markOne() {
    PBORI_ASSERT(empty());
    push(navigator());
  }

  bool_type markedOne() const {
    if PBORI_UNLIKELY(empty())
      return false;
    else
      return !m_stack.front().isValid();
  }

  void clearOne() {
    pop();
    PBORI_ASSERT(empty());
  } 

  deg_type deg() const {
    return (markedOne()? 0: (deg_type)size());
  }

  void restart(navigator navi) {
    PBORI_ASSERT(empty());
    push(navi);
  }

  bool isOne() const { return markedOne(); }
  bool isZero() const { return empty(); }

  bool atBegin() const { return empty(); }

  bool atEnd() const { return atEnd(top()); }
  bool atEnd(navigator navi) const { return navi.isConstant(); }

  bool validEnd() const {  return validEnd(top()); }
  bool validEnd(navigator navi) const { 
    while(!navi.isConstant()) {
      navi.incrementElse();
    }
    return navi.terminalValue();
  }

  void print() const{
    std::cout <<"(";
    std::copy(begin(), end(), std::ostream_iterator<int>(std::cout, ", ")); 
    std::cout <<")";
  }

  stack_iterator stackBegin() const { 
    if (markedOne())
      return m_stack.end();
    else
      return m_stack.begin(); 
  }

  stack_iterator stackEnd() const {
    return m_stack.end();
  }

  stack_reverse_iterator stackRBegin() const { 
    if (markedOne())
      return m_stack.rend();
    else
      return m_stack.rbegin(); 
  }

  stack_reverse_iterator stackREnd() const {
    return m_stack.rend();
  }
protected:

  template <class TermStack>
  void append(const TermStack& rhs) { 
    PBORI_ASSERT(empty() || rhs.empty() || ((*rhs.begin()) > (*top())) );
    m_stack.insert(m_stack.end(), rhs.m_stack.begin(), rhs.m_stack.end());
  }


private:
  stack_type m_stack;

  navigator m_zero;
};



/** @class CTermStack
 * @brief This class defines an iterator for the monomials in a Boolean
 * polynomial.
 *
 **/
template <class NavigatorType, class Category, class BaseType = internal_tag>
class CTermStack:
  public CTermStackBase<NavigatorType, BaseType> {

public:
  typedef CTermStackBase<NavigatorType, BaseType> base;
  typedef CTermStack<NavigatorType, Category, BaseType> self;

  /// Defining a type for simple stacking of term elements
  typedef CTermStack<NavigatorType, Category, internal_tag> purestack_type;
  typedef Category iterator_category;

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

  /// Copy constructor
  CTermStack(const CTermStack& rhs):
    base(rhs), handleElse(rhs.handleElse) { }

  /// Construct from initial navigator, third argument is just for having the
  /// same interface with block and degree-stacks
  template <class Dummy>
  CTermStack(navigator navi, const Dummy&): base(navi) { }

  void init() {
    if (!base::empty()){
      followThen();
      terminate();
    }
  }

  void initLast() {
    if (!base::empty()){
      followElse();
      terminate();
    }
  }

  void incrementElse() {
    handleElse(base::top());
    base::incrementElse();
  }

  void next() {

    bool invalid = true;
    while (!base::empty() && invalid) {
      incrementElse();
      if ((invalid = base::isInvalid()))
        base::decrementNode();
    }
  }

  void previous() {
    previous(Category());
  }


  void increment() {
    PBORI_ASSERT(!base::empty());
    if PBORI_UNLIKELY(base::markedOne()) {
      base::clearOne();
      return;
    }
      
    next();
    if PBORI_UNLIKELY(!base::empty()) {
      followThen();
      terminate();
    }

  }

   void decrement() {

    if PBORI_UNLIKELY(base::markedOne()) {
      base::clearOne();
    }
    previous();
    if PBORI_UNLIKELY(!base::empty()){
      followElse();
      base::decrementNode();
    }

  }

  void terminate() {
    PBORI_ASSERT(!base::empty());

    bool isZero = base::isInvalid();
    base::decrementNode();
    if PBORI_UNLIKELY(base::empty() && !isZero)
      base::markOne();
  }


  void followElse() {
    while( !base::isConstant() ) // if still in interior of a path
      incrementValidElse();
  }
  
  void incrementValidElse() {
    PBORI_ASSERT(!base::empty() && !base::isConstant());
    if(!base::top().elseBranch().isEmpty())
      incrementElse();          // go in direction of last term, if possible
    else
      incrementThen();
  } 

protected:
  template <class TermStack>
  void append(const TermStack& rhs) {
    base::append(rhs);
    append(rhs, Category());
  }

private:
  void previous(std::forward_iterator_tag);
  void previous(std::bidirectional_iterator_tag);

  template <class TermStack>
  void append(const TermStack&, std::forward_iterator_tag){}

  template <class TermStack>
  void append(const TermStack& rhs, std::bidirectional_iterator_tag){
     handleElse.append(rhs.handleElse); 
  }
};


template <class NavigatorType, class Category, class BaseType>
inline void CTermStack<NavigatorType, Category, BaseType>::previous(
  std::forward_iterator_tag) { }

template <class NavigatorType, class Category, class BaseType>
inline void CTermStack<NavigatorType, Category, BaseType>::previous(
  std::bidirectional_iterator_tag) { 

  if(handleElse.empty()) {
    base::clear();
    return;
  }

  navigator navi = handleElse.top();
  PBORI_ASSERT(base::empty() || base::top().isValid());

  while(!base::empty() && (base::index() >= *navi) ) {
    base::decrementNode();
  }

  handleElse.pop();
  base::push(navi);
  incrementThen();
}

/** @class CReverseTermStack
 * @brief This class defines an iterator for the monomials in a Boolean
 * polynomial in reversed lexicographicxal order.
 *
 **/
template <class NavigatorType, class Category>
class CReverseTermStack:
  public CTermStack<NavigatorType, Category> {
public: 
  typedef NavigatorType navigator;
  typedef CTermStack<NavigatorType, Category> base;

  /// Default constructor
  CReverseTermStack(): base() { }

  /// Construct from initial navigator
  CReverseTermStack(navigator navi): base(navi) {  }

  /// Copy constructor
  CReverseTermStack(const CReverseTermStack& rhs):  base(rhs) { }

  /// Construct from initial navigator, second argument is just for having the
  /// same interface with block and degree-stacks
  template <class Dummy>
  CReverseTermStack(navigator navi, const Dummy&): base(navi) { }

  void init() { base::initLast(); }
  void initLast() { base::init(); }
  void increment() { base::decrement(); }
  void decrement() { base::increment(); }
};

template <class NavigatorType, class BlockProperty, class Category, class
          BaseType = internal_tag>
class CDegStackCore;

/// @brief for pure degree stacks
template <class NavigatorType, class Category, class BaseType>
class CDegStackCore<NavigatorType, invalid_tag, Category, BaseType>:
  public CTermStack<NavigatorType, Category, BaseType> {

public:
  typedef CTermStack<NavigatorType, Category, BaseType> base;
  typedef NavigatorType navigator;
  typedef typename cached_deg<navigator>::manager_type manager_type;

  //  CDegStackCore(): base(), getDeg(manager_type()) {}

  CDegStackCore(navigator navi, const manager_type& mgr):
    base(navi), getDeg(mgr) {}

  CDegStackCore(const CDegStackCore& rhs):
    base(rhs), getDeg(rhs.getDeg) {}

  void gotoEnd()  {
     PBORI_ASSERT(!base::empty());
     while(!base::isConstant()) {
       base::incrementElse();
     }
  }

  cached_deg<navigator> getDeg;
};

/// @brief for block stacks
template <class NavigatorType, class Category, class BaseType>
class CDegStackCore<NavigatorType, valid_tag, Category, BaseType> :
  public CTermStack<NavigatorType, Category, BaseType> {

public:
  typedef CTermStack<NavigatorType, Category, BaseType> base;
  typedef NavigatorType navigator;
  typedef typename base::idx_type idx_type;
  typedef typename base::deg_type deg_type;
  typedef typename base::size_type size_type;
  typedef typename cached_block_deg<navigator>::manager_type manager_type;

  //  CDegStackCore(): base(), block(manager_type()) {}
  CDegStackCore(navigator navi, const manager_type& mgr): 
    base(navi), block(mgr) {}

  CDegStackCore(const CDegStackCore& rhs):
    base(rhs), block(rhs.block) {}

  deg_type getDeg(navigator navi) const { return block(navi); }

  bool atBegin() const { 
    return base::empty() || (base::index() < block.min()); 
  }

  bool atEnd() const { return atEnd(base::top()); }
  bool atEnd(navigator navi) const {
    return navi.isConstant() || (*navi >= block.max());
  }

  bool validEnd() const{ return validEnd(base::top()); }
  bool validEnd(navigator navi) const {

    while(!atEnd(navi))
      navi.incrementElse();

    return (navi.isConstant()? navi.terminalValue(): *navi >= block.max());
  }

  void next() {

    bool invalid = true;
    while (!atBegin() && invalid) {
      PBORI_ASSERT(!base::isConstant());
      base::incrementElse();
      if ((invalid = base::isInvalid()))
        base::decrementNode();
    }
  }
  void previous() {

    if( base::handleElse.empty() || (*base::handleElse.top() < block.min()) ) {
      while(!atBegin())
        base::decrementNode();
      return;
    }
    navigator navi =  base::handleElse.top();
    PBORI_ASSERT(base::top().isValid());

    while(!atBegin() && (base::index() >= *navi) ) {
      base::decrementNode();
    }

    if (base::empty() || (base::index() < *navi)) {
      base::handleElse.pop();
      base::push(navi);
    }
      base::incrementThen();
  }

  void gotoEnd()  {
     PBORI_ASSERT(!base::empty());
     while( (!base::isConstant()) && (base::index() < block.max()) ) {
       base::incrementElse();
     }
  }

protected:
   cached_block_deg<navigator> block;
};

template <class NavigatorType, class BlockProperty, class DescendingProperty,
          class BaseType = internal_tag>
class CDegStackBase;

template <class NavigatorType, class BlockProperty, class BaseType>
class CDegStackBase<NavigatorType, valid_tag, BlockProperty, BaseType>:
  public CDegStackCore<NavigatorType, BlockProperty, 
                       std::forward_iterator_tag, BaseType> {

public:
  typedef CDegStackCore<NavigatorType, BlockProperty, 
                        std::forward_iterator_tag, BaseType> base;

  typedef typename base::size_type size_type;
  typedef typename base::deg_type deg_type;
  typedef std::greater<size_type> size_comparer;
  typedef typename base::manager_type manager_type;

  //  CDegStackBase(): base() {}
  CDegStackBase(NavigatorType navi, const manager_type& mgr): base(navi, mgr) {}

  CDegStackBase(const CDegStackBase& rhs):  base(rhs) {}

  integral_constant<bool, false> takeLast;

  void proximate() { base::next(); }

  void incrementBranch() { base::incrementThen(); }

  bool maxOnThen(deg_type deg) const {
    return (base::getDeg(base::top().thenBranch()) + 1 == deg);
  }

};


template <class NavigatorType, class BlockProperty, class BaseType>
class CDegStackBase<NavigatorType, invalid_tag, BlockProperty, BaseType>:
    public CDegStackCore<NavigatorType, BlockProperty, 
                         std::bidirectional_iterator_tag, BaseType> {

public:
  typedef CDegStackCore<NavigatorType, BlockProperty, 
                         std::bidirectional_iterator_tag, BaseType> base;
  typedef typename base::size_type size_type;
  typedef typename base::deg_type deg_type;
  typedef std::greater_equal<size_type> size_comparer;
  typedef typename base::manager_type manager_type;

  //  CDegStackBase(): base() {}
  CDegStackBase(NavigatorType navi, const manager_type& mgr): base(navi, mgr) {}

  CDegStackBase(const CDegStackBase& rhs):  base(rhs) {}

  integral_constant<bool, true> takeLast;

  void proximate() { base::previous(); }

  void incrementBranch() { base::incrementValidElse(); }

  bool maxOnThen(deg_type deg) const {
    return !(base::getDeg(base::top().elseBranch())  ==  deg);
  }
};


template <class NavigatorType, class DescendingProperty, 
          class BlockProperty = invalid_tag, class BaseType = internal_tag>
class CDegTermStack:
  public CDegStackBase<NavigatorType, DescendingProperty, BlockProperty, BaseType> {

public:
  typedef CDegStackBase<NavigatorType, DescendingProperty, BlockProperty, BaseType> base;
  typedef CDegTermStack<NavigatorType, DescendingProperty, BlockProperty, BaseType> self;

  typedef typename base::navigator navigator;
  typedef typename navigator::size_type size_type;
  typedef typename navigator::deg_type deg_type;
  typedef typename base::manager_type manager_type;

  //  CDegTermStack(): base(), m_start() {}
  CDegTermStack(navigator navi, const manager_type& mgr):
    base(navi, mgr), m_start(navi), m_zero(mgr.zero().navigation()) {}

  CDegTermStack(const CDegTermStack& rhs):
    base(rhs), m_start(rhs.m_start), m_zero(rhs.m_zero) {}

  void init() {  
    if (!base::empty()) {
      followDeg();
      base::terminate();    
    }
  }
  void followDeg() {
    PBORI_ASSERT(!base::empty());
    
    deg_type deg = base::getDeg(base::top());

    while (deg > 0) {

      if ( base::maxOnThen(deg) ) {
        --deg;
        base::incrementThen();
      }
      else
        base::incrementElse();
        
    }
  }

  void increment() {
    PBORI_ASSERT(!base::empty());
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

    PBORI_ASSERT(!base::isConstant());
    bool doloop;
    do {
      PBORI_ASSERT(!base::empty());
      base::proximate();

      if (base::atBegin()) 
        return;

        while (!base::atEnd() && (base::size() < size) ) {
          base::incrementBranch();
        }
        base::gotoEnd();

        if ((doloop = (base::isInvalid() || (base::size() != size)) ) )
          base::decrementNode();

    } while (!base::empty() && doloop);

  }

 
  void decrement() {}

  void findTerm(size_type upperbound) {
    PBORI_ASSERT(!base::empty());

    typename base::purestack_type max_elt, current(base::top());
    base::decrementNode();

    typename base::size_comparer comp;

    while (!current.empty() && 
           (base::takeLast() || (max_elt.size() != upperbound)) ) {
      
      while (!base::atEnd(current.top()) && (current.size() < upperbound) )
        current.incrementThen();
      
      if (base::validEnd(current.top())) {
        if (comp(current.size(), max_elt.size()))
          max_elt = current;
        current.decrementNode();
      }
      current.next();
    }
    base::append(max_elt);

    if(max_elt.empty())
      invalidate();
  }

  void restart() { base::restart(m_start); }

  void invalidate() {
    PBORI_ASSERT(m_zero.isValid());
    PBORI_ASSERT(m_zero.isEmpty());

    base::push(m_zero);
  }

private:
  navigator m_start, m_zero;
};



//////////////////////////////////////////////////////////
template <class NavigatorType, class DescendingProperty, class BaseType = internal_tag>
class CBlockTermStack:
  public CDegTermStack<NavigatorType, DescendingProperty, valid_tag, BaseType> {

public:
  typedef CDegTermStack<NavigatorType, DescendingProperty, valid_tag, BaseType> base; 
  typedef CBlockTermStack<NavigatorType, DescendingProperty, BaseType> self;

  typedef typename base::navigator navigator;
  typedef typename navigator::size_type size_type;
  typedef typename navigator::idx_type idx_type;
  typedef typename base::manager_type manager_type;

  /// Construct stack from navigator
  CBlockTermStack(navigator navi, const manager_type& mgr):
    base(navi, mgr) { }

  /// Copy constructor
  CBlockTermStack(const CBlockTermStack& rhs):  base(rhs) { }

  void init() {
    if (!base::empty()) {
      followDeg();
      base::terminate();
    }
  }

  void increment() {
    PBORI_ASSERT(!base::empty());

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

    followDeg();

    PBORI_ASSERT(!base::empty());
    base::terminate();
  }

  void followBlockDeg() { base::followDeg(); }

  void followDeg() {
    PBORI_ASSERT(base::top().isValid());

    if (!base::isConstant() ) 
      followBlockDeg();

    while (!base::isConstant()  ) {
      ++base::block;
      followBlockDeg();
    }
  }

  void incrementBlock() {

    PBORI_ASSERT(!base::empty());
    size_type size = base::size() + 1;
    
    if (base::index() < base::block.min()) {
      base::invalidate();
      return;
    }
    
    base::degTerm();
    
    if (base::size() == size) return;
    
    if (base::empty())
      base::restart();
    else {
      PBORI_ASSERT(base::index() < base::block.min());
      base::incrementThen();
    }
    
    while (!base::isConstant() && (base::index() <  base::block.min()))
      base::incrementElse();
    
    PBORI_ASSERT(size > base::size()); 
    
    base::findTerm(size - base::size());
    base::gotoEnd();
  }
};

END_NAMESPACE_PBORI

#endif
