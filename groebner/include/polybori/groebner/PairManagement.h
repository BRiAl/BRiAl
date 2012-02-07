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
  //#include "ChainCriterion.h"


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
    int i;
    std::vector<Pair> temp_store;
    while(!(queue.empty())){
        Pair temp=queue.top();
        queue.pop();
        if (temp.getType()==DELAYED_PAIR){
            Polynomial pt=((PolyPairData*) temp.data.get())->p;
            if (!(pt.isZero()))
                vec.push_back(pt);
        }
        temp_store.push_back(temp);
        
    }
    for(i=0;i<temp_store.size();i++){
        queue.push(temp_store[i]);
    }
}
 



  void introducePair(const Pair& pair) {
    if (!((strategy().optHFE) && (pair.getType()==IJ_PAIR) && (pair.sugar>4)))
      queue.push(pair);
  };

  template<class Type>
  Polynomial nextSpoly(Type& gen) {

  //PBORI_ASSERT(!(pairSetEmpty()));
  if (UNLIKELY(pairSetEmpty())) return gen.leadingTerms.ring().zero();
  bool replaced_used=false;
  
  Polynomial replaced(gen.leadingTerms.ring().zero());
  Pair act_pair(queue.top());
  queue.pop();
  //const IJPairData* ij= dynamic_cast<IJPairData*>(queue.top().data.get());
  if (act_pair.getType()==IJ_PAIR){
    IJPairData* ij= (IJPairData*)(act_pair.data.get());
    int i=ij->i;
    int j=ij->j;
    
    replacePair(gen, i,j);
    if ((i!=ij->i)||(ij->j!=j)){
        
        

      replaced=spoly(gen[i].p,gen[j].p);
      replaced_used=true;
      status.setToHasTRep(i,j);
      //this->status.setToHasTRep(ij->i,j);
      //this->status.setToHasTRep(i,ij->j);
    }
    
    //ij->i=i;
    //ac->j;
    //cout<<"mark1"<<endl;
    status.setToHasTRep(ij->i,ij->j);
  } else{
    
    
    if (act_pair.getType()==VARIABLE_PAIR){
      
      const VariablePairData *vp=(VariablePairData*)(act_pair.data.get());
      gen(vp->i).vPairCalculated.insert(vp->v);
      int i=vp->i;
      Polynomial res=act_pair.extract(gen);
      
      
      if (!(res.isZero())){
        Monomial lm=res.lead();
        //uses p itself as reductor has a great potential of extinguishing many terms
        if (lm==gen[i].lead)
          res+=gen[i].p;
      }
      
      return res;
      
    }
  }
  if (replaced_used) return replaced;
  Polynomial res=act_pair.extract(gen);
  
  
  
  return res;
  
}

  bool pairSetEmpty() const{
    return queue.empty();
  }

  void cleanTopByChainCriterion() {
    while( !pairSetEmpty()) {
      if (queue.top().getType()==IJ_PAIR){
        const IJPairData* ij= (IJPairData*)(queue.top().data.get());

        ///@todo implement this
          const int i=ij->i;
          const int j=ij->j;
          
          if (!status.hasTRep(i,j)) {
            if (strategy().checkPairCriteria(queue.top().lm, i, j))
              status.setToHasTRep(i,j);
            else
              return;
          }
      }
      else if (queue.top().getType() == VARIABLE_PAIR) {
        const VariablePairData *vp=(VariablePairData*)(queue.top().data.get());
        if (!UNLIKELY(strategy().generators[vp->i].isSingleton() ||
                      strategy().generators[vp->i].literal_factors.occursAsLeadOfFactor(vp->v) )
            /*strategy().log("delayed variable linear factor criterion");*/
            
            || strategy().checkVariableChainCriterion(vp->i))  {
          return;
        }
      }
      else 
        return;
      queue.pop();   
    }
  }

  PairStatusSet status;
  queue_type queue;

protected:
  template <class Type>
  void replacePair(const Type& gen, int& i, int& j){
     MonomialSet m=gen.leadingTerms.divisorsOf(gen[i].leadExp.LCM(gen[j].leadExp));
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
       it++;
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
