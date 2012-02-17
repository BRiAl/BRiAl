// -*- c++ -*-
//*****************************************************************************
/** @file PolyEntryIndices.h 
 *
 * @author Michael Brickenstein (original) and Alexander Dreyer (refactored)
 * @date 2012-01-05
 *
 * This file includes the definition of the class @c PolyEntryIndices.
 *
 * @par Copyright:
 *   (c) 2006-2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_PolyEntryIndices_h_
#define polybori_groebner_PolyEntryIndices_h_

// include basic definitions
#include "groebner_defs.h"
#include "PolyEntry.h"

BEGIN_NAMESPACE_PBORIGB

typedef Monomial::idx_map_type lm2Index_map_type;
typedef Exponent::idx_map_type exp2Index_map_type;

/** @class PolyEntryIndices
 * @brief This class defines PolyEntryIndices.
 *
 **/

class PolyEntryIndices {

  class check {};
  class uncheck {};

public:
  typedef Monomial::idx_map_type lm2Index_map_type;
  typedef Exponent::idx_map_type exp2Index_map_type;
  typedef lm2Index_map_type::value_type value_type;
  typedef value_type::second_type data_type;

  PolyEntryIndices():
    lm2Index(), exp2Index() {}

  /// Consistently inserting element @todo for some reason not inlinable
  void insert(const PolyEntry& entry, const data_type& rhs) {
    exp2Index[entry.leadExp] = rhs;
    lm2Index[entry.lead] = rhs;
  }

  /// Consistently replacing keys
  void update(const Monomial& key, const PolyEntry& entry) {

    if PBORI_UNLIKELY(entry.lead != key) {
      lm2Index_map_type::iterator lm_pos = lm2Index.find(key);
      PBORI_ASSERT(lm_pos != lm2Index.end());
      lm2Index[entry.lead] = lm_pos->second;
      exp2Index[entry.leadExp] = lm_pos->second;
      lm2Index.erase(lm_pos);
      exp2Index.erase(exp2Index.find(key.exp()));
    }
  }

  /// Lookup indices @note could fail for invalid keys
  template <class KeyType>
  data_type operator()(const KeyType& key) const {
    return get(key, uncheck());
  }

  /// Savely looking up indices, returns -1 if key is invalid
  template <class KeyType>
  data_type checked(const KeyType& key) const {
    return get(key, check());
  }

protected:


  template <class CheckType>
  data_type get(data_type key, CheckType) const { return key; }

  template <class CheckType>
  data_type get(const Exponent& key, CheckType dummy) const {
    return get(exp2Index, key, dummy);
  }

  template <class CheckType>
  data_type get(const Monomial& key, CheckType dummy) const {
    return get(lm2Index, key, dummy);
  }

  template <class CheckType>
  data_type get(const PolyEntry& key, CheckType dummy) const {
    return get(lm2Index, key.lead, dummy);
  }

  template <class MapType, class KeyType>
  data_type get(const MapType& map, const KeyType& key, check) const {

    typename MapType::const_iterator result(map.find(key));

    if (result == map.end())
      return (data_type)-1;

    PBORI_ASSERT(result->second != data_type(-1));
    return result->second;
  }

  template <class MapType, class KeyType>
  data_type get(const MapType& map, const KeyType& key, uncheck) const {

    typename MapType::const_iterator result(map.find(key));
    PBORI_ASSERT(result != map.end());
    return result->second;
  }

private:
  lm2Index_map_type lm2Index;
  exp2Index_map_type exp2Index;
};


END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_PolyEntryIndices_h_ */
