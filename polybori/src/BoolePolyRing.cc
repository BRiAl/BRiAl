// -*- c++ -*-
//*****************************************************************************
/** @file 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file implements the class BoolePolyRing, where carries the definition of
 * a polynomial ring over Booleans. 
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
 * Revision 1.8  2006/03/17 16:53:37  dreyer
 * ADD added nNodes(), operator*= to BoolePolynomial
 *
 * Revision 1.7  2006/03/16 17:09:13  dreyer
 * ADD BoolePolynial functionality started
 *
 * Revision 1.6  2006/03/13 12:27:24  dreyer
 * CHANGE: consistent function names
 *
 * Revision 1.5  2006/03/12 05:34:23  bricken
 * *bricken: made destructor safer, clean current_ring
 *
 * Revision 1.4  2006/03/11 08:41:25  bricken
 * *bricken: standard/copy constructor
 *
 * Revision 1.3  2006/03/10 15:13:06  dreyer
 * ADD: Added static access to current ring
 *
 * Revision 1.2  2006/03/10 08:25:54  dreyer
 * + refined header style
 *
 * Revision 1.1.1.1  2006/03/09 14:34:31  dreyer
 * + Project started
 *
 * @endverbatim
**/
//*****************************************************************************


// load header file
# include "BoolePolyRing.h"

// get error types
# include "PBoRiError.h"


BEGIN_NAMESPACE_PBORI

// initialize pointer to active ring with 0 pointer
BoolePolyRing* BoolePolyRing::current_ring = NULL;

// interface with cudd's variable management
BoolePolyRing::BoolePolyRing(size_type nvars_, bool_type make_active) :
  mgr(0, nvars_), nvars(nvars_) {

  PBORI_TRACE_FUNC( "BoolePolyRing(size_type)" );

  if(make_active)
    activate();
}
BoolePolyRing::BoolePolyRing(const BoolePolyRing& r) :
mgr(0, r.nvars), nvars(r.nvars) {
  
  PBORI_TRACE_FUNC( "BoolePolyRing(const BoolePolyRing&)" );
  
}

// destructor
BoolePolyRing::~BoolePolyRing() {

  PBORI_TRACE_FUNC( "~BoolePolyRing()" );

  // clear global ring setting, if current active ring is killed
  if (BoolePolyRing::current_ring == this)
    BoolePolyRing::current_ring = NULL;

  // Note: Compiler automatically called mgr.~manager_type()
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

// access nvar-th variable of the active ring
BoolePolyRing::dd_type
BoolePolyRing::ringVariable(idx_type nvar) {

  PBORI_TRACE_FUNC( "BoolePolyRing::ringVariable(idx_type nvar)" );

  return ring().variable(nvar); 

}

// get number of ring variables
// access nvar-th ring variable
BoolePolyRing::dd_type
BoolePolyRing::empty() const {

  PBORI_TRACE_FUNC( "BoolePolyRing::empty() const" );

  return mgr.zddZero();
}

// access nvar-th variable of the active ring
BoolePolyRing::dd_type
BoolePolyRing::ringEmpty() {

  PBORI_TRACE_FUNC( "BoolePolyRing::ringEmpty()" );

  return ring().empty(); 

}


BoolePolyRing::size_type
BoolePolyRing::nVariables() const {

  PBORI_TRACE_FUNC( "BoolePolyRing::nvariables() const" );

  return nvars;
}

// get number of ring variables of the active ring
BoolePolyRing::size_type
BoolePolyRing::nRingVars() {

  PBORI_TRACE_FUNC( "BoolePolyRing::nRingvariables() const" );

  return ring().nVariables();
}

// access current global ring setting
BoolePolyRing&
BoolePolyRing::ring() {

  PBORI_TRACE_FUNC( "BoolePolyRing::ring() const" );

  if(current_ring != NULL){
    return *current_ring;
  }
  else {
    throw PBoRiError(CTypes::no_ring);
  }
    

}



//  make this global ring
void
BoolePolyRing::activate() {

  PBORI_TRACE_FUNC( "BoolePolyRing::activate() const" );

  current_ring = this;
}

END_NAMESPACE_PBORI
