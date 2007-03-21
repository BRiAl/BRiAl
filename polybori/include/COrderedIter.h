// -*- c++ -*-
//*****************************************************************************
/** @file CDegLexIter.h
 *
 * @author Alexander Dreyer
 * @date 2006-09-06
 *
 * This file defines an iterator, which respects the current ordering.
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
 * Revision 1.7  2007/03/21 08:55:08  dreyer
 * ADD: first version of block_dlex running
 *
 * Revision 1.6  2007/03/19 16:49:39  dreyer
 * CHANGE: ordered iterators made more generic
 *
 * Revision 1.5  2007/03/16 16:59:20  dreyer
 * CHANGE: started to rewrite CGenericIter using boost:iterator_facade
 *
 * Revision 1.4  2006/10/04 15:46:49  dreyer
 * ADD: divisorsOf(exp_type), orderedExpBegin/End;
 * CHANGE: Polynomials printing respects ordering
 *
 * Revision 1.3  2006/09/12 14:56:55  dreyer
 * ADD bidirectional term iterator template
 *
 * Revision 1.2  2006/09/08 16:15:27  dreyer
 * ADD: Added ordering-dependent term iteration
 *
 * Revision 1.1  2006/09/08 14:31:39  dreyer
 * ADD: COrderedIter and infrastructure for order-dependent iterator
 *
 * @endverbatim
**/
//*****************************************************************************


// include basic definitions
#include "pbori_defs.h"

 
#include "BoolePolynomial.h"
#include "BoolePolyRing.h"
#include "OrderedManager.h"
#include "CDelayedTermIter.h"
#include "CBidirectTermIter.h"
#include <algorithm>

#include "CGenericIter.h"
  //#include "CIndirectIter.h"

#ifndef COrderedIter_h_
#define COrderedIter_h_

BEGIN_NAMESPACE_PBORI


template <class MonomType>
class CAbstractTermIter {

public:

  /// Fix type for monomials
  typedef MonomType monom_type;

  /// Extract plain Boolean type
  typedef typename monom_type::bool_type bool_type;

  /// @name Interface types for standard iterator access
  //@{
  typedef monom_type value_type;
  typedef std::forward_iterator_tag iterator_category;
  //  typedef typename iterator::difference_type difference_type;
  typedef void pointer;
  typedef value_type reference;
  //@}

  /// Generic access to type of *this
  typedef CAbstractTermIter<monom_type> self;

  // Default Constructor
  CAbstractTermIter() {}

  // Destructor
  ~CAbstractTermIter() {}

  /// Constant dereference operator
  virtual reference operator*() const = 0;

  /// Prefix increment operator
  virtual self& operator++() = 0;

  /// Postfix increment operator
  virtual self operator++(int) = 0;

  /// Equality test
  virtual bool_type operator!=(const self&) const = 0;

  /// Inequality test
  virtual bool_type operator==(const self&) const = 0;
};

#if 0
template <class IteratorType>
class CIndirectIter {

public:

  /// Fix type of direct iterator
  typedef IteratorType iterator_type;
 
  // Store shared pointer of iterator
  typedef PBORI_SHARED_PTR(iterator_type) iterator_pointer;

  /// Extract plain Boolean type
  typedef typename iterator_type::bool_type bool_type;

  /// @name Interface types for standard iterator access
  //@{
  typedef typename iterator_type::value_type value_type;
  typedef typename iterator_type::iterator_category iterator_category;
  //  typedef typename iterator::difference_type difference_type;
  typedef typename iterator_type::pointer pointer;
  typedef typename iterator_type::reference reference;
  //@}

  /// Generic access to type of *this
  typedef CIndirectIter<iterator_type> self;

  // Default Constructor
  CIndirectIter() {}

  // Destructor
  ~CIndirectIter() {}

  /// Constant dereference operator
  reference operator*() const { return **p_iter; }

  /// Prefix increment operator
  self& operator++()  { 
    ++(*p_iter); 
    return *this;
  }

