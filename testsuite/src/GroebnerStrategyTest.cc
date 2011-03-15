// -*- c++ -*-
//*****************************************************************************
/** @file GroebnerStrategyTest.cc
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

#include <groebner_alg.h>

USING_NAMESPACE_PBORI
USING_NAMESPACE_PBORIGB
struct Fstrat {
  Fstrat(): 
    ring(1000) {
    BOOST_TEST_MESSAGE( "setup fixture" );
    x = BooleVariable(0);
    y = BooleVariable(1);
    z = BooleVariable(2);
    v = BooleVariable(3);
    w = BooleVariable(4);
  }
  ~Fstrat() { BOOST_TEST_MESSAGE( "teardown fixture" ); }

  BoolePolyRing ring;
  BooleVariable x, y, z, v, w;
};

BOOST_FIXTURE_TEST_SUITE(GroebnerStrategyTestSuite, Fstrat )

BOOST_AUTO_TEST_CASE(test_nf) {

  GroebnerStrategy strat;
  for (int i=0;i<1000;i++){
    strat.addGenerator(BoolePolynomial(BooleVariable(i)+BoolePolynomial(true)));
  }

  ///@TODO: GroebnerStrategy needs more tests
  BoolePolynomial poly = x+1;
  BOOST_CHECK_EQUAL(strat.nf(poly), BoolePolynomial());
}

BOOST_AUTO_TEST_SUITE_END()
