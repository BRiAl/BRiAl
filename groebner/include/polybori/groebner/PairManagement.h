// -*- c++ -*-
//*****************************************************************************
/** @file PairManager.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-29 
 *
 * This file includes the definition of the class @c PairManager.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_PairManagement_h_
#define polybori_groebner_PairManagement_h_

#include "PairStatusSet.h"


// include basic definitions
#include "groebner_defs.h"
#include "PairManager.h"

BEGIN_NAMESPACE_PBORIGB

/** @class PairManagerWithStrategy
 * @brief This class defines PairManagement.
 *
 * It defines variants of @c PairManager::cleanTopByChainCriterion() and 
 * @c PairManager::introducePair(pair) with explicit statement of the strategy.
 *
 * @note This is done better in @c PairManagement without storing an
 * additional reference. For now - to keep the interface constant - we continue
 * to allow code like @c strat.pairs.cleanTopByChainCriterion() and
 * @c strat.pairs.introducePair(pair) .
 **/
template <class StrategyType>
class PairManagerWithStrategy:
  public PairManager {

public:
  PairManagerWithStrategy(const PairManager& mgr, StrategyType& strategy):
    PairManager(mgr), m_strategy(strategy) { }

  /// Unhiding the appreciated variant @c cleanTopByChainCriterion(strategy)
  using PairManager::cleanTopByChainCriterion;

  /// @note Deprecated, use @c PairManagement::cleanTopByChainCriterion() instead
  void cleanTopByChainCriterion() { cleanTopByChainCriterion(m_strategy); }

  /// @note Deprecated, use @c PairManagement::introducePair(pair) instead
  void introducePair(const Pair& pair) { introducePair(pair, isHFE()); };

  /// Unhiding the appreciated variant @c introducePair(pair, isHFE)
  using PairManager::introducePair;

protected:
  bool isHFE() const { return m_strategy.optHFE; }

private:
  StrategyType& m_strategy;
};

/** @class PairManagement
 * @brief This class defines PairManagement.
 *
 **/
template <class StrategyType>
class PairManagement {
  typedef PairManagement self;
  typedef StrategyType strategy_type;

public:

  PairManagement(const BoolePolyRing& ring):
    pairs(ring, get()) {}

  PairManagement(const self& rhs):
    pairs(rhs.pairs, get()) { }


  void cleanTopByChainCriterion() { pairs.cleanTopByChainCriterion(get()); }
  void introducePair(const Pair& pair) { pairs.introducePairs(pair, isHFE()); }

  PairManagerWithStrategy<strategy_type> pairs;

private:
  bool isHFE() const { return get().optHFE; }
  strategy_type& get() { return static_cast<strategy_type&>(*this); }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_PairManagement_h_ */
