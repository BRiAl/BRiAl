// -*- c++ -*-
//*****************************************************************************
/** @file PolyEntryVector.h 
 *
 * @author Alexander Dreyer
 * @date 2012-01-04
 *
 * This file includes the definition of the class @c PolyEntryVector.
 *
 * @par Copyright:
 *   (c) 2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_PolyEntryVector_h_
#define polybori_groebner_PolyEntryVector_h_

#include "PolyEntryReference.h"
#include "PolyEntryIndices.h"
#include "PolyEntry.h"

// include basic definitions
#include "groebner_defs.h"
#include <stdexcept>

BEGIN_NAMESPACE_PBORIGB

class PolyEntryVector {

  typedef std::vector<PolyEntry> data_type;
public:
  typedef data_type::value_type value_type;
  typedef data_type::size_type size_type;
  typedef data_type::const_iterator const_iterator;
  typedef data_type::const_reference const_reference;
  typedef PolyEntryReference reference;

  size_type size() const { return m_data.size(); }
  const_iterator begin() const { return m_data.begin(); }
  const_iterator end() const { return m_data.end(); }
  const_reference front() const { return m_data.front(); }
  const_reference back() const { return m_data.back(); }

  PolyEntryVector():
    m_data(), m_indices() {}

  /// Just insert element
  template <class ElementType>
  void append(const ElementType& element) {
    m_data.push_back(element);

    PBORI_ASSERT(m_indices.checked(back().lead) == (size_type)-1);
    m_indices.insert(back(), size() - 1);
  }

public: 
  template <class KeyType>
  const_reference operator[](const KeyType& rhs) const { return access(rhs); }

  template <class KeyType>
  const_reference access(const KeyType& rhs) const {return m_data[index(rhs)];}

  template <class KeyType>
  reference access(const KeyType& rhs) { 
    return reference(m_data[index(rhs)], m_indices);
  }

  template <class KeyType, class Type>
  void exchange(const KeyType& key, const Type& rhs) {
    update(m_data[index(key)], rhs);
  }

  template <class KeyType>
  size_type index(const KeyType& rhs) const {
    return m_indices(rhs);
  }

  template <class KeyType>
  size_type checked_index(const KeyType& rhs) const {
    return m_indices.checked(rhs);
  }

private:
  template <class Type>
  void update(PolyEntry& entry, const Type& rhs) {
    Monomial lm(entry.lead);
    entry = rhs;
    m_indices.update(lm, entry);
  }

  data_type m_data;
  PolyEntryIndices m_indices;
};



END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_PolyEntryVector_h_ */
