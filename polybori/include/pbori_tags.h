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
 * Revision 1.2  2006/12/13 18:07:04  dreyer
 * ADD: poly /= exp
 *
 * Revision 1.1  2006/10/06 12:52:01  dreyer
 * ADD easy_equility_property and used in lex_compare
 *
 * @endverbatim
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
