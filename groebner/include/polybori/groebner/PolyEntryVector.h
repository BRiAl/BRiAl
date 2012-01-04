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


template <class CompareType>
inline MonomialSet::exp_iterator
min_exponent(const MonomialSet& ms, const CompareType& comp) {
  if (ms.isZero())
    return ms.expEnd();
  return std::min_element(ms.expBegin(), ms.expEnd(), comp);
}

template <>
inline MonomialSet::exp_iterator
min_exponent(const MonomialSet& ms, const lex_tag&) {
  return ms.expBegin();
}
template <class CompareType>
inline MonomialSet::const_iterator
min_monomial(const MonomialSet& ms, const CompareType& comp) {
    if (ms.isZero())
      return ms.end();

  return std::min_element(ms.begin(), ms.end(), comp);
}

template <>
inline MonomialSet::const_iterator
min_monomial(const MonomialSet& ms, const lex_tag&) {
  return ms.begin();
}


typedef Monomial::idx_map_type lm2Index_map_type;
typedef Exponent::idx_map_type exp2Index_map_type;





class PolyEntryIndexException { };

class PolyEntryVector {

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

  template <class CompareType>
  inline int min_lm_index(const MonomialSet& ms,
			 const CompareType& comp) const {
    if (ms.isZero())
      return -1;
    
    return lm2Index.find(min_monomial(ms, comp))->second;
  }
  /*
  template <class CompareType>
  inline int min_exp_index(const MonomialSet& ms,
			   const CompareType& comp) const {
    if (ms.isZero())
      return -1;
    
    return exp2Index.find(min_exponent(ms, comp))->second;
  }
  */
  /// Consistently insert element
  template <class ElementType>
  void push_back(const ElementType& element) {
    m_data.push_back(element);
    exp2Index[back().leadExp] = lm2Index[back().lead] = size() - 1;
  }

  template <class KeyType>
  const_reference operator[](const KeyType& rhs) const { return access(rhs); }


  template <class KeyType>
  const_reference access(const KeyType& rhs) const { return m_data[index(rhs)]; }

  template <class KeyType>
  PolyEntryReference access(const KeyType& rhs) {
    return m_data[index(rhs)];
  }


  void exchange(size_type idx, const PolyEntry& rhs) {
    PolyEntry& entry = m_data[idx];
    Monomial lm = entry.lead;
    entry = rhs;
    updateMaps(entry, lm);
  }

  void exchange(size_type idx, const Polynomial& rhs) {
    PolyEntry& entry = m_data[idx];
    Monomial lm = entry.lead;
    entry.p = rhs;
    entry.recomputeInformation();
    updateMaps(entry, lm);
  }

  const size_type& index(const size_type& rhs) const {
    return rhs;
  }

  size_type index(const Monomial& rhs) const {
    return get_index(lm2Index, rhs);
  }

  size_type index(const Exponent& rhs) const {
    return get_index(exp2Index, rhs);
  }
  /*
  size_type save_index(const Monomial& rhs) const {
    return get_save_index(lm2Index, rhs);
  }

  size_type save_index(const Exponent& rhs) const {
    return get_save_index(exp2Index, rhs);
  }
  */

  int checked_index(const Monomial& rhs) const {
    return get_checked_index(lm2Index, rhs);
  }

  int checked_index(const Exponent& rhs) const {
    return get_checked_index(exp2Index, rhs);
  }
  /*
  template <class KeyType>
  const_reference get(const KeyType& rhs) const {
    return m_data[save_index(rhs)];
  }
  */

  template <class Iterator, class CompareType>
  size_type min_index(Iterator start, Iterator finish, const CompareType& comp)
    const {
    start = std::min_element(start, finish, comp);
    if UNLIKELY(start == finish)
      return size_type(-1);
    
    return index(*start);
  }

  template <class SetType, class CompareType>
  size_type min_index(const SetType& terms, const CompareType& comp) const {
    return min_index(terms.begin(), terms.end(), comp);
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

  template <class MapType, class KeyType>
  size_type get_index(const MapType& map, const KeyType& key) const {

    typename MapType::const_iterator result(map.find(key));
    PBORI_ASSERT(result != map.end());
    return result->second;
  }

  template <class MapType, class KeyType>
  size_type get_save_index(const MapType& map, const KeyType& key) const {

    size_type result(get_checked_index(map, key));
    if (result == (size_type)-1)
      throw PolyEntryIndexException();

    return result;
  }

  template <class MapType, class KeyType>
  size_type get_checked_index(const MapType& map, const KeyType& key) const {

    typename MapType::const_iterator result(map.find(key));
    if (result != map.end())
      return (size_type)-1;

    PBORI_ASSERT(result->second != size_type(-1));
    return result->second;
  }

private:
  data_type m_data;
  lm2Index_map_type lm2Index;
  exp2Index_map_type exp2Index;
};



END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_PolyEntryVector_h_ */
