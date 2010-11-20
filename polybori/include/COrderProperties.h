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
 *   (c) 2006 by The PolyBoRi Team
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.11  2007/11/06 15:03:34  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.10  2007/03/16 16:59:20  dreyer
 * CHANGE: started to rewrite CGenericIter using boost:iterator_facade
 *
 * Revision 1.9  2006/10/06 12:52:01  dreyer
 * ADD easy_equility_property and used in lex_compare
 *
 * Revision 1.8  2006/10/05 12:51:31  dreyer
 * CHANGE: Made lex-based comparisions more generic.
 *
 * Revision 1.7  2006/10/04 11:48:28  dreyer
 * ADD: isDegreeReverseLexicograpical()
 *
 * Revision 1.6  2006/09/08 16:15:27  dreyer
 * ADD: Added ordering-dependent term iteration
 *
 * Revision 1.5  2006/09/05 11:10:44  dreyer
 * ADD: BoolePolyRing::Compare(...), fixed assertion in groebner
 *
 * Revision 1.4  2006/09/05 08:48:32  dreyer
 * ADD: BoolePolyRing::is(Total)DegreeOrder()
 *
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
  isDegreeReverseLexicograpical;
};


END_NAMESPACE_PBORI

#endif
