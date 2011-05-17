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

#include <polybori/groebner/groebner_alg.h>

USING_NAMESPACE_PBORI
USING_NAMESPACE_PBORIGB
struct Fstrat {
  Fstrat(const BoolePolyRing& input_ring = BoolePolyRing(1000)): 
    ring(input_ring),
    x(0, input_ring), y(1, input_ring), z(2, input_ring),
    v(3, input_ring), w(4, input_ring) {

    BOOST_TEST_MESSAGE( "setup fixture" ); 
  }
  ~Fstrat() { BOOST_TEST_MESSAGE( "teardown fixture" ); }

  BoolePolyRing ring;
  BooleVariable x, y, z, v, w;
};

BOOST_FIXTURE_TEST_SUITE(GroebnerStrategyTestSuite, Fstrat )

BOOST_AUTO_TEST_CASE(test_nf) {

  GroebnerStrategy strat(ring);
  for (int i=0;i<1000;i++){
    strat.addGenerator(BoolePolynomial(BooleVariable(i, ring)+BoolePolynomial(true,ring)));
  }

  ///@TODO: GroebnerStrategy needs more tests
  BoolePolynomial poly = x+1;
  BOOST_CHECK_EQUAL(strat.nf(poly), BoolePolynomial(0 ,ring));
}

BOOST_AUTO_TEST_SUITE_END()
