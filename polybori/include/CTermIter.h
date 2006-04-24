// -*- c++ -*-
//*****************************************************************************
/** @file CTermIter.h
 *
 * @author Alexander Dreyer
 * @date 2006-04-21
 *
 * 
 *
 * @par Copyright:
 *   (c) 2006 by
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
 * Revision 1.2  2006/04/24 05:21:05  bricken
 * + name rhs of parameter was missing
 *
 * Revision 1.1  2006/04/21 16:17:11  dreyer
 * ADD template class CTermIter<>
 *
 * @endverbatim
**/
//*****************************************************************************

// get standard header
#include <stack>

// include basic definitions
#include "pbori_defs.h"

#ifndef CTermIter_h_
#define CTermIter_h_

BEGIN_NAMESPACE_PBORI


/** @class CTermIter
 * @brief This class defines an iterator for the monomials in a Boolean
 * polynomial.
 *
 **/

template <class TermType, class NavigatorType, 
          class ForwardOp, class BackwardOp>
class CTermIter {

public:

  /// Type for indices
  typedef typename TermType::idx_type idx_type;

  /// Type for hashing
  typedef typename TermType::hash_type hash_type;

  /// Type for boolean results
  typedef typename TermType::bool_type bool_type;

  /// Type for boolean results
  typedef TermType term_type;

  /// Get type of navigators
  typedef NavigatorType navigator_type;

  /// Get operational, which changes the current term on forward steps
  typedef ForwardOp forwardop_type;

   /// Get operational, which changes the current term on backward steps
  typedef BackwardOp backwardop_type;

 /// Return type of dereferencing operator
  typedef term_type value_type;

  /// Get type of *this
  typedef CTermIter<term_type, navigator_type, 
                    forwardop_type, backwardop_type> self;

  /// @name Interface types for standard iterator access
  //@{
  typedef std::forward_iterator_tag iterator_category;
  typedef typename std::iterator_traits<navigator_type>::difference_type 
  difference_type;
  typedef void pointer;
  typedef const value_type& reference;
  //@}

  /// Define type for stacking
  typedef std::stack<navigator_type> stack_type;

  /// result type of top()
  typedef typename stack_type::value_type top_type;

  /// Default constructor
  CTermIter(): 
    thestack(), value(), forwardop(), backwardop() { 
    thestack.push(navigator_type()); 
  }

  /// Construct from initial navigator
  CTermIter(navigator_type navi, 
            forwardop_type fop_ = forwardop_type(), 
            backwardop_type bop_ = backwardop_type() ):
    thestack(), value(),  forwardop(fop_), backwardop(bop_)  {

    while(!navi.isConstant()){
      thestack.push(navi);
      forwardop(value, *navi);
      navi.incrementThen();
    }
  }

  /// Copy Constructor
  CTermIter(const self& rhs):
    thestack(rhs.thestack), value(rhs.value),  
    forwardop(rhs.forwardop), backwardop(rhs.backwardop) {};

  /// Destructor
  ~CTermIter() {};

  /// Constant dereference operator
  const value_type& operator*() const { return value; }

  /// Equality test
  bool_type operator==(const self& rhs) const { return top() == rhs.top(); }

  /// Nonequality test
  bool_type operator!=(const self& rhs) const { return top() != rhs.top(); }

  /// Get element on stack
  const top_type& top() const { return thestack.top(); }

  /// Get element on stack
  top_type& top() { return thestack.top(); }

  /// Prefix increment operator
  self& operator++() {

    navigator_type navi = top();
    if ( !navi.isValid() )
      return *this;

    backwardop(value, *navi);
    navi.incrementElse(); 
    thestack.pop();
   
    if ( thestack.empty() ) {
      *this = self();
    }
    else{
      while (navi.isConstant() &&  !navi.terminalValue()  ) {    
        navi = top();
        backwardop(value, *navi);
        navi.incrementElse(); 
        thestack.pop();
      }

      while(!navi.isConstant()){

        thestack.push(navi);
        forwardop(value, *navi);
        navi.incrementThen();
      }
    }

    return *this;
  }

  /// Postfix increment operator
  self operator++(int) { 
    self tmp(*this); 
    operator++(); 
    return tmp;
  };


private:
  stack_type thestack;
  value_type value;
  forwardop_type forwardop;
  backwardop_type backwardop;

};

END_NAMESPACE_PBORI

#endif
