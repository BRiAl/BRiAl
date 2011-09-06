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
 *   (c) 2006 by The PolyBoRi Team
**/
//*****************************************************************************

#ifndef polybori_iterators_CTermIter_h_
#define polybori_iterators_CTermIter_h_

// include basic definitions
#include <polybori/pbori_defs.h>

// include polybori functionals
#include <polybori/routines/pbori_func.h>

// include polybori properties
#include <polybori/common/traits.h>


// include boost's interator facade
#include <boost/iterator/iterator_facade.hpp>

#include <polybori/BooleEnv.h>


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

  /// Type for degrees
  typedef typename navigator::deg_type deg_type;

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
  template <class MgrType>
  CTermIter(navigator navi, const MgrType& mgr): 
    m_getTerm(mgr), m_stack(navi, mgr) {
    m_stack.init(); 
  }

  /// Destructor
  ~CTermIter() {}

  /// Incrementation operation
  void increment() {
    m_stack.increment();
  }

  /// Decrementation operation
  void decrement() {
    m_stack.decrement();
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

  /// Check, whether end of iteration is reached
  bool_type isEnd() const { return isZero(); }

  /// Get degree of current term
  deg_type deg() const { return m_stack.deg(); }

  /// Get first index of current term 
  idx_type firstIndex() const { 
    PBORI_ASSERT(!m_stack.empty()); 
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

