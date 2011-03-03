// -*- c++ -*-
//*****************************************************************************
/** @file CCheckedIdx.c
 *
 * @author Alexander Dreyer
 * @date 2011-02-31
 *
 * 
 *
 * @par Copyright:
 *   (c) by The PolyBoRi Team
 *
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"
#include "CCheckedIdx.h"
#include <errors/PBoRiGenericError.h>

BEGIN_NAMESPACE_PBORI

/** @class CCheckedIdx
 * @brief This class defines CCheckedIdx.
 *
 **/
void CCheckedIdx::handle_error() const {
  throw PBoRiGenericError<CTypes::out_of_bounds>();
}

END_NAMESPACE_PBORI

