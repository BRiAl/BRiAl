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


/** @class is_lex_tagged
 * @brief This class determins lexicographical properties 
 *
 **/
template <class TagType>
class is_lex_tagged;


template <>
class is_lex_tagged<lex_tag>{
public:
  /// Type for booleans
  typedef CTypes::bool_type bool_type;  

  /// The actual test for lexicographical orderings
  bool_type operator()() const {
    return true;
  }
};

template <>
class is_lex_tagged<nonlex_tag>{
public:
  /// Type for booleans
  typedef CTypes::bool_type bool_type;  

  /// The actual test for lexicographical orderings
  bool_type operator()() const {
    return false;
  }
};

/** @class is_ordered_tagged
 * @brief This class determines orderes properties of polybori ordering classes
 *
 **/
template <class TagType>
class is_ordered_tagged;


template <>
class is_ordered_tagged<ordered_tag>{
public:
  /// Type for booleans
  typedef CTypes::bool_type bool_type;  

  /// The actual test whether iterated respect the order
  bool_type operator()() const {
    return true;
  }
};

template <>
class is_ordered_tagged<nonordered_tag>{
public:
  /// Type for booleans
  typedef CTypes::bool_type bool_type;  

  /// The actual test whether iterated respect the order
  bool_type operator()() const {
    return false;
  }
};

END_NAMESPACE_PBORI

#endif // order_tags_h_
