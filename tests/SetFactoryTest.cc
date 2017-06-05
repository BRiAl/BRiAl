// -*- c++ -*-
//*****************************************************************************
/** @file SetFactoryTest.cc
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
#include <polybori/factories/SetFactory.h>
#include <polybori/BoolePolyRing.h>

USING_NAMESPACE_PBORI

struct Fsetfac {
  Fsetfac(const BoolePolyRing& input_ring = BoolePolyRing(5)):
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

  ~Fsetfac() { BOOST_TEST_MESSAGE( "teardown fixture" ); }

  BoolePolyRing ring;
  BooleVariable x, y, z, v, w;
};


BOOST_FIXTURE_TEST_SUITE(SetFactoryTest, Fsetfac )

BOOST_AUTO_TEST_CASE(test_constructors) {

  SetFactory factory(ring);
  SetFactory factory_copy(factory);

  BOOST_CHECK_EQUAL(factory.parent().hash(), ring.hash());
  BOOST_CHECK_EQUAL(factory_copy.parent().hash(), ring.hash());
}

BOOST_AUTO_TEST_CASE(test_pseudo_constructors) {

  SetFactory factory(ring);

  BOOST_TEST_MESSAGE( "Constant sets..." );
  BOOST_CHECK_EQUAL(factory(), ring.zero());
  BOOST_CHECK_EQUAL(factory(ring), ring.zero());

  BOOST_TEST_MESSAGE( "Sets from navigators..." );
  BoolePolynomial poly = y + y*z + z + v;

  BOOST_CHECK_EQUAL(factory(poly.navigation(), ring), poly.set());
  BOOST_CHECK_EQUAL(factory(poly.navigation()), poly.set());

  BOOST_CHECK_EQUAL(factory(*poly.navigation(),
			    poly.navigation().thenBranch(),
			    poly.navigation().elseBranch(), ring), poly.set());

  BOOST_CHECK_EQUAL(factory(*poly.navigation(),
			    poly.navigation().thenBranch(),
			    poly.navigation().elseBranch()), poly.set());

  BOOST_TEST_MESSAGE( "Combine sets..." );


  BOOST_CHECK_EQUAL(factory(*poly.navigation(),
			    poly.set().subset1(*poly.navigation()),
			    poly.set().subset0(*poly.navigation())), poly.set());

}

BOOST_AUTO_TEST_SUITE_END()
