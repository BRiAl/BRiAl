// -*- c++ -*-
//*****************************************************************************
/** @file PBoRiError.h 
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
 * Revision 1.3  2007/11/06 15:03:36  dreyer
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
//*****************************************************************************

// load PolyBoRi settings
# include "pbori_defs.h"


#ifndef PBoRiError_h_
#define PBoRiError_h_


BEGIN_NAMESPACE_PBORI

/** @class PBoRiError
 * @brief This class is used for polybori's exception handling.
 *
 * It's mainly a class wrapper for CTypes::errorcode.
 * 
 **/
class PBoRiError {

public:
  /// adopt global error code enumeration
  typedef CTypes::errornum_type errornum_type;

  /// adopt global error text type
  typedef CTypes::errortext_type errortext_type;

  /// generic access to type of @c *this
  typedef PBoRiError self;

  /// construct instance from error code
  PBoRiError(errornum_type err = CTypes::alright);

  /// copy constructor
  PBoRiError(const self&);

  /// destructor
  ~PBoRiError();

  /// get error code
  errornum_type code() const;

  /// get error text
  errortext_type text() const;

protected:
  errornum_type error;
};

END_NAMESPACE_PBORI

#endif // of #ifndef PBoRiError_h_
