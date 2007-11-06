// -*- c++ -*-
//*****************************************************************************
/** @file polybori.h 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file includes all header files, which are necessary
 * for using the polybori lib.
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
 * Revision 1.10  2007/11/06 15:03:37  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.9  2007/05/11 11:38:42  dreyer
 * ADD: started pbori_algorithms.h and term_accumulation()
 *
 * Revision 1.8  2007/05/03 16:04:46  dreyer
 * CHANGE: new-style CTermIter integrated
 *
 * Revision 1.7  2006/10/04 13:09:56  dreyer
 * ADD: added compile-time optimied iterators and genericBegin/genericEnd
 *
 * Revision 1.6  2006/08/24 14:47:50  dreyer
 * ADD: BooleExponent integrated, FIX: multiples (for indices < first)
 *
 * Revision 1.5  2006/04/20 08:31:21  dreyer
 * ADD BooleMonomial::mulples(...)
 *
 * Revision 1.4  2006/04/19 15:55:53  dreyer
 * ADD BooleMonomial, BoolePolynomial::fetchTerms() and ::terms()
 *
 * Revision 1.3  2006/03/16 17:09:13  dreyer
 * ADD BoolePolynial functionality started
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

#ifndef polybori_h_
#define polybori_h_

#include "BooleSet.h"
#include "BoolePolyRing.h"
#include "BoolePolynomial.h"
#include "BooleMonomial.h"
#include "BooleVariable.h"
#include "BooleExponent.h"
#include "PBoRiError.h"

#include "CGenericIter.h"
#include "CExpIter.h"

#include "pbori_algorithms.h"
#endif // of #ifndef polybori_h_
