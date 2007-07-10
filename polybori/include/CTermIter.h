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
 * Revision 1.23  2007/07/10 14:53:28  dreyer
 * CHANGE: clean-up
 *
 * Revision 1.22  2007/05/18 11:48:39  dreyer
 * ADD: sophisticated term_accumulate
 *
 * Revision 1.21  2007/05/14 08:44:07  dreyer
 * ADD: isOne()/isZero() to term iterators
 *
 * Revision 1.20  2007/05/03 16:04:45  dreyer
 * CHANGE: new-style CTermIter integrated
 *
 * Revision 1.19  2007/04/24 15:23:03  dreyer
 * FIX: minor changes fixing -Wall warnings
 *
 * Revision 1.18  2007/03/21 08:55:08  dreyer
 * ADD: first version of block_dlex running
 *
 * Revision 1.17  2007/02/14 10:28:48  dreyer
 * FIX: wrong constant term
 *
 * Revision 1.16  2007/01/16 10:53:54  dreyer
 * FIX: bug in CTermIter::deg(), wrong degree for monomial 1.
 *
 * Revision 1.15  2007/01/11 17:08:04  dreyer
 * ADD: deg() and FirstIndex() for term iterators; deque instead of stack
 *
 * Revision 1.14  2006/09/12 15:33:16  dreyer
 * CHANGE: project_ith<0> corresponds to do-nothing (void return_type)
 *
 * Revision 1.13  2006/09/12 15:11:14  dreyer
 * FIX: Stricter syntax of gcc 4
 *
 * Revision 1.12  2006/09/12 13:48:18  dreyer
 * ADD Preparations for bidirectional iterator
 *
 * Revision 1.11  2006/09/04 15:58:42  dreyer
 * ADD: DegLexOrder and preparations
 *
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

// include basic definitions
#include "pbori_defs.h"

// include polybori functionals
#include "pbori_func.h"

// include polybori properties
#include "pbori_traits.h"


// include boost's interator facade
#include <boost/iterator/iterator_facade.hpp>


#ifndef CTermIter_h_
#define CTermIter_h_

BEGIN_NAMESPACE_PBORI


/** @class CTermIter
 * @brief This class defines an iterator for the monomials in a Boolean
 * polynomial.
 *
 **/

template <class StackType, class TermGeneratorType>
class CTermIter: 
  public boost::iterator_facade<
  CTermIter<StackType, TermGeneratorType>,
  typename TermGeneratorType::value_type,
  typename StackType::iterator_category, 
  typename TermGeneratorType::result_type
  > {

public:

  /// Define type for storing current path (term) in stack of nodes 
  typedef StackType stack_type;
  
  /// Get type of navigators
  typedef typename stack_type::navigator navigator;

  /// Type for indices
  typedef typename navigator::idx_type idx_type;

  /// Type for Boolean results
  typedef typename navigator::bool_type bool_type;

  /// Type for lengths
  typedef typename navigator::size_type size_type;

  /// Type for functional, which generates actual term, for current path
  typedef TermGeneratorType term_generator;

  /// @name Iterators over current term (without explicite construction)
  //@{
  typedef typename stack_type::const_iterator const_iterator;
  typedef typename stack_type::const_reverse_iterator 
  const_reverse_iterator;
  //@}

  /// Copy constructor
  CTermIter(const CTermIter& rhs): 
    m_getTerm(rhs.m_getTerm), m_stack(rhs.m_stack) {
  }

  /// Construct from navigator over decision diagram
  CTermIter(navigator navi, term_generator get_term = term_generator()): 
    m_getTerm(get_term), m_stack(navi) {
    m_stack.init(); 
  }

  ///  Default constructor   
  CTermIter():  m_getTerm(), m_stack() {}

  /// Destructor
  ~CTermIter() {}

  /// Incrementation operation
  void increment() {
    m_stack.increment();
  }

  /// Equality test
  bool_type equal (const CTermIter& rhs) const {
     return m_stack.equal(rhs.m_stack);
  }

  /// Dereferencing of the iterator
  typename term_generator::result_type dereference() const {
    return m_getTerm(m_stack);
  }

  /// @name Interface for iteration over term without constructing
  //@{
  const_iterator begin() const { return m_stack.begin(); }
  const_iterator end() const { return m_stack.end(); }
  const_reverse_iterator rbegin() const { return m_stack.rbegin(); }
  const_reverse_iterator rend() const { return m_stack.rend(); }
  //@}

  /// Determine whether term is one (without explicit constructing)
  bool_type isOne() const { return m_stack.isOne(); }

  /// Determine whether term is zero (without explicit constructing)
  bool_type isZero() const { return m_stack.isZero(); }

  /// Get degree of current term
  size_type deg() const { return m_stack.deg(); }

  /// Get first index of current term 
  idx_type firstIndex() const { 
    assert(!m_stack.empty()); 
    return *begin(); 
  }

  /// Get navigator of term start
  navigator navigation() const {
    return m_stack.navigation();
  }

protected:
  /// The functional which defines the dereferecing operation
  term_generator m_getTerm;

  /// The stack, which carries the current path
  stack_type m_stack;
};

END_NAMESPACE_PBORI

#endif

