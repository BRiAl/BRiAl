// -*- c++ -*-
//*****************************************************************************
/** @file 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file contains elementary tests for accumulating terms
 *
 * @par Copyright:
 *   (c) 2007 by
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
 * Revision 1.1  2007/05/14 16:10:30  dreyer
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


    BoolePolynomial poly = x*y +z;

    std::cout << "x*y + z: "<<std::endl;
    std::cout << poly <<std::endl;


    BoolePolynomial result = term_accumulate(poly.begin(), poly.end(),
                                             BoolePolynomial());

    std::cout << "Result (lex-ordered): "<<std::endl;
    std::cout << result <<std::endl;

    std::cout << "Result (current order): "<<std::endl;
    result = term_accumulate(poly.orderedBegin(), poly.orderedEnd(),
                             BoolePolynomial());
    
    std::cout << result <<std::endl;

    std::cout << "Finished."<<std::endl;
  }
  catch (PBoRiError& err) {
    std::cout << "  Caught error # "<< err.code() <<std::endl;   
    std::cout << "    which says: "<< err.text() <<std::endl;  
 }

  return 0;
}
