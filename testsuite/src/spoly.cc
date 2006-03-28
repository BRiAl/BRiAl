// -*- c++ -*-
//*****************************************************************************
/** @file 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file contains simple tests for the spolys.
 *
 * @par Copyright:
 *   (c) 2006 by
 *   Dep. of Mathematics, Kaiserslautern University of Technology and @n
 *   Fraunhofer Institute for Industrial Mathematics (ITWM)
 *   D-67663 Kaiserslautern, Germany
 *
 * @internal 
 * @version \$Id$
 * @endverbatim
**/
//*****************************************************************************


// load standard iostream capapilities
#include <iostream>

// load polybori header file
# include "polybori.h"

USING_NAMESPACE_PBORI

int
main(){

  std::cout << "Testing boolean ring variables" <<std::endl;   

  try {
    BoolePolyRing the_ring(5);

    BoolePolynomial x = BooleVariable(0);
    BoolePolynomial y = BooleVariable(1);
    BoolePolynomial z = BooleVariable(2);
    BoolePolynomial v = BooleVariable(3);
    BoolePolynomial w = BooleVariable(4);


    BoolePolynomial poly1 = x*y + x*z + y + w;
    BoolePolynomial poly2 = x*w + w;

    std::cout << "Poly 1: "<<std::endl;
    std::cout << poly1 <<std::endl;

    std::cout << "Poly 2: "<<std::endl;
    std::cout << poly2 <<std::endl;

    BoolePolynomial theSpoly = spoly(poly1, poly2);

    std::cout << "spoly: " <<std::endl;
    std::cout << theSpoly <<std::endl;

    std::cout << "Expected result: " <<std::endl;
    std::cout << ( x*z*w + w)  <<std::endl;

    std::cout << "Equality? " <<std::endl;
    std::cout << ( ( x*z*w + w) == theSpoly)  <<std::endl;

    std::cout << "Finished."<<std::endl;
  }
  catch (PBoRiError& err) {
    std::cout << "  Caught error # "<< err.code() <<std::endl;   
    std::cout << "    which says: "<< err.text() <<std::endl;  
 }

  return 0;
}
