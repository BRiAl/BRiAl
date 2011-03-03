// -*- c++ -*-
//*****************************************************************************
/** @file pbori_routines_hash.h
 *
 * @author Alexander Dreyer
 * @date 2006-08-23
 *
 * This file includes files, which defines miscellaneous function templates.
 *
 * @par Copyright:
 *   (c) 2006 by The PolyBoRi Team
**/
//*****************************************************************************

#ifndef pbori_routines_hash_h_
#define pbori_routines_hash_h_
// include basic definitions
#include "polybori/pbori_defs.h"
#include <boost/functional/hash.hpp>


BEGIN_NAMESPACE_PBORI



template <class HashType, class NaviType>
void
stable_hash_range(HashType& seed, NaviType navi) {

  if (navi.isConstant()) {
    if (navi.terminalValue())
      boost::hash_combine(seed, CTypes::max_index());
    return;
  }

  boost::hash_combine(seed, *navi);

  stable_hash_range(seed, navi.thenBranch());
  stable_hash_range(seed, navi.elseBranch());
}

template <class NaviType>
std::size_t
stable_hash_range(NaviType navi) {

  std::size_t seed = 0;
  stable_hash_range(seed, navi);

  return seed;
}

template <class HashType>
void
finalize_term_hash(HashType& seed) {
  boost::hash_combine(seed, CTypes::max_index());
}

template <class HashType, class NaviType>
void
stable_first_hash_range(HashType& seed, NaviType navi) {

  while (!navi.isConstant()) {
    boost::hash_combine(seed, *navi);
    navi.incrementThen();
  }
  if (navi.terminalValue())
    finalize_term_hash(seed);

}

template <class NaviType>
std::size_t
stable_first_hash_range(NaviType navi) {

  std::size_t seed = 0;
  stable_first_hash_range(seed, navi);

  return seed;
}

template <class HashType, class Iterator>
void
stable_term_hash(HashType& seed, Iterator start, Iterator finish) {
  boost::hash_range(seed, start, finish);
  finalize_term_hash(seed);
}

template <class Iterator>
std::size_t
stable_term_hash(Iterator start, Iterator finish) {

  std::size_t seed(0);
  stable_term_hash(seed, start, finish);

  return seed;
}


// The following may be used without polybori. Hence, we have to load it in the
// namespace here

// Get generic hash functions
// #include "polybori/generic_hash.h"

// #ifndef PBORI_HASH_TAG
// #define PBORI_HASH_TAG js_tag
// #endif


// typedef generic_hash_tags::PBORI_HASH_TAG pbori_hash_tag;

END_NAMESPACE_PBORI

#endif
