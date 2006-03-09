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
