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
 * Revision 1.6  2006/12/13 18:07:04  dreyer
 * ADD: poly /= exp
 *
 * Revision 1.5  2006/12/06 09:20:09  dreyer
 * CHANGE: poly * exp now recursive
 *
 * Revision 1.4  2006/04/06 13:05:54  dreyer
 * CHANGE more suitable names for CCuddNavigator functions
 *
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

  /// Interface type for standard iterator access
  typedef std::forward_iterator_tag iterator_category;

  /// Get type of *this
  typedef CCuddFirstIter self;

  /// Get base type 
  typedef CCuddNavigator base;

  /// Default constructor and construct from node pointer
  CCuddFirstIter(pointer_type ptr = NULL);

  /// Copy Constructor
  CCuddFirstIter(const self& rhs): base(rhs) {}

  /// Copy Constructor
  CCuddFirstIter(const base& rhs): base(rhs) {}

  /// Destructor
  ~CCuddFirstIter() {}

  /// Prefix increment operator
  self& operator++();

  /// Postfix increment operator
  self operator++(int);

protected:
  /// Constant nodes are marked as the end of a path
  void terminateConstant();
};

// constructor
inline 
CCuddFirstIter::CCuddFirstIter(pointer_type ptr):
  base(ptr) {

  PBORI_TRACE_FUNC( "CCuddFirstIter::CCuddFirstIter(pointer_type)" );
  terminateConstant();
}



// prefix increment operator
inline CCuddFirstIter&
CCuddFirstIter::operator++() {

  PBORI_TRACE_FUNC( "CCuddFirstIter::operator++()" );

  incrementThen();
  terminateConstant();
  return *this;
}

// postfix increment operator
inline CCuddFirstIter
CCuddFirstIter::operator++(int) {

  PBORI_TRACE_FUNC( "CCuddFirstIter::operator++(int)" );

  self tmp(*this);
  operator++();

  return tmp;
}

// go to valid node
inline void
CCuddFirstIter::terminateConstant() {

  PBORI_TRACE_FUNC( "CCuddFirstIter::terminateConstant()" );

  if (isConstant()) 
      *this = self();           // mark end of path reached

}


END_NAMESPACE_PBORI

#endif
