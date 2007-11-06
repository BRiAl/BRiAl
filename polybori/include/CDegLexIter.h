// -*- c++ -*-
//*****************************************************************************
/** @file CDegLexIter.h
 *
 * @author Alexander Dreyer
 * @date 2006-09-06
 *
 * This file defines a degree lexicographic iterator.
 *
 * @par Copyright:
 *   (c) 2006 by The PolyBoRi Team
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.5  2007/11/06 15:03:34  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.4  2007/04/30 15:20:30  dreyer
 * CHANGE: Switching from CTermIter to iterators based on CTermStack
 *
 * Revision 1.3  2006/09/08 14:31:38  dreyer
 * ADD: COrderedIter and infrastructure for order-dependent iterator
 *
 * Revision 1.2  2006/09/08 10:22:59  dreyer
 * FIX: Gcc 4 ist more pedantic
 *
 * Revision 1.1  2006/09/07 16:04:32  dreyer
 * ADD: CDegLexIter.h
 *
 * @endverbatim
**/
//*****************************************************************************


// include basic definitions
#include "pbori_defs.h"


#include "BoolePolynomial.h"
#include "CDelayedTermIter.h"
#include "CRestrictedIter.h"

#include <algorithm>

#ifndef CDegLexIter_h_
#define CDegLexIter_h_

BEGIN_NAMESPACE_PBORI
#if 0
template<class PolyType, class PolyDegIter = typename PolyType::deg_iterator>
class CDegLexIter {

public:

  /// Fix type for polynomials
  typedef PolyType poly_type;

  /// Fix type for sizes
  typedef typename poly_type::size_type size_type;

  /// Fix type for Boolean values
  typedef typename poly_type::bool_type bool_type;

  /// Fix type for monomials
  typedef typename poly_type::monom_type monom_type;

  /// Set type for terms
  typedef monom_type term_type;

  /// Fix type for polynomials
  typedef typename poly_type::deg_iterator deg_iterator;

  /// @name Interface types for standard iterator access
  //@{
  typedef term_type value_type;
  typedef std::forward_iterator_tag iterator_category;
  typedef typename deg_iterator::difference_type difference_type;
  typedef void pointer;
  typedef value_type reference;
  //@}

  /// Get type, this is inherited from
  typedef CDelayedTermIter<monom_type, 
                           change_assign<monom_type>, project_ith<2>, 
                           deg_iterator> delayed_term_iterator;

  typedef CRestrictedIter<delayed_term_iterator> bounded_iterator;

  /// Generic access to type of *this
  typedef CDegLexIter self;

  // Constructor
  CDegLexIter(const delayed_term_iterator& start, 
              const delayed_term_iterator& finish ): 
    m_iter(std::max_element(start, finish)), m_start(start), m_finish(finish) {

  }
  // Default Constructor
  CDegLexIter():  m_iter(), m_start(), m_finish() {}

  /// Constant dereference operator
  reference operator*() const {
    return m_iter.term();
  }

  /// Prefix increment operator
  self& operator++() {
    if (m_iter != m_finish) {
      size_type deg = *m_iter;
      ++m_iter;
      m_iter = std::find(m_iter, m_finish, deg);
      
      if(m_iter == m_finish) {
        m_iter = std::max_element( bounded_iterator(m_start, deg),
                                   bounded_iterator(m_finish, deg) );

      }
    }

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
  delayed_term_iterator m_iter, m_start, m_finish;
};

#endif

END_NAMESPACE_PBORI

#endif
