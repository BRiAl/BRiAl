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
**/
//*****************************************************************************

// include basic definitions
#include <polybori/pbori_defs.h>

#include <cudd/cudd.h>
#include <cudd/cuddInt.h>

BEGIN_NAMESPACE_PBORI

// dummy for cuddcache
DdNode* 
pboriCuddZddUnionXor__(DdManager *, DdNode *, DdNode *){ return NULL; } // LCOV_EXCL_LINE

END_NAMESPACE_PBORI

