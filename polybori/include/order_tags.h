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


/** @class same_property
 * @brief This classgenerate
 **/
template <bool Value>
class property_holds {
public:
  /// Type for booleans
  typedef CTypes::bool_type bool_type;  
  enum { value = Value };

  /// Return given value
  bool_type operator()() const {
    return value;
  }
};





/** @class lex_tag
 * @brief This class marks an ordering as being lexicographical
 **/
struct lex_tag {};


/** @class lex_tag
 * @brief This class marks an ordering as being non-lexicographical
 **/
struct nonlex_tag {};

/** @class ordered_tag
 * @brief This class shows, whether iterators respect the order
 **/
struct ordered_tag {};

/** @class ordered_tag
 * @brief This class shows, whether iterators respect the order
 **/
struct nonordered_tag {};

/** @class symmetric_tag
 * @brief This class shows, that an order is symmetric wrt. reorderung
 * variables.
 **/
struct symmetric_tag {};

/** @class nonsymmetric_tag
 * @brief This class shows, that an order is not symmetric wrt. reorderung
 * variables.
 **/
struct nonsymmetric_tag {};

END_NAMESPACE_PBORI

#endif // order_tags_h_
