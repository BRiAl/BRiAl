// -*- c++ -*-
//*****************************************************************************
/** @file COrderProperties.h
 *
 * @author Alexander Dreyer
 * @date 2006-07-20
 *
 * 
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

// include ordering tags
#include "order_tags.h"

BEGIN_NAMESPACE_PBORI

template <class OrderType>
class COrderProperties {

 public:
  /// Get type of ordering
  typedef OrderType order_type;

  /// @name define generic property markers
  //@{
  typedef typename order_type::lex_property lex_property; 
  typedef typename order_type::ordered_property ordered_property;
  //@}

  /// Define test whether iterators respect the ordering
  is_ordered_tagged<ordered_property> isOrdered;

  /// Define test whether ordering is lexicographical
  is_lex_tagged<lex_property> isLexicographical;
};


END_NAMESPACE_PBORI
