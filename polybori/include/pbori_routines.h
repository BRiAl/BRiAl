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
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.6  2007/11/06 15:03:37  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.5  2006/09/20 07:06:39  dreyer
 * ADD BoolePolynomial/CDDInterface::isConstant(), used it in deg()
 *
 * Revision 1.4  2006/09/14 10:56:49  dreyer
 * ADD: generic hash
 *
 * Revision 1.3  2006/09/04 15:58:43  dreyer
 * ADD: DegLexOrder and preparations
 *
 * Revision 1.2  2006/08/24 14:47:50  dreyer
 * ADD: BooleExponent integrated, FIX: multiples (for indices < first)
 *
 * Revision 1.1  2006/08/23 14:24:54  dreyer
 * ADD: BooleSet::usedVariables and infrastructure
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

// include polybori algorithms and functionals
#include "pbori_algo.h"
#include "pbori_func.h"

#ifndef PBORI_ROUTINES_H_
#define PBORI_ROUTINES_H_


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
