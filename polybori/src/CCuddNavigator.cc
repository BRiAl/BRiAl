// -*- c++ -*-
//*****************************************************************************
/** @file CCuddNavigator.cc
 *
 * @author Alexander Dreyer
 * @date 2006-03-26
 *
 * This file implements an iterator for navigating through then and else branches
 * of Cudd decision diagrams.
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
 * Revision 1.5  2006/06/07 06:54:30  bricken
 * + 64BIT workaround
 *
 * Revision 1.4  2006/04/25 07:05:02  dreyer
 * FIX postfix++ of CCuddFirstIter
 *
 * Revision 1.3  2006/04/06 14:10:59  dreyer
 * ADD hash_type and CCuddNavigator::hash()
 *
 * Revision 1.2  2006/04/06 13:05:54  dreyer
 * CHANGE more suitable names for CCuddNavigator functions
 *
 * Revision 1.1  2006/04/04 15:31:07  dreyer
 * ADD: BoolePolynomial::navigator() and corresp. class CCuddNavigator
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "CCuddNavigator.h"

BEGIN_NAMESPACE_PBORI

/** @class CCuddNavigator
 * @brief This class defines an iterator over the first minimal term of a given
 * ZDD node.
 *
 **/

// constructor
CCuddNavigator::CCuddNavigator(pointer_type ptr):
  pNode(ptr) {

  PBORI_TRACE_FUNC( "CCuddNavigator::CCuddNavigator(pointer_type)" );
}

// copy constructor
CCuddNavigator::CCuddNavigator(const self& rhs):
  pNode(rhs.pNode) {

  PBORI_TRACE_FUNC( "CCuddNavigator::CCuddNavigator(const self&)" );
}


// destructor
CCuddNavigator::~CCuddNavigator() {

  PBORI_TRACE_FUNC( "CCuddNavigator::~CCuddNavigator()" );
  // does nothing, because pNode is manages outside the class
}

// increment in then direction
CCuddNavigator&
CCuddNavigator::incrementThen() {

  PBORI_TRACE_FUNC( "CCuddNavigator::incrementThen()" );

  if (isValid())
    pNode = Cudd_T(pNode);

  return *this;
}

// go to then direction
CCuddNavigator
CCuddNavigator::thenBranch() const {

  PBORI_TRACE_FUNC( "CCuddNavigator::thenBranch() const" );

  self copy(*this);
  return copy.incrementThen();
}

// increment in else direction
CCuddNavigator&
CCuddNavigator::incrementElse() {

  PBORI_TRACE_FUNC( "CCuddNavigator::incrementElse()" );

  if(isValid()) 
    pNode = Cudd_E(pNode);

  return *this;
}

// go to else direction
CCuddNavigator
CCuddNavigator::elseBranch() const {

  PBORI_TRACE_FUNC( "CCuddNavigator::ielseBranch() const" );

  self copy(*this);
  return copy.incrementElse();
}

// constant pointer access operator
CCuddNavigator::value_type
CCuddNavigator::operator*() const {

  PBORI_TRACE_FUNC( "CCuddNavigator::operator*() const" );
  return Cudd_Regular(pNode)->index;
}

// constant pointer access operator
const CCuddNavigator::pointer_type
CCuddNavigator::operator->() const {

  PBORI_TRACE_FUNC( "CCuddNavigator::operator->() const" );
  return pNode;
}

// constant pointer access operator
CCuddNavigator::hash_type
CCuddNavigator::hash() const {

  PBORI_TRACE_FUNC( "CCuddNavigator::hash() const" );
  return reinterpret_cast<long>(pNode);
}

// equality test
CCuddNavigator::bool_type 
CCuddNavigator::operator==(const self& rhs) const {

  PBORI_TRACE_FUNC( "CCuddNavigator::operator==(const self&) const" );

  return (pNode == rhs.pNode);
}

// nonequality test
CCuddNavigator::bool_type 
CCuddNavigator::operator!=(const self& rhs) const {

  PBORI_TRACE_FUNC( "CCuddNavigator::operator!=(const self&) const" );
  return (pNode != rhs.pNode);
}

// whether constant node was reached
CCuddNavigator::bool_type 
CCuddNavigator::isConstant() const {

  PBORI_TRACE_FUNC( "CCuddNavigator::isConstant() const" );
  return isValid() && Cudd_IsConstant(pNode);
}

// constant node value
CCuddNavigator::bool_type 
CCuddNavigator::terminalValue() const {

  PBORI_TRACE_FUNC( "CCuddNavigator::terminalValue() const" );
  return Cudd_V(pNode);
}

// whether iterator is at end
CCuddNavigator::bool_type 
CCuddNavigator::isValid() const {

  PBORI_TRACE_FUNC( "CCuddNavigator::isValid()() const" );

  // convention: all non-Null pointers are valid 
  return (pNode != NULL);
}


END_NAMESPACE_PBORI
