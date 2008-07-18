// -*- c++ -*-
//*****************************************************************************
/** @file CStackSelector.h
 *
 * @author Alexander Dreyer
 * @date 2007-05-03
 *
 * This file defines the template class CStackSelector, whose only use is to
 * generate the correct (term-)stack type for a given ordering.
 *
 * @par Copyright:
 *   (c) 2007 by The PolyBoRi Team
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.4  2008/07/18 22:37:50  dreyer
 * Fix: doxygen clean-up (removed inclusion loop)
 *
 * Revision 1.3  2007/11/06 15:03:35  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.2  2007/05/03 16:09:06  dreyer
 * ADD: file description
 *
 * Revision 1.1  2007/05/03 16:04:45  dreyer
 * CHANGE: new-style CTermIter integrated
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

// include polybori's functionals
#include "pbori_func.h"

// get stack types, which are used to store and handle paths of current terms
#include "CTermStack.h"

// include standard iterator functionality
#include <iterator>


#ifndef CStackSelector_h_
#define CStackSelector_h_

BEGIN_NAMESPACE_PBORI
/** @class CStackSelector
 *
 **/
template <class OrderType, class NaviType, class BaseType = internal_tag> 
class CStackSelector {

  /// @name Abbreviations for internal types and tags
  //@{
  typedef BaseType base;
  typedef NaviType navigator;
  typedef typename OrderType::descending_property descending;
  typedef typename OrderType::degorder_property deg_prop;
  typedef typename OrderType::blockorder_property block_prop;
  //@}

  /// Natural and lexicographical iterator's stack
  typedef CTermStack<navigator, std::forward_iterator_tag, base> lex_type;

  /// Iterator's stack for degree-orderings
  typedef CDegTermStack<navigator, descending, invalid_tag, base> deg_type;

  /// Iterator's stack for block-orderings
  typedef CBlockTermStack<navigator, descending, base> block_type;

  /// Abbreviation for non-block orderings
  typedef typename on_same_type<deg_prop, valid_tag,
                                deg_type, lex_type>::type nonblock_type;
  
public:
  /// The resulting type for iterator's stack
  typedef typename on_same_type<block_prop, valid_tag,
                                block_type, nonblock_type>::type type;
};


END_NAMESPACE_PBORI

#endif
