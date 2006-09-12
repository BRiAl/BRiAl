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

#ifndef COrderedIter_h_
#define COrderedIter_h_

BEGIN_NAMESPACE_PBORI

class COrderedIter {

public:
  //  typedef BoolePolyRing::manager_type 

  typedef OrderedManagerBase<Cudd> manager_type;

  /// Fix type for polynomials
  typedef BoolePolynomial poly_type;

  /// Fix type for sizes
  typedef  poly_type::size_type size_type;

  /// Fix type for Boolean values
  typedef  poly_type::bool_type bool_type;

  /// Fix type for decision diagram navigations
  typedef  poly_type::navigator navigator;

  /// Fix type for monomials
  typedef  poly_type::monom_type monom_type;

  /// Set type for terms
  typedef monom_type term_type;

  /// Fix type for (bidirectional) iterating over terms
  typedef poly_type::bidirectional_iterator iterator;

  /// @name Interface types for standard iterator access
  //@{
  typedef term_type value_type;
  typedef std::forward_iterator_tag iterator_category;
  typedef  iterator::difference_type difference_type;
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



END_NAMESPACE_PBORI

#endif
