// -*- c++ -*-
//*****************************************************************************
/** @file term_accumulateTest.cc
 *
 * @author Ket Kalda, Alexander Dreyer
 * @date 2011-03-15
 *
 * boost/test-driven unit test
 * 
 * @par Copyright:
 *   (c) 2011 by The PolyBoRi Team
 *
 **/
//*****************************************************************************


#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp> 
using boost::test_tools::output_test_stream;

#include "polybori.h"

USING_NAMESPACE_PBORI

struct Ftermaccu {
  Ftermaccu(): 
    ring(5,COrderEnums::dp_asc) {
    BOOST_TEST_MESSAGE( "setup fixture" );
    x = BooleVariable(0);
    y = BooleVariable(1);
    z = BooleVariable(2);
    v = BooleVariable(3);
    w = BooleVariable(4);
  }
  ~Ftermaccu() { BOOST_TEST_MESSAGE( "teardown fixture" ); }

  BoolePolyRing ring;
  BooleMonomial x, y, z, v, w;
};

BOOST_FIXTURE_TEST_SUITE(term_accumulateTestSuite, Ftermaccu )

BOOST_AUTO_TEST_CASE(test_termaccu) {

  BOOST_TEST_MESSAGE("term_accumulate");

  BoolePolynomial poly = x*y +z +x*z +y*z;

  BoolePolynomial result = term_accumulate(poly.begin(), poly.end(),
                                          BoolePolynomial());

  std::cout << "Result (lex-ordered): "<<std::endl;std::cout.flush();
  std::cout << result <<std::endl;

  std::cout << "Result (current order): "<<std::endl;
  result = term_accumulate(poly.orderedBegin(), poly.orderedEnd(),
                          BoolePolynomial());
  
  std::cout << result <<std::endl;

  BoolePolynomial::ordered_iterator start(poly.orderedBegin()),
  start2(poly.orderedBegin());

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
  std::cout << "summing >= "<<*start <<" .. <  "<< 
    ((start2 ==  poly.orderedEnd())? BoolePolynomial(0) : 
    BoolePolynomial(*start2)) << std::endl;
  std::cout.flush();

  std::cout <<term_accumulate(start, start2, BoolePolynomial(0)) <<std::endl;
}
BOOST_AUTO_TEST_SUITE_END()
