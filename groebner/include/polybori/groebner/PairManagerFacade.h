// -*- c++ -*-
//*****************************************************************************
/** @file PairManagerFacade.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-29 
 *
 * This file includes the definition of the class @c PairManagerFacade which
 * deals as a facade for strategy classes like @c GroebnerStrategy .
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_PairManagerFacade_h_
#define polybori_groebner_PairManagerFacade_h_

#include "PairStatusSet.h"


// include basic definitions
#include "groebner_defs.h"
#include "PairManager.h"

BEGIN_NAMESPACE_PBORIGB

/** @class PairManagerWithStrategy
 * @brief This class extends @c PairManager with a reference to a strategy.
 *
 * It defines variants of @c PairManager::cleanTopByChainCriterion() and 
 * @c PairManager::introducePair(pair) with explicit statement of the strategy.
 *
 * @note This is done better in @c PairManagerFacade without storing an
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

  /// @note Deprecated, use @c PairManagerFacade::cleanTopByChainCriterion() instead
  void cleanTopByChainCriterion() { cleanTopByChainCriterion(m_strategy); }

  /// @note Deprecated, use @c PairManagerFacade::introducePair(pair) instead
  void introducePair(const Pair& pair) { introducePair(pair, isHFE()); };

  /// Unhiding the appreciated variant @c introducePair(pair, isHFE)
  using PairManager::introducePair;

protected:
  bool isHFE() const { return m_strategy.optHFE; }

private:
  StrategyType& m_strategy;
};

/** @class PairManagerFacade
 * @brief This class defines a facade for a given Strategy, which
 *
 * @note Use it as an inheritance base for - say - @c NewStrategy like:

class NewStrategy:
  public PairManagerFacade<NewStrategy> {
  // definition of strategy
};

 **/
template <class StrategyType>
class PairManagerFacade {
  typedef PairManagerFacade self;
  typedef StrategyType strategy_type;

public:

  PairManagerFacade(const BoolePolyRing& ring):
    pairs(ring, get()) {}

  PairManagerFacade(const self& rhs):
    pairs(rhs.pairs, get()) { }

  void cleanTopByChainCriterion() { pairs.cleanTopByChainCriterion(get()); }
  void introducePair(const Pair& pair) { pairs.introducePairs(pair, isHFE()); }

  /// For compatibility reasons make this a public member
  PairManagerWithStrategy<strategy_type> pairs;

private:
  bool isHFE() const { return get().optHFE; }
  strategy_type& get() { return static_cast<strategy_type&>(*this); }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_PairManagerFacade_h_ */
