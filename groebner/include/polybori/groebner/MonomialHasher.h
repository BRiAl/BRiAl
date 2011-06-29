// -*- c++ -*-
//*****************************************************************************
/** @file MonomialHasher.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-29 
 *
 * This file includes the definition of the class @c MonomialHasher.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_MonomialHasher_h_
#define polybori_groebner_MonomialHasher_h_

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class MonomialHasher
 * @brief This class defines MonomialHasher.
 *
 **/
class MonomialHasher{
public:
  size_t operator() (const Monomial & m) const{
    return m.hash();
  }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_MonomialHasher_h_ */
