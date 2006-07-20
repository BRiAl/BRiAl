// -*- c++ -*-
//*****************************************************************************
/** @file order_traits.h
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

// include orderings
#include "LexOrder.h"

#ifndef order_traits_h_
#define order_traits_h_

BEGIN_NAMESPACE_PBORI

/** @class lex_tag
 * @brief This class marks an ordering lexicographical
 **/
struct lex_tag {};


struct 

/** @class order_traits 
 * @brief This class contains properties of polybori ordering classes
 *
 **/
template <class OrderType>
class order_traits {
 public:
  typedef CTypes::bool_type bool_type;

  bool_type isLexicographical() const { return false; }
  bool_type isLexicographical() const { return false; }
};


END_NAMESPACE_PBORI

#endif // order_traits_h_
