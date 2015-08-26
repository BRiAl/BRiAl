// -*- c++ -*-
//*****************************************************************************
/** @file BooleSetSequence.h 
 *
 * @author Alexander Dreyer
 * @date 2012-01-19
 *
 * This file includes the definition of the class @c BooleSetSequence.
 *
 * @par Copyright:
 *   (c) 2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_BooleSetSequence_h_
#define polybori_BooleSetSequence_h_

// include basic definitions
#include <polybori/pbori_defs.h>

#include <vector>
#include <algorithm>

#include <polybori/BooleSet.h>

BEGIN_NAMESPACE_PBORI

/** @class BooleSetSequence
 * @brief This class converts a @c BooleSet to a sequence-like structure.
 *
 **/
class BooleSetSequence {

public:
  typedef std::vector<BooleSet> vector_type;
  typedef vector_type::const_iterator const_iterator;
  typedef vector_type::const_reverse_iterator const_reverse_iterator;

  /// Standard constructor yields an empty set
  BooleSetSequence():  m_termsets() { }


  /// Construct sequence from a set of terms and an unary conversion function
  template <class SetType, class Operator>
  BooleSetSequence(const SetType& terms, const Operator& func):
    m_termsets(terms.size(), terms.ring()) {
    std::transform(terms.begin(), terms.end(), m_termsets.begin(), func);
  }
  
  /// Sequence-like interface
  //@{
  const_iterator begin() const { return m_termsets.begin(); }
  const_iterator end() const { return m_termsets.end(); }
  const_reverse_iterator rbegin() const { return m_termsets.rbegin(); }
  const_reverse_iterator rend() const { return m_termsets.rend(); }
  bool empty() const { return m_termsets.empty(); }
  //@}

private:
  vector_type m_termsets;
};


END_NAMESPACE_PBORI

#endif /* polybori_BooleSetSequence_h_ */
