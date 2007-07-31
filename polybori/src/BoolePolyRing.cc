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
 * Revision 1.37  2007/07/31 07:43:50  dreyer
 * ADD: getBaseOrderCode(), lieInSameBlock(...), isSingleton(), isPair()...
 *
 * Revision 1.36  2007/06/21 11:35:58  dreyer
 * ADD: lastBlockBegin
 *
 * Revision 1.35  2007/03/21 08:55:09  dreyer
 * ADD: first version of block_dlex running
 *
 * Revision 1.34  2007/03/16 16:59:20  dreyer
 * CHANGE: started to rewrite CGenericIter using boost:iterator_facade
 *
 * Revision 1.33  2007/02/03 17:31:15  dreyer
 * FIX: deactivated workaround for old rings
 *
 * Revision 1.32  2007/01/23 12:37:21  dreyer
 * + Workaround for segfault after order change
 *
 * Revision 1.31  2006/12/19 12:36:46  dreyer
 * ADD: BoolePolyRing::clear{Ring}Cache()
 *
 * Revision 1.30  2006/11/27 16:25:14  dreyer
 * CHANGE: CDegreeCache, now inherited from standard cache; dlex-lead cached
 *
 * Revision 1.29  2006/10/24 06:39:38  dreyer
 * CHANGE: changed interface type
 *
 * Revision 1.28  2006/10/23 16:05:55  dreyer
 * ADD: BoolePolyRing::set/get(Ring)VariableName()
 *
 * Revision 1.27  2006/10/04 12:22:32  dreyer
 * ADD: getOrderCode()
 *
 * Revision 1.26  2006/10/02 09:28:38  dreyer
 * ADD BoolePolyRing::changeOrdering and infrastructure
 *
 * Revision 1.25  2006/09/05 11:10:45  dreyer
 * ADD: BoolePolyRing::Compare(...), fixed assertion in groebner
 *
 * Revision 1.24  2006/09/05 08:48:33  dreyer
 * ADD: BoolePolyRing::is(Total)DegreeOrder()
 *
 * Revision 1.23  2006/07/20 08:55:49  dreyer
 * ADD isOrdered() and  isLexicographical()
 *
 * Revision 1.22  2006/05/23 15:55:24  dreyer
 * CHANGE order of arguments in constructor
 *
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

#include "OrderedManager.h"

// get error types
# include "PBoRiError.h"

#include "pbori_order.h"

#include "BooleExponent.h"
#include "BooleMonomial.h"

#include "cuddInt.h"

BEGIN_NAMESPACE_PBORI

// initialize pointer to active ring
BoolePolyRing::manager_ptr BoolePolyRing::current_mgr;

#ifdef PBORI_KEEP_OLD_RINGS
std::list<BoolePolyRing::manager_ptr> BoolePolyRing::old_rings;
#endif

// interface with cudd's variable management
BoolePolyRing::BoolePolyRing(size_type nvars, ordercode_type order,
                             bool_type make_active) : base(),
  pMgr( get_ordered_manager(nvars, order) ) {

  PBORI_TRACE_FUNC( "BoolePolyRing(size_type)" );

  if(make_active)
    activate();
}

void
BoolePolyRing::changeOrdering(ordercode_type order) {

  PBORI_TRACE_FUNC( "changeOrdering(ordercode_type)" );

  BoolePolyRing the_ring(get_ordered_manager(activeManager(), order));
  the_ring.activate();
}


// copy constructor (shallow copy)
BoolePolyRing::BoolePolyRing(const BoolePolyRing& rhs) : base(),
  pMgr(rhs.pMgr) {
  
  PBORI_TRACE_FUNC( "BoolePolyRing(const BoolePolyRing&)" );

}

// construct from pointer to manager
BoolePolyRing::BoolePolyRing(manager_ptr pRhs) : base(),
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

// access persistend nvar-th ring variable
BoolePolyRing::dd_type
BoolePolyRing::persistentVariable(idx_type nvar) const {

  PBORI_TRACE_FUNC( "BoolePolyRing::variable(idx_type) const" );

  return pMgr->persistentVariable(nvar);
}

// access nvar-th variable of the active ring
BoolePolyRing::dd_type
BoolePolyRing::persistentRingVariable(idx_type nvar) {

  PBORI_TRACE_FUNC( "BoolePolyRing::ringVariable(idx_type nvar)" );

  // return ring().variable(nvar);
   return current_mgr->persistentVariable(nvar);
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

#ifdef PBORI_KEEP_OLD_RINGS
  if(current_mgr)
    old_rings.push_back(current_mgr);
#endif  
  current_mgr = pMgr;
}

// print statistics about the current ring
void
BoolePolyRing::printInfo() {

  PBORI_TRACE_FUNC( "BoolePolyRing::info() const" );

  return current_mgr->printInfo();
}

// test whether current ring's ordering is lexicographical
BoolePolyRing::bool_type 
BoolePolyRing::isLexicographical() {

  PBORI_TRACE_FUNC( "BoolePolyRing::isLexicographical()" );
  return activeManager().isLexicographical() ;
} 

// test whether current ring's iterators respect the ordering 
BoolePolyRing::bool_type 
BoolePolyRing::orderedStandardIteration() {

  PBORI_TRACE_FUNC( "BoolePolyRing::isOrdered()" );
  return activeManager().orderedStandardIteration() ;
}

// test whether variable pertubation do not change the order
BoolePolyRing::bool_type 
BoolePolyRing::isSymmetric() {

  PBORI_TRACE_FUNC( "BoolePolyRing::isSymmetric()" );
  return activeManager().isSymmetric() ;
}
// test whether we deal with a degree-ordering
BoolePolyRing::bool_type 
BoolePolyRing::isDegreeOrder() {

  PBORI_TRACE_FUNC( "BoolePolyRing::isDegreeOrder()" );
  return activeManager().isDegreeOrder() ;
}

