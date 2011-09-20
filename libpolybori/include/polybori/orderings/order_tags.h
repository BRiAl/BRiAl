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

#ifndef polybori_orderings_order_tags_h_
#define polybori_orderings_order_tags_h_

// include basic definitions
#include <polybori/pbori_defs.h>


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

/**@class block_tag
 * @brief Marker for block orderings.
 **/
template <class TagType>
struct block_tag {};

/// Marker for block deg-lex ordering.
typedef block_tag<dlex_tag> block_dlex_tag;

/// Marker for ascending block deg-rev-lex ordering.
typedef block_tag<dp_asc_tag> block_dp_asc_tag;

END_NAMESPACE_PBORI

#endif // order_tags_h_
