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
  operator()(const NavigatorType& navi) const {
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
    m_indices(BoolePolyRing::blockRingBegin()), 
    m_current_block(BoolePolyRing::blockRingBegin()),
    m_deg_cache(BoolePolyRing::activeManager()) { }

  typename NavigatorType::size_type 
  operator()(const NavigatorType& navi) const {
    return dd_cached_block_degree(m_deg_cache, navi, max());
  }

  idx_type min() const {
     return ( m_current_block == m_indices? 0: *(m_current_block - 1) );
  }

  idx_type max() const {
    return *m_current_block;
  }
  self& operator++(){
    ++m_current_block;
    return *this;
  }

  self& operator--(){
    --m_current_block;
    return *this;
  }

private:
  block_iterator m_indices;
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


template <class NavigatorType, class BlockProperty, class Category>
class CDegStackBase;

// @brief for pure degree stacks
template <class NavigatorType, class Category>
class CDegStackBase<NavigatorType, invalid_tag, Category>:
  public CTermStack<NavigatorType, Category> {

public:
  typedef CTermStack<NavigatorType, Category> base;
  typedef NavigatorType navigator;

  CDegStackBase(): base() {}
  CDegStackBase(const navigator& navi): base(navi), getDeg() {}

  bool atBegin() const { return base::empty(); }
  template <class Type>
  bool atEnd(const  Type& rhs) const { 
    return rhs.isConstant();
  }  
  template <class Type>
  bool checkEnd(const Type& rhs) const { return rhs.isTermEnd(); }
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
  CDegStackBase(const navigator& navi): base(navi), block() {}

  idx_type getDeg(const navigator& navi) const { return block(navi); }

  bool atBegin() const { return base::empty() || (*base::top() < block.min()); }

  template <class Type>
  bool atEnd(const  Type& rhs) const { 
    return rhs.isConstant() || (*rhs.top() >= block.max());

  }

  template <class Type>
  bool checkEnd(const  Type& rhs) const {
    assert(!rhs.empty());
    navigator navi(rhs.top());
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

  cached_block_deg<navigator> block;
};

template <class NavigatorType, class BlockProperty, class DescendingPropert>
class CProxTermStack;

template <class NavigatorType, class BlockProperty>
class CProxTermStack<NavigatorType, BlockProperty, valid_tag>:
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
class CProxTermStack<NavigatorType, BlockProperty, invalid_tag>:
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
  public CProxTermStack<NavigatorType, BlockProperty, DescendingProperty> {

public:
  typedef CProxTermStack<NavigatorType, BlockProperty, DescendingProperty> base;

  typedef NavigatorType navigator;
  typedef typename navigator::size_type size_type;
  typedef DescendingProperty descending_property;
  typedef CDegTermStack<navigator, descending_property, BlockProperty> self;
  typedef BoolePolynomial poly_type;
  typedef typename poly_type::dd_type dd_type;



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
      base::push(m_start);
      findTerm(upperbound);
    }
    if(!base::empty())
      base::terminate();
  }


  void degTerm() {
    size_type size = base::size() + 1;

    assert(!base::isConstant());

    base::incrementThen();
  
    do {
      base::decrementNode();
      base::proximateTerm();
      if( base::empty() )
        return;
  
      while(!base::isConstant() && (base::size() < size) )
        base::incrementBranch();
  
    } while  (!base::empty() && 
              !(base::isTermEnd() && (base::size() == size) )  );

  }

 
  void decrementTerm() {}


//   void findTerm(size_type upperbound){



//     //    base::push(m_start);
//     typename base::purestack_type max_elt(m_start);
//     base::push(m_start);
//     findTerm(max_elt, upperbound);

//   }
//   template <class Type>
  void
  findTerm(size_type upperbound) {
    assert(!base::empty());
    typename base::purestack_type tmp, current(base::top());
    base::decrementNode();

    typename base::size_comparer comp;

    while (!current.empty() && (base::takeLast()|| (tmp.size() != upperbound)) ) {
      
      while (!base::atEnd(current) && (current.size() < upperbound) )
        current.incrementThen();
      
      if (base::checkEnd(current)) {
        if (comp(current.size(), tmp.size()))
          tmp = current;
        current.decrementNode();
      }
      
      current.nextTerm();
    }
    //    current =tmp;

    base::append(tmp);
    if(tmp.empty())
      base::push(BoolePolynomial(false).navigation());
  }


private:
  navigator m_start;
};