  /// Postfix increment operator
  self operator++(int) {  return (*p_iter)++;  }

  /// Equality test
  bool_type operator !=(const self& rhs) const {
    return (*p_iter) != (*rhs.p_iter);
  }

  /// Inequality test
  bool_type operator==(const self& rhs) const  {
    return (*p_iter) == (*rhs.p_iter);
  }

protected:
  iterator_pointer p_iter;
};
#endif


template <class IteratorType, class MonomType>
class CIndirectIter:
  public boost::iterator_facade<
  CIndirectIter<IteratorType, MonomType>,
  MonomType, boost::forward_traversal_tag, MonomType
  > {

public:


  typedef CAbstractIterCore<IteratorType, MonomType> iterator_core;


  /// Fix type of direct iterator
  typedef IteratorType iterator_type;
 
  // Store shared pointer of iterator
  typedef PBORI_SHARED_PTR(iterator_core) core_pointer;

  /// Extract plain Boolean type
  typedef typename iterator_type::bool_type bool_type;

  // Default Constructor

  CIndirectIter(core_pointer rhs): p_iter(rhs) {}

  // Destructor
  ~CIndirectIter() {}

  template <class RhsType>
  bool equal(const RhsType& rhs) const { 
    return  p_iter->equality(rhs.p_iter->m_iter); }

  /// Incrementation
  void increment() { p_iter->increment(); }

  /// Dereferencing operation
  MonomType dereference() const { return p_iter->dereference(); }

protected:
  core_pointer p_iter;
};

#if 0
template<class MonomType>
class COrderedIter {

public:
  //  typedef BoolePolyRing::manager_type 

  typedef OrderedManagerBase<Cudd> manager_type;

  /// Fix type for polynomials
  typedef BoolePolynomial poly_type;

  /// Fix type for sizes
  typedef typename poly_type::size_type size_type;

  /// Fix type for Boolean values
  typedef typename poly_type::bool_type bool_type;

  /// Fix type for decision diagram navigations
  typedef typename poly_type::navigator navigator;

  /// Fix type for monomials
  // typedef  poly_type::monom_type monom_type;
  typedef MonomType monom_type;

  /// Set type for terms
  typedef monom_type term_type;

  /// Fix type for (bidirectional) iterating over terms
  typedef poly_type::bidirectional_iterator iterator;

  /// @name Interface types for standard iterator access
  //@{
  typedef term_type value_type;
  typedef std::forward_iterator_tag iterator_category;
  typedef typename iterator::difference_type difference_type;
  typedef void pointer;
  typedef value_type reference;
  //@}

  /// Get type, this is inherited from
  typedef CDelayedTermIter<monom_type, 
                           change_assign<monom_type>, project_ith<2>, 
                           iterator> delayed_term_iterator;

  /// Generic access to type of *this
  typedef COrderedIter self;

  // Constructor
  COrderedIter(const poly_type& poly): 
    m_mgr(BoolePolyRing::activeManager()),
    m_iter(BoolePolyRing::activeManager().leadIterator(poly)), 
    m_poly(poly) {

  }
  // Constructor
  COrderedIter(): 
    m_mgr(BoolePolyRing::activeManager()),
    m_iter(), m_poly() {

  }
  /// Constant dereference operator
  reference operator*() const {
    return delayed_term_iterator(m_iter).term();
  }

  /// Prefix increment operator
  self& operator++() {
    m_iter = m_mgr.incrementIterator(m_iter, m_poly);
    return *this; 
  }

  self operator++(int) {
    self result(*this);
    operator++();
    return result;
  }


  bool_type operator!=(const self& rhs) const {
    return (m_iter != rhs.m_iter);
  }

  bool_type operator==(const self& rhs) const {
    return (m_iter == rhs.m_iter);
  }

private:
  iterator m_iter;
  poly_type m_poly;
  const manager_type& m_mgr;
};
#endif


END_NAMESPACE_PBORI

#endif
