// -*- c++ -*-
//*****************************************************************************
/** @file SetBitUInt.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30 
 *
 * This file includes the definition of the class @c SetBitUInt.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_SetBitUInt_h_
#define polybori_groebner_SetBitUInt_h_

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class SetBitUInt
 * @brief This class defines SetBitUInt.
 *
 **/
class SetBitUInt{
    public:
        void operator() (unsigned int& vec, int index){
            vec|=(1<<index);
        }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_SetBitUInt_h_ */
