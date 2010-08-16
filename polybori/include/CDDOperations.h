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
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.8  2007/12/17 16:12:02  dreyer
 * CHANGE: reviewed and optimized merge frim sf.net
 *
 * Revision 1.7  2007/11/06 15:03:34  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.6  2007/05/24 14:01:30  dreyer
 * CHANGE: Recursive routine for usedVariables()
 *
 * Revision 1.5  2007/05/22 11:05:28  dreyer
 * FIX: ambigous overload
 *
 * Revision 1.4  2006/11/27 16:25:14  dreyer
 * CHANGE: CDegreeCache, now inherited from standard cache; dlex-lead cached
 *
 * Revision 1.3  2006/09/14 10:57:26  dreyer
 * ADD: usedVariablesExp()
 *
 * Revision 1.2  2006/08/29 12:09:26  dreyer
 * using CDDOperations for generic dd functions
 *
 * Revision 1.1  2006/08/29 10:37:56  dreyer
 * CHANGE: non-const version of diagram() now internalDiagram()
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

// Get Cudd definitions
#include "cudd.h"
#include "extrafwd.h"

#include "pbori_routines.h"
#include "CCacheManagement.h"

#include "BoolePolyRing.h"

#ifndef CDDOperations_h_
#define CDDOperations_h_

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
