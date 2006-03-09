// -*- c++ -*-
// $Id$
//
//*****************************************************************************
/** @file pbori_defs.h
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file defines the class PBoriError, which is used for polybori's
 * exception handling.
 *
 *  (c) 2006 by
 *   Dep. of Mathematics, Kaiserslautern University of Technology and
 *   Fraunhofer Institute for Industrial Mathematics (ITWM)
 *   D-67663 Kaiserslautern, Germany
 *
**/
//*****************************************************************************
//
// Last edit by $Author$ on $Date$
//
// $Log$
// Revision 1.1  2006/03/09 14:34:31  dreyer
// Initial revision
//

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
PBoRiError::~PBoRiError() {
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
