// -*- c++ -*-
//*****************************************************************************
/** @file TransformedSequence.h 
 *
 * @author Alexander Dreyer
 * @date 2012-01-19
 *
 * This file includes the definition of the class @c TransformedSequence.
 *
 * @par Copyright:
 *   (c) 2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_TransformedSequence_h_
#define polybori_TransformedSequence_h_

// include basic definitions
#include <polybori/pbori_defs.h>
#include <polybori/iterators/TransformedIterator.h>

#include <vector>
#include <algorithm>

BEGIN_NAMESPACE_PBORI


/** @class TransformedSequence
 * @brief This class converts a sequence-like structure when iterating.
 *
 **/
template <class SequenceType, class OperatorType>
class TransformedSequence {

public:
  typedef SequenceType sequence_type;
  typedef typename sequence_type::const_iterator sequence_iterator;
  typedef OperatorType operator_type;
  typedef TransformedIterator<sequence_iterator, operator_type> const_iterator;


  /// Construct sequence from a set of terms and an unary conversion function
  TransformedSequence(const sequence_type& seq, const operator_type& func):
    m_sequence(seq), m_func(func) {
  }
  
  /// Sequence-like interface
  //@{
  const_iterator begin() const {
    return const_iterator(m_sequence.begin(), m_func); }

  const_iterator end() const {
    return const_iterator(m_sequence.end(), m_func); }
  //@}

private:
  sequence_type m_sequence;
  operator_type m_func;
};


END_NAMESPACE_PBORI

#endif /* polybori_TransformedSequence_h_ */
