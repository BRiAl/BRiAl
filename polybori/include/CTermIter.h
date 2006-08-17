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
 * Revision 1.10  2006/08/17 15:39:16  bricken
 * + reference type is now equal to value type, as references would lie in the iterator and not in the polynomial
 *
 * Revision 1.9  2006/06/06 10:56:59  dreyer
 * CHANGE usedVariables() more efficient now.
 *
 * Revision 1.8  2006/05/24 07:46:11  bricken
 * + added a missing return statement
 *
 * Revision 1.7  2006/04/25 09:30:42  dreyer
 * FIX end of CTermIterm for constants, CHANGE consistent functional names
 *
 * Revision 1.6  2006/04/24 14:45:35  dreyer
 * FIX CTermIter; ADD BoolePolynomial uses CTermIter
 *
 * Revision 1.5  2006/04/24 10:26:00  dreyer
 * FIX CTermIter::reference (back)
 *
 * Revision 1.4  2006/04/24 10:24:36  dreyer
 * FIX CTermIter::reference
 *
 * Revision 1.3  2006/04/24 08:40:46  dreyer
 * FIX CTermIter<> treats o/1 Monomials correctly.
 *
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

// include polybori functionals
#include "pbori_func.h"

// include polybori properties
#include "pbori_traits.h"

#ifndef CTermIter_h_
#define CTermIter_h_

BEGIN_NAMESPACE_PBORI


/** @class CTermIter
 * @brief This class defines an iterator for the monomials in a Boolean
 * polynomial.
 *
 **/

template <class TermType, class NavigatorType, 
          class ForwardOp, class BackwardOp, 
          class TerminalValueOp = project_ith<2> >
class CTermIter {

public:

  /// Type for boolean results
  typedef TermType term_type;

  /// Type for indices
  typedef typename pbori_traits<term_type>::idx_type idx_type;

  /// Type for hashing
  typedef typename pbori_traits<term_type>::hash_type hash_type;

  /// Type for boolean results
  typedef typename pbori_traits<term_type>::bool_type bool_type;

  /// Get type of navigators
  typedef NavigatorType navigator_type;

  /// Get operational, which changes the current term on forward steps
  typedef ForwardOp forwardop_type;

   /// Get operational, which changes the current term on backward steps
  typedef BackwardOp backwardop_type;

  /// Get operational, which generates terminal value corresponding to Boolean
  typedef TerminalValueOp termvalop_type;

  /// Return type of dereferencing operator
  typedef term_type value_type;

  /// Get type of *this
  typedef CTermIter<term_type, navigator_type, 
                    forwardop_type, backwardop_type, termvalop_type> self;

  /// @name Interface types for standard iterator access
  //@{
  typedef std::forward_iterator_tag iterator_category;
  typedef typename std::iterator_traits<navigator_type>::difference_type 
  difference_type;
  typedef void pointer;
  typedef const value_type reference;
  //@}

  /// Define type for stacking
  typedef std::stack<navigator_type> stack_type;

  /// result type of top()
  typedef typename stack_type::value_type top_type;

  /// Default constructor
  CTermIter(): 
    m_stack(), m_value( termvalop_type()(value_type(), false) ), 
    forwardop(), backwardop(), termvalop() { 
  }

  /// Construct from initial navigator
  CTermIter(navigator_type navi, 
            forwardop_type fop_ = forwardop_type(), 
            backwardop_type bop_ = backwardop_type(), 
            termvalop_type tvop_ = termvalop_type() ):
    m_stack(), m_value(),  forwardop(fop_), backwardop(bop_), 
    termvalop(tvop_)  {

    if (navi.isValid()){
      followThen(navi);
      terminate(navi);
    }
    else 
      m_value = termvalop(m_value, false);
  }

  /// Copy Constructor
  CTermIter(const self& rhs):
    m_stack(rhs.m_stack), m_value(rhs.m_value),  
    forwardop(rhs.forwardop), backwardop(rhs.backwardop),
    termvalop(rhs.termvalop) {};

  /// Destructor
  ~CTermIter() {};

  /// Constant dereference operator
  reference operator*() const { return m_value; }

  /// Equality test (assume iterators from same instance)
  bool_type operator==(const self& rhs) const {
    if(empty() || rhs.empty())
      return (empty() && rhs.empty());
    else
      return top() == rhs.top(); 
  }

  /// Nonequality test
  bool_type operator!=(const self& rhs) const { return !(*this == rhs);  }

  /// Get element on stack
  const top_type& top() const { return m_stack.top(); }

  /// Get element on stack
  top_type& top() { return m_stack.top(); }

  /// Check whether stack is empty
  bool empty() const { return m_stack.empty(); }

  /// Prefix increment operator
  self& operator++() {
   
    if (!empty()){              //  Iterations not finished yet
      navigator_type navi = top();
      if ( !navi.isValid() ){   //  Case: final iteration
        clear();        //  reset value
      }
      else {
        // Go back to begin of next path
        nextElse(navi);
      
        if( !terminate(navi) ) {
          while( !m_stack.empty() && navi.isConstant() && 
                 !navi.terminalValue() ){
            navi = top();
            nextElse(navi);
          }

          // Find end of current path
          followThen(navi);
        }
        terminate(navi);
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

protected:

  void followThen(navigator_type& navi) {
    while(!navi.isConstant()){
      m_stack.push(navi);
      m_value = forwardop(m_value, *navi);
      navi.incrementThen(); 
    }
  }

  void nextElse(navigator_type& navi) {
    m_value = backwardop(m_value, *navi);
    navi.incrementElse();
    m_stack.pop();
  }

  bool terminate(const navigator_type& navi) {
    if ( m_stack.empty() && navi.isConstant() ) { // Constant monomial

      if (navi.terminalValue()) { // Case: Monomial one
        m_value = termvalop(m_value, true);
        m_stack.push(navigator_type()); // One more iteration for showing 1
      }
      else                      // Case: Monomial zero
        clear();

      return true;
    }
    return false;
  }

  void clear() {
    m_value = termvalop(m_value, false);
    m_stack = stack_type();
  }

private:
  stack_type m_stack;
  value_type m_value;
  forwardop_type forwardop;
  backwardop_type backwardop;
  termvalop_type termvalop;
};

END_NAMESPACE_PBORI

#endif
