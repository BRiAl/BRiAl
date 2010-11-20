// -*- c++ -*-
//*****************************************************************************
/** @file CVariableIter.h
 *
 * @author Alexander Dreyer
 * @date 2008-03-05
 *
 * This file defines an stl-like iterator for accessing the variables of monomial.
 *
 * @par Copyright:
 *   (c) 2008 by The PolyBoRi Team
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.3  2008/05/26 12:06:39  dreyer
 * ADD: isEnd() as end of iteration check, sing iterator_facade
 *
 * Revision 1.2  2008/03/06 09:18:53  bricken
 * + implement postincrement correctly
 *
 * Revision 1.1  2008/03/05 16:23:37  dreyer
 * CHANGE: BooleMonomial::variableBegin()|End(); monom/monom = 0 throws
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

#ifndef CVariableIter_h_
#define CVariableIter_h_

BEGIN_NAMESPACE_PBORI

/** @class CVariableIter
 * @brief This template class defines an iterator for monomial types.
 *
 * It uses a given iterator, and converts indices to Boolean variables (via a
 * given ring).
 **/


template <class Iterator, class VariableType> 
class CVariableIter : 
  public boost::iterator_facade<
  CVariableIter<Iterator, VariableType>,
  VariableType,
  typename Iterator::iterator_category,
  VariableType
  > {

public:
  /// Fixing Iterator type to be extended
  typedef Iterator iterator_type;

  /// Fixing variables, which is used for dereferencing
  typedef VariableType var_type;

  /// Fixing ring, which is used to generate variables
  typedef typename var_type::ring_type ring_type;

  /// Get type of *this
  typedef CVariableIter<iterator_type, var_type> self;

  /// Default constructor
  CVariableIter(): m_iter(), m_ring() {}

  /// Constructor for given iterator (and ring)
  CVariableIter(const iterator_type& rhs, const ring_type& ring): 
    m_iter(rhs), m_ring(ring) {}

  /// Check, whether end of iteration is reached
  bool isEnd() const { return m_iter.isEnd(); }

  /// Increment operation
  void increment() { ++m_iter; }

  /// Constant dereference operator
  var_type dereference() const {  return var_type(*m_iter, m_ring); }

  /// Equality check
  bool equal(const self& rhs) const { return  m_iter == rhs.m_iter; }

private:
  /// The actual iterator
  iterator_type m_iter;

  /// The ring, we take the variables from
  ring_type m_ring;
};

END_NAMESPACE_PBORI


#endif // CVariableIter_h_
