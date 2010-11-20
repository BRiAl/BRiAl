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
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.4  2007/11/06 15:03:34  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.3  2006/03/10 08:25:54  dreyer
 * + refined header style
 *
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
