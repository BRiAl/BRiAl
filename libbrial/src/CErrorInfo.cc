// -*- c++ -*-
//*****************************************************************************
/** @file: CErrorInfo.cc 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file defines the class CErrorInfo, which to conver error coedes to
 * meaningful text.
 *
 * @par Copyright:
 *   (c) 2006 by The PolyBoRi Team
**/
//*****************************************************************************

// load header file
#include <polybori/except/CErrorInfo.h>

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
  pErrorText[CTypes::monomial_zero] = "Monomial operation resulted in zero.";
  pErrorText[CTypes::illegal_on_zero] = "Illegal operation on zero diagram or (sub-)polynomial.";
  pErrorText[CTypes::division_by_zero] = "Division by zero.";
  pErrorText[CTypes::invalid_ite] = 
    "Node index must be smaller than top indices of then- and else-branch.";
  pErrorText[CTypes::not_implemented] =
    "Sorry! Functionality not implemented yet.";
  pErrorText[CTypes::matrix_size_exceeded] = "Built-in matrix-size exceeded!";
}

// convert error code to description text
CErrorInfo::errortext_type 
CErrorInfo::text(errornum_type num) {

  PBORI_TRACE_FUNC( "CErrorInfo::text(errornum_type) const" );
  if PBORI_UNLIKELY(num < 0 || num >= CTypes::last_error)
    return "Unknown error occured.";

  return pErrorText[num];
}

// initialize array of error names 
static CErrorInfo init_error_info;

END_NAMESPACE_PBORI
