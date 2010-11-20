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
 *   (c) 2006 by The PolyBoRi Team
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.7  2007/11/06 15:03:36  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.6  2007/03/21 08:55:09  dreyer
 * ADD: first version of block_dlex running
 *
 * Revision 1.5  2006/10/06 12:52:01  dreyer
 * ADD easy_equility_property and used in lex_compare
 *
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

/**@class lex_tag
 * @brief Marker for lex ordering.
 **/
struct lex_tag {};

/**@class dlex_tag
 * @brief Marker for deg-lex ordering.
 **/
struct dlex_tag {};

/**@class dp_asc_tag
 * @brief Marker for ascending deg-rev-lex ordering.
 **/
struct dp_asc_tag {};

/**@class block_dlex_tag
 * @brief Marker for block deg-lex ordering.
 **/
struct block_dlex_tag {};

/**@class block_dp_asc_tag
 * @brief Marker for ascending block deg-rev-lex ordering.
 **/
struct block_dp_asc_tag {};

END_NAMESPACE_PBORI

#endif // order_tags_h_
