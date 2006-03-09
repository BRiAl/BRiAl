// -*- c++ -*-
// $Id$
//
//*****************************************************************************
/** @file
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file defines the class CErrorInfo, which to conver error coedes to
 * meaningful text.
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
#include "CErrorInfo.h"

BEGIN_NAMESPACE_PBORI

CErrorInfo::errortext_type 
CErrorInfo::pErrorText[CTypes::last_error];

// default constructor (call once for text initialization) 
CErrorInfo::CErrorInfo() {

  PBORI_TRACE_FUNC( "CErrorInfo()" );

  pErrorText[CTypes::alright] = "No error.";
  pErrorText[CTypes::failed] = "Unspecified error.";
  pErrorText[CTypes::no_ring] = "No polynomial ring structure defined.";

}

// convert error code to description text
CErrorInfo::errortext_type 
CErrorInfo::text(errornum_type num) {

  PBORI_TRACE_FUNC( "CErrorInfo::text(errornum_type) const" );
  return pErrorText[num];
}

// initialize array of error names 
static CErrorInfo init_error_info;

END_NAMESPACE_PBORI
