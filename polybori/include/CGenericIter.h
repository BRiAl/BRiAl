// -*- c++ -*-
//*****************************************************************************
/** @file CGenericIter.h
 *
 * @author Alexander Dreyer
 * @date 2006-09-06
 *
 * This file defines a degree lexicographic iterator.
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
 * Revision 1.1  2006/10/04 13:09:56  dreyer
 * ADD: added compile-time optimied iterators and genericBegin/genericEnd
 *
 * @endverbatim
**/
//*****************************************************************************


// include basic definitions
#include "pbori_defs.h"
#include "pbori_routines.h"

#include "BoolePolynomial.h"



#ifndef CGenericIter_h_
#define CGenericIter_h_

BEGIN_NAMESPACE_PBORI

template<class OrderType, class PolyType, class DelayedIterType>
class CGenericIter {

public:

   /// Fix type for order
  typedef OrderType order_type;

  /// Fix type for polynomials
  typedef PolyType poly_type;

  /// Fix type for underlying iterator
  typedef DelayedIterType iterator;

  /// Fix type for sizes
  typedef typename poly_type::size_type size_type;

  /// Fix type for Boolean values
  typedef typename poly_type::bool_type bool_type;

  /// Fix type for monomials
  typedef typename poly_type::monom_type monom_type;

  /// Set type for terms
  typedef monom_type term_type;

  /// Get data type for storing additional information
  typedef typename generic_iteration<order_type, iterator>::data_type data_type;

  /// @name Interface types for standard iterator access
  //@{
  typedef term_type value_type;
  typedef std::forward_iterator_tag iterator_category;
  typedef typename iterator::difference_type difference_type;
  typedef void pointer;
  typedef term_type reference;
  //@}

  typedef CDelayedTermIter<monom_type, 
                           change_assign<>, project_ith<2>, 
                           iterator> delayed_iterator;

  /// Generic access to type of *this
  typedef CGenericIter<order_type, poly_type, iterator> self;

  // Constructor
  CGenericIter(const poly_type& poly): 
    m_iter( generic_iteration<order_type, iterator>().leadIterator(poly) ), 
    m_data(poly) {
  }

  // (for end-of-path marker)
  CGenericIter(const poly_type& poly, int): 
    m_iter(poly.endOfNavigation()),  m_data(poly) {
  }

  /// Constant dereference operator
  reference operator*() const {
    return m_iter.term();
  }

  /// Prefix increment operator
  self& operator++() {
    generic_iteration<order_type, iterator>().incrementIterator(m_iter, m_data);
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
  delayed_iterator m_iter; 
  data_type m_data;
};



END_NAMESPACE_PBORI

#endif
