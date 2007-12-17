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
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.4  2007/12/17 16:12:02  dreyer
 * CHANGE: reviewed and optimized merge frim sf.net
 *
 * Revision 1.3  2007/12/13 15:53:49  dreyer
 * CHANGE: Ordering in BoolePolyRing again; BooleEnv manages active ring
 *
 * Revision 1.2  2007/11/06 15:03:35  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.1  2006/11/22 15:46:22  dreyer
 * ADD: CacheManager replacing CCacheManagement for external use
 * CHANGE: CacheManager used, where necessary
 * CHANGE: multiplesOf, uses cached recursion
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

// get internal cache management classes
#include "CCacheManagement.h"


#ifndef CacheManager_h_
#define CacheManager_h_

BEGIN_NAMESPACE_PBORI

class BooleEnv;
template <class CacheType>
class CacheManager:
  public CCacheManagement<CacheType, CacheType::nargs> {

public:
  /// Define base type
  typedef CCacheManagement<CacheType, CacheType::nargs> base;
  typedef typename base::manager_type manager_type;

  /// Constructor
  CacheManager(const manager_type& mgr):
    base(mgr) {}

  /// Destructor
  ~CacheManager() {}
};

template <class CacheType>
class CommutativeCacheManager:
  public CCommutativeCacheManagement<CacheType> {

public:
  /// Define base type
  typedef CCommutativeCacheManagement<CacheType> base;
  typedef typename base::manager_type manager_type;

  /// Constructor
  CommutativeCacheManager(const manager_type& mgr):
    base(mgr) {}

  /// Destructor
  ~CommutativeCacheManager() {}
};


END_NAMESPACE_PBORI

#endif
