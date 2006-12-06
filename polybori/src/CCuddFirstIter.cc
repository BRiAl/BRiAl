// -*- c++ -*-
//*****************************************************************************
/** @file CCuddFirstIter.cc
 *
 * @author Alexander Dreyer
 * @date 2006-03-26
 *
 * This file implements an stl-like iterator for accessing the nonzero indices
 * of the leading (first) minimal term of Cudd decision diagrams.
 *
 * @note: Tested for Cudd's ZDDs only.
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
 * Revision 1.7  2006/12/06 09:20:09  dreyer
 * CHANGE: poly * exp now recursive
 *
 * Revision 1.6  2006/08/01 11:12:22  dreyer
 * CHANGE: removed ';' at end of functions (pedantic)
 *
 * Revision 1.5  2006/04/25 07:05:02  dreyer
 * FIX postfix++ of CCuddFirstIter
 *
 * Revision 1.4  2006/04/06 13:05:54  dreyer
 * CHANGE more suitable names for CCuddNavigator functions
 *
 * Revision 1.3  2006/04/04 15:31:07  dreyer
 * ADD: BoolePolynomial::navigator() and corresp. class CCuddNavigator
 *
 * Revision 1.2  2006/03/30 08:59:42  dreyer
 * FIX: CCuddFirstIter works for empty and zero polynomials now
 *
 * Revision 1.1  2006/03/29 16:26:46  dreyer
 * ADD: Class CCuddFirstIter used for BoolePolynomial::lead()
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "CCuddFirstIter.h"

BEGIN_NAMESPACE_PBORI

// moved everything to inline functions




END_NAMESPACE_PBORI
