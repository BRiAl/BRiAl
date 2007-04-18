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



template<class NaviType, class DescendingProperty = valid_tag>
class bounded_restricted_stack {
public:
  typedef NaviType navigator;
  typedef DescendingProperty descending_property;
  typedef bounded_restricted_stack<navigator, descending_property> self;
  typedef std::vector<navigator> stack_type;
  typedef unsigned size_type;
  typedef unsigned idx_type;

  bounded_restricted_stack (): 
    m_stack(), m_max_idx(0), m_upperbound(0), m_next() {}

  is_same_type<descending_property, valid_tag> descendingVariables;

  bounded_restricted_stack (navigator navi, size_type upperbound, 
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


/** @class CTermStack
 * @brief This class defines an iterator for the monomials in a Boolean
 * polynomial.
 *
 **/


template <class NavigatorType, 
          class BaseType = internal_tag>
class CTermStackBase :
  public BaseType {

public:
  typedef CTermStackBase<NavigatorType, BaseType> self;

  /// Get type of navigators
  typedef NavigatorType navigator_type;
  typedef navigator_type navigator;
  /// Type for indices
  typedef typename navigator::idx_type idx_type;

  /// Type for lengths
  typedef typename navigator::size_type size_type;
  typedef typename navigator::bool_type bool_type;


  /// Define type for stacking
  typedef std::deque<navigator_type> stack_type;

  typedef typename stack_type::reference       reference_type;
  typedef typename stack_type::const_reference const_reference_type;

  typedef boost::indirect_iterator<typename stack_type::const_iterator,
                                   typename navigator::value_type, 
                                   boost::use_default, 
                                   typename navigator::reference>
  const_iterator;

  typedef boost::indirect_iterator<typename 
                                   stack_type::const_reverse_iterator,
                                   typename navigator::value_type, 
                                   boost::use_default, 
                                   typename navigator::reference>
  const_reverse_iterator;


  bool empty() const { return m_stack.empty(); }
  void clear() { m_stack.clear(); }

  size_type size() const { return m_stack.size(); }
  reference_type top() { return m_stack.back(); }
  const_reference_type top() const { return m_stack.back(); }
  void push(const navigator_type& __x) { m_stack.push_back(__x); }
  void pop() { m_stack.pop_back(); }

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
  CTermStackBase(): BaseType(), m_stack() { }

  /// Construct from initial navigator
  CTermStackBase(navigator navi): BaseType(), m_stack() {
    push(navi);
  }

  /// default Copy Constructor


  /// Equality test (assume iterators from same instance)
  bool_type equal(const CTermStackBase& rhs) const {
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
    std::copy(begin(), end(), std::ostream_iterator<int>(cout, ", "));  std::cout <<")";
  }
protected:

  void append(const self& rhs) { 
    assert(empty() || rhs.empty() || ((*rhs.begin()) > (*top())) );
    m_stack.insert(m_stack.end(), rhs.m_stack.begin(), rhs.m_stack.end());
  }


private: // Change? 
protected:
  /// @todo: change to private, temporarily, e.g. for block iteration
public:
  stack_type m_stack;
};




template <class NavigatorType, class Category = std::forward_iterator_tag,
          class BaseType = CTermStackBase<NavigatorType> >
class CTermStack :
  public BaseType  {

public:
  typedef BaseType base;
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


template <class NavigatorType, class Category, class BaseType>
inline void CTermStack<NavigatorType, Category, BaseType>::previousTerm(
  std::forward_iterator_tag) { }

template <class NavigatorType, class Category, class BaseType >
inline void CTermStack<NavigatorType, Category, BaseType>::previousTerm(
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



template <class NavigatorType, class DescendingProperty = valid_tag,
          class BaseType = 
          CTermStack<NavigatorType, 
                     typename
                     on_same_type<DescendingProperty, valid_tag,
                                  std::forward_iterator_tag, 
                                  std::bidirectional_iterator_tag>::type
          >,
          class TMPType = internal_tag
          >
class CDegTermStack :
  public BaseType {

public:
  typedef BaseType base;
  typedef NavigatorType navigator;
  typedef typename navigator::size_type size_type;
  typedef DescendingProperty descending_property;
  typedef CDegTermStack<navigator, descending_property, base> self;
  typedef BoolePolynomial poly_type;
  typedef typename poly_type::dd_type dd_type;
  typedef typename  on_same_type<TMPType, internal_tag, self, TMPType>::type
  tmp_type;


  CDegTermStack(): base(), m_start() {}
  CDegTermStack(navigator navi): base(navi), m_start(navi) {}


  bool nextOnThen(size_type deg, valid_tag) const {
    return (cachedDeg(base::top().thenBranch()) + 1 == deg);
  }

  bool nextOnThen(size_type deg, invalid_tag) const {
    return !(cachedDeg(base::top().elseBranch())  ==  deg);
  }
  size_type cachedDeg(const navigator& navi) const {
    return dd_cached_degree(CDegreeCache<>(BoolePolyRing::activeManager()), 
                            navi);
  }
  void firstTerm() {

    typename on_same_type<descending_property, valid_tag, 
      std::greater<size_type>,
      std::greater_equal<size_type> >::type comp;
    
    assert(!base::empty());
    
    size_type deg = cachedDeg(base::top());

    while (deg > 0) {

      if ( nextOnThen(deg, descending_property()) ) {
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
    degTerm(descending_property());

    if(base::empty())
      findTerm(upperbound);

    if(!base::empty())
      base::terminate();
  }


  void degTerm(valid_tag) {
    size_type size = base::size() + 1;

    assert(!base::isConstant());

    base::incrementThen();
  
    do {
      base::decrementNode();
      base::nextTerm();
      if( base::empty() )
        return;
  
      while(!base::isConstant() && (base::size() < size) )
        base::incrementThen();
  
    } while  (!base::empty() && 
              !(base::isTermEnd() && (base::size() == size) )  );

  }

 
  void decrementTerm() {}


  void degTerm(invalid_tag) {
    size_type size = base::size() + 1;

    assert(!base::isConstant());
    base::incrementThen();

    do {
      base::decrementNode();
      base::previousTerm();
      if( base::empty() )
        return;

      while( !base::isConstant() && (base::size() < size) ) {
        // if still in interior of a path
        if(!base::top().elseBranch().isEmpty()) {
          base::incrementElse();   // go in direction of last term, if possible
        }
        else
          base::incrementThen();
      } 


    } while  (!base::empty() && 
              !(base::isTermEnd() && (base::size() == size) )  );

  }

  void findTerm(size_type upperbound){

    typename on_same_type<descending_property, valid_tag, 
      std::greater<size_type>,
      std::greater_equal<size_type> >::type comp;

    is_same_type<descending_property, invalid_tag> takeLast;

    base::push(m_start);
    tmp_type tmp;
    
    while (!base::empty() && (takeLast() || (tmp.size() != upperbound)) ) {
      
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


template <class StackType, class NaviType, class IdxType>
class deg_next_term {
public:

  deg_next_term(StackType& thestack, IdxType mini, IdxType maxi,
                NaviType navi):
    m_stack(thestack), min_idx(mini), max_idx(maxi)/*, m_deg_cache(deg_cache)*/ 
    , m_navi(navi)
{

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

  void print() const {
      std::cout << ":";
      std::cout.flush();
    StackType thestack( m_stack);
    while (!thestack.empty()){
      if (thestack.top().isValid())
        std::cout << *(thestack.top()) << ", ";
      else 
        std::cout << "one";
      std::cout.flush();
      thestack.pop();
    }
  }


  NaviType operator()() {

    assert(!m_stack.empty());
    unsigned deg = m_stack.size();
    NaviType current;

    if (*m_stack.top() < min_idx)
      return BoolePolynomial(false).navigation();

    
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

     if (m_stack.empty() &&(deg == 0) )
      return current;

     if (m_stack.empty() )
         current = m_navi;
     else {
       current = m_stack.top();
       assert(!current.isConstant());
       current.incrementThen();
     }


     while (!current.isConstant() && (*current < min_idx))
       current.incrementElse();


     if (*current < min_idx)
       return BoolePolynomial(false).navigation();


     assert(deg > m_stack.size()); 
     bounded_restricted_stack<NaviType,  valid_tag>
      bstart(current,  deg - m_stack.size() - 1, max_idx),
      bend;
     bstart = std::max_element(bstart, bend);

    dummy_append(m_stack, bstart.begin(), bstart.end());

    //  std::cout << "bstart.next();"<<*bstart.next()<<std::endl;
    return bstart.next();
  }


protected:
  StackType& m_stack;
  NaviType(m_navi);
  IdxType min_idx, max_idx;
};


//////////////////////////////////////////////////////////
template <class NavigatorType, class DescendingProperty = valid_tag>
class CBlockTermStack :
  public           CTermStack<NavigatorType, 
                     typename
                     on_same_type<DescendingProperty, valid_tag,
                                  std::forward_iterator_tag, 
                                  std::bidirectional_iterator_tag>::type
          > {
public:

  typedef           CTermStack<NavigatorType, 
                     typename
                     on_same_type<DescendingProperty, valid_tag,
                                  std::forward_iterator_tag, 
                                  std::bidirectional_iterator_tag>::type
          >  base;
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

  /// Incrementation operation

              //  typedef DelayedIterator base;
              //  typedef CBlockIterator<base> self;
              //  typedef typename base::stack_type stack_type;

  typedef typename PolyType::idx_type idx_type;
   /// Type for block indices
  typedef std::vector<idx_type> block_idx_type;

  typedef typename base::stack_type stack_type;

  /// Type for block iterators
  typedef typename block_idx_type::const_iterator block_iterator;


//   CBlockIterator(navigator navi, unsigned* indices, 
//                  const CBlockDegreeCache<>& deg_cache):
//     base(),  m_indices(indices), m_deg_cache(deg_cache), 
//     m_current_block(indices) {
//     findTerminal(navi);   
//   }

  size_type currentBlockDegree(const navigator& navi) const {
    return dd_cached_block_degree(m_deg_cache, navi, *m_current_block);
  }
  
  bool nextOnThen(size_type deg, valid_tag) const {
    return (currentBlockDegree(base::top().thenBranch()) + 1 == deg);
  }

  bool nextOnThen(size_type deg, invalid_tag) const {
    return !(currentBlockDegree(base::top().elseBranch())  ==  deg);
  }

  void incrementBlock() {
    size_type deg = currentBlockDegree(base::top());

    while(deg > 0) {
  
      if ( nextOnThen(deg, descending_property()) ){
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
    
//       deg_next_term<base, navigator, unsigned>
//         nextop(*this,  blockMin(), blockMax(), m_navi);
  
//       current = nextop();

      degTerm();
      assert(!base::empty()); 
      current = base::top();
      base::pop();

    } while (!base::empty() && current.isEmpty());
    //   std::cout << "empty "<<base::empty() << " "<<current.isEmpty()<<std::endl;


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

      if( base::handleElse.empty() ||  (*base::handleElse.top() <  blockMin()) ) {
      while(!base::empty() && (*base::top() >= blockMin()) )
        base::decrementNode();
      return;
    }
      //    std::cout << "prev1 "<< (base::handleElse.empty()? -1 :*base::handleElse.top() )<<" " << 
      // (base::empty()? -1 :*base::top() ) <<std::endl;
      //   print();


    navigator navi =  base::handleElse.top();
    
    //    std::cout << "elsetop "<<*navi<<std::endl;


    assert(base::top().isValid());

    //    print();
    while(!base::empty() && (*base::top() >= *navi) 
          && (*base::top() >= blockMin()) ) {
      //    print();
      base::decrementNode();
    }
    //    print();   std::cout << "prev2 "<<blockMin()<<std::endl;
    if (base::empty() || (*base::top() < *navi)) {
      base::handleElse.pop();
      base::push(navi);
    }
      base::incrementThen();
      //    } 
      //    print();
      //       std::cout << "prevend"<<std::endl;
  }

  void degTerm();


  void forwardTerm() {
    forwardTerm(descending_property());
  }
  void forwardTerm(valid_tag) {
    nextTerm();
  }
  void forwardTerm(invalid_tag) {
    previousTerm();
  }

  void forwardBranch() {
    forwardBranch(descending_property());
  }

  void forwardBranch(valid_tag) {
    base::incrementThen();
  }
  void forwardBranch(invalid_tag) {
    if(!base::top().elseBranch().isEmpty()) {
      base::incrementElse();  
    }
    else
      base::incrementThen();
  }

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
      //    std::cout << "do "<<*base::top()<<std::endl;
      //      print();
      forwardTerm();
      //               std::cout << "loop"<<std::endl;
               //    print();
      if ( !base::empty() && (notfound = (*base::top() >= min_idx)) ) {
        while (!base::isConstant() && (base::size() <= deg) && 
               (*base::top() < max_idx)) {

          forwardBranch();   
          //print(); std::cout << "fwd "<<*base::top()<<base::size() << " "<<deg<<std::endl;
        }
        //   print();      std::cout << "gotoBlockEnd"<<std::endl;
        gotoBlockEnd();
        current = base::top();
        base::decrementNode();
      }
    } while ( !base::empty() && notfound &&
              !(!current.isEmpty() && (base::size() == deg) ) );
    //       std::cout <<std::endl;
    //      print();
    //      std::cout <<std::endl<< "samedeg"<<std::endl<<std::endl;
//     std::cout <<std::endl<< "??size??????? "<<*current << (base::size() == deg)<<std::endl;
//     print();
       if (base::size() == deg) {
      base::push(current);
      return;
    }

    if (base::empty())
      base::push(m_navi);
//     else if (current.isEmpty()) {
//       //base::push(current);
//       //       base::incrementThen();
//     } 
    else {
      //base::push(current);
      base::incrementThen();
    }
    //       print();
//  std::cout <<std::endl<< "????????? "<<*current <<"termin? "
//               <<current.isTerminated()    <<current.isEmpty()<<deg<<"!"<< base::size()<<std::endl<<std::endl;
//         print();
     while (!base::isConstant() && (*base::top() < min_idx))
       base::incrementElse();
     //      print();
     if (*base::top()  < min_idx) { 
       base::pop();
       base::push( BoolePolynomial(false).navigation());
       return;
     }

     //     std::cout << "ds "<<deg  <<" "<<base::size()<<std::endl;

     assert(deg >= base::size()); 

//      typename on_same_type<descending_property, valid_tag, 
//        std::less<size_type>,
//        std::less_equal<size_type> >::type comp;

//     std::cout << "dummy1" <<std::endl;
//     print(); 
       base tmp, max_elt(base::top());

     typename on_same_type<descending_property, valid_tag, 
       std::greater<size_type>,
       std::greater_equal<size_type> >::type comp;
     
     is_same_type<descending_property, invalid_tag> takeLast;

     unsigned upperbound(deg - base::size()+1);
//          std::cout << "deg "<< deg<<" "<<upperbound <<" "<<max_idx<<std::endl;
//          print();
     while (!max_elt.empty() && (takeLast() || (tmp.size() != upperbound)) ) {
      
       while (!max_elt.isConstant() && (max_elt.size() < upperbound) 
              && (*max_elt.top() < max_idx) )
        max_elt.incrementThen();
       //       std::cout << "top     "<< *max_elt.top()<<" "<< max_idx << 
       //         "  "<<max_elt.size()  <<std::endl; 
    //    while(*max_elt.top() ) {

//        }
       navigator navi(max_elt.top());
       while(!navi.isConstant() && (*navi < max_idx)) {
         navi.incrementElse();
       }

       //      std::cout << "size     "<< max_elt.size()  <<std::endl;
       //      max_elt.print();
      if (navi.isTerminated() || 
          (!navi.isConstant () &&(*navi >= max_idx)) ) {
        if (comp(max_elt.size(), tmp.size())) {
          tmp = max_elt;
          tmp.decrementNode();
          tmp.push(navi);
        }
        max_elt.decrementNode();
      }
      //max_elt.decrementNode();
      //   std::cout << "b4next "<<std::endl;
      //  max_elt.print();
      max_elt.nextTerm();
//       bool invalid = true;
//       while (!max_elt.empty() && (*max_elt.top() >= min_idx) && invalid) {
//         max_elt.incrementElse();
//         if (invalid = max_elt.isInvalid())
//           max_elt.decrementNode();
//       }
     }
     //  *this = tmp;
     //   std::cout << "tmp     "<< *tmp.begin()<<" "<< max_idx << "
     //   "<<tmp.size()  <<std::endl; 

     base::decrementNode();

     base::append(tmp);
     if(tmp.empty())
       base::push(BoolePolynomial(false).navigation());

// print();
    
//      std::cout << "last "<<tmp.top().isTerminated()<<std::endl;
//      tmp.print();

 //  bounded_restricted_stack<NaviType,  descending_property>
//        bstart(base::top(),  deg - base::size(), max_idx),
//        bend;
//      bstart = std::max_element(bstart, bend, comp);
//      base::pop();
//      //  std::cout << "dummy1" <<*base::top()<<std::endl;

//      dummy_append(*this, bstart.begin(), bstart.end());
//      //     std::cout << "dummy2" <<*base::top()<<std::endl;
//      base::push(bstart.next());
//     print();
    //  std::cout << "bstart.next();"<<*bstart.next()<<std::endl;
}

#if 0
template <class NavigatorType, class DescendingProperty>
inline void
CBlockTermStack<NavigatorType, DescendingProperty>::degTerm(idx_type min_idx, idx_type max_idx) {

  assert(!base::empty());
  assert(!base::isInvalid());

    unsigned size = base::size()+1;

    //  navigator current;

    if (*base::top() < min_idx) {
      base::decrementNode();
      base::push(BoolePolynomial(false).navigation());
      return;
    }

    assert(!base::isConstant());
//     base::incrementThen();
    
//          std::cout << "do0"<<std::endl;
//     do {
//       std::cout << "do1"<<std::endl;
//       base::decrementNode();
//       nextTerm();
//       std::cout << "do1a"<<std::endl;
//       if( base::empty() )
//         return;
//         std::cout << "do2"<<std::endl;
//       while(!base::isConstant() && (*base::top() < max_idx) &&
//             (base::size() < size) )
//         base::incrementThen();
  
//     } while  (!base::empty() && (*base::top() >= min_idx) &&
//               !(base::isTermEnd() && (base::size() == size) )  );

//       std::cout << "done1"<<std::endl;

    base::incrementThen();
    do {

      assert(!base::empty());  
      base::decrementNode();

      if( !base::empty()) {

//       current = base::top();
//       base::pop();

//       current.incrementElse();
        assert(!base::isConstant()); 
        base::incrementElse();
        while (!base::isConstant() && *base::top() < max_idx) {

          base::incrementThen();
        }
      }
    } while ( !base::empty() && (*base::top() >= min_idx) &&
              (!base::isTermEnd() || (base::size() != size)) );
  
     if (base::size() == size)
       return;// current;

     if (base::empty() &&(size == 0) )
       return;// current;

     if (base::empty() ){
       base::push(m_navi);
     }
     else {
       //       current = base::top();
       assert(!base::isConstant());
       base::incrementThen();
     }


     while (!base::isConstant() && (*base::top() < min_idx))
       base::incrementElse();


     if (*base::top() < min_idx)
       base::push(BoolePolynomial(false).navigation());


     assert(size > base::size()); 
     base tmp, max_elt(base::top());

     typename on_same_type<descending_property, valid_tag, 
       std::greater<size_type>,
       std::greater_equal<size_type> >::type comp;
     
     is_same_type<descending_property, invalid_tag> takeLast;
     unsigned upperbound(deg);

     while (!max_elt.empty() && (takeLast() || (tmp.size() != upperbound)) ) {
      
       while (!max_elt.isConstant() && (max_elt.size() < upperbound) 
              && (*max_elt.top() < max_idx) )
        max_elt.incrementThen();
      
      if (max_elt.isTermEnd() || (*max_elt.top() > max_idx) ) {
        if (comp(max_elt.size(), tmp.size()))
          tmp = max_elt;
        max_elt.decrementNode();
      }
      
      ////max_elt.nextTerm();
      bool invalid = true;
      while (!max_elt.empty() && (*max_elt.top() >= min_idx) && invalid) {
        assert(!max_elt.isConstant());
        max_elt.incrementElse();
        if (invalid = max_elt.isInvalid())
          max_elt.decrementNode();
      }
     }
     //  *this = tmp;
     base::decrementNode();
     base::append(tmp);
     std::cout << "tmp "<<*tmp.top()<<": "<< *base::top() <<std::endp;
    /*  bounded_restricted_stack<navigator,  valid_tag>
      bstart(base::top(),  size - base::size() - 1, max_idx),
      bend;
     bstart = std::max_element(bstart, bend);

     assert((*bstart.begin())!=base::top());

    dummy_append(*this, bstart.begin(), bstart.end());

    //  std::cout << "bstart.next();"<<*bstart.next()<<std::endl;
    base::push(bstart.next());
    */
  }
#endif

END_NAMESPACE_PBORI

#endif
