// -*- c++ -*-
//*****************************************************************************
/** @file ZeroFunction.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30 
 *
 * This file includes the definition of the class @c ZeroFunction.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_ZeroFunction_h_
#define polybori_groebner_ZeroFunction_h_

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class ZeroFunction
 * @brief This class defines ZeroFunction.
 *
 **/
class ZeroFunction{
    public:
        unsigned int operator() (unsigned int val){
            return 0;
        }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_ZeroFunction_h_ */
