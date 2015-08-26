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
**/

// load header file
#include <polybori/except/PBoRiError.h>
#include <polybori/except/CErrorInfo.h>

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
