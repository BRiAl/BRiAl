// -*- c++ -*-
//*****************************************************************************
/** @file CErrorInfo.h 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file includes all header files, which are necessary
 * for using the polybori lib.
 *
 * @par Copyright:
 *   (c) 2006 by The PolyBoRi Team
**/
//*****************************************************************************

#ifndef polybori_error_CErrorInfo_h_
#define polybori_error_CErrorInfo_h_

#include <polybori/pbori_defs.h>

BEGIN_NAMESPACE_PBORI

/** @internal @class CErrorInfo
 * @brief Converts error codes to meaningful text.
 **/
class CErrorInfo {
 public:
  /// adopt global error code enumeration
  typedef CTypes::errornum_type errornum_type;

  /// adopt global error text type
  typedef CTypes::errortext_type errortext_type;

  /// default constructor (call once for text initialization) 
  CErrorInfo();

  /// convert error code to description text
  static errortext_type text(errornum_type);

protected:
  /// table of error messages in readable form
  static errortext_type pErrorText[];

};


END_NAMESPACE_PBORI

#endif
