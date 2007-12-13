// -*- c++ -*-
//*****************************************************************************
/** @file leadterm.cc 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file contains elementary tests for getting thwe leading term
 *
 * @par Copyright:
 *   (c) 2006 by The PolyBoRi Team
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.7  2007/12/13 15:53:50  dreyer
 * CHANGE: Ordering in BoolePolyRing again; BooleEnv manages active ring
 *
 * Revision 1.6  2007/11/06 15:03:43  dreyer
 * CHANGE: More generic copyright
 *
 * Revision 1.5  2006/05/24 11:57:51  dreyer
 * CHANGE alternative modulus operation
 *
 * Revision 1.4  2006/04/19 15:55:53  dreyer
 * ADD BooleMonomial, BoolePolynomial::fetchTerms() and ::terms()
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

    BooleMonomial x = BooleVariable(0);
    BooleMonomial y = BooleVariable(1);

    BooleMonomial z = BooleVariable(2);

    BooleMonomial v = BooleVariable(3);
    BooleMonomial w = BooleVariable(4);


    BoolePolynomial poly = x;
    poly *= y;
    poly += z;

    std::cout << "x*y + z: "<<std::endl;
    std::cout << poly <<std::endl;

    std::cout << "leading term: "<<std::endl;
    std::cout << poly.lead() <<std::endl;

    poly *= w;

    std::cout << "x*y*w + z*w: "<<std::endl;
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

    poly = BooleEnv::variable(3);
    std::cout <<std::endl << "Leading term of simple monomial "<<std::endl;
    std::cout << poly.lead() <<std::endl;

 
    poly = BooleEnv::zero();
    std::cout << "Leading term of empty diagram "<<std::endl;
    std::cout << poly.lead() <<std::endl;

    poly = BooleEnv::one();
    std::cout << "Leading term of constant polynomial one"<<std::endl;
    std::cout << poly.lead() <<std::endl;

    std::cout << "Checkung handling of constants: x*y + z + 1"<<std::endl;
    std::cout <<  x*y + z + 1 <<std::endl;

    std::cout << "Checkung handling of constants: x*y + z + 0"<<std::endl;
    std::cout <<  x*y + z + 0 <<std::endl;

    std::cout << "Test Modulus"<<std::endl;

    std::cout << (x*y+z ) % y <<std::endl;
    std::cout << (x*y+z ) % (x*y) <<std::endl;
    std::cout << (x*y+z ) % (y*z) <<std::endl;
    std::cout << (x*y*z  +x +y +z) % (x*y) <<std::endl;

    std::cout << "Finished."<<std::endl;
  }
  catch (PBoRiError& err) {
    std::cout << "  Caught error # "<< err.code() <<std::endl;   
    std::cout << "    which says: "<< err.text() <<std::endl;  
 }

  return 0;
}
