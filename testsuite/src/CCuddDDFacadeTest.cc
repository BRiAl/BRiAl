// -*- c++ -*-
//*****************************************************************************
/** @file CCuddDDFacadeTest.cc
 *
 * @author Ket Kalda, Alexander Dreyer
 * @date 2010-11-09
 *
 * boost/test-driven unit test
 * 
 * @par Copyright:
 *   (c) 2010 by The PolyBoRi Team
 *
 **/
//*****************************************************************************

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
using boost::test_tools::output_test_stream;

#include "pbori_defs.h"
#include "BooleVariable.h"
#include "BooleMonomial.h"
#include "BoolePolynomial.h"
#include "BooleExponent.h"
#include "BoolePolyRing.h"

USING_NAMESPACE_PBORI

struct Fdd {
  typedef CCuddDDFacade<BoolePolyRing, BooleSet> dd_type;
  Fdd():
    ring(5) {
      x = BooleVariable(0);
      y = BooleVariable(1);
      z = BooleVariable(2);
      v = BooleVariable(3);
      w = BooleVariable(4);
      BOOST_TEST_MESSAGE( "setup fixture" );
      ring.setVariableName(0, "x");
      ring.setVariableName(1, "y");
      ring.setVariableName(2, "z");
      ring.setVariableName(3, "v");
      ring.setVariableName(4, "w");
      poly = x*y*z + v*z - x*v + y;
    }

  ~Fdd() { BOOST_TEST_MESSAGE( "teardown fixture" ); }

  BoolePolyRing ring;
  BooleVariable x, y, z, v, w;
  BoolePolynomial poly;
};

BOOST_FIXTURE_TEST_SUITE(CCuddDDFacadeTestSuite, Fdd )

BOOST_AUTO_TEST_CASE(test_facade) {
  dd_type facade(poly.set());
  facade.printIntern(std::cout);
}

BOOST_AUTO_TEST_CASE(test_size) {

  dd_type facade(poly.set());

  BOOST_TEST_MESSAGE( "count, countDouble, nNodes, rootIndex" );
  BOOST_CHECK_EQUAL(facade.count(), 4);
  BOOST_CHECK_EQUAL(facade.countDouble(), 4);
  BOOST_CHECK_EQUAL(facade.nNodes(), 8);
  BOOST_CHECK_EQUAL(facade.rootIndex(), 0);
}

BOOST_AUTO_TEST_SUITE_END()
