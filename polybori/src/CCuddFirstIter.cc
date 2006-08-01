// -*- c++ -*-
//*****************************************************************************
/** @file CCuddFirstIter.cc
 *
 * @author Alexander Dreyer
 * @date 2006-03-26
 *
 * This file implements an stl-like iterator for accessing the nonzero indices
 * of the leading (first) minimal term of Cudd decision diagrams.
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
 * Revision 1.6  2006/08/01 11:12:22  dreyer
 * CHANGE: removed ';' at end of functions (pedantic)
 *
 * Revision 1.5  2006/04/25 07:05:02  dreyer
 * FIX postfix++ of CCuddFirstIter
 *
 * Revision 1.4  2006/04/06 13:05:54  dreyer
 * CHANGE more suitable names for CCuddNavigator functions
 *
 * Revision 1.3  2006/04/04 15:31:07  dreyer
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
#include "CCuddFirstIter.h"

BEGIN_NAMESPACE_PBORI

// constructor
CCuddFirstIter::CCuddFirstIter(pointer_type ptr):
  base(ptr) {

  PBORI_TRACE_FUNC( "CCuddFirstIter::CCuddFirstIter(pointer_type)" );
  terminateConstant();
}

// copy constructor
CCuddFirstIter::CCuddFirstIter(const self& rhs):
  base(rhs) {

  PBORI_TRACE_FUNC( "CCuddFirstIter::CCuddFirstIter(const self&)" );
}


// destructor
CCuddFirstIter::~CCuddFirstIter() {

  PBORI_TRACE_FUNC( "CCuddFirstIter::~CCuddFirstIter()" );
  // does nothing, because everything is managed in base
}

// prefix increment operator
CCuddFirstIter&
CCuddFirstIter::operator++() {

  PBORI_TRACE_FUNC( "CCuddFirstIter::operator++()" );

  incrementThen();
  terminateConstant();
  return *this;
}

// postfix increment operator
CCuddFirstIter
CCuddFirstIter::operator++(int) {

  PBORI_TRACE_FUNC( "CCuddFirstIter::operator++(int)" );

  self tmp(*this);
  operator++();

  return tmp;
}

// go to valid node
void
CCuddFirstIter::terminateConstant() {

  PBORI_TRACE_FUNC( "CCuddFirstIter::terminateConstant()" );

  if (isConstant()) 
      *this = self();           // mark end of path reached

}


END_NAMESPACE_PBORI
