// -*- c++ -*-
//*****************************************************************************
/** @file PairManager.cc 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-30 
 *
 * This file includes the definition of the class @c PairManager.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************


// include basic definitions
#include <polybori/groebner/PairManager.h>
#include <polybori/groebner/ChainCriterion.h> 
#include <polybori/groebner/HasTRepOrExtendedProductCriterion.h> // for extended_product_criterion

BEGIN_NAMESPACE_PBORIGB

void PairManager::replacePair(int& i, int& j){
  MonomialSet m=strat->generators.leadingTerms.divisorsOf(strat->generators[i].leadExp.LCM(strat->generators[j].leadExp));
  MonomialSet::exp_iterator it=m.expBegin();
  MonomialSet::exp_iterator end=m.expEnd();
  int i_n=i;
  int j_n=j;
  while(it!=end){
    Exponent curr=*it;
    int index=strat->generators.index(curr);
    wlen_type curr_wl=strat->generators[index].weightedLength;
    if ((index!=i) && (index!=j)){
    if ((status.hasTRep(index,i)) &&
      (strat->generators[i_n].weightedLength>curr_wl) &&
      (strat->generators[index].ecart()<=strat->generators[i].ecart())){
      i_n=index;
    }
    if ((status.hasTRep(index,j)) &&
      (strat->generators[j_n].weightedLength>curr_wl) &&
      (strat->generators[index].ecart()<=strat->generators[j].ecart())
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
void PairManager::introducePair(const Pair& pair){
  if (!((strat->optHFE)&&(pair.getType()==IJ_PAIR)&& (pair.sugar>4)))
  queue.push(pair);
}
bool PairManager::pairSetEmpty() const{
  return queue.empty();
}
Polynomial PairManager::nextSpoly(const PolyEntryVector& gen){
  //PBORI_ASSERT(!(pairSetEmpty()));
  if (UNLIKELY(pairSetEmpty())) return strat->r.zero();
  bool replaced_used=false;
  
  Polynomial replaced(strat->r.zero());
  Pair act_pair(queue.top());
  queue.pop();
  //const IJPairData* ij= dynamic_cast<IJPairData*>(queue.top().data.get());
  if (act_pair.getType()==IJ_PAIR){
    IJPairData* ij= (IJPairData*)(act_pair.data.get());
    int i=ij->i;
    int j=ij->j;
    
    replacePair(i,j);
    if ((i!=ij->i)||(ij->j!=j)){
        
        

      replaced=spoly(strat->generators[i].p,strat->generators[j].p);
      replaced_used=true;
      this->status.setToHasTRep(i,j);
      //this->status.setToHasTRep(ij->i,j);
      //this->status.setToHasTRep(i,ij->j);
    }
    
    //ij->i=i;
    //ac->j;
    //cout<<"mark1"<<endl;
    this->status.setToHasTRep(ij->i,ij->j);
  } else{
    
    
    if (act_pair.getType()==VARIABLE_PAIR){
      
      const VariablePairData *vp=(VariablePairData*)(act_pair.data.get());
      this->strat->generators.access(vp->i).vPairCalculated.insert(vp->v);
      int i=vp->i;
      Polynomial res=act_pair.extract(gen);
      
      
      if (!(res.isZero())){
        Monomial lm=res.lead();
        //uses p itself as reductor has a great potential of extinguishing many terms
        if (lm==this->strat->generators[i].lead)
          res+=this->strat->generators[i].p;
      }
      
      return res;
      
    }
  }
  if (replaced_used) return replaced;
  Polynomial res=act_pair.extract(gen);
  
  
  
  return res;
  
}
void PairManager::cleanTopByChainCriterion(){
  while(!(this->pairSetEmpty())){
    //cout<<"clean"<<endl;
    
    //const IJPairData* ij= dynamic_cast<IJPairData*>(queue.top().data.get());
    if (queue.top().getType()==IJ_PAIR){
      const IJPairData* ij= (IJPairData*)(queue.top().data.get());

    ///@todo implement this
      const int i=ij->i;
      const int j=ij->j;
      if (strat->pairs.status.hasTRep(i,j)) {queue.pop();continue;}
      if ((strat->generators[i].length==1) &&(strat->generators[j].length==1)){
        this->queue.pop();
        strat->pairs.status.setToHasTRep(i,j);
        continue;
        
      }
      if (extended_product_criterion(strat->generators[i], strat->generators[j])){
        //cout<<"Delayed Extended PC"<<endl;
        this->queue.pop();
        strat->pairs.status.setToHasTRep(i,j);
        this->strat->extendedProductCriterions++;
        continue;
      }
      const Exponent lm=queue.top().lm;
      //cout<<"try chain crit"<<endl;
      const MonomialSet lms=this->strat->generators.leadingTerms.intersect(lm.divisors(this->strat->r));
      PBORI_ASSERT(lm==strat->generators[i].leadExp.LCM(strat->generators[j].leadExp));
      //PBORI_ASSERT(strat->generators.leadingTerms.divisorsOf(strat->generators[i].lmExp.LCM(strat->generators[j].lmExp))==lms);
      //should be set
      //if (lms.length()>2)
        //cout<<"non empty"<<endl;
      if (std::find_if(lms.expBegin(),lms.expEnd(),ChainCriterion(*(this->strat),i,j))!=lms.expEnd()){
        this->queue.pop();
        strat->pairs.status.setToHasTRep(i,j);
        //cout<<"Chain Criterion"<<endl;
        //cout.flush();
        strat->chainCriterions++;
        continue;
      } else {
          
          // CountCriterion c(*(this->strat), i);
          // if (UNLIKELY(c(j))) {
          //      this->queue.pop();
          //         strat->pairs.status.setToHasTRep(i,j);
          // 
          //         continue;
          // }
        return;
      }
    }else {
        //const VariablePairData *vp=dynamic_cast<VariablePairData*>(queue.top().data.get());
        if (queue.top().getType()==VARIABLE_PAIR)
        {
          const VariablePairData *vp=(VariablePairData*)(queue.top().data.get());
          if UNLIKELY(strat->generators[vp->i].length==1){
            //has been shortened
            queue.pop();
            continue;
          }
          //const MonomialSet lms=this->strat->generators.leadingTerms.intersect(strat->generators[vp->i].lm.divisors());
          
          //Monomial lm=strat->generators[vp->i].lm;
          //Exponent lmExp=strat->generators[vp->i].lmExp;
          if UNLIKELY(strat->generators[vp->i].literal_factors.occursAsLeadOfFactor(vp->v)){
            strat->log("delayed variable linear factor criterion");
            queue.pop();
            continue;
          }
          /*if (!(strat->generators.leadingTerms.intersect(lmExp.divisors()).diff(Polynomial(lm)).isZero())){
            strat->variableChainCriterions++;
           queue.pop();
          } else {
            if (std::find_if(lms.expBegin(),lms.expEnd(),ChainVariableCriterion(*(this->strat),vp->i,vp->v))!=lms.expEnd()){
              strat->generators[vp->i].vPairCalculated.insert(vp->v);
              this->queue.pop();
              //cout<<"Variable Chain Criterion"<<endl;
              //cout.flush();
              strat->variableChainCriterions++;
            } else return;}
          
          */
          if (!(strat->generators[vp->i].minimal)){
             this->queue.pop();
             strat->variableChainCriterions++;
             continue;
          }
          return;
        } else return;
    }
  }
}
void PairManager::appendHiddenGenerators(std::vector<Polynomial>& vec){
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
END_NAMESPACE_PBORIGB
