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
    while(!isConstant())
      incrementThen();
  }

  void markOne() {
    assert(empty());
    push(navigator());
  }

  bool_type markedOne() const {
    assert(!empty());
    return !m_stack.front().isValid();
  }

  void clearOne() {
    pop();
    assert(empty());
  }

protected:


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

  void incrementTerm() {
    assert(!base::empty());
    if (base::markedOne()) {
      base::clearOne();
      return;
    }
      
    nextTerm();
    followThen();
    terminate();
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
private:
  void previousTerm(std::forward_iterator_tag);
  void previousTerm(std::bidirectional_iterator_tag);
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
                     typename on_same_type<DescendingProperty, valid_tag,
                                           std::forward_iterator_tag, 
                                  std::bidirectional_iterator_tag>::type >
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
  void leadTerm() {

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
              !(base::isTerminated() && (base::size() == size) )  );

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
              !(base::isTerminated() && (base::size() == size) )  );

  }

  void findTerm(size_type upperbound){

    typename on_same_type<descending_property, valid_tag, 
      std::greater<size_type>,
      std::greater_equal<size_type> >::type comp;

      base::push(m_start);
      self tmp;

      while (!base::empty() && (tmp.size() != upperbound) ) {
             
        while (!base::isConstant() && (base::size() < upperbound) )
          base::incrementThen();
   
        if (base::isConstant()) {
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

END_NAMESPACE_PBORI

#endif
