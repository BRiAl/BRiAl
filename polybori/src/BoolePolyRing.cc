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
 * Revision 1.11  2006/03/22 08:06:59  dreyer
 * ADD: Template specializations CDDInterface<ZDD>, CDDManager<Cudd>; ring uses shared_ptr now
 *
 * Revision 1.10  2006/03/20 14:51:01  dreyer
 * CHANGE: Use CDDInterface temple specializations instead of raw dd_type
 *
 * Revision 1.9  2006/03/20 09:52:57  dreyer
 * CHANGE: BooleVariable uses composition; variable generated in BoolePolyRing
 *
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
BoolePolyRing::manager_ptr BoolePolyRing::current_mgr;

// interface with cudd's variable management
BoolePolyRing::BoolePolyRing(size_type nvars, bool_type make_active) :
  pMgr( new manager_type(nvars) ) {

  PBORI_TRACE_FUNC( "BoolePolyRing(size_type)" );

  if(make_active)
    activate();
}

// copy constructor (shallow copy)
BoolePolyRing::BoolePolyRing(const BoolePolyRing& rhs) :
  pMgr(rhs.pMgr) {
  
  PBORI_TRACE_FUNC( "BoolePolyRing(const BoolePolyRing&)" );
  
}

// copy constructor (shallow copy)
BoolePolyRing::BoolePolyRing(manager_ptr pRhs) :
  pMgr(pRhs) {
  
  PBORI_TRACE_FUNC( "BoolePolyRing(manager_ptr)" );
  
}

// destructor
BoolePolyRing::~BoolePolyRing() {

  PBORI_TRACE_FUNC( "~BoolePolyRing()" );
  // deconstruction is managed using the shared pointer class.
}


// //  cast to base operator
// BoolePolyRing::operator manager_type&() {

//   PBORI_TRACE_FUNC( "BoolePolyRing::operator manager_type()" );

//   return *pMgr;
// }

// access to base
BoolePolyRing::manager_type&
BoolePolyRing::manager() {

  PBORI_TRACE_FUNC( "BoolePolyRing::manager()" );

  return *pMgr;
}
//  constant access to base
const BoolePolyRing::manager_type&
BoolePolyRing::manager() const {

  PBORI_TRACE_FUNC( "BoolePolyRing::manager() const" );

  return *pMgr;
}

// access nvar-th ring variable
BoolePolyRing::dd_type
BoolePolyRing::ddVariable(idx_type nvar) const {

  PBORI_TRACE_FUNC( "BoolePolyRing::ddVariable(idx_type) const" );

  return (*pMgr).variable(nvar);
}

// access nvar-th ring variable
BoolePolyRing::dd_type
BoolePolyRing::variable(idx_type nvar) const {

  PBORI_TRACE_FUNC( "BoolePolyRing::variable(idx_type) const" );

  dd_type dd( ddVariable(nvar) );
  size_type nlen = BoolePolyRing::nRingVariables();

  for(size_type i = 0; i < nlen; ++i)
    if (i != nvar)
      dd.subset0Assign(i);
 
  return dd;
}

// access nvar-th variable of the active ring
BoolePolyRing::dd_type
BoolePolyRing::ringVariable(idx_type nvar) {

  PBORI_TRACE_FUNC( "BoolePolyRing::ringVariable(idx_type nvar)" );

  self the_ring(current_mgr);
  return the_ring.variable(nvar); 

}

// get number of ring variables
// access nvar-th ring variable
BoolePolyRing::dd_type
BoolePolyRing::empty() const {

  PBORI_TRACE_FUNC( "BoolePolyRing::empty() const" );

  return pMgr->zeroDD();
}

// access nvar-th variable of the active ring
BoolePolyRing::dd_type
BoolePolyRing::ringEmpty() {

  PBORI_TRACE_FUNC( "BoolePolyRing::ringEmpty()" );

  return ring().empty(); 

}

// get number of ring variables
// access nvar-th ring variable
BoolePolyRing::dd_type
BoolePolyRing::full() const {

  PBORI_TRACE_FUNC( "BoolePolyRing::empty() const" );

  return pMgr->oneDD();
}

// access nvar-th variable of the active ring
BoolePolyRing::dd_type
BoolePolyRing::ringFull() {

  PBORI_TRACE_FUNC( "BoolePolyRing::ringEmpty()" );

  return ring().full(); 

}


BoolePolyRing::size_type
BoolePolyRing::nVariables() const {

  PBORI_TRACE_FUNC( "BoolePolyRing::nvVriables() const" );

  return pMgr->nVariables();
}

// get number of ring variables of the active ring
BoolePolyRing::size_type
BoolePolyRing::nRingVariables() {

  PBORI_TRACE_FUNC( "BoolePolyRing::nRingVariables() const" );

  return ring().nVariables();
}

// access current global ring setting
BoolePolyRing
BoolePolyRing::ring() {

  PBORI_TRACE_FUNC( "BoolePolyRing::ring() const" );

  if(current_mgr != NULL){
    return self(current_mgr);
  }
  else {
    throw PBoRiError(CTypes::no_ring);
  }
    

}



//  make this global ring
void
BoolePolyRing::activate() {

  PBORI_TRACE_FUNC( "BoolePolyRing::activate() const" );

  current_mgr = pMgr;
}

END_NAMESPACE_PBORI
