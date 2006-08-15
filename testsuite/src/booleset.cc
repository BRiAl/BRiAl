// -*- c++ -*-
//*****************************************************************************
/** @file booleset.cc
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file contains elementary tests for the Boolean sets.
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
 * Revision 1.2  2006/08/15 14:17:29  dreyer
 * ADD minimalElements(), hasTermOfVariables()
 *
 * Revision 1.1  2006/08/09 12:52:32  dreyer
 * CHANGE/ADD: added lowlevel implementation of BooleSet::divisorsOf()
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

    BooleMonomial x = BooleVariable(0);
    BooleMonomial y = BooleVariable(1);

    BooleMonomial z = BooleVariable(2);

    BooleMonomial v = BooleVariable(3);
    BooleMonomial w = BooleVariable(4);

    BoolePolynomial poly;

    std::cout <<std::endl<<  "BooleSet divisorsOf test"<<std::endl;

    poly = x*y*z;

    BooleSet bset = poly.lmDivisors();

    BooleSet bset2 (bset.divisorsOf(x*y*w)); 
     std::cout <<  bset2 << std::endl;

    std::cout <<   bset.divisorsOf((x*y*w) ) << std::endl;
    
    std::cout <<   bset.hasTermOfVariables((x*y*w) ) << std::endl;

    std::cout <<   "next" << std::endl;

    std::cout <<   bset.hasTermOfVariables((v*w) ) << std::endl;

    std::cout <<   bset.divisorsOf(v*w) << std::endl;

    std::cout <<   bset.intersect((v*w).divisors()) << std::endl;

    std::cout <<   bset.intersect((v*w).divisors()).emptiness() << std::endl;
   std::cout <<   "next" << std::endl;

   std::cout <<   bset.hasTermOfVariables((z) ) << std::endl;


    bset = BoolePolynomial(x*z*w).lmDivisors();

    std::cout <<   bset.divisorsOf((x*y*w) ) << std::endl;
    std::cout <<   bset.divisorsOf((x*y*w) ) << std::endl;

    std::cout <<   "testing minimal elements" << std::endl;
//     poly = x*y + x*y*z +y*v + y*v*w;

//     bset = poly.diagram();

//     std::cout <<   "bset "<<  bset << std::endl;
//     std::cout <<   bset.minimalElements() << std::endl;
//     std::cout <<   bset.minimalElements() << std::endl;

    std::cout <<   "testing next minimal elements" << std::endl;
     poly = y + x*y*z +y*v + y*v*w;

   bset = poly.diagram();
   std::cout <<  "bset "<<  bset << std::endl;
//     std::cout <<   bset.minimalElements() << std::endl;
//     std::cout <<   "second" << std::endl;

    std::cout <<   bset.minimalElements() << std::endl;
     poly = y*v + x*y*z*v*w;

   bset = poly.diagram();
   std::cout <<  "bset "<<  bset << std::endl<< std::endl<< std::endl;;
//     std::cout <<   bset.minimalElements() << std::endl;
//     std::cout <<   "second" << std::endl;

    std::cout <<   bset.minimalElements() << std::endl;
//      std::cout <<   "testing next minimal elements" << std::endl;
//      poly =  x*y*z*v*w + y*w*z*v;

//    bset = poly.diagram();
//     std::cout <<   "bset "<<  bset << std::endl;
//      std::cout <<   bset.minimalElements() << std::endl;
//      std::cout <<   "second" << std::endl;

//     std::cout <<   bset.minimalElements() << std::endl;
//     std::cout <<std::endl<<  "Finished."<<std::endl;
 }
  catch (PBoRiError& err) {
    std::cout << "  Caught error # "<< err.code() <<std::endl;   
    std::cout << "    which says: "<< err.text() <<std::endl;  
 }

  return 0;
}
