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
#include "polybori.h"
#include "groebner.h"
#include "groebner_defs.h"
#include "groebner_alg.h"
#include "nf.h"

USING_NAMESPACE_PBORIGB
USING_NAMESPACE_PBORI

int
main() {		
  BoolePolyRing ring(5);
	  
  BooleMonomial x = BooleVariable(0);    
  BooleMonomial y = BooleVariable(1);    
  BooleMonomial z = BooleVariable(2);    
  BooleMonomial v = BooleVariable(3);    
  BooleMonomial w = BooleVariable(4);
 		
  std::vector<Polynomial> I(4);
  I[0]=x*y*z*v+1;
  I[1]=x*y+z*v+y*z+x;
  I[2]=x*y*z+v*y*w+x*v+1;
  I[3]=x+y+z+v;
  
    	 
  GroebnerStrategy strat;    
  for(int iter = 0; iter < I.size(); ++iter){
    if(!I[iter].isZero())
      strat.addGeneratorDelayed(I[iter]);	 
  }
  strat.symmGB_F2();    	  
  
  std::vector<Polynomial> res = strat.minimalizeAndTailReduce();    
  for(int i = 0; i < res.size(); ++i)
    std::cout << res[i] << std::endl;				
  
  return 0;
}
