// -*- c++ -*-
//*****************************************************************************
/** @file ExpGreater.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30 
 *
 * This file includes the definition of the class @c ExpGreater.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_ExpGreater_h_
#define polybori_groebner_ExpGreater_h_

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class ExpGreater
 * @brief This class defines ExpGreater.
 *
 **/
class ExpGreater:
  public CFactoryBase {
public:
  ExpGreater(const BoolePolyRing& ring): CFactoryBase(ring) {}

  bool operator()(const BooleExponent& lhs,
		  const BooleExponent& rhs) const {
    return parent().ordering().compare(lhs,rhs)==CTypes::greater_than;
  }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_ExpGreater_h_ */
