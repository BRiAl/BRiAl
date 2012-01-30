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

BEGIN_NAMESPACE_PBORIGB

class PolyEntryVector {
  typedef std::vector<PolyEntry> data_type;

public:
  /// Vector-style interface
  //@{
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
  reference back() { return reference(m_data.back(), m_indices); }
  //@}

  /// Default constructor
  PolyEntryVector():
    m_data(), m_indices() {}

  /// Just insert element
  virtual void append(const PolyEntry& element) {
    m_data.push_back(element);

    PBORI_ASSERT(m_indices.checked(back().lead) == (size_type)-1);
    m_indices.insert(back(), size() - 1);
  }

  /// Read-only access to element by index, leading term or monomial
  template <class KeyType>
  const_reference operator[](const KeyType& rhs) const { 
    return operator()(rhs); 
  }

  /// Read-only access to element by index, leading term or monomial
  template <class KeyType>
  const_reference operator()(const KeyType& key) const {
    return m_data[index(key)];
  }

  /// (Partially) write access to element by index, leading term or monomial
  template <class KeyType>
  reference operator()(const KeyType& rhs) {
    return reference(m_data[index(rhs)], m_indices);
  }

  /// Exchange element given by @c key
  template <class KeyType, class Type>
  void exchange(const KeyType& key, const Type& rhs) { operator()(key) = rhs; }

  /// Retrieve index associated to @c key
  template <class KeyType>
  size_type index(const KeyType& key) const { return m_indices(key); }

  /// Retrieve index associated to @c key if @c key exists, -1 otherwise
  template <class KeyType>
  size_type checked_index(const KeyType& key) const {
    return m_indices.checked(key);
  }
  /// Retrieve polynomial associated to @c key
  template <class KeyType>
  const Polynomial& polynomial(const KeyType& key) const {
    return operator[](key).p;
  }

private:
  data_type m_data;
  PolyEntryIndices m_indices;
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_PolyEntryVector_h_ */