//////////////////////////////////////////////////////////
template <class NavigatorType, class DescendingProperty>
class CBlockTermStack:
  public CDegTermStack<NavigatorType, DescendingProperty, valid_tag> {

public:
  typedef CDegTermStack<NavigatorType, DescendingProperty, valid_tag> base; 

  typedef NavigatorType navigator;
  typedef typename navigator::size_type size_type;
  typedef DescendingProperty descending_property;
  typedef CBlockTermStack<navigator, descending_property> self;
  typedef BoolePolynomial poly_type;
  typedef typename poly_type::dd_type dd_type;
  typedef poly_type PolyType;


  CBlockTermStack(navigator navi): 
    base(navi),  m_navi(navi) {    

  }
  void firstTerm() {
    assert(!base::empty());
    findTerminal();
    base::terminate();
  }

  // Default Constructor
  CBlockTermStack(): base(), m_navi(){}


  typedef typename PolyType::idx_type idx_type;

  typedef typename base::stack_type stack_type;


//   size_type currentBlockDegree(const navigator& navi) const {
//     return dd_cached_block_degree(m_deg_cache, navi, *m_current_block);
//   }
  
  void incrementBlock() {
    base::findTerminal();
//     size_type deg = base::block(base::top());

//     while(deg > 0) {
  
//       if ( base::maxOnThen(deg) ){
//         --deg;
//         assert(!base::isConstant());
//         base::incrementThen(); 
//       }
//       else {
//         assert(!base::isConstant());
//         base::incrementElse();
//         assert(!base::isConstant());
//       }
//     }
  }


//   idx_type blockMin() const {
//     return ( m_current_block == m_indices? 0: *(m_current_block - 1) );
//   }

//   idx_type blockMax() const {
//     return *m_current_block;
//   }

 void incrementTerm() {

    assert(!base::empty());
    if (base::markedOne()) {
      base::clearOne();
      return;
    }

    navigator current = base::top(); 
  
    while (*current < base::block.min())
      --base::block;

   
    ++base::block;

    do {
      --base::block;
    
      degTerm();
      assert(!base::empty()); 
      current = base::top();
      base::pop();

    } while (!base::empty() && current.isEmpty());

    base::push(current);
    findTerminal();

    assert(!base::empty());

    base::terminate();
 
  }

   bool isBlockEnd() const {
     assert(!base::empty());
     navigator navi(base::top());
     while( (!navi.isConstant()) && (*navi < base::block.max()) ) {
       navi.incrementElse();
     }
     return (navi.isConstant()? navi.terminalValue(): *navi >= base::block.max());
   }

  void gotoBlockEnd()  {
     assert(!base::empty());
     navigator navi(base::top());
     while( (!base::isConstant()) && (*base::top() < base::block.max()) ) {
       base::incrementElse();
     }
  }

  void findTerminal() {
    assert(base::top().isValid());
    if (!base::isConstant() ) 
      incrementBlock();

    while (!base::isConstant()  ) {
      assert (base::block.max() != CUDD_MAXINDEX);
      ++base::block;

      incrementBlock();
    }
  }



  void degTerm();



  using base::print;

  navigator m_navi;


};

template <class NavigatorType, class DescendingProperty>
inline void
CBlockTermStack<NavigatorType, DescendingProperty>::degTerm() {

  idx_type min_idx(base::block.min()), max_idx(base::block.max());

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
      base::proximateTerm();

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

     typename base::purestack_type tmp, max_elt(base::top());


     unsigned upperbound(deg - base::size()+1);

#if 0
     typename base::size_comparer comp;    
       

     while (!max_elt.empty() && 
            (base::takeLast() || (tmp.size() != upperbound)) ) {
      
       while (!base::atEnd(max_elt)&& (max_elt.size() < upperbound) 

              /*max_elt.isConstant() && (max_elt.size() < upperbound) 
                && (*max_elt.top() < max_idx)*/ )
        max_elt.incrementThen();

       navigator navi(max_elt.top());
       while(!navi.isConstant() && (*navi < max_idx)) {
         navi.incrementElse();
       }
     
      if (navi.isTerminated() || 
          (!navi.isConstant () &&(*navi >= max_idx)) ) {
        if (comp(max_elt.size(), tmp.size())) {
          tmp = max_elt;
//           tmp.decrementNode();
//           tmp.push(navi);
        }
        max_elt.decrementNode();
      }
      max_elt.nextTerm();
     }
     max_elt = tmp;
       base::decrementNode();
       base::append(max_elt);
#else    
       //   base::decrementNode();
    base::findTerm(upperbound);
#endif
     //  base::decrementNode();
     // base::append(max_elt);

//     if(noterm)
//        base::push(BoolePolynomial(false).navigation());
//      else {
       gotoBlockEnd();
//        navigator navi = base::top();
//        while(!navi.isConstant() && (*navi < max_idx)) {
//          navi.incrementElse();
//        }

//        base::decrementNode();
//        base::push(navi);
//     }
       

}

END_NAMESPACE_PBORI

#endif
