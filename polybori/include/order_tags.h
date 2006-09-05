// -*- c++ -*-
//*****************************************************************************
/** @file order_tags.h
 *
 * @author Alexander Dreyer
 * @date 2006-07-20
 *
 * Decribes properties of polybori ordering classes
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
 * Revision 1.4  2006/09/05 08:48:32  dreyer
 * ADD: BoolePolyRing::is(Total)DegreeOrder()
 *
 * Revision 1.3  2006/09/01 11:02:48  dreyer
 * ADD: OrderedManager::isSymmetric()
 *
 * Revision 1.2  2006/07/20 10:13:59  dreyer
 * CHANGE: Made COrderProperties more generic
 *
 * Revision 1.1  2006/07/20 08:55:49  dreyer
 * ADD isOrdered() and  isLexicographical()
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

#ifndef order_tags_h_
#define order_tags_h_

BEGIN_NAMESPACE_PBORI


/** @class invalid_tag
 * @brief This class shows, whether a property of an order is invalid.
 **/
struct invalid_tag {};

/** @class valid_tag
 * @brief This class shows, whether a property of an order is valid.
 **/
struct valid_tag {};

END_NAMESPACE_PBORI

#endif // order_tags_h_
