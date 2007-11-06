// -*- c++ -*-
//*****************************************************************************
/** @file pbori_routines.cc
 *
 * @author Alexander Dreyer
 * @date 2006-10-26
 *
 * This file includes some compiled routines.
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
 * Revision 1.9  2007/11/06 15:03:39  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.8  2007/07/27 14:38:40  dreyer
 * CHANGE: Addition internally inlined
 *
 * Revision 1.7  2007/07/06 14:04:22  dreyer
 * ADD: newly written C++_interface for Cudd
 *
 * Revision 1.6  2006/11/24 14:49:01  dreyer
 * CHANGE: divisorsOf (less recursions/cache-lookups)
 *
 * Revision 1.5  2006/11/22 15:46:22  dreyer
 * ADD: CacheManager replacing CCacheManagement for external use
 * CHANGE: CacheManager used, where necessary
 * CHANGE: multiplesOf, uses cached recursion
 *
 * Revision 1.4  2006/11/22 10:10:23  dreyer
 * ADD: dd_first_divisors_of
 *
 * Revision 1.3  2006/10/31 09:18:45  dreyer
 * FIX: Bug (union-xor) on 64-bit systems should be fixed now
 *
 * Revision 1.2  2006/10/30 13:30:33  dreyer
 * FIX: library compiles for PBORI_ADD_BY_* switches, not using *XOR
 *
 * Revision 1.1  2006/10/26 12:58:25  dreyer
 * ADD: lowlevel routine for union-xor (easy cudd-style variant)
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

#include "cudd.h"
#include "cuddInt.h"
#include "pbori_algo.h"

#include "CacheManager.h"
#include "CCuddGetNode.h"

BEGIN_NAMESPACE_PBORI

// dummy for cuddcache
DdNode* 
pboriCuddZddUnionXor__(DdManager *, DdNode *, DdNode *){ return NULL; }

END_NAMESPACE_PBORI

