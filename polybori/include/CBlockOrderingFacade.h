// -*- c++ -*-
//*****************************************************************************
/** @file CBlockOrderingFacade.h
 *
 * @author Ket Shcherbakova, Alexander Dreyer
 * @date 2011-01-11
 *
 * Defines generic classes for block ordering management for polynomial rings.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#include "COrderingFacade.h"

#ifndef CBlockOrderingFacade_h_
#define CBlockOrderingFacade_h_


BEGIN_NAMESPACE_PBORI

/** @class CBlockOrderingFacade
 * @brief This class initialize the interface for orderings of
 * CDynamicOrderBase for a given OrderType. OrderType must inherit from
 * COrderingFacade<OrderType>.
 */
template <class OrderType>
class CBlockOrderingFacade:
    public COrderingFacade <OrderType> {
  typedef CBlockOrderingFacade self;
    /// Actual base type
  typedef COrderingBase base_type;
public:
  /// *this is to be used as base for @c OrderType only
  typedef self base;
};

END_NAMESPACE_PBORI

#endif
