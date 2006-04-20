/*
 *  groebner_alg.ccc
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 20.04.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "groebner_alg.h"
BEGIN_NAMESPACE_PBORIGB
void introducePairs(std::vector<Pair> pairs){
  
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
}
END_NAMESPACE_PBORIGB