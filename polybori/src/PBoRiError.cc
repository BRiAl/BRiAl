// -*- c++ -*-
//*****************************************************************************
/** @file: PBoRiError.cc 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file defines the class PBoriError, which is used for polybori's
 * exception handling.
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
 * Revision 1.3  2007/11/06 15:03:39  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.2  2006/03/10 08:25:54  dreyer
 * + refined header style
 *
 * Revision 1.1.1.1  2006/03/09 14:34:31  dreyer
 * + Project started
 *
 * @endverbatim
**/

// load header file
#include "PBoRiError.h"
#include "CErrorInfo.h"

BEGIN_NAMESPACE_PBORI

// usual constructor 
PBoRiError::PBoRiError(errornum_type err):
  error(err) {
  PBORI_TRACE_FUNC( "PBoRiError(errorcode)" );
}

// destructor 
PBoRiError::~PBoRiError() throw() {
  PBORI_TRACE_FUNC( "~PBoRiError()" );
}

// copy constructor 
PBoRiError::PBoRiError(const self& rhs):
  error(rhs.error) {
  PBORI_TRACE_FUNC( "PBoRiError(const self&)" );
}

// get error code
PBoRiError::errornum_type 
PBoRiError::code() const {

  PBORI_TRACE_FUNC( "PBoRiError::code() const" );
  return error;
}

PBoRiError::errortext_type 
PBoRiError::text() const {

  PBORI_TRACE_FUNC( "PBoRiError::info() const" );

  return CErrorInfo::text(error);
}


END_NAMESPACE_PBORI
