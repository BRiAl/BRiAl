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
 * Revision 1.21  2006/05/23 15:26:25  dreyer
 * CHANGE BoolePolyRing  can handle different orderings (only lex yet)
 *
 * Revision 1.20  2006/04/11 09:26:57  dreyer
 * ADD  BoolePolyRing::printInfo();
 *
 * Revision 1.19  2006/03/30 11:57:11  dreyer
 * CHANGE: Made use of 0/1 constants and the sets {}, {{}} consistent
 *
 * Revision 1.18  2006/03/29 16:26:46  dreyer
 * ADD: Class CCuddFirstIter used for BoolePolynomial::lead()
 *
 * Revision 1.17  2006/03/27 15:18:09  dreyer
 * + Removed some debug code.
 *
 * Revision 1.16  2006/03/27 13:47:58  dreyer
 * ADD operator + and *, CHANGE BoolePolyRing::variable(i) generation
 *
 * Revision 1.15  2006/03/24 15:02:44  dreyer
 * ADD: Reference to manager_type can also be used for CDDManager<> -nterface
 * ADD: lead(), (n)usedVariables(), lmDeg() implemented in BoolePolynomial
 *
 * Revision 1.14  2006/03/23 17:15:04  dreyer
 * ADD: lead() and lmdeg() functionality to BoolePolynomial,
 * BoolePolyRing(const manager_type &); leading term exampl.
 *
 * Revision 1.13  2006/03/23 09:23:11  dreyer
 * ADD: pbori_shared_ptr_postclean, used by ~BoolePolyRing() to clean
 * current_mgr, if pbori_shared_ptr<> is used (not for boost::shared_ptr<>).
 *
 * Revision 1.12  2006/03/22 16:48:13  dreyer
 * ADD alternative to shared_ptr (if not available)
 *
 * Revision 1.11  2006/03/22 08:06:59  dreyer
 * ADD: Template specializations CDDInterface<ZDD>, CDDManager<Cudd>; 
 * ring uses shared_ptr now
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

#include "pbori_order.h"

BEGIN_NAMESPACE_PBORI

// initialize pointer to active ring
BoolePolyRing::manager_ptr BoolePolyRing::current_mgr;

// interface with cudd's variable management
BoolePolyRing::BoolePolyRing(size_type nvars, bool_type make_active,
                             ordercode_type order) :
  pMgr( get_ordered_manager(nvars, order) ) {

  PBORI_TRACE_FUNC( "BoolePolyRing(size_type)" );

  if(make_active)
    activate();
}

// copy constructor (shallow copy)
BoolePolyRing::BoolePolyRing(const BoolePolyRing& rhs) :
  pMgr(rhs.pMgr) {
  
  PBORI_TRACE_FUNC( "BoolePolyRing(const BoolePolyRing&)" );

}

// construct from pointer to manager
BoolePolyRing::BoolePolyRing(manager_ptr pRhs) :
  pMgr(pRhs) {
  
  PBORI_TRACE_FUNC( "BoolePolyRing(manager_ptr)" );
}

// // construct from manager
// BoolePolyRing::BoolePolyRing(const manager_type& manager) :
//   pMgr( new manager_type(manager) ) {

//   PBORI_TRACE_FUNC( "BoolePolyRing(const manager_type&)" );
// }

// destructor
BoolePolyRing::~BoolePolyRing() {

  PBORI_TRACE_FUNC( "~BoolePolyRing()" );
  // deconstruction is managed using the shared pointer class.

  // Clean current_mgr, if necessary
  pbori_shared_ptr_postclean(pMgr, current_mgr);
}

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
BoolePolyRing::ringDdVariable(idx_type nvar) {

  PBORI_TRACE_FUNC( "BoolePolyRing::ringDdVariable(idx_type)" );

  return current_mgr->ddVariable(nvar);
}

// access nvar-th ring variable
BoolePolyRing::dd_type
BoolePolyRing::ddVariable(idx_type nvar) const {

  PBORI_TRACE_FUNC( "BoolePolyRing::ddVariable(idx_type) const" );

  return pMgr->ddVariable(nvar);
}

// access nvar-th ring variable
BoolePolyRing::dd_type
BoolePolyRing::variable(idx_type nvar) const {

  PBORI_TRACE_FUNC( "BoolePolyRing::variable(idx_type) const" );

  return pMgr->variable(nvar);
}

// access nvar-th variable of the active ring
BoolePolyRing::dd_type
BoolePolyRing::ringVariable(idx_type nvar) {

  PBORI_TRACE_FUNC( "BoolePolyRing::ringVariable(idx_type nvar)" );

  // return ring().variable(nvar);
   return current_mgr->variable(nvar);
}

// get empty decision diagram
BoolePolyRing::dd_type
BoolePolyRing::zero() const {

  PBORI_TRACE_FUNC( "BoolePolyRing::zero() const" );

  return pMgr->empty();
}

// get empty decision diagram of the active ring
BoolePolyRing::dd_type
BoolePolyRing::ringZero() {

  PBORI_TRACE_FUNC( "BoolePolyRing::ringZero()" );

  return current_mgr->empty();
}

// get decision diagram with all variables negated
BoolePolyRing::dd_type
BoolePolyRing::one() const {

  PBORI_TRACE_FUNC( "BoolePolyRing::empty() const" );

  return pMgr->blank();
}

// get decision diagram with all variables negated of the active ring
BoolePolyRing::dd_type
BoolePolyRing::ringOne() {

  PBORI_TRACE_FUNC( "BoolePolyRing::ringEmpty()" );

  return current_mgr->blank();
}


BoolePolyRing::size_type
BoolePolyRing::nVariables() const {

  PBORI_TRACE_FUNC( "BoolePolyRing::nVariables() const" );

  return pMgr->nVariables();
}

// get number of ring variables of the active ring
BoolePolyRing::size_type
BoolePolyRing::nRingVariables() {

  PBORI_TRACE_FUNC( "BoolePolyRing::nRingVariables() const" );

  return current_mgr->nVariables();
}

// access current global ring setting
BoolePolyRing
BoolePolyRing::ring() {

  PBORI_TRACE_FUNC( "BoolePolyRing::ring() const" );

  if(current_mgr){
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

// print statistics about the current ring
void
BoolePolyRing::printInfo() {

  PBORI_TRACE_FUNC( "BoolePolyRing::info() const" );

  return current_mgr->printInfo();
}
END_NAMESPACE_PBORI
