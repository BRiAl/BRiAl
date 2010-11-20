// -*- c++ -*-
//*****************************************************************************
/** @file CCuddNavigator.cc
 *
 * @author Alexander Dreyer
 * @date 2006-03-26
 *
 * This file implements an iterator-like class over the first minimal term of a
 * given ZDD node.
 * @note: Tested for Cudd's ZDDs only.
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
 * Revision 1.9  2008/07/13 22:49:36  dreyer
 * Fix: Doxygen clean-up
 *
 * Revision 1.8  2007/11/06 15:03:39  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.7  2006/11/21 10:33:22  dreyer
 * CHANGE: added inlinable member functions
 *
 * Revision 1.6  2006/08/01 11:12:22  dreyer
 * CHANGE: removed ';' at end of functions (pedantic)
 *
 * Revision 1.5  2006/06/07 06:54:30  bricken
 * + 64BIT workaround
 *
 * Revision 1.4  2006/04/25 07:05:02  dreyer
 * FIX postfix++ of CCuddFirstIter
 *
 * Revision 1.3  2006/04/06 14:10:59  dreyer
 * ADD hash_type and CCuddNavigator::hash()
 *
 * Revision 1.2  2006/04/06 13:05:54  dreyer
 * CHANGE more suitable names for CCuddNavigator functions
 *
 * Revision 1.1  2006/04/04 15:31:07  dreyer
 * ADD: BoolePolynomial::navigator() and corresp. class CCuddNavigator
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "CCuddNavigator.h"

BEGIN_NAMESPACE_PBORI

/* @class CCuddNavigator
 * @brief This class defines an iterator over the first minimal term of a given
 * ZDD node.
 *
 **/

// All functions inline now.

END_NAMESPACE_PBORI
