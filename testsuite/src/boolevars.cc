// -*- c++ -*-
// $Id$
//
//*****************************************************************************
/** @file 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file contains tests for the class PBoRiError.
 *
 *  (c) 2006 by
 *   Dep. of Mathematics, Kaiserslautern University of Technology and
 *   Fraunhofer Institute for Industrial Mathematics (ITWM)
 *   D-67663 Kaiserslautern, Germany
 *
**/
//*****************************************************************************
//
// Last edit by $Author$ on $Date$
// 
// $Log$
// Revision 1.1  2006/03/16 17:09:13  dreyer
// ADD BoolePolynial functionality started
//
// Revision 1.2  2006/03/16 13:37:41  dreyer
// ADD: added error codes and PBORI_DEVELOPER marker
//
// Revision 1.1.1.1  2006/03/09 14:35:25  dreyer
// + Project started
//

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

    std::cout << x << std::endl;
    std::cout << y << std::endl;

    BoolePolynomial poly = x;
    poly += y;

    std::cout << "Sum: "<<std::endl;
    std::cout << poly <<std::endl;

    std::cout << "Finished."<<std::endl;
  }
  catch (PBoRiError& err) {
    std::cout << "  Caught error # "<< err.code() <<std::endl;   
    std::cout << "    which says: "<< err.text() <<std::endl;  
 }

  return 0;
}
