// -*- c++ -*-
//*****************************************************************************
/** @file CBidirectTermIter.h
 *
 * @author Alexander Dreyer
 * @date 2006-04-21
 *
 * 
 *
 * @par Copyright:
 *   (c) 2006 by The PolyBoRi Team
**/
//*****************************************************************************

#ifndef polybori_iterators_CBidirectTermIter_h_
#define polybori_iterators_CBidirectTermIter_h_

// get standard header
#include <stack>
#include <utility>

// include basic definitions
#include <polybori/pbori_defs.h>

// Get forward term iterator
#include "CTermIter.h"

BEGIN_NAMESPACE_PBORI

template<class NavigatorType>
class handle_else :
  public std::deque<NavigatorType> {
public:

  typedef NavigatorType navigator_type;
  typedef std::deque<NavigatorType> base;

  void operator()(const navigator_type& navi) {

    while(!base::empty() && (*top() >= *navi) )
      base::pop_back();

    base::push_back(navi);
  }
  void push(const navigator_type& navi) { base::push_back(navi); }
  void pop() { base::pop_back(); }

  const navigator_type& top() const { return base::back(); };

  void append(const handle_else& rhs) {
    PBORI_ASSERT(base::empty() || rhs.empty() || ((**rhs.begin()) > (*top())) );
    base::insert(base::end(), rhs.begin(), rhs.end());
  }
};

#if 0
/** @class CBidirectTermIter
 * @brief This class defines an iterator for the monomials in a Boolean
 * polynomial.
 *
 **/

template <class TermType, class NavigatorType, 
          class ForwardOp, class BackwardOp, 
          class TerminalValueOp = project_ith<2> >
class CBidirectTermIter:
  public CTermIter<TermType, NavigatorType, 
                   ForwardOp, BackwardOp, 
                   TerminalValueOp, 
                   handle_else<NavigatorType> >{

public:

  /// Type for boolean results
  typedef TermType term_type;

  /// Get type of navigators
  typedef NavigatorType navigator_type;

  /// Get operational, which changes the current term on forward steps
  typedef ForwardOp forwardop_type;

   /// Get operational, which changes the current term on backward steps
  typedef BackwardOp backwardop_type;

  /// Get operational, which generates terminal value corresponding to Boolean
  typedef TerminalValueOp termvalop_type;

  /// Type, which stacks else banches.
  typedef handle_else<navigator_type> elsehandle_type;

  /// Get type of *this
  typedef CBidirectTermIter<term_type, navigator_type, 
                    forwardop_type, backwardop_type, termvalop_type> self;

  /// Generic access to base type;
  typedef CTermIter<term_type, navigator_type, 
                    forwardop_type, backwardop_type, termvalop_type,
                    elsehandle_type> base;

  /// @name Interface types for standard iterator access
  //@{
  typedef std::bidirectional_iterator_tag iterator_category;
  typedef typename base::difference_type difference_type;
  typedef typename base::pointer pointer;
  typedef typename base::reference reference;
  //@}

  /// Generic access to member 
  using base::handleElse;

  /// Default constructor
  CBidirectTermIter(): 
    base() {}

  /// Construct from initial navigator
  CBidirectTermIter(navigator_type navi, 
            forwardop_type fop_ = forwardop_type(), 
            backwardop_type bop_ = backwardop_type(), 
            termvalop_type tvop_ = termvalop_type() ):
    base(navi, fop_, bop_, tvop_) {}

  /// Construct last term from initial navigator
  CBidirectTermIter(navigator_type navi, dummy_iterator):
    base() { 
    if(navi.isValid()) {
      followElse(navi); 
      terminate(navi);
    }
 }
  
  /// Copy Constructor
  CBidirectTermIter(const self& rhs):
    base(rhs) {};

  /// Destructor
  ~CBidirectTermIter() {};

  /// Prefix increment operator
  self& operator++() {
    base::operator++();
    return *this;
  }

  /// Postfix increment operator
  self operator++(int dummy) { 
    return base::operator++(dummy);
  };

  /// Prefix decrement operator
  self& operator--() {

    if (!handleElse.empty()){
      navigator_type navi = handleElse.top();
      base::popToIndex(*navi);


      handleElse.pop();
      base::nextThen(navi);

      followElse(navi);
    }
    else
      base::clear();
    return *this;
  }

  /// Postfix decrement operator
  self operator--(int) { 
    self tmp(*this); 
    operator--(); 
    return tmp;
  };

protected:


  void followElse(navigator_type& navi) {
    while( !navi.isConstant() ) {       // if still in interior of a path
      if(!navi.elseBranch().isEmpty()) {
        handleElse.push(navi);
        navi.incrementElse();   // go in direction of last term, if possible
      }
      else
        base::nextThen(navi);
    }  
  }

};

#endif

END_NAMESPACE_PBORI

#endif
