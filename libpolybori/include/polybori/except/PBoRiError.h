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
**/
//*****************************************************************************

#ifndef polybori_error_PBoRiError_h_
#define polybori_error_PBoRiError_h_

// load PolyBoRi settings
#include <polybori/pbori_defs.h>
#include <exception>

BEGIN_NAMESPACE_PBORI

/** @class PBoRiError
 * @brief This class is used for polybori's exception handling.
 *
 * It's mainly a class wrapper for CTypes::errorcode.
 * 
 **/
class PBoRiError:
  public std::exception, public CTypes {

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
  ~PBoRiError() throw();

  /// get error code
  errornum_type code() const;

  /// get error text
  errortext_type text() const;

  /// std::exception compatible interface
  const char* what() const throw() { return text(); }

protected:
  errornum_type error;
};

END_NAMESPACE_PBORI

#endif // of #ifndef PBoRiError_h_
