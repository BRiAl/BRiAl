// -*- c++ -*-
//*****************************************************************************
/** @file COrderProperties.h
 *
 * @author Alexander Dreyer
 * @date 2006-07-20
 *
 * @todo Unused, to be removed?
 *
 * @par Copyright:
 *   (c) 2006 by The PolyBoRi Team
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

// include ordering tags
#include "pbori_tags.h"

// include polybori functionals
#include "pbori_func.h"

#ifndef COrderProperties_h_
#define COrderProperties_h_

BEGIN_NAMESPACE_PBORI

/** @class COrderProperties
 * @brief This class determins ordering dependent properties 
 *
 **/
template <class OrderType>
class COrderProperties {

 public:
  /// Get type of ordering
  typedef OrderType order_type;

  /// @name define generic property markers
  //@{
  typedef typename order_type::lex_property lex_property; 
  typedef typename order_type::ordered_property ordered_property;
  typedef typename order_type::symmetry_property symmetry_property;
  typedef typename order_type::degorder_property degorder_property;
  typedef typename order_type::blockorder_property blockorder_property;
  typedef typename order_type::totaldegorder_property totaldegorder_property;
  typedef typename order_type::ascending_property ascending_property;
  typedef typename order_type::descending_property descending_property;
  typedef typename order_type::degrevlexorder_property degrevlexorder_property;
  //@}

   /// @name Adopt auxiliary type definitions
  //@{
  typedef typename order_type::size_type size_type;
  typedef typename order_type::idx_type idx_type;
  typedef typename order_type::bool_type bool_type;
  typedef typename order_type::ostream_type ostream_type;
  typedef typename order_type::hash_type hash_type;
  //@}

  /// Get order code
  enum { order_code = order_type::order_code };

  /// Get binary predicate for index comparision
  typedef typename order_type::idx_comparer_type idx_comparer_type;
  
  /// Define test whether iterators respect the ordering
  is_same_type<ordered_property, valid_tag> orderedStandardIteration;

  /// Define test whether ordering is lexicographical
  is_same_type<lex_property, valid_tag> isLexicographical;

  /// Define test whether ordering is symmetric
  is_same_type<symmetry_property, valid_tag> isSymmetric;

  /// Define test whether ordering is a degree-ordering
  is_same_type<degorder_property, valid_tag> isDegreeOrder;

  /// Define test whether ordering is a block-ordering
  is_same_type<blockorder_property, valid_tag> isBlockOrder;

  /// Define test whether ordering is a total degree-ordering
  is_same_type<totaldegorder_property, valid_tag> isTotalDegreeOrder;

  /// Define test whether ordering is symmetric
  is_same_type<ascending_property, valid_tag> ascendingVariables;

  /// Define test whether ordering is symmetric
  is_same_type<descending_property, valid_tag> descendingVariables;

  /// Define test whether ordering is deg-rev-lex ordering
  is_same_type<degrevlexorder_property, valid_tag> 
  isDegreeReverseLexicographical;
};


END_NAMESPACE_PBORI

#endif
