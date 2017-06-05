// -*- c++ -*-
//*****************************************************************************
/** @file VariableFactoryTest.cc
 *
 * @author Alexander Dreyer
 * @date 2011-05-05
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

#include <polybori/pbori_defs.h>
#include <polybori/factories/VariableFactory.h>
#include <polybori/BoolePolyRing.h>

USING_NAMESPACE_PBORI

struct Fvarfac {
  Fvarfac(const BoolePolyRing& input_ring = BoolePolyRing(50)):
    ring(input_ring),
    x(0, input_ring), y(1, input_ring), z(2, input_ring),
    v(3, input_ring), w(4, input_ring) {

    BOOST_TEST_MESSAGE( "setup fixture" );

    ring.setVariableName(0, "x");
    ring.setVariableName(1, "y");
    ring.setVariableName(2, "z");
    ring.setVariableName(3, "v");
    ring.setVariableName(4, "w");
  }

  ~Fvarfac() { BOOST_TEST_MESSAGE( "teardown fixture" ); }

  BoolePolyRing ring;
  BooleVariable x, y, z, v, w;
};


BOOST_FIXTURE_TEST_SUITE(VariableFactoryTest, Fvarfac )

BOOST_AUTO_TEST_CASE(test_constructors) {

  VariableFactory factory(ring);
  VariableFactory factory_copy(factory);

  BOOST_CHECK_EQUAL(factory.parent().hash(), ring.hash());
  BOOST_CHECK_EQUAL(factory_copy.parent().hash(), ring.hash());
}

BOOST_AUTO_TEST_CASE(test_pseudo_constructors) {

  VariableFactory factory(ring);

  BOOST_TEST_MESSAGE( "Default Variables..." );
  BOOST_CHECK_EQUAL(factory(), BooleVariable(0, ring));
  BOOST_CHECK_EQUAL(factory(ring), BooleVariable(0, ring));

  BOOST_TEST_MESSAGE( "i-th Variable..." );

  for (int idx = 0; idx != 17; ++idx) {
    BOOST_CHECK_EQUAL(factory(idx), BooleVariable(idx, ring));
    BOOST_CHECK_EQUAL(factory(idx, ring), BooleVariable(idx, ring));
  }
}

BOOST_AUTO_TEST_SUITE_END()
