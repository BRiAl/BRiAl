// -*- c++ -*-
//*****************************************************************************
/** @file boolevars.cc 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file contains tests for the Boolean Variables.
 *
 * @par Copyright:
 *   (c) 2006 by The PolyBoRi Team
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
#if 1
  try {
    BoolePolyRing the_ring(5);


    BoolePolynomial x = BooleVariable(0);
    std::cout << x << std::endl;

    BoolePolynomial y = BooleVariable(1);
    std::cout << y << std::endl;

    BoolePolynomial poly = x;
    poly += y;

    std::cout << "Sum: "<<std::endl;
    std::cout << poly <<std::endl;

    BoolePolynomial zeroPoly;

    std::cout << "Zero polynomial: " <<std::endl;
    std::cout << zeroPoly <<std::endl;

    std::cout << "0 += x " <<std::endl;
    zeroPoly += x;
    std::cout << zeroPoly <<std::endl;


    std::cout << "poly *= var" <<std::endl;
    std::cout <<  (poly *= BooleVariable(0)) << std::endl;
    std::cout << "poly % var" <<std::endl;
    std::cout <<  (poly % BooleVariable(0)) << std::endl;

    std::cout << "poly %= var" <<std::endl;
    std::cout <<  (poly %= BooleVariable(0)) << std::endl;
    std::cout << "Finished."<<std::endl;


    std::cout << "var * monom" <<std::endl;
    std::cout <<  (BooleVariable(0)  * BooleMonomial(BooleVariable(1)))<<
      std::endl;
  }
  catch (PBoRiError& err) {
    std::cout << "  Caught error # "<< err.code() <<std::endl;   
    std::cout << "    which says: "<< err.text() <<std::endl;  
 }
#endif
  return 0;
}
