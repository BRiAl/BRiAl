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
 * Revision 1.1  2006/03/29 16:26:46  dreyer
 * ADD: Class CCuddFirstIter used for BoolePolynomial::lead()
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "CCuddFirstIter.h"

// Get Cudd's internal definitions
//#include "cuddInt.h"

BEGIN_NAMESPACE_PBORI

/** @class CCuddFirstIter
 * @brief This class defines an iterator over the first minimal term of a given
 * ZDD node.
 *
 **/

// constructor
CCuddFirstIter::CCuddFirstIter(pointer_type ptr):
  pNode(ptr) {

  PBORI_TRACE_FUNC( "CCuddFirstIter::CCuddFirstIter(pointer_type)" );

//   if( (pNode != NULL)&& Cudd_IsConstant(pNode)) {
//     if( Cudd_V(pNode) == 0 ) 
//       pNode = Cudd_E(pNode);
//     else
//       pNode = NULL;
//   }


}

// copy constructor
CCuddFirstIter::CCuddFirstIter(const self& rhs):
  pNode(rhs.pNode) {

  PBORI_TRACE_FUNC( "CCuddFirstIter::CCuddFirstIter(const self&)" );
}


// destructor
CCuddFirstIter::~CCuddFirstIter() {

  PBORI_TRACE_FUNC( "CCuddFirstIter::~CCuddFirstIter()" );
  // does nothing, because pNode is manages outside the class
}

// prefix increment operator
CCuddFirstIter&
CCuddFirstIter::operator++() {

  PBORI_TRACE_FUNC( "CCuddFirstIter::operator++()" );

  if(pNode != NULL) {

    pNode = Cudd_T(pNode);
    while ((pNode != NULL) && Cudd_IsConstant(pNode)) {
      if( Cudd_V(pNode) == 0 ) 
        pNode = Cudd_E(pNode);
      else
        pNode = NULL;         // at end of path
    }


  }

  return *this;
};

// postfix increment operator
CCuddFirstIter
CCuddFirstIter::operator++(int) {

  PBORI_TRACE_FUNC( "CCuddFirstIter::operator++(int)" );

  self tmp(*this);
  operator++();

  return tmp;
};

// constant pointer access operator
CCuddFirstIter::value_type
CCuddFirstIter::operator*() const {

  PBORI_TRACE_FUNC( "CCuddFirstIter::operator*() const" );
  PBORI_TRACE_FUNC( pNode );
  PBORI_TRACE_FUNC( Cudd_Regular(pNode)->index);
  return Cudd_Regular(pNode)->index;
};

// constant pointer access operator
const CCuddFirstIter::pointer_type
CCuddFirstIter::operator->() const {

  PBORI_TRACE_FUNC( "CCuddFirstIter::operator->() const" );
  return pNode;
};

// equality test
CCuddFirstIter::bool_type 
CCuddFirstIter::operator==(const self& rhs) const {

  PBORI_TRACE_FUNC( "CCuddFirstIter::operator==(const self&) const" );
  return (pNode == rhs.pNode);
};

// nonequality test
CCuddFirstIter::bool_type 
CCuddFirstIter::operator!=(const self& rhs) const {

  PBORI_TRACE_FUNC( "CCuddFirstIter::operator!=(const self&) const" );
  return (pNode != rhs.pNode);
};


END_NAMESPACE_PBORI
