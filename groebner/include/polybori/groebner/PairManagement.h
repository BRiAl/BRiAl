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
#include <polybori/routines/pbori_algo.h>


#include <utility>

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
  typedef std::priority_queue<Pair,std::vector<PairE>, PairECompare> queue_type;

  PairManagement(const BoolePolyRing& ring):
    queue(ring)  { }

  void appendHiddenGenerators(std::vector<Polynomial>& vec) {
    queue_type temp(queue);
    while(!temp.empty()){
      if (temp.top().getType() == DELAYED_PAIR) {
	Polynomial pt = ((PolyPairData*) temp.top().data.get())->p;
	if (!pt.isZero())
	  vec.push_back(pt);
      }
      temp.pop();
    }
  }
 
  void introducePair(const Pair& pair) {
    if (!((strategy().optHFE) && (pair.getType() == IJ_PAIR) &&
	  (pair.sugar > 4)))
      queue.push(pair);
  };

  template<class Type>
  Polynomial nextSpoly(Type& gen) {

    if (UNLIKELY(pairSetEmpty())) 
      return gen.leadingTerms.ring().zero();

    Pair act_pair(queue.top());
    queue.pop();
    
    if (act_pair.getType() == IJ_PAIR){
      const IJPairData* ij= (IJPairData*)(act_pair.data.get());
      int i = ij->i, j = ij->j;
      
      replacePair(gen, i, j);
      status.setToHasTRep(ij->i, ij->j);
      if ((i != ij->i) || (ij->j != j)){
	status.setToHasTRep(i,j);
	return spoly(gen[i].p, gen[j].p);
      }
    }
    Polynomial res = act_pair.extract(gen);
    if (act_pair.getType() == VARIABLE_PAIR) {
      const VariablePairData *vp=(VariablePairData*)(act_pair.data.get());
      gen(vp->i).vPairCalculated.insert(vp->v);
      if (!res.isZero() && (res.lead() == gen[vp->i].lead))
	res += gen[vp->i].p;
    }
    return res;
  }

  bool pairSetEmpty() const { return queue.empty(); }

  void cleanTopByChainCriterion() {
    while(!pairSetEmpty()) {
      int id = queue.top().getType();
      if (id == IJ_PAIR) {
	if (finished(ijpair())) return;
      }
      else if (id == VARIABLE_PAIR) {
	if (finished(variablepair())) return;
      }
      else 
        return;
      queue.pop();   
    }
  }

  PairStatusSet status;
  queue_type queue;

protected:
  const IJPairData& ijpair() const {
    return *((IJPairData*)queue.top().data.get());
  }
  const VariablePairData& variablepair() const {
    return *((VariablePairData*)queue.top().data.get());
  }

  bool finished(const IJPairData& ij) {
    return !(status.hasTRep(ij.i, ij.j) || checkPairCriteria(ij.i, ij.j));
  }
  
  bool finished(const VariablePairData& vp) {
    return !strategy().checkVariableCriteria(vp.i, vp.v);
  }
  
  bool checkPairCriteria(int i, int j) {
    if (strategy().checkPairCriteria(queue.top().lm, i, j)) {
      status.setToHasTRep(i, j);
      return true;
    }
    return false;
  }

  template <class Type>
  void replacePair(const Type& gen, int& i, int& j){
     MonomialSet m = 
       gen.leadingTerms.divisorsOf(gen[i].leadExp.LCM(gen[j].leadExp));
     MonomialSet::exp_iterator it=m.expBegin();
     MonomialSet::exp_iterator end=m.expEnd();
     int i_n=i;
     int j_n=j;
     while(it!=end){
       Exponent curr=*it;
       int index=gen.index(curr);
       wlen_type curr_wl=gen[index].weightedLength;
       if ((index!=i) && (index!=j)){
         if ((status.hasTRep(index,i)) &&
             (gen[i_n].weightedLength>curr_wl) &&
             (gen[index].ecart()<=gen[i].ecart())){
           i_n=index;
         }
         if ((status.hasTRep(index,j)) &&
             (gen[j_n].weightedLength>curr_wl) &&
             (gen[index].ecart()<=gen[j].ecart())
             ){
           j_n=index;
         }
       }
       ++it;
     }
     PBORI_ASSERT(i_n!=j_n);
     if (i_n!=j_n){
       i=i_n;
       j=j_n;
     }
   }

  strategy_type& strategy() { return static_cast<strategy_type&>(*this); }
  const strategy_type& strategy() const { 
    return static_cast<const strategy_type&>(*this); 
  }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_PairManagement_h_ */
