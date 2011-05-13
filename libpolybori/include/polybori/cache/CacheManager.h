// -*- c++ -*-
//*****************************************************************************
/** @file CacheManager.h
 *
 * @author Alexander Dreyer
 * @date 2006-11-22
 *
 * This file contains the user variant for the (internal) CCacheManagement,
 * which can find the active ring.
 *
 * @par Copyright:
 *   (c) 2006 by The PolyBoRi Team
**/
//*****************************************************************************

#ifndef polybori_cache_CacheManager_h_
#define polybori_cache_CacheManager_h_

// include basic definitions
#include <polybori/pbori_defs.h>

// get internal cache management classes
#include "CCacheManagement.h"

#include <polybori/BoolePolyRing.h>

BEGIN_NAMESPACE_PBORI

class BooleEnv;
template <class CacheType>
class CacheManager:
  public CCacheManagement<BoolePolyRing, CacheType, CacheType::nargs> {

public:
  /// Define base type
  typedef CCacheManagement<BoolePolyRing, CacheType, CacheType::nargs> base;
  typedef typename base::manager_type manager_type;

  /// Constructor
  CacheManager(const manager_type& mgr):
    base(mgr) {}

  /// Destructor
  ~CacheManager() {}
};

template <class CacheType>
class CommutativeCacheManager:
  public CCommutativeCacheManagement<BoolePolyRing, CacheType> {

public:
  /// Define base type
  typedef CCommutativeCacheManagement<BoolePolyRing, CacheType> base;
  typedef typename base::manager_type manager_type;

  /// Constructor
  CommutativeCacheManager(const manager_type& mgr):
    base(mgr) {}

  /// Destructor
  ~CommutativeCacheManager() {}
};


END_NAMESPACE_PBORI

#endif
