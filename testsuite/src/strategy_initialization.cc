/*
 *  strategy_initialization.cc
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 27.04.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "strategy_initialization.h"
#include <polybori.h>
#include <groebner_alg.h>
#include <iostream>
USING_NAMESPACE_PBORI
USING_NAMESPACE_PBORIGB
using namespace std;
int main(int argc, const char* argv[]){
  int n=1000;
  BoolePolyRing r(n);
  GroebnerStrategy strat;
  int i;
#if 1
  for (i=n-1;i>=0;i--){
#else
 for (i=0;i<n;i++){
#endif
    strat.addGenerator(BoolePolynomial(BooleVariable(i)+BoolePolynomial(true)));
    //cout<<i<<endl;
  }
}
