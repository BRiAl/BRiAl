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
#include <stdexcept>
#include "groebner_defs.h"
#include "PolyEntry.h"

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

class PolyEntryVector:
  public std::vector<PolyEntry> {

  typedef std::vector<PolyEntry> base;
public:

  //friend class CountCriterion;  // temporarily
  //  friend class ChainCriterion;  // temporarily
  // friend class ChainVariableCriterion;  // temporarily
  //  friend class HasTRepOrExtendedProductCriterion; // temporarily
  //  friend class GroebnerStrategy; // temporarily

  PolyEntryVector():
    base(), lm2Index(), exp2Index() {}

  template <class Iterator>
  PolyEntryVector(Iterator start, Iterator finish):
    base(start, finish), lm2Index(), exp2Index() {

    const_iterator iter(begin()), iend(end());
    size_type idx(0);
    while (iter !=iend) {
      lm2Index[iter->lead] = exp2Index[iter->leadExp] = idx;
      ++start; ++idx;
    }

  }

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
    base::push_back(element);
    exp2Index[back().leadExp] = lm2Index[back().lead] = size() - 1;
  }


  base::const_reference operator[](const Monomial& rhs) const {
    return base::operator[](index(rhs));
  }

  base::reference operator[](const Monomial& rhs) {
    return base::operator[](index(rhs));
  }

  base::const_reference operator[](const Exponent& rhs) const {
    return base::operator[](index(rhs));
  }

  base::reference operator[](const Exponent& rhs) {
    return base::operator[](index(rhs));
  }

  using base::operator[];

  const base::size_type& index(const base::size_type& rhs) const {
    return rhs;
  }

  base::size_type index(const Monomial& rhs) const {
    return get_index(lm2Index, rhs);
  }

  base::size_type index(const Exponent& rhs) const {
    return get_index(exp2Index, rhs);
  }

  base::size_type save_index(const Monomial& rhs) const {
    return get_save_index(lm2Index, rhs);
  }

  base::size_type save_index(const Exponent& rhs) const {
    return get_save_index(exp2Index, rhs);
  }

  template <class KeyType>
  base::const_reference get(const KeyType& rhs) const {
    return base::operator[](save_index(rhs));
  }

protected:

  template <class MapType, class KeyType>
  base::size_type get_index(const MapType& map, const KeyType& key) const {

    typename MapType::const_iterator result(map.find(key));
    PBORI_ASSERT(result != map.end());
    return result->second;
  }

  template <class MapType, class KeyType>
  base::size_type get_save_index(const MapType& map, const KeyType& key) const {

    typename MapType::const_iterator result(map.find(key));
    if (result != map.end())
      throw PolyEntryIndexException();

    return result->second;
  }

private:

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
