// -*- c++ -*-
//*****************************************************************************
/** @file minimalCPP.c
 *
 * @author Alexander Dreyer
 * @date 2010-07-12
 *
 *
 * @par Copyright:
 *   (c) 2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

// include basic definitions
#include <iostream>
#include <algorithm>
#include <iterator>
#include "polybori.h"
#include "groebner.h"
#include "groebner_defs.h"
#include "groebner_alg.h"
#include "nf.h"

USING_NAMESPACE_PBORIGB
USING_NAMESPACE_PBORI
using namespace std;

int
main() {		
  BoolePolyRing ring(5);
	  
  BooleMonomial x = BooleVariable(0);    
  BooleMonomial y = BooleVariable(1);    
  BooleMonomial z = BooleVariable(2);    
  BooleMonomial v = BooleVariable(3);    
  BooleMonomial w = BooleVariable(4);
 		
  std::vector<BoolePolynomial> ideal(4);
  ideal[0] = x*y*z*v+1;
  ideal[1] = x*y+z*v+y*z+x;
  ideal[2] = x*y*z+v*y*w+x*v+1;
  ideal[3] = x+y+z+v;
  
    	 
  GroebnerStrategy strat;
  for(std::vector<BoolePolynomial>::const_iterator iter = ideal.begin(); 
      iter != ideal.end(); ++iter) {
    if(!(iter->isZero()))
      strat.addGeneratorDelayed(*iter);	 
  }

  strat.symmGB_F2();    	  
  
  std::vector<BoolePolynomial> res = strat.minimalizeAndTailReduce();    
  std::copy(res.begin(), res.end(), ostream_iterator<BoolePolynomial>(cout, "\n"));
  
  return 0;
}
