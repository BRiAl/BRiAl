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
