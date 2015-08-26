// -*- c++ -*-
//*****************************************************************************
/** @file pbori_routines.h
 *
 * @author Alexander Dreyer
 * @date 2006-08-23
 *
 * This file includes files, which define function templates for internal use 
 * in the polybori library.
 *
 * @par Copyright:
 *   (c) 2006 by The PolyBoRi Team
**/
//*****************************************************************************

#ifndef polybori_routines_pbori_routines_h_
#define polybori_routines_pbori_routines_h_

// include basic definitions
#include <polybori/pbori_defs.h>

// include polybori algorithms and functionals
#include "pbori_algo.h"
#include "pbori_func.h"

// Get routines, which add features related to decision diagrams
#include "pbori_routines_dd.h"

// Get routines, which add features related to Cudd library
#include "pbori_routines_cuddext.h"

// Get additional routines
#include "pbori_routines_misc.h"

// Get order-related routines
#include "pbori_routines_order.h"

// Get hash-related routines
#include "pbori_routines_hash.h"

#endif
