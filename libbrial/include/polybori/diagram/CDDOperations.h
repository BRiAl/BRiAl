// -*- c++ -*-
//*****************************************************************************
/** @file CDDOperations.h
 *
 * @author Alexander Dreyer
 * @date 2006-08-29
 *
 * This file implements an internal template class for geenric CDDOperations;
 *
 * @par Copyright:
 *   (c) 2006 by The PolyBoRi Team
**/
//*****************************************************************************

#ifndef polybori_diagram_CDDOperations_h_
#define polybori_diagram_CDDOperations_h_

// include basic definitions
#include <polybori/pbori_defs.h>

// Get Cudd definitions
#include <polybori/cudd/cudd.h>

#include <polybori/routines/pbori_routines.h>
#include <polybori/cache/CCacheManagement.h>
#include <polybori/BoolePolyRing.h>

BEGIN_NAMESPACE_PBORI

/// @class CDDOperation
/// @brief Generic class containing decision diagram operations
template <class DDType, class MonomType>
class CDDOperations {
public:

  // This is only a work-around, since a monomial should not be generated from
  // a dd.
  MonomType getMonomial(const DDType& dd) const {
    return MonomType(dd);
  }
  
  MonomType usedVariables(const DDType& dd){

  // get type definitions from DDType
  typedef typename DDType::idx_type idx_type;
  typedef typename DDType::navigator navigator;
  typedef MonomType monom_type;

  CCacheManagement<BoolePolyRing, CCacheTypes::used_variables> cache_mgr((BoolePolyRing)dd.ring());
  return cached_used_vars(cache_mgr, dd.navigation(),  
                          MonomType(cache_mgr.one()));
}

};



END_NAMESPACE_PBORI

#endif
