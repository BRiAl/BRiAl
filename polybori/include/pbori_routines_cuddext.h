// -*- c++ -*-
//*****************************************************************************
/** @file pbori_routines_cuddext.h
 *
 * @author Alexander Dreyer
 * @date 2006-08-23
 *
 * This file includes files, which define function templates related to
 * decision diagrams. It should be loaded from pbori_routines.h only
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
 * Revision 1.2  2006/10/26 12:58:25  dreyer
 * ADD: lowlevel routine for union-xor (easy cudd-style variant)
 *
 * Revision 1.1  2006/08/23 14:24:54  dreyer
 * ADD: BooleSet::usedVariables and infrastructure
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"


BEGIN_NAMESPACE_PBORI
extern "C" {
  extern DdNode * pboriCudd_zddUnionXor (DdManager *dd, DdNode *P, DdNode *Q);
}

END_NAMESPACE_PBORI
