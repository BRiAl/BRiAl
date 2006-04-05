// -*- c++ -*-
//*****************************************************************************
/** @file 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file defines the class CErrorInfo, which to conver error coedes to
 * meaningful text.
 *
 * @par Copyright:
 *   (c) 2006 by
 *   Dep. of Mathematics, Kaiserslautern University of Technology and @n
 *   Fraunhofer Institute for Industrial Mathematics (ITWM)
 *   D-67663 Kaiserslautern, Germany
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.5  2006/04/05 14:56:38  dreyer
 * ADD prettyPrint functions for dot-output to stdout or file
 *
 * Revision 1.4  2006/03/22 08:06:59  dreyer
 * ADD: Template specializations CDDInterface<ZDD>, CDDManager<Cudd>; 
 * ring uses shared_ptr now
 *
 * Revision 1.3  2006/03/16 13:37:41  dreyer
 * ADD: added error codes and PBORI_DEVELOPER marker
 *
 * Revision 1.2  2006/03/10 08:25:54  dreyer
 * + refined header style
 *
 * Revision 1.1.1.1  2006/03/09 14:34:31  dreyer
 * + Project started
 *
 * @endverbatim
**/
//*****************************************************************************

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
  pErrorText[CTypes::invalid] = "Invalid operation called.";
  pErrorText[CTypes::out_of_bounds] = "Variable index out of bounds.";
  pErrorText[CTypes::io_error] = "I/O error.";
#ifdef PBORI_DEVELOPER
  pErrorText[CTypes::not_implemented] = 
    "Sorry! Functionality not implemented yet.";
#endif
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
