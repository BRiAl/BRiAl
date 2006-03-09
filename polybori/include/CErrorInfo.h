// -*- c++ -*-
//*****************************************************************************
/** @file 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file includes all header files, which are necessary
 * for using the polybori lib.
 *
 *  (c) 2006 by
 *   Dep. of Mathematics, Kaiserslautern University of Technology and
 *   Fraunhofer Institute for Industrial Mathematics (ITWM)
 *   D-67663 Kaiserslautern, Germany
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.2  2006/03/09 16:22:15  dreyer
 * + File header revised
 *
 * Revision 1.1.1.1  2006/03/09 14:34:31  dreyer
 * + Project started
 *
 * @endverbatim
**/
//*****************************************************************************

#include "pbori_defs.h"

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
