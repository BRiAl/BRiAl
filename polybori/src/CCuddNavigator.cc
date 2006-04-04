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

  if (isValid()) {
    pNode = Cudd_T(pNode);
    //    validate();
  }

  return *this;
};

// increment in else direction
CCuddNavigator&
CCuddNavigator::incrementElse() {

  PBORI_TRACE_FUNC( "CCuddNavigator::incrementElse()" );

  if(isValid()) {
    pNode = Cudd_E(pNode);
    //  validate();
  }

  return *this;
};

// constant pointer access operator
CCuddNavigator::value_type
CCuddNavigator::operator*() const {

  PBORI_TRACE_FUNC( "CCuddNavigator::operator*() const" );
  return Cudd_Regular(pNode)->index;
};

// constant pointer access operator
const CCuddNavigator::pointer_type
CCuddNavigator::operator->() const {

  PBORI_TRACE_FUNC( "CCuddNavigator::operator->() const" );
  return pNode;
};

// equality test
CCuddNavigator::bool_type 
CCuddNavigator::operator==(const self& rhs) const {

  PBORI_TRACE_FUNC( "CCuddNavigator::operator==(const self&) const" );
  return (pNode == rhs.pNode);
};

// nonequality test
CCuddNavigator::bool_type 
CCuddNavigator::operator!=(const self& rhs) const {

  PBORI_TRACE_FUNC( "CCuddNavigator::operator!=(const self&) const" );
  return (pNode != rhs.pNode);
};

// whether constant node was reached
CCuddNavigator::bool_type 
CCuddNavigator::terminated() const {

  PBORI_TRACE_FUNC( "CCuddNavigator::terminated() const" );
  return isValid() && Cudd_IsConstant(pNode);
};

// constant node value
CCuddNavigator::bool_type 
CCuddNavigator::finished() const {

  PBORI_TRACE_FUNC( "CCuddNavigator::finished() const" );
  return Cudd_V(pNode);
};

// whether iterator is at end
CCuddNavigator::bool_type 
CCuddNavigator::isValid() const {

  PBORI_TRACE_FUNC( "CCuddNavigator::isValid()() const" );

  // convention: all non-Null pointers are valid 
  return (pNode != NULL);
};


END_NAMESPACE_PBORI
