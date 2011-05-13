// -*- c++ -*-
//*****************************************************************************
/** @file COrderingTags.h
 *
 * @author Alexander Dreyer
 * @date 2010-08-10
 *
 * Template class, which is a base for classes, which define ordering tags and
 * properties.
 *
 * @par Copyright:
 *   (c) 2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_orderings_COrderingTags_h_
#define polybori_orderings_COrderingTags_h_

// include basic definitions
#include <polybori/pbori_defs.h>

// include basic definitions for tags
#include <polybori/common/tags.h>
#include "order_tags.h"

BEGIN_NAMESPACE_PBORI

/** @class COrderingTags
 * @brief This tempalte class is base for classes which define ordering tags and
 * properties. 
 *
 **/
template <class OrderTag,
          class LexProp, class DegOrderProp, class BlockOrderProp,
          class DescendingProp, 
          class OrderedProp = LexProp,
          class TotelDegOrderProp = DegOrderProp, 
          class SymmetryProp =  typename invert_tag<BlockOrderProp>::type >

class COrderingTagsBase {
public:
  typedef OrderTag order_tag;
  typedef LexProp lex_property; 
  typedef OrderedProp ordered_property;
  typedef SymmetryProp symmetry_property;
  typedef DegOrderProp degorder_property;
  typedef BlockOrderProp blockorder_property;
  typedef TotelDegOrderProp totaldegorder_property;
  typedef typename invert_tag<DescendingProp>::type ascending_property;
  typedef typename and_tag<DegOrderProp, ascending_property>::type
  degrevlexorder_property; 
  typedef DescendingProp descending_property;
};

template <class OrderTag, class DescProp>
class CDegreeOrderingTags:
  public COrderingTagsBase <OrderTag, 
                            invalid_tag, valid_tag, invalid_tag, DescProp> {
};

template <class OrderTag, class DescProp>
class CBlockDegreeOrderingTags:
  public COrderingTagsBase <OrderTag, 
                            invalid_tag, invalid_tag, valid_tag, DescProp> {
};

template <class OrderTag>
class COrderingTags;

template <>
class COrderingTags<lex_tag>:
  public COrderingTagsBase <lex_tag, valid_tag, invalid_tag, invalid_tag, valid_tag> { };

template <>
class COrderingTags<dlex_tag>:
  public CDegreeOrderingTags <dlex_tag, valid_tag> { };


template <>
class COrderingTags<dp_asc_tag>:
  public CDegreeOrderingTags<dp_asc_tag, invalid_tag> { };

template <>
class COrderingTags<block_dlex_tag>:
  public CBlockDegreeOrderingTags<block_dlex_tag, valid_tag> { };


template <>
class COrderingTags<block_dp_asc_tag>:
  public CBlockDegreeOrderingTags<block_dp_asc_tag, invalid_tag> { };

END_NAMESPACE_PBORI

#endif
