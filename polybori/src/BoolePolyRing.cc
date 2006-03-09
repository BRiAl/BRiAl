// -*- c++ -*-
// $Id$
//
//*****************************************************************************
/** @file 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file implements the class BoolePolyRing, where carries the definition of
 * a polynomial ring over Booleans.
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
// Revision 1.1.1.1  2006/03/09 14:34:31  dreyer
// + Project started
//

// load header file
# include "BoolePolyRing.h"

BEGIN_NAMESPACE_PBORI

// interface with cudd's variable management
BoolePolyRing::BoolePolyRing(size_type nvars) :
  mgr(0, nvars) {

  PBORI_TRACE_FUNC( "BoolePolyRing(size_type)" );
}

// destructor
BoolePolyRing::~BoolePolyRing() {

  PBORI_TRACE_FUNC( "~BoolePolyRing()" );

  // call manager's destructor
  mgr.~manager_type();
}


//  cast to base operator
BoolePolyRing::operator manager_type&() {

  PBORI_TRACE_FUNC( "BoolePolyRing::operator manager_type()" );

  return mgr;
}

// access to base
BoolePolyRing::manager_type&
BoolePolyRing::manager() {

  PBORI_TRACE_FUNC( "BoolePolyRing::manager()" );

  return mgr;
}
//  constant access to base
const BoolePolyRing::manager_type&
BoolePolyRing::manager() const {

  PBORI_TRACE_FUNC( "BoolePolyRing::manager() const" );

  return const_cast<const manager_type &>(mgr);
}

// access nvar-th ring variable
BoolePolyRing::dd_type
BoolePolyRing::variable(idx_type nvar) const {

  PBORI_TRACE_FUNC( "BoolePolyRing::variable(idx_type) const" );

  return mgr.zddVar(nvar);
}
END_NAMESPACE_PBORI
