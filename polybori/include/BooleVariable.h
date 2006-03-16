// -*- c++ -*-
//*****************************************************************************
/** @file 
 *
 * @author Alexander Dreyer
 * @date 2006-03-10
 *
 * This file carries the definition of class @c BooleVariable, which can be used
 * to access the boolean variables of the currently active Boolean polynomial
 * ring. 
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
 * Revision 1.1  2006/03/16 17:09:13  dreyer
 * ADD BoolePolynial functionality started
 *
 * @endverbatim
**/
//*****************************************************************************

#ifndef BooleVariable_h_
#define BooleVariable_h_

// include basic definitions
#include "pbori_defs.h"

// get BoolePolynomial's definition
#include "BoolePolynomial.h"

BEGIN_NAMESPACE_PBORI

/** @class BooleVariable
 * @brief This class is just a wrapper for using variables from @c cudd's
 * decicion diagram. 
 *
 * @note @c BooleVariable is actually a specialization of @c BoolePolynomial
 * with a special constructor.
 *
 **/
class BooleVariable:
  public BoolePolynomial {

 public:
  //-------------------------------------------------------------------------
  // types definitions
  //-------------------------------------------------------------------------

  /// Generic access to current type
  typedef BooleVariable self;

  /// Generic access to base type
  typedef BoolePolynomial base;

  /// Constructor idx-th variable of active ring
  BooleVariable(idx_type idx = 0);

  /// Copy constructor
  BooleVariable(const self&);



};


END_NAMESPACE_PBORI

#endif // of BooleVariable_h_
