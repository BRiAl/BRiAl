// -*- c++ -*-
//*****************************************************************************
/** @file order_traits.h
 *
 * @author Alexander Dreyer
 * @date 2006-07-20
 *
 * File retired, but kept for future use.
 *
 * @par Copyright:
 *   (c) 2006 by The PolyBoRi Team
**/
//*****************************************************************************

#ifndef polybori_orderings_order_traits_h_
#define polybori_orderings_order_traits_h_

// include basic definitions
#include <polybori/pbori_defs.h>

// include ordering tags
#include "order_tags.h"

BEGIN_NAMESPACE_PBORI

template <class OrderTag>
struct order_traits_code;

template <>
struct order_traits_code<lex_tag> { enum {order_code = CTypes::lp}; };
template <>
struct order_traits_code<dlex_tag> { enum {order_code = CTypes::dlex}; };
template <>
struct order_traits_code<dp_asc_tag> { enum {order_code = CTypes::dp_asc}; };
template <>
struct order_traits_code<block_dlex_tag> { enum {order_code = CTypes::block_dlex}; };
template <>
struct order_traits_code<block_dp_asc_tag> { enum {order_code = CTypes::block_dp_asc}; };

template <class OrderTag>
class order_traits;

template <class BaseOrderTag>
class order_traits<block_tag<BaseOrderTag> > {
public:
  enum { order_code = order_traits_code<block_tag<BaseOrderTag> >::order_code,
         baseorder_code = order_traits_code<BaseOrderTag>::order_code };
};

template <class OrderTag>
class order_traits {
public:
  enum { order_code = order_traits_code<OrderTag>::order_code,
         baseorder_code = order_code };
};

END_NAMESPACE_PBORI

#endif // order_traits_h_
