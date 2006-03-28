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


    BoolePolynomial poly = x;
    poly *= y;
    poly += z;

    std::cout << "x*y + z: "<<std::endl;
    std::cout << poly <<std::endl;

    std::cout << "leading term: "<<std::endl;
    std::cout << poly.lead() <<std::endl;

    poly *= w;

    std::cout << "x*y*w + z*: "<<std::endl;
    std::cout << poly <<std::endl;

    std::cout << "leading term: "<<std::endl;
    std::cout << poly.lead() <<std::endl;

    std::cout << "# ring variabled: "<<std::endl;
    std::cout << the_ring.nVariables()<<std::endl;
 
    std::cout << "lmdeg()"<<std::endl;
    std::cout << poly.lmDeg() <<std::endl;

    std::cout << "usedVariables()"<<std::endl;
    std::cout << poly.usedVariables() <<std::endl;

    poly = x;
    poly *= y;

    poly += v;
    poly *= w;
    poly += w;    poly += y;
    std::cout << poly <<std::endl;

    std::cout << "usedVariables()"<<std::endl;
    std::cout << poly.usedVariables() <<std::endl;

    std::cout << "# usedVariables()"<<std::endl;
    std::cout << poly.nUsedVariables() <<std::endl;


    std::cout << "Finished."<<std::endl;
  }
  catch (PBoRiError& err) {
    std::cout << "  Caught error # "<< err.code() <<std::endl;   
    std::cout << "    which says: "<< err.text() <<std::endl;  
 }

  return 0;
}
