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

// include basic definitions
#include "pbori_defs.h"

// include basic definitions for tags
#include "pbori_tags.h"

#ifndef COrderingTags_h_
#define COrderingTags_h_

BEGIN_NAMESPACE_PBORI

/** @class COrderingTags
 * @brief This tempalte class is base for classes which define ordering tags and
 * properties. 
 *
 **/
template <class OrderTag>
class COrderingTags {
public:
  typedef OrderTag order_tag;
  typedef invalid_tag lex_property; 
  typedef invalid_tag ordered_property;
  typedef invalid_tag symmetry_property;
  typedef invalid_tag degorder_property;
  typedef invalid_tag blockorder_property;
  typedef invalid_tag degrevlexorder_property; 
  typedef invalid_tag totaldegorder_property;
  typedef invalid_tag ascending_property;
  typedef invalid_tag descending_property;
};



#undef RESOLVE_PROPERTY

END_NAMESPACE_PBORI

#endif
