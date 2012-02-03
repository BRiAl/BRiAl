// -*- c++ -*-
//*****************************************************************************
/** @file include/polybori/polybori.h 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file includes all header files, which are necessary
 * for using the polybori lib.
 *
 * @par Copyright:
 *   (c) 2006 by The PolyBoRi Team
**/
//*****************************************************************************

#ifndef polybori_polybori_h_
#define polybori_polybori_h_

#include "BooleSet.h"
#include "BoolePolyRing.h"
#include "BooleEnv.h"
#include "BoolePolynomial.h"
#include "BooleMonomial.h"
#include "BooleVariable.h"
#include "BooleExponent.h"
#include "ring/WeakRingPtr.h"

#include "except/PBoRiError.h"
#include "except/PBoRiGenericError.h"

#include "iterators/CGenericIter.h"
#include "iterators/CExpIter.h"

#include "routines/pbori_algorithms.h"
#include "orderings/pbori_order.h"
#include "orderings/pbori_order.h"

#include "factories/MonomialFactory.h"
#include "factories/PolynomialFactory.h"
#include "factories/SetFactory.h"
#include "factories/VariableBlock.h"
#include "factories/VariableFactory.h"

#endif // of #ifndef polybori_polybori_h_
