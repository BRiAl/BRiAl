// -*- c++ -*-
//*****************************************************************************
/** @file NextSpoly.h 
 *
 * @author Michael Brickenstein (original) Alexander Dreyer (refactored)
 * @date 2012-02-08
 *
 * This file includes the definition of the class @c NextSpoly.
 *
 * @par Copyright:
 *   (c) 2006-2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_NextSpoly_h_
#define polybori_groebner_NextSpoly_h_

// include basic definitions
#include "groebner_defs.h"
#include "ReductionStrategy.h"

BEGIN_NAMESPACE_PBORIGB

/** @class NextSpoly
 * @brief This class computes the next spoly for a generator pair.
 *
 **/

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

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_NextSpoly_h_ */
