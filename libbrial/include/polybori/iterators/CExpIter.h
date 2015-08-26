// -*- c++ -*-
//*****************************************************************************
/** @file CExpIter.h
 *
 * @author Alexander Dreyer
 * @date 2007-05-03
 *
 * This file defines a variant for the natural (lexicographical) iterator, which
 * iterator over all exponent vectors of a polynomial.
 *
 * @par Copyright:
 *   (c) 2007 by The PolyBoRi Team
**/
//*****************************************************************************

#ifndef polybori_iterators_CExpIter_h_
#define polybori_iterators_CExpIter_h_

// include basic definitions
#include <polybori/pbori_defs.h>

// get stuff for term iteration
#include "CTermStack.h"
#include "CTermIter.h"

BEGIN_NAMESPACE_PBORI


template <class ExpType>
class CExpGenerator {

public:
  typedef ExpType value_type;
  typedef const value_type& result_type;
  typedef typename value_type::size_type size_type;

  /// Default constructor
  CExpGenerator(): m_result() {}

  /// Return currently stored results
  template <class SequenceType>
  result_type operator()(const SequenceType&) const{
    return m_result;
  }

  /// Take the first nlen elements of the exponent vector only
  void resize(size_type nlen) { m_result.resize(nlen); }

  /// Prepare space for nlen elements
  void reserve(size_type nlen) { m_result.reserve(nlen); }

  /// Get current size
  size_type size() const { return m_result.size(); }

  /// Append elements to exponent vector
  template <class Iterator>
  void append(Iterator start, Iterator finish) { 
    while (start != finish){
      m_result.push_back(*start);
      ++start;
    }
  }

private:
  value_type m_result;
};


template <class NaviType, class ExpType>
struct pbori_base<CExpIter<NaviType, ExpType> > {

  typedef CTermStack<NaviType, std::forward_iterator_tag> stack_type;
  typedef CTermIter<stack_type, CExpGenerator<ExpType> > type;
};

template <class NaviType, class ExpType>
class CExpIter : 
  public pbori_base<CExpIter<NaviType, ExpType> >::type {

public:
  /// Name type of *this
  typedef CExpIter<NaviType, ExpType> self;

  /// Get base type
  typedef typename pbori_base<self>::type base;

  /// Construct iteraor from navigator over decision diagram structure
  CExpIter(NaviType navi): base(navi, typename base::term_generator() ) {
    base::m_getTerm.reserve(base::m_stack.size());
    base::m_getTerm.append(base::begin(), base::end()); 
  }
  

  /// Incrementation operation core
  void increment() { 
    PBORI_ASSERT(!base::m_stack.empty());
    if (base::m_stack.markedOne()) {
      base::m_stack.clearOne();
    }
    else {
      base::m_stack.next();
      base::m_getTerm.resize( base::m_stack.size() == 0 ?
                              0: 
                              base::m_stack.size() - 1);

      if (!base::m_stack.empty()) {
        base::m_stack.followThen();
        base::m_stack.terminate();
     }
    }
    base::m_getTerm.reserve(base::m_stack.size());
    base::m_getTerm.append(base::begin() + base::m_getTerm.size(), base::end());
  }

  /// Prefix incrementation operation
  self& operator++() {
    increment();
    return *this;
  }
  /// Postfix incrementation operation
  self operator++(int) {
    self copy(*this);
    increment();
    return copy;
  }
};

END_NAMESPACE_PBORI

#endif
