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
 * Revision 1.3  2007/07/06 18:46:31  dreyer
 * ADD: rewritten C++-Interface for Cudd
 *
 * Revision 1.2  2007/05/18 11:48:39  dreyer
 * ADD: sophisticated term_accumulate
 *
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


    BoolePolynomial poly = x*y +z +x*z +y*z;

    std::cout << "x*y + z +x*z +y*z: "<<std::endl;
    std::cout << poly <<std::endl;


    BoolePolynomial result = term_accumulate(poly.begin(), poly.end(),
                                             BoolePolynomial());

    std::cout << "Result (lex-ordered): "<<std::endl;std::cout.flush();
    std::cout << result <<std::endl;

    std::cout << "Result (current order): "<<std::endl;
    result = term_accumulate(poly.orderedBegin(), poly.orderedEnd(),
                             BoolePolynomial());
    
    std::cout << result <<std::endl;
    std::cout << "Test iter: "<<std::endl;


    BoolePolynomial::const_iterator lstart(poly.begin()),
    lstart2(poly.begin());

    ++lstart2;
    std::cout << *lstart <<"=="<<*lstart2 <<std::endl<< (lstart ==lstart2) <<std::endl;
    ++lstart;  ++lstart2;
    std::cout << *lstart <<"=="<<*lstart2 <<std::endl<< (lstart ==lstart2)
    <<std::endl;
    ++lstart;  ++lstart2;
    std::cout << *lstart <<"=="<<*lstart2 <<std::endl<< (lstart ==lstart2)
    <<std::endl;
   
    std::cout << "Test ordered iter: "<<std::endl;


    BoolePolynomial::ordered_iterator start(poly.orderedBegin()),
    start2(poly.orderedBegin());

    ++start2;
    std::cout << *start <<"=="<<*start2 <<std::endl<< (start ==start2) <<std::endl;
    ++start;  ++start2;
    std::cout << *start <<"=="<<*start2 <<std::endl<< (start ==start2)
    <<std::endl;
    ++start;  ++start2;
    std::cout << *start <<"=="<<*start2 <<std::endl<< (start ==start2)
    <<std::endl;


    std::cout << "Testing sophisticated accumulation: "<<std::endl;
    start = poly.orderedBegin();
    ++start;

    start2 = start;
    ++start2;
    ++start2;
    std::cout << "Correct result:       " << 
      std::accumulate(start, start2, BoolePolynomial(0))  <<std::endl;
    std::cout << "Sophisticated result: " << 
      term_accumulate(start.begin(), start.end(), start.navigation(),
                        start2.begin(), start2.end(), BoolePolynomial(0))  <<std::endl;


    poly = x*z + x*w+ y*z + y*w + y;

    start = poly.orderedBegin();
    ++start;
    start2 = start;
    ++start2;
    ++start2;

    std::cout << "testing...       "<<poly << std::endl;

    std::cout << "lower " << 
      lower_term_accumulate(start.navigation(),
                        start2.begin(), start2.end(), BoolePolynomial(0))  <<std::endl;

    std::cout << "summing >= "<<*start <<" .. <  "<<*start2 << std::endl;
    std::cout<<  term_accumulate(start, start2, BoolePolynomial(0))
      <<std::endl;
    

    poly = x*y +x*z +y*z + 1;
    std::cout << "testing...       "<<poly << std::endl;

    start = poly.orderedBegin();
    ++start;
    start2 = start;
    ++start2;
    ++start2;
    std::cout << "summing >= "<<*start <<" .. <  "<<*start2 << std::endl;

    std::cout <<term_accumulate(start, start2, BoolePolynomial(0)) <<std::endl;

    ++start2;
    std::cout << "summing >= "<<*start <<" .. <  "<<*start2 << std::endl;

    std::cout <<term_accumulate(start, start2, BoolePolynomial(0)) <<std::endl;

    std::cout << "Finished."<<std::endl;
  }
  catch (PBoRiError& err) {
    std::cout << "  Caught error # "<< err.code() <<std::endl;   
    std::cout << "    which says: "<< err.text() <<std::endl;  
 }

  return 0;
}
