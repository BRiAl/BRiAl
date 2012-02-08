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
    m_pairs(ring), queue(m_pairs.queue), status(m_pairs.status), 
    pairs(*this)  { }

  PairManagement(const self& rhs):
    m_pairs(rhs.m_pairs), queue(m_pairs.queue), status(m_pairs.status),
    pairs(*this)  { }

  void appendHiddenGenerators(std::vector<Polynomial>& vec) {
    m_pairs.appendHiddenGenerators(vec);
  }

  void introducePair(const Pair& pair) {
    if (!((strategy().optHFE) && (pair.getType() == IJ_PAIR) &&
	  (pair.sugar > 4)))
      m_pairs.introduce(pair);
  };

  Polynomial nextSpoly(ReductionStrategy& gen) {
    return m_pairs.nextSpoly(gen);
  }
  bool pairSetEmpty() const { return m_pairs.pairSetEmpty(); }

  void cleanTopByChainCriterion() {  cleanTopByChainCriterion(strategy()); }

  void cleanTopByChainCriterion(strategy_type& strat) {
    m_pairs.cleanTopByChainCriterion(strat);   
  }

  /// @name Just for keeping the interface constant - @todo for now
  //@{
  PairStatusSet& status;
  self& pairs;
  PairManager::queue_type& queue;
  //@}

private:
  strategy_type& strategy() { return static_cast<strategy_type&>(*this); }
  const strategy_type& strategy() const { 
    return static_cast<const strategy_type&>(*this); 
  }
  
  PairManager m_pairs;
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_PairManagement_h_ */
