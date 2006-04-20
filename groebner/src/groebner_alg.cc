/*
 *  groebner_alg.ccc
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 20.04.06.
 *  Copyright 2006 Matematisches Forschungsinstitut Oberwolfach. All rights reserved.
 *
 */

#include "groebner_alg.h"
BEGIN_NAMESPACE_PBORIGB
void PairManager::introducePair(const Pair& pair){
  queue.push(pair);
}
bool PairManager::pairSetEmpty(){
  return queue.empty();
}
Polynomial PairManager::nextSpoly(const PolyEntryVector& gen){
  assert(!(pairSetEmpty()));
  Polynomial res=Pair(queue.top()).extract(gen);
  queue.pop();
  return res;
  
}
void PairManager::cleanTopByChainCriterion(){
  const IJPairData* ij= dynamic_cast<const IJPairData*>(queue.top().data.get());
  if (ij!=NULL){
    ///@todo implement this
  }
}
PolyEntry::PolyEntry(const Polynomial &p){
  this->p=p;
  this->lm=p.lead();
  this->weightedLength=p.eliminationLength();
  this->deg=p.deg();
  this->lmDeg=p.lmDeg();
}
void GroebnerStrategy::addGenerator(const BoolePolynomial& p){
  PolyEntry e(p);
  generators.push_back(e);
  pairs.status.prolong();
  const int s=generators.size()-1;
  int i;
  for(i=0;i<s;i++){
    if (GCD(this->generators[i].lm, generators[s].lm).deg()>0){
      this->pairs.introducePair(Pair(i,s,generators));
    } else{
      //product criterion
      this->pairs.status.calculated(i,s);
    }
  }
  Monomial::const_iterator it=generators[s].lm.begin();
  Monomial::const_iterator end=generators[s].lm.end();
  while(it!=end){
    this->pairs.introducePair(Pair(s,*it,generators,VARIABLE_PAIR));
    it++;
    
  }
  
}
END_NAMESPACE_PBORIGB
