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
#include "PolyEntry.h"

// include basic definitions
#include "groebner_defs.h"
#include <stdexcept>

BEGIN_NAMESPACE_PBORIGB

typedef Monomial::idx_map_type lm2Index_map_type;
typedef Exponent::idx_map_type exp2Index_map_type;

class PolyEntryVector {

  class check {};
  class uncheck {};

public:
  typedef std::vector<PolyEntry> data_type;
  typedef data_type::size_type size_type;
  typedef data_type::value_type value_type;
  typedef data_type::const_iterator const_iterator;
  typedef data_type::const_reference const_reference;

  size_type size() const { return m_data.size(); }
  const_iterator begin() const { return m_data.begin(); }
  const_iterator end() const { return m_data.end(); }
  const_reference front() const { return m_data.front(); }
  const_reference back() const { return m_data.back(); }
  data_type::reference back()  { return m_data.back(); } // TODO remove

  PolyEntryVector():
    m_data(), lm2Index(), exp2Index() {}

  /// Consistently insert element
  template <class ElementType>
  void push_back(const ElementType& element) {
    m_data.push_back(element);
    exp2Index[back().leadExp] = lm2Index[back().lead] = size() - 1;
  }

  template <class KeyType>
  const_reference operator[](const KeyType& rhs) const { return access(rhs); }

  template <class KeyType>
  const_reference access(const KeyType& rhs) const {return m_data[index(rhs)];}

  template <class KeyType>
  PolyEntryReference access(const KeyType& rhs) { return m_data[index(rhs)]; }

  template <class Type>
  void exchange(size_type idx, const Type& rhs) {
    PolyEntry& entry = m_data[idx];
    Monomial lm = entry.lead;
    entry = rhs;
    updateMaps(entry, lm);
  }

  template <class KeyType>
  size_type index(const KeyType& rhs) const {
    return index(rhs, uncheck());
  }

  template <class KeyType>
  size_type checked_index(const KeyType& rhs) const {
    return index(rhs, check());
  }

protected:

  void updateMaps(const PolyEntry& entry, const Monomial& oldlead) {

    if UNLIKELY(entry.lead != oldlead) {

      lm2Index_map_type::iterator lm_pos = lm2Index.find(entry.lead);
      exp2Index_map_type::iterator exp_pos = exp2Index.find(entry.leadExp);

      PBORI_ASSERT(&(m_data[lm_pos->second]) == &entry);

      PBORI_ASSERT(lm_pos != lm2Index.end());
      PBORI_ASSERT(exp_pos != exp2Index.end());

      PBORI_ASSERT(lm2Index.find(entry.lead) == lm2Index.end());
      PBORI_ASSERT(exp2Index.find(entry.leadExp) == exp2Index.end());
      exp2Index[entry.leadExp] = lm2Index[entry.lead] = lm_pos->second;

      lm2Index.erase(lm_pos);
      exp2Index.erase(exp_pos);
    }
  }

  template <class CheckType>
  size_type index(size_type rhs, CheckType) const { return rhs; }

  template <class CheckType>
  size_type index(const Exponent& key, CheckType dummy) const {
    return index(exp2Index, key, dummy);
  }
  template <class CheckType>
  size_type index(const Monomial& key, CheckType dummy) const {
    return index(lm2Index, key, dummy);
  }

  template <class MapType, class KeyType>
  size_type index(const MapType& map, const KeyType& key, check) const {
    typename MapType::const_iterator result(map.find(key));
    if (result != map.end())
      return (size_type)-1;
    
    PBORI_ASSERT(result->second != size_type(-1));
    return result->second;
  }

  template <class MapType, class KeyType>
  size_type index(const MapType& map, const KeyType& key, uncheck) const {
    typename MapType::const_iterator result(map.find(key));
    PBORI_ASSERT(result != map.end());
    return result->second;
  }

private:
  data_type m_data;
  lm2Index_map_type lm2Index;
  exp2Index_map_type exp2Index;
};



END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_PolyEntryVector_h_ */
