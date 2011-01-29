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
template <class OrderType, class OrderTag>
class CBlockOrderingFacade:
  public COrderingFacade <OrderType, OrderTag> {

  typedef CBlockOrderingFacade self;
  typedef COrderingFacade<OrderType, OrderTag> base_type;

public:

  /// *this is to be used as base for @c OrderType only
  typedef self base;

  /// @name Declare template argument-dependent types
  //@{
  typedef typename base_type::order_lead_tag order_lead_tag;
  typedef typename base_type::poly_type poly_type;
  typedef typename base_type::monom_type monom_type;
  typedef typename base_type::exp_type exp_type;
  typedef typename base_type::deg_type deg_type;
  typedef typename base_type::set_type set_type;
  typedef typename base_type::comp_type comp_type;
  //@}

  /// Default Constructor
  CBlockOrderingFacade(): base_type(), m_indices() {
    m_indices.push_back(0);
    m_indices.push_back(CTypes::max_index()); 
  };

  /// Copy Constructor
  CBlockOrderingFacade(const self& rhs): base_type(rhs), m_indices(rhs.m_indices) {};

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

  /// generic block lead
  monom_type lead(const poly_type& poly) const {

    CBlockDegreeCache<set_type> blockDegCache(poly.ring());
    CacheManager<order_lead_tag> cache_mgr(poly.ring());
    typename base_type::descending_property descending;

    return monom(  dd_block_degree_lead(cache_mgr, blockDegCache, 
                                        poly.navigation(), m_indices.begin(),
                                        set_type(), descending) );
  }

  /// Leading monomial with bound (just the ordinary leading monomial)
  /// @note falls back to @c lead, ignores second argument
  monom_type lead(const poly_type& poly, deg_type) const {
    return lead(poly); 
  }

  /// Extraction of leading exponent - just using lead
  exp_type leadExp(const poly_type& poly) const {
    return lead(poly).exp();
  }

  /// Leading exponent with bound  (just the ordinary leading monomial)
  /// @note falls back to @c leadExp, ignores second argument
  exp_type leadExp(const poly_type& poly, deg_type) const {
    return leadExp(poly);
  }

protected:

  /// Comparison of monomials/expoinents template
  template <class TermType, class BinOpType>
  comp_type compare_terms(const TermType& lhs, const TermType& rhs,
                          const BinOpType& idx_comparer) const {
  
    return block_dlex_compare(lhs.begin(), lhs.end(),rhs.begin(), rhs.end(),
                              blockBegin(), blockEnd(), idx_comparer);
  }

  /// index data
  COrderingBase::block_idx_type m_indices;
};

END_NAMESPACE_PBORI

#endif
