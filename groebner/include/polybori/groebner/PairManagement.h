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
#include "pairs.h"
#include "PolyEntry.h"
#include "ReductionStrategy.h"
#include <polybori/routines/pbori_algo.h>


#include <utility>

BEGIN_NAMESPACE_PBORIGB

class NextSpoly {
public:

  NextSpoly(ReductionStrategy& gen, PairStatusSet& status):
    m_gen(gen), m_status(status) {}

  Polynomial operator()(const Pair& act_pair) {
    return compute(act_pair, act_pair.extract(m_gen));
  }

protected:
  Polynomial compute(const Pair& act_pair, const Polynomial& result) {
    if (act_pair.getType() == IJ_PAIR)
      return compute(act_pair.ijPair(), result);
    else
      if (act_pair.getType() == VARIABLE_PAIR)
        return compute(act_pair.variablePair(), result);
    
    return result;
  }

  Polynomial compute(const IJPairData& ij, const Polynomial& res) {
    int i = ij.i, j = ij.j;
    replacePair(i, j);
    m_status.setToHasTRep(ij.i, ij.j);
    if ((i != ij.i) || (ij.j != j)){
      m_status.setToHasTRep(i,j);
      return spoly(m_gen[i].p, m_gen[j].p);
    }
    return res;
  }

  Polynomial compute(const VariablePairData& vp, const Polynomial& res) {
    m_gen(vp.i).vPairCalculated.insert(vp.v);
    return (!res.isZero() && (res.lead() == m_gen[vp.i].lead)?
            res + m_gen[vp.i].p: res);
  }

  void replacePair(int& first, int& second) {
    MonomialSet m =
      m_gen.leadingTerms.divisorsOf(m_gen[first].leadExp.LCM(m_gen[second].leadExp));
    
    replacePair(m.expBegin(), m.expEnd(), first, second);
  }

  template <class Iterator>
  void replacePair(Iterator start, Iterator finish, int& first, int& second) {
    std::pair<int, int> original(first, second);
    while(start != finish)
      replaceGenerators(m_gen.index(*start++), original, first, second);
  }

private:
  void replaceGenerators(int index, std::pair<int, int> original,
                         int& first, int& second) const {

    if ((index != original.first) && (index != original.second)) {
      replaceGenerator(index, original.first, first);
      replaceGenerator(index, original.second, second);    
    }
    PBORI_ASSERT(first != second);
  }

  void replaceGenerator(int next, int original, int& current) const {
    if (m_status.hasTRep(next, original) &&
        (m_gen[current].weightedLength > m_gen[next].weightedLength) &&
        (m_gen[next].ecart() <= m_gen[original].ecart()))
      current = next;
  }

  ReductionStrategy& m_gen;
  PairStatusSet& m_status;
};

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


/** @class PairManagement
 * @brief This class defines PairManagement.
 *
 **/
template <class StrategyType>
class PairManagement {
  typedef PairManagement self;
  typedef StrategyType strategy_type;

public:
  typedef std::priority_queue<Pair, std::vector<PairE>, PairECompare> queue_type;

  PairManagement(const BoolePolyRing& ring):
    queue(ring)  { }

  void appendHiddenGenerators(std::vector<Polynomial>& vec) {
    queue_type temp(queue);
    while(!temp.empty()){
      appendTo(vec, temp.top());
      temp.pop();
    }
  }

  void introducePair(const Pair& pair) {
    if (!((strategy().optHFE) && (pair.getType() == IJ_PAIR) &&
	  (pair.sugar > 4)))
      queue.push(pair);
  };

  Polynomial nextSpoly(ReductionStrategy& gen) {
    if (UNLIKELY(pairSetEmpty()))
      return gen.leadingTerms.ring().zero();

    return NextSpoly(gen, status)(popped());
  }

  bool pairSetEmpty() const { return queue.empty(); }

  void cleanTopByChainCriterion() {  cleanTopByChainCriterion(strategy()); }

  void cleanTopByChainCriterion(strategy_type& strat) {
    CheckChainCriterion<strategy_type> continuing(strat, status);
    while(!pairSetEmpty() && continuing(queue.top()))
      queue.pop();   
  }

  PairStatusSet status;
  queue_type queue;

protected:

  /// Append delayed pairs only
   void appendTo(std::vector<Polynomial>& vec, const Pair& current) const {
    if (current.getType() == DELAYED_PAIR)
      appendTo(vec, current.delayedPair().p);
  }

  /// Append nonzero polynomials only
  void appendTo(std::vector<Polynomial>& vec, const Polynomial& poly) const {
    if (!poly.isZero())
      vec.push_back(poly);
  }

  /// @c top() and @c pop() at once
  Pair popped() {
    Pair result = queue.top();
    queue.pop();
    return result;
  }

  strategy_type& strategy() { return static_cast<strategy_type&>(*this); }
  const strategy_type& strategy() const { 
    return static_cast<const strategy_type&>(*this); 
  }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_PairManagement_h_ */
