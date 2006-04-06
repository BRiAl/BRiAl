// -*- c++ -*-
//*****************************************************************************
/** @file 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file contains elementary tests for getting thwe leading term
 *
 * @par Copyright:
 *   (c) 2006 by
 *   Dep. of Mathematics, Kaiserslautern University of Technology and @n
 *   Fraunhofer Institute for Industrial Mathematics (ITWM)
 *   D-67663 Kaiserslautern, Germany
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.1  2006/04/06 13:05:54  dreyer
 * CHANGE more suitable names for CCuddNavigator functions
 *
 * Revision 1.3  2006/03/30 11:57:11  dreyer
 * CHANGE: Made use of 0/1 constants and the sets {}, {{}} consistent
 *
 * Revision 1.2  2006/03/30 08:52:57  dreyer
 * CHANGE: testsuite revised
 *
 * Revision 1.1  2006/03/28 07:34:03  dreyer
 * + Initial Version
 *
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

  std::cout << "Testing leading terms" <<std::endl;   

  try {
    BoolePolyRing the_ring(5);

    BoolePolynomial x = the_ring.variable(0);
    BoolePolynomial y = the_ring.variable(1);

    BoolePolynomial z = BooleVariable(2);

    BoolePolynomial v = BooleVariable(3);
    BoolePolynomial w = BooleVariable(4);


    BoolePolynomial poly;


    poly =  ( x*z  )    * (z + v*w) + y;


   std::cout << poly <<std::endl;

    std::cout << "poly:  "<<std::endl;
    std::cout << poly <<std::endl;

    std::cout << "leading term: "<<std::endl;
    std::cout << poly.lead() <<std::endl;

    std::cout << "Navigation: "<<std::endl;

    BoolePolynomial::navigator navi = poly.navigation();

    std::cout << "* "<< *navi  <<std::endl;


    navi.incrementThen();
    std::cout << "inc * "<< *navi  <<std::endl;
    navi.incrementThen();
    std::cout << "inc * "<< *navi  <<std::endl;

    std::cout << "value "<< navi.terminalValue()  <<std::endl;

    navi.incrementElse(); 
    //navi.incrementThen();
    std::cout << "inc * "<< *navi  <<std::endl;
    std::cout << "inc * "<<""  <<std::endl;
    std::cout << "value "<< navi.terminalValue()  <<std::endl;

    std::cout << "Finished."<<std::endl;
  }
  catch (PBoRiError& err) {
    std::cout << "  Caught error # "<< err.code() <<std::endl;   
    std::cout << "    which says: "<< err.text() <<std::endl;  
 }

  return 0;
}
