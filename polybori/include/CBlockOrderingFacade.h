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
 * @brief This class implements block orderings for COrderingFacade.
 * OrderType must inherit from COrderingFacade<OrderType>.
 */
template <class OrderType>
class CBlockOrderingFacade:
    public COrderingFacade <OrderType> {
  typedef CBlockOrderingFacade self;
    /// Actual base type
  typedef COrderingFacade<OrderType> facade_type;
public:
  /// *this is to be used as base for @c OrderType only
  typedef self base;

  /// Default Constructor
  CBlockOrderingFacade(): facade_type(), m_indices() {
    m_indices.push_back(0);
    m_indices.push_back(CTypes::max_index()); 
  };

  /// Copy Constructor
  CBlockOrderingFacade(const self& rhs): facade_type(rhs), m_indices(rhs.m_indices) {};

  /// Destructor
  ~CBlockOrderingFacade() { }

  /// @name interface for block orderings
  //@{
  COrderingBase::block_iterator blockBegin() const { return m_indices.begin() + 1; }
  COrderingBase::block_iterator blockEnd() const { return m_indices.end(); }
  void appendBlock(COrderingBase::idx_type idx) {
    // idx should always be >= max(int) (==blockEnd() -1)
    // and if there are existing blocks (blockBegin()!=blockEnd() -1)...
    // the added block should be strictly larger than the previously added block
    if((idx >= *(blockEnd()-1)) || ((blockBegin() != (blockEnd()-1)) && (idx <= *(blockEnd()-2))))
      throw std::runtime_error("Blocks must be added in a strictly increasing order.");
    m_indices.back() = idx;
    m_indices.push_back(CTypes::max_index());
  }
  void clearBlocks() {
    m_indices.clear();
    m_indices.push_back(0); 
    m_indices.push_back(CTypes::max_index());
  }
  //@}

protected:
  COrderingBase::block_idx_type m_indices;
};

END_NAMESPACE_PBORI

#endif
