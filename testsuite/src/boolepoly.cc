// -*- c++ -*-
//*****************************************************************************
/** @file 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file contains elementary tests for the Boolean Variables.
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
 * Revision 1.5  2006/03/27 15:02:43  dreyer
 * ADD: BoolePolynomial::operator/=(const self&) and spoly
 *
 * Revision 1.4  2006/03/27 13:47:58  dreyer
 * ADD operator + and *, CHANGE BoolePolyRing::variable(i) generation
 *
 * Revision 1.3  2006/03/23 17:15:04  dreyer
 * ADD: lead() and lmdeg() functionality to BoolePolynomial,
 * BoolePolyRing(const manager_type &); leading term exampl.
 *
 * Revision 1.2  2006/03/22 08:06:59  dreyer
 * ADD: Template specializations CDDInterface<ZDD>, CDDManager<Cudd>; 
 * ring uses shared_ptr now
 *
 * Revision 1.1  2006/03/20 10:40:22  dreyer
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

  std::cout << "Testing Boolean polynomials" <<std::endl;   

  try {
    BoolePolyRing the_ring(5);

    BoolePolynomial x = the_ring.variable(0);
    BoolePolynomial y = the_ring.variable(1);

    BoolePolynomial z = BooleVariable(2);

    BoolePolynomial v = BooleVariable(3);
    BoolePolynomial w = BooleVariable(4);

    std::cout << x << std::endl;
    std::cout << y << std::endl;
    std::cout << z << std::endl;
    std::cout << v << std::endl;
    std::cout << w << std::endl;


    BoolePolynomial poly = x;
    poly *= y;
    std::cout << "x*y : "<<std::endl;
    std::cout << poly <<std::endl;

    poly += z;

    std::cout << "x*y + z: "<<std::endl;
    std::cout << poly <<std::endl;


    poly = x;
    poly += y;

    poly *= z;

    std::cout << "(x + y)* z: "<<std::endl;
    std::cout << poly <<std::endl;

    poly = x;
    poly *= v;

    BoolePolynomial poly2 = y;
    poly2 *= v;

    poly += poly2;

    poly += z;

    BoolePolynomial poly3 = y;
    poly3 *= v;


    std::cout << "(x*v + y*v + z ) "<<std::endl;
    std::cout << poly <<std::endl;

    poly *= poly3;
    std::cout << "(x*v + y*v + z )* (v*y): "<<std::endl;
    std::cout << poly <<std::endl;

    std::cout << "Node count: "<< poly.nNodes() <<std::endl;
    std::cout << "N: "<< BoolePolyRing::nRingVariables() <<std::endl;

    std::cout << std::endl << "(x*v + y*v + z )* (v*y): " <<std::endl;
    std::cout <<  (x*v + y*v + z )* (v*y) <<std::endl;

    std::cout <<  "Equality? "  <<std::endl;
    std::cout <<  (poly == ((x*v + y*v + z )* (v*y)) ) <<std::endl;

    std::cout << std::endl << "Nonequality? "  <<std::endl;
    std::cout <<  (poly != ((x*v + y*v + z )* (v*y)) ) <<std::endl;

    std::cout << std::endl << "Division ok? "  <<std::endl;
    std::cout <<  ( ( (x*y*z + x*z*w +v) / (x*z) ) == ( y + w ) ) <<std::endl;

    std::cout << std::endl;

    std::cout << "Finished."<<std::endl;
  }
  catch (PBoRiError& err) {
    std::cout << "  Caught error # "<< err.code() <<std::endl;   
    std::cout << "    which says: "<< err.text() <<std::endl;  
 }

  return 0;
}
