// -*- c++ -*-
//*****************************************************************************
/** @file CCuddFirstIter.h
 *
 * @author Alexander Dreyer
 * @date 2006-03-26
 *
 * This file defines an stl-like iterator for accessing the nonzero indices of
 * the leading (first) minimal term of Cudd decision diagrams.
 *
 * @note: Tested for Cudd's ZDDs only.
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
 * Revision 1.3  2006/04/04 15:31:06  dreyer
 * ADD: BoolePolynomial::navigator() and corresp. class CCuddNavigator
 *
 * Revision 1.2  2006/03/30 08:59:42  dreyer
 * FIX: CCuddFirstIter works for empty and zero polynomials now
 *
 * Revision 1.1  2006/03/29 16:26:46  dreyer
 * ADD: Class CCuddFirstIter used for BoolePolynomial::lead()
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "CCuddNavigator.h"

#ifndef CCuddFirstIter_h_
#define CCuddFirstIter_h_

BEGIN_NAMESPACE_PBORI

/** @class CCuddFirstIter
 * @brief This class defines an iterator over the first minimal term of a given
 * ZDD node.
 *
 **/

class CCuddFirstIter :
  public CCuddNavigator {

public:

  /// Get type of *this
  typedef CCuddFirstIter self;

  /// Get base type 
  typedef CCuddNavigator base;

  /// Default constructor and construct from node pointer
  CCuddFirstIter(pointer_type ptr = NULL);

  /// Copy Constructor
  CCuddFirstIter(const self&);

  /// Destructor
  ~CCuddFirstIter();

  /// Prefix increment operator
  self& operator++();

  /// Postfix increment operator
  self operator++(int);

protected:
  /// Go to valid node
  void validate();
};


END_NAMESPACE_PBORI

#endif
