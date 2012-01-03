// -*- c++ -*-
//*****************************************************************************
/** @file PairData.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30 
 *
 * This file includes the definition of the class @c PairData.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_PairData_h_
#define polybori_groebner_PairData_h_

// include basic definitions

#include "groebner_defs.h"
#include "PolyEntry.h"

#include <stdexcept>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

BEGIN_NAMESPACE_PBORIGB


template <class CompareType>
inline Exponent
min_exponent(const MonomialSet& ms, const CompareType& comp) {
  
  return *(std::min_element(ms.expBegin(), ms.expEnd(), comp));
}

template <>
inline Exponent
min_exponent(const MonomialSet& ms, const lex_tag&) {

  return *ms.expBegin();
}
template <class CompareType>
inline Monomial
min_monomial(const MonomialSet& ms, const CompareType& comp) {
  
  return *(std::min_element(ms.begin(), ms.end(), comp));
}

template <>
inline Monomial
min_monomial(const MonomialSet& ms, const lex_tag&) {

  return *ms.begin();
}

typedef Monomial::idx_map_type lm2Index_map_type;
typedef Exponent::idx_map_type exp2Index_map_type;

class PolyEntryIndexException { };

class PolyEntryVector {

  typedef std::vector<PolyEntry> data_type;
public:
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

  template <class CompareType>
  inline int min_exp_index(const MonomialSet& ms,
			   const CompareType& comp) const {
    if (ms.isZero())
      return -1;
    
    return exp2Index.find(min_exponent(ms, comp))->second;
  }

  /// Consistently insert element
  template <class ElementType>
  void push_back(const ElementType& element) {
    m_data.push_back(element);
    exp2Index[back().leadExp] = lm2Index[back().lead] = size() - 1;
  }

  template <class KeyType>
  const_reference operator[](const KeyType& rhs) const {
    return m_data[index(rhs)];
  }

  template <class KeyType>	// TODO: remove
  data_type::reference operator[](const KeyType& rhs) {
    return m_data[index(rhs)];
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

  size_type save_index(const Monomial& rhs) const {
    return get_save_index(lm2Index, rhs);
  }

  size_type save_index(const Exponent& rhs) const {
    return get_save_index(exp2Index, rhs);
  }

  template <class KeyType>
  const_reference get(const KeyType& rhs) const {
    return m_data[save_index(rhs)];
  }

protected:

  template <class MapType, class KeyType>
  size_type get_index(const MapType& map, const KeyType& key) const {

    typename MapType::const_iterator result(map.find(key));
    PBORI_ASSERT(result != map.end());
    return result->second;
  }

  template <class MapType, class KeyType>
  size_type get_save_index(const MapType& map, const KeyType& key) const {

    typename MapType::const_iterator result(map.find(key));
    if (result != map.end())
      throw PolyEntryIndexException();

    return result->second;
  }

private:
  data_type m_data;
  lm2Index_map_type lm2Index;
  exp2Index_map_type exp2Index;
};

/** @class PairData
 * @brief This class defines PairData.
 *
 **/
class PairData{
public:
  //gives back encoded
  virtual ~PairData() {}
  //we don't demand that the pair is in a consistent state later
  virtual Polynomial extract(const PolyEntryVector& v)=0;
};

END_NAMESPACE_PBORIGB

#endif /* polybori_PairData_h_ */
