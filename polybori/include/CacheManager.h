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
 *   (c) 2006 by
 *   Dep. of Mathematics, Kaiserslautern University of Technology and @n
 *   Fraunhofer Institute for Industrial Mathematics (ITWM)
 *   D-67663 Kaiserslautern, Germany
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
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

// get defintions of polynomial ring
#include "BoolePolyRing.h"

#ifndef CacheManager_h_
#define CacheManager_h_

BEGIN_NAMESPACE_PBORI


template <class CacheType, unsigned ArgumentLength = CacheType::nargs>
class CacheManager:
  public CCacheManagement<CacheType, ArgumentLength> {

public:
  /// Define base type
  typedef CCacheManagement<CacheType, ArgumentLength> base;
  typedef typename base::manager_type manager_type;

  /// (Default) Constructor
  CacheManager(const manager_type& mgr = BoolePolyRing::activeManager()):
    base(mgr) {}

  /// Destructor
  ~CacheManager() {}

  /// Find cached value wrt. given node
  using base::find;

  /// Store cached value wrt. given node  
  using base::insert;
};

template <class CacheType>
class CommutativeCacheManager:
  public CCommutativeCacheManagement<CacheType> {

public:
  /// Define base type
  typedef CCommutativeCacheManagement<CacheType> base;
  typedef typename base::manager_type manager_type;

  /// (Default) Constructor
  CommutativeCacheManager(const manager_type& mgr = 
                          BoolePolyRing::activeManager()):
    base(mgr) {}

  /// Destructor
  ~CommutativeCacheManager() {}

  /// Find cached value wrt. given node
  using base::find;

  /// Store cached value wrt. given node  
  using base::insert;
};


END_NAMESPACE_PBORI

#endif
