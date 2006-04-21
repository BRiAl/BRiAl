// -*- c++ -*-
//*****************************************************************************
/** @file BooleSet.cc
 *
 * @author Alexander Dreyer
 * @date 2006-04-19
 *
 * This file implements the class BooleSet, where carries the definition
 * of sets of Boolean variables.
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
 * Revision 1.3  2006/04/21 06:35:45  dreyer
 * CHANGE Default constructor starts with empty set
 *
 * Revision 1.2  2006/04/20 17:31:05  dreyer
 * FIX removed casting operator, which caused ambigeous overloads
 *
 * Revision 1.1  2006/04/20 16:59:47  dreyer
 * routine
 *
 * @endverbatim
**/
//*****************************************************************************


// load header files
# include "BooleSet.h"
# include "BooleMonomial.h"
# include "BoolePolyRing.h"

BEGIN_NAMESPACE_PBORI

//-------------------------------------------------------------------------
// Constructors and destructor
//------------------------------------------------------------------------

// Default constructor
BooleSet::BooleSet():
  base( BoolePolyRing::ringZero() )  {

  PBORI_TRACE_FUNC( "BooleSet()" );

}

// Copy constructor
BooleSet::BooleSet(const self& rhs) :
  base(rhs) {
  
  PBORI_TRACE_FUNC( "BooleSet(const self&)" );
}

// Copy constructor
BooleSet::BooleSet(const base& rhs) :
  base(rhs) {
  
  PBORI_TRACE_FUNC( "BooleSet(const base&)" );
}


// Destructor
BooleSet::~BooleSet() {

  PBORI_TRACE_FUNC( "~BooleSet()" );
}

// Add given monomial to sets and assign
BooleSet&
BooleSet::addAssign(const term_type& rhs) {

  PBORI_TRACE_FUNC( "BooleSet::addAssign(const term_type&) const" );

  uniteAssign(rhs.diagram());
  return *this;
}

// Add given monomial to sets
BooleSet
BooleSet::add(const term_type& rhs) const {

  PBORI_TRACE_FUNC( "BooleSet::addAssign(const term_type&) const" );

  return self(*this).addAssign(rhs);
}

// Check whether rhs is includes in *this
BooleSet::bool_type
BooleSet::owns(const term_type& rhs) const {

  PBORI_TRACE_FUNC( "BooleSet::owns(const term_type&) const" );

  return !(intersect(rhs.diagram()).emptiness());
}


END_NAMESPACE_PBORI
