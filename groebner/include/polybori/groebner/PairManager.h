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

#ifndef polybori_groebner_PairManager_h_
#define polybori_groebner_PairManager_h_

#include "PairStatusSet.h"


// include basic definitions
#include "groebner_defs.h"
#include "pairs.h"
#include "PolyEntry.h"
#include "NextSpoly.h"
#include "CheckChainCriterion.h"
#include <polybori/routines/pbori_algo.h>
#include <utility>

BEGIN_NAMESPACE_PBORIGB

/** @class PairManager
 * @brief This class defines PairManager.
 *
 **/
class PairManager {
  typedef PairManager self;

public:
  typedef std::priority_queue<Pair, std::vector<PairE>, PairECompare>
  queue_type;

  PairManager(const BoolePolyRing& ring):
    queue(ring)  { }

  void appendHiddenGenerators(std::vector<Polynomial>& vec) {
    queue_type temp(queue);
    while(!temp.empty()){
      appendTo(vec, temp.top());
      temp.pop();
    }
  }

  void introducePair(const Pair& pair, bool isHFE) {  
    if (!skip(pair, isHFE)) queue.push(pair);
  };  

  Polynomial nextSpoly(ReductionStrategy& gen) {
    if (PBORI_UNLIKELY(pairSetEmpty()))
      return gen.leadingTerms.ring().zero();

    return NextSpoly(gen, status)(popped());
  }

  bool pairSetEmpty() const { return queue.empty(); }

  template <class StrategyType>
  void cleanTopByChainCriterion(StrategyType& strat) {
    CheckChainCriterion<StrategyType> continuing(strat, status);
    while(!pairSetEmpty() && continuing(queue.top()))
      queue.pop();   
  }

  PairStatusSet status;
  queue_type queue;

protected:
  /// @c top() and @c pop() at once
  Pair popped() {
    Pair result = queue.top();
    queue.pop();
    return result;
  }

private:
  bool skip(const Pair& pair, bool isHFE) {
    return isHFE && (pair.getType() == IJ_PAIR) && (pair.sugar > 4);
  };
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
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_PairManager_h_ */
