// -*- c++ -*-
//*****************************************************************************
/** @file CCuddFirstIter.cc
 *
 * @author Alexander Dreyer
 * @date 2006-03-26
 *
 * This file implements an stl-like iterator for accessing the nonzero indices
 * of the lexicographical-minimal (last) term of Cudd decision diagrams.
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
 * Revision 1.1  2006/06/07 08:37:50  dreyer
 * ADD CCuddLastIter and BooleSet::lastLexicographicalTerm()
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "CCuddLastIter.h"

BEGIN_NAMESPACE_PBORI

// constructor
CCuddLastIter::CCuddLastIter(pointer_type ptr):
  base(ptr) {

  PBORI_TRACE_FUNC( "CCuddLastIter::CCuddLastIter(pointer_type)" );

  self tmp(*this);

  if(isValid()){

    // go in the direction of last term
    incrementElse();
    while(!isConstant()){
      tmp = *this;
      incrementElse();
    }

    if(isConstant() && !terminalValue())
      *this = tmp;

    terminateConstant();
  }

}

// copy constructor
CCuddLastIter::CCuddLastIter(const self& rhs):
  base(rhs) {

  PBORI_TRACE_FUNC( "CCuddLastIter::CCuddLastIter(const self&)" );
}


// destructor
CCuddLastIter::~CCuddLastIter() {

  PBORI_TRACE_FUNC( "CCuddLastIter::~CCuddLastIter()" );
  // does nothing, because everything is managed in base
}

// prefix increment operator
CCuddLastIter&
CCuddLastIter::operator++() {

  PBORI_TRACE_FUNC( "CCuddLastIter::operator++()" );

  if(isValid()) {

    // this node is done; go to next possible node
    incrementThen();

    if( !isConstant() ) {       // if still in interior of a path

      self prev(*this);         // store copy of *this
      incrementElse();          // go in direction of last term, if possible

      // restore previous value, of else branch was invalid
      if( isConstant() && !terminalValue() )
        *this = prev; 
    }
    terminateConstant();
  }
  return *this;
};

// postfix increment operator
CCuddLastIter
CCuddLastIter::operator++(int) {

  PBORI_TRACE_FUNC( "CCuddLastIter::operator++(int)" );

  self tmp(*this);
  operator++();

  return tmp;
};

// go to valid node
void
CCuddLastIter::terminateConstant() {

  PBORI_TRACE_FUNC( "CCuddLastIter::terminateConstant()" );

  if (isConstant()) 
      *this = self();           // mark end of path reached

};


END_NAMESPACE_PBORI
