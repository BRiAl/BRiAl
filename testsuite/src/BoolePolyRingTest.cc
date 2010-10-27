// -*- c++ -*-
//*****************************************************************************
/** @file BoolePolyRingTest.cc
 *
 * @author Ket Kalda, Alexander Dreyer
 * @date 2010-10-27
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
#include "BoolePolyRing.h"
#include "BooleVariable.h"
#include "pbori_order.h"

USING_NAMESPACE_PBORI

struct Fring {
  typedef BoolePolyRing ring_type;
  Fring() {
    x = (BooleVariable(0));
    y = (BooleVariable(1));
    z = (BooleVariable(2));
    v = (BooleVariable(3));
    w = (BooleVariable(4));
    BOOST_TEST_MESSAGE( "setup fixture" );
  }

  ~Fring() { BOOST_TEST_MESSAGE( "teardown fixture" ); }

  BooleVariable x, y, z, v, w;
};

BOOST_FIXTURE_TEST_SUITE(BoolePolyRingTestSuite, Fring )

BOOST_AUTO_TEST_CASE(test_constructors) {

  BOOST_TEST_MESSAGE( "Constant rings..." );
}

BOOST_AUTO_TEST_CASE(test_variables) {

  ring_type ring(3, 0, false);
  ring_type empty(0);
  ring.activate();
  ring_type defaultr;

  BOOST_TEST_MESSAGE( "nVariables, getVariableName, setVariableName" );

  BOOST_CHECK_EQUAL(ring.getVariableName(-1), "UNDEF");
  BOOST_CHECK_EQUAL(ring.getVariableName(0), "x(0)");
  BOOST_CHECK_EQUAL(ring.getVariableName(1), "x(1)");
  BOOST_CHECK_EQUAL(ring.getVariableName(2), "x(2)");
  BOOST_CHECK_EQUAL(ring.getVariableName(3), "UNDEF");
  BOOST_CHECK_EQUAL(ring.getVariableName(323223), "UNDEF");
  BOOST_CHECK_EQUAL(ring.nVariables(), 3);
  BOOST_CHECK_EQUAL(empty.getVariableName(-1), "UNDEF");
  BOOST_CHECK_EQUAL(empty.getVariableName(0), "UNDEF");
  BOOST_CHECK_EQUAL(empty.getVariableName(1), "UNDEF");
  BOOST_CHECK_EQUAL(empty.nVariables(), 0);
  BOOST_CHECK_EQUAL(defaultr.getVariableName(-1), ring.getVariableName(-1));
  BOOST_CHECK_EQUAL(defaultr.getVariableName(0), ring.getVariableName(0));
  BOOST_CHECK_EQUAL(defaultr.getVariableName(1), ring.getVariableName(1));
  BOOST_CHECK_EQUAL(defaultr.getVariableName(2), ring.getVariableName(2));
  BOOST_CHECK_EQUAL(defaultr.getVariableName(3), ring.getVariableName(3));
  BOOST_CHECK_EQUAL(defaultr.nVariables(), ring.nVariables());

  ring.setVariableName(0, "x");
  defaultr.setVariableName(1, "y");
  ring.setVariableName(3, "v"); // Unwanted behaviour?
  //defaultr.setVariableName(-1, "w"); //memory access violation at address: 0x00000039
  empty.setVariableName(0, "x"); // Unwanted behaviour?
  empty.setVariableName(1, "y"); // Unwanted behaviour?
  //empty.setVariableName(-1, "z");//memory access violation at address: 0x00000019
  BOOST_CHECK_EQUAL(ring.getVariableName(-1), "UNDEF");
  BOOST_CHECK_EQUAL(ring.getVariableName(0), "x");
  BOOST_CHECK_EQUAL(ring.getVariableName(1), "y");
  BOOST_CHECK_EQUAL(ring.getVariableName(2), "x(2)");
  BOOST_CHECK_EQUAL(ring.getVariableName(3), "v");
  BOOST_CHECK_EQUAL(ring.nVariables(), 3);
  BOOST_CHECK_EQUAL(defaultr.getVariableName(-1), ring.getVariableName(-1));
  BOOST_CHECK_EQUAL(defaultr.getVariableName(0), ring.getVariableName(0));
  BOOST_CHECK_EQUAL(defaultr.getVariableName(1), ring.getVariableName(1));
  BOOST_CHECK_EQUAL(defaultr.getVariableName(2), ring.getVariableName(2));
  BOOST_CHECK_EQUAL(defaultr.getVariableName(3), ring.getVariableName(3));
  BOOST_CHECK_EQUAL(defaultr.nVariables(), ring.nVariables());
  BOOST_CHECK_EQUAL(empty.getVariableName(-1), "UNDEF");
  BOOST_CHECK_EQUAL(empty.getVariableName(0), "x");
  BOOST_CHECK_EQUAL(empty.getVariableName(1), "y");
  BOOST_CHECK_EQUAL(empty.nVariables(), 0);

  defaultr.setVariableName(0, "y");
  ring.setVariableName(3, "-y");
  ring.setVariableName(2, "*");
  defaultr.setVariableName(4, "ö");
  empty.setVariableName(0, "variable");
  BOOST_CHECK_EQUAL(ring.getVariableName(-1), "UNDEF");
  BOOST_CHECK_EQUAL(ring.getVariableName(0), "y");
  BOOST_CHECK_EQUAL(ring.getVariableName(1), "y");
  BOOST_CHECK_EQUAL(ring.getVariableName(2), "*");
  BOOST_CHECK_EQUAL(ring.getVariableName(3), "-y");
  BOOST_CHECK_EQUAL(ring.getVariableName(4), "ö");
  BOOST_CHECK_EQUAL(ring.nVariables(), 3);
  BOOST_CHECK_EQUAL(defaultr.getVariableName(-1), ring.getVariableName(-1));
  BOOST_CHECK_EQUAL(defaultr.getVariableName(0), ring.getVariableName(0));
  BOOST_CHECK_EQUAL(defaultr.getVariableName(1), ring.getVariableName(1));
  BOOST_CHECK_EQUAL(defaultr.getVariableName(2), ring.getVariableName(2));
  BOOST_CHECK_EQUAL(defaultr.getVariableName(3), ring.getVariableName(3));
  BOOST_CHECK_EQUAL(defaultr.getVariableName(4), ring.getVariableName(4));
  BOOST_CHECK_EQUAL(defaultr.nVariables(), ring.nVariables());
  BOOST_CHECK_EQUAL(empty.getVariableName(-1), "UNDEF");
  BOOST_CHECK_EQUAL(empty.getVariableName(0), "variable");
  BOOST_CHECK_EQUAL(empty.getVariableName(1), "y");
  BOOST_CHECK_EQUAL(empty.nVariables(), 0);

  ring.changeOrdering(COrderEnums::lp);
  //BOOST_CHECK_EQUAL(ring.ordering(), get_ordering(COrderEnums::lp)); // How to compare orderings?
}

//BOOST_AUTO_TEST_CASE(test_ordering) {} // induces unknown error? previous rings not properly destroyed?

BOOST_AUTO_TEST_SUITE_END()
