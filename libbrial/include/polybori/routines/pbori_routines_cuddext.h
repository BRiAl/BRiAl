// -*- c++ -*-
//*****************************************************************************
/** @file pbori_routines_cuddext.h
 *
 * @author Alexander Dreyer
 * @date 2006-08-23
 *
 * This file includes files, which define function templates related to
 * decision diagrams. It should be loaded from pbori_routines.h only
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_routines_pbori_routines_cuddext_h_
#define polybori_routines_pbori_routines_cuddext_h_

// include basic definitions
#include <polybori/pbori_defs.h>
#include <map>


BEGIN_NAMESPACE_PBORI

/// Other routines currently at external public header file pbori_algo.h

/// Function template
template<class MapType, class NaviType>
inline typename MapType::mapped_type
dd_long_count_step(MapType& cache, NaviType navi) {

  if(navi.isConstant())
    return navi.terminalValue();

  {
    typename MapType::iterator iter = cache.find(navi);
    if (iter != cache.end())
      return iter->second;
  }

  return cache[navi] = 
    dd_long_count_step(cache, navi.thenBranch()) +
    dd_long_count_step(cache, navi.elseBranch());
}

/// Function template for generically computing number of terms
template <class IntType, class NaviType>
inline IntType
dd_long_count(NaviType navi) {

  std::map<NaviType, IntType> local_cache;
  return dd_long_count_step(local_cache, navi);
}
template <class IntType, class NaviType>
inline IntType
dd_long_count_without_cache(NaviType navi) {
  if(navi.isConstant())
    return navi.terminalValue();

  return  dd_long_count<IntType, NaviType>(navi.thenBranch()) +
    dd_long_count<IntType, NaviType>(navi.elseBranch());
}

END_NAMESPACE_PBORI

#endif
