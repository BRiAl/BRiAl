// -*- c++ -*-
//*****************************************************************************
/** @file pbori_tags.h
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
#include "order_tags.h"

#ifndef pbori_tags_h_
#define pbori_tags_h_

BEGIN_NAMESPACE_PBORI

/** @class invalid_tag
 * @brief This class shows, whether a property of an order is invalid.
 **/
struct invalid_tag {};

/** @class valid_tag
 * @brief This class shows, whether a property of an order is valid.
 **/
struct valid_tag {};

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
