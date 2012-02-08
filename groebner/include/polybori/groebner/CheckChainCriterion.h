// -*- c++ -*-
//*****************************************************************************
/** @file CheckChainCriterion.h 
 *
 * @author Michael Brickenstein (original) and Alexander Dreyer (refactored)
 * @date 2012-02-02 
 *
 * This file includes the definition of the class @c CheckChainCriterion.
 *
 * @par Copyright:
 *   (c) 2006-2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_CheckChainCriterion_h_
#define polybori_groebner_CheckChainCriterion_h_

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class CheckChainCriterion
 * @brief This class defines CheckChainCriterion.
 *
 **/

template <class StrategyType>
class CheckChainCriterion {
public:
  typedef StrategyType strategy_type;

  CheckChainCriterion(strategy_type& strategy, PairStatusSet& status):
    m_strategy(strategy), m_status(status) {}

  /// Clean up @c current and return whether it has a T-representation
  bool operator()(const Pair& current) {
    switch (current.getType()) {
    case IJ_PAIR: return compute(current.ijPair(), current.lm);
    case VARIABLE_PAIR: return compute(current.variablePair());
    }
    return false;
  }

protected:
  bool compute(const IJPairData& ij, const Exponent& exp) {
    return m_status.hasTRep(ij.i, ij.j) || 
      checkPairCriteria(exp, ij.i, ij.j);
  }
  
  bool compute(const VariablePairData& vp) {
    return m_strategy.checkVariableCriteria(vp.i, vp.v);
  }
  
  bool checkPairCriteria(const Exponent& exp, int i, int j) {
    if (m_strategy.checkPairCriteria(exp, i, j)) {
      m_status.setToHasTRep(i, j);
      return true;
    }
    return false;
  }

private:
  strategy_type& m_strategy;
  PairStatusSet& m_status;
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_CheckChainCriterion_h_ */