// test whether we deal with a block-ordering
BoolePolyRing::bool_type 
BoolePolyRing::isBlockOrder() {

  PBORI_TRACE_FUNC( "BoolePolyRing::isBlockOrder()" );
  return activeManager().isBlockOrder() ;
}

// test whether we deal with a degree-ordering
BoolePolyRing::bool_type 
BoolePolyRing::isTotalDegreeOrder() {

  PBORI_TRACE_FUNC( "BoolePolyRing::isTotalDegreeOrder()" );
  return activeManager().isTotalDegreeOrder() ;
}

// test whether we deal with a degree-ordering
BoolePolyRing::bool_type 
BoolePolyRing::isDegreeReverseLexicograpical() {

  PBORI_TRACE_FUNC( "BoolePolyRing::isDegreeReverseLexicograpical()" );
  return activeManager().isDegreeReverseLexicograpical() ;
}

// test whether we deal with a degree-ordering
BoolePolyRing::ordercode_type
BoolePolyRing::getOrderCode() {

  PBORI_TRACE_FUNC( "BoolePolyRing::getOrderCode()" );
  return activeManager().getOrderCode() ;
}
BoolePolyRing::ordercode_type
BoolePolyRing::getBaseOrderCode() {

  PBORI_TRACE_FUNC( "BoolePolyRing::getBaseOrderCode()" );
  return activeManager().getBaseOrderCode() ;
}
// test whether we deal with a degree-ordering
BoolePolyRing::bool_type 
BoolePolyRing::ascendingVariables() {

  PBORI_TRACE_FUNC( "BoolePolyRing::ascendingVariables()" );
  return activeManager().ascendingVariables() ;
}

// test whether we deal with a degree-ordering
BoolePolyRing::bool_type 
BoolePolyRing::descendingVariables() {

  PBORI_TRACE_FUNC( "BoolePolyRing::descendingVariables()" );
  return activeManager().descendingVariables() ;
}

// test whether two indices are in the same block
BoolePolyRing::bool_type 
BoolePolyRing::lieInSameBlock(idx_type first, idx_type second) {

  PBORI_TRACE_FUNC( "BoolePolyRing::lieInSameBlock()" );
  return activeManager().lieInSameBlock(first, second);
}

// order-dependent comparison
BoolePolyRing::comp_type 
BoolePolyRing::compare(idx_type lhs, idx_type rhs) {

  PBORI_TRACE_FUNC( "BoolePolyRing::compare(idx_type, idx_type)" );
  return activeManager().compare(lhs, rhs);
}

BoolePolyRing::comp_type 
BoolePolyRing::compare(const monom_type& lhs, const monom_type& rhs) {

  PBORI_TRACE_FUNC( "BoolePolyRing::compare(const monom_type&, const monom_type&)" );
  return activeManager().compare(lhs, rhs);
}

BoolePolyRing::comp_type 
BoolePolyRing::compare(const exp_type& lhs, const exp_type& rhs) {

  PBORI_TRACE_FUNC( "BoolePolyRing::compare(const exp_type&, const exp_type&)");
  return activeManager().compare(lhs, rhs);
}

void
BoolePolyRing::setVariableName(idx_type idx, vartext_type varname) {

  PBORI_TRACE_FUNC( "setVariableName(idx_typ, vartext_type)");
  pMgr->setVariableName(idx, varname);
}


void
BoolePolyRing::setRingVariableName(idx_type idx, vartext_type varname) {

  PBORI_TRACE_FUNC( "setRingVariableName(idx_typ, const vartext_type&)");
  activeManager().setVariableName(idx, varname);
}


BoolePolyRing::vartext_type
BoolePolyRing::getVariableName(idx_type idx) {

  PBORI_TRACE_FUNC( "getVariableName(idx_typ)");
  return pMgr->getVariableName(idx);
}


BoolePolyRing::vartext_type
BoolePolyRing::getRingVariableName(idx_type idx) {

  PBORI_TRACE_FUNC( "getRingVariableName(idx_typ)");
  return activeManager().getVariableName(idx);
}



BoolePolyRing::block_iterator 
BoolePolyRing::blockRingBegin() {

  PBORI_TRACE_FUNC( "BoolePolyRing::blockRingBegin() const");
  return activeManager().blockBegin();
}

BoolePolyRing::block_iterator
BoolePolyRing::blockRingEnd() {

  PBORI_TRACE_FUNC( "BoolePolyRing::blockRingEnd() const");
  return activeManager().blockEnd();
}

void BoolePolyRing::appendRingBlock(idx_type idx) {

  PBORI_TRACE_FUNC( "BoolePolyRing::appendRingBlock(idx_type)");
  return activeManager().appendBlock(idx);
}

void BoolePolyRing::clearRingBlocks() {

  PBORI_TRACE_FUNC( "BoolePolyRing::clearRingBlocks()");
  return activeManager().clearBlocks();
}

BoolePolyRing::idx_type
BoolePolyRing::lastBlockStart() {

  PBORI_TRACE_FUNC( "BoolePolyRing::lastBlockStart() const");
  if (isBlockOrder()) {
    return *(blockRingEnd() - 2);
  }
  else if (isLexicographical()) {
    return CTypes::max_idx;
  }
  return 0;
}

void
BoolePolyRing::clearCache() {

  PBORI_TRACE_FUNC( "clearCache()");
  cuddCacheFlush(pMgr->manager().getManager());
}


void
BoolePolyRing::clearRingCache() {

  PBORI_TRACE_FUNC( "clearRingCache()");
  cuddCacheFlush(activeManager().manager().getManager());
}

END_NAMESPACE_PBORI
