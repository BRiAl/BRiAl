// -*- c++ -*-
//*****************************************************************************
/** @file tags.h
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

#ifndef polybori_common_common_tags_h_
#define polybori_common_common_tags_h_

// include basic definitions
#include <polybori/pbori_defs.h>
#include <polybori/orderings/order_tags.h>

BEGIN_NAMESPACE_PBORI

/** @class invalid_tag
 * @brief This class shows, whether a property of an order is invalid.
 **/
struct invalid_tag { };

/** @class valid_tag
 * @brief This class shows, whether a property of an order is valid.
 **/
struct valid_tag { };


/** @class invert_tag
 * @brief Changes valid_tag -> invalid_tag and vice versa.
 **/
template <class Tag>
struct invert_tag;

template <>
struct invert_tag<valid_tag> { typedef invalid_tag type; };

template <>
struct invert_tag<invalid_tag> { typedef valid_tag type; };

/** @class and_tag
 * @brief and_tag<valid_tag, valid_tag> -> valid_tag, otherwise invalid_tag.
 **/
template <class Tag1, class Tag2>
struct and_tag { typedef invalid_tag type; };

template <>
struct and_tag<valid_tag, valid_tag> { typedef valid_tag type; };


/** @class navigator_tag
 * @brief for iterator_category
 **/
struct navigator_tag {};

/** @class easy_equality_property
 *  @brief defines generic base for properties
 **/
template <class ValidityTag>
class equality_property {
public:

  /// Set marker for validity
  typedef ValidityTag easy_equality_property;
};


END_NAMESPACE_PBORI

#endif // order_tags_h_
