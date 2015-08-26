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
**/
//*****************************************************************************

#ifndef polybori_iterators_CStackSelector_h_
#define polybori_iterators_CStackSelector_h_

// include basic definitions
#include <polybori/pbori_defs.h>

// include polybori's functionals
#include <polybori/routines/pbori_func.h>

// get stack types, which are used to store and handle paths of current terms
#include "CTermStack.h"

// include standard iterator functionality
#include <iterator>

#include <polybori/orderings/COrderingTags.h>

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
  typedef COrderingTags<typename OrderType::order_tag> orderings_tags;

  typedef typename orderings_tags::descending_property descending;
  typedef typename orderings_tags::degorder_property deg_prop;
  typedef typename orderings_tags::blockorder_property block_prop;
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
