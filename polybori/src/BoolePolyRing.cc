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
 *   (c) 2006 by The PolyBoRi Team
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.41  2007/12/13 15:53:49  dreyer
 * CHANGE: Ordering in BoolePolyRing again; BooleEnv manages active ring
 *
 * Revision 1.40  2007/12/11 15:37:35  dreyer
 * ADD: BooleOrdering started
 *
 * Revision 1.39  2007/12/07 17:06:19  dreyer
 * CHANGE: First try: ring and order separated
 *
 * Revision 1.38  2007/11/06 15:03:38  dreyer
 * CHANGE: More generic copyright
 *
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
#include "BooleEnv.h"
#include "OrderedManager.h"

// get error types
# include "PBoRiError.h"

#include "pbori_order.h"

#include "BooleExponent.h"
#include "BooleMonomial.h"

#include "cuddInt.h"

BEGIN_NAMESPACE_PBORI

// interface with cudd's variable management
BoolePolyRing::BoolePolyRing(size_type nvars, ordercode_type order,
                             bool_type make_active) : 
  base(),
  pMgr(CTypes::manager_ptr(new CNamedManager<CTypes::manager_base>(nvars))),
  pOrder(get_ordering(order)) {

  PBORI_TRACE_FUNC( "BoolePolyRing(size_type)" );
  if(make_active)
    activate();
}

void
BoolePolyRing::changeOrdering(ordercode_type order) {

  PBORI_TRACE_FUNC( "changeOrdering(ordercode_type)" );

  pOrder = get_ordering(order); 
}


//  make this global ring
void
BoolePolyRing::activate() {

  PBORI_TRACE_FUNC( "BoolePolyRing::activate() const" );

  BooleEnv::set(*this);

}


END_NAMESPACE_PBORI
