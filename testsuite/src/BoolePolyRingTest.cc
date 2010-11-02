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
  Fring() { BOOST_TEST_MESSAGE( "setup fixture" ); }

  ~Fring() { BOOST_TEST_MESSAGE( "teardown fixture" ); }
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
}

BOOST_AUTO_TEST_CASE(test_ordering) {

  ring_type ring(3, 0, false);
  ring_type empty(0);
  ring.activate();
  ring_type defaultr;

  BOOST_TEST_MESSAGE( "ordering, changeOrdering" );

  //change ring
  ring.changeOrdering(COrderEnums::lp);
  BOOST_CHECK_EQUAL(ring.ordering().getOrderCode(), COrderEnums::lp);
  BOOST_CHECK_EQUAL(defaultr.ordering().getOrderCode(), COrderEnums::lp);
  ring.changeOrdering(COrderEnums::dlex);
  BOOST_CHECK_EQUAL(ring.ordering().getOrderCode(), COrderEnums::dlex);
  BOOST_CHECK_EQUAL(defaultr.ordering().getOrderCode(), COrderEnums::dlex);
  ring.changeOrdering(COrderEnums::dp_asc);
  BOOST_CHECK_EQUAL(ring.ordering().getOrderCode(), COrderEnums::dp_asc);
  BOOST_CHECK_EQUAL(defaultr.ordering().getOrderCode(), COrderEnums::dp_asc);
  ring.changeOrdering(COrderEnums::block_dlex);
  BOOST_CHECK_EQUAL(ring.ordering().getOrderCode(), COrderEnums::block_dlex);
  BOOST_CHECK_EQUAL(defaultr.ordering().getOrderCode(), COrderEnums::block_dlex);
  ring.changeOrdering(COrderEnums::block_dp_asc);
  BOOST_CHECK_EQUAL(ring.ordering().getOrderCode(), COrderEnums::block_dp_asc);
  BOOST_CHECK_EQUAL(defaultr.ordering().getOrderCode(), COrderEnums::block_dp_asc);
  ring.changeOrdering(-1);
  BOOST_CHECK_EQUAL(ring.ordering().getOrderCode(), COrderEnums::lp);//default is lp
  BOOST_CHECK_EQUAL(defaultr.ordering().getOrderCode(), COrderEnums::lp);
  ring.changeOrdering(COrderEnums::block_dp_asc);
  BOOST_CHECK_EQUAL(ring.ordering().getOrderCode(), COrderEnums::block_dp_asc);
  ring.changeOrdering(5);
  BOOST_CHECK_EQUAL(ring.ordering().getOrderCode(), COrderEnums::lp);//default is lp
  BOOST_CHECK_EQUAL(defaultr.ordering().getOrderCode(), COrderEnums::lp);
  // change defaultr
  defaultr.changeOrdering(COrderEnums::lp);
  BOOST_CHECK_EQUAL(ring.ordering().getOrderCode(), COrderEnums::lp);
  BOOST_CHECK_EQUAL(defaultr.ordering().getOrderCode(), COrderEnums::lp);
  defaultr.changeOrdering(COrderEnums::dlex);
  BOOST_CHECK_EQUAL(ring.ordering().getOrderCode(), COrderEnums::dlex);
  BOOST_CHECK_EQUAL(defaultr.ordering().getOrderCode(), COrderEnums::dlex);
  defaultr.changeOrdering(COrderEnums::dp_asc);
  BOOST_CHECK_EQUAL(ring.ordering().getOrderCode(), COrderEnums::dp_asc);
  BOOST_CHECK_EQUAL(defaultr.ordering().getOrderCode(), COrderEnums::dp_asc);
  defaultr.changeOrdering(COrderEnums::block_dlex);
  BOOST_CHECK_EQUAL(ring.ordering().getOrderCode(), COrderEnums::block_dlex);
  BOOST_CHECK_EQUAL(defaultr.ordering().getOrderCode(), COrderEnums::block_dlex);
  defaultr.changeOrdering(COrderEnums::block_dp_asc);
  BOOST_CHECK_EQUAL(ring.ordering().getOrderCode(), COrderEnums::block_dp_asc);
  BOOST_CHECK_EQUAL(defaultr.ordering().getOrderCode(), COrderEnums::block_dp_asc);
  defaultr.changeOrdering(-1);
  BOOST_CHECK_EQUAL(ring.ordering().getOrderCode(), COrderEnums::lp);//default is lp
  BOOST_CHECK_EQUAL(defaultr.ordering().getOrderCode(), COrderEnums::lp);
  defaultr.changeOrdering(COrderEnums::block_dp_asc);
  BOOST_CHECK_EQUAL(ring.ordering().getOrderCode(), COrderEnums::block_dp_asc);
  defaultr.changeOrdering(5);
  BOOST_CHECK_EQUAL(ring.ordering().getOrderCode(), COrderEnums::lp);//default is lp
  BOOST_CHECK_EQUAL(defaultr.ordering().getOrderCode(), COrderEnums::lp);
  // change empty
  empty.changeOrdering(COrderEnums::lp);
  BOOST_CHECK_EQUAL(ring.ordering().getOrderCode(), COrderEnums::lp);
  empty.changeOrdering(COrderEnums::dlex);
  BOOST_CHECK_EQUAL(ring.ordering().getOrderCode(), COrderEnums::lp); // Is this supposed to be like this?
  empty.changeOrdering(COrderEnums::dp_asc);
  BOOST_CHECK_EQUAL(ring.ordering().getOrderCode(), COrderEnums::lp); // Is this supposed to be like this?
  empty.changeOrdering(COrderEnums::block_dlex);
  BOOST_CHECK_EQUAL(ring.ordering().getOrderCode(), COrderEnums::lp); // Is this supposed to be like this?
  empty.changeOrdering(COrderEnums::block_dp_asc);
  BOOST_CHECK_EQUAL(ring.ordering().getOrderCode(), COrderEnums::lp); // Is this supposed to be like this?
  empty.changeOrdering(-1);
  BOOST_CHECK_EQUAL(ring.ordering().getOrderCode(), COrderEnums::lp);//default is lp
  empty.changeOrdering(COrderEnums::block_dp_asc);
  BOOST_CHECK_EQUAL(ring.ordering().getOrderCode(), COrderEnums::lp); // Is this supposed to be like this?
  empty.changeOrdering(5);
  BOOST_CHECK_EQUAL(ring.ordering().getOrderCode(), COrderEnums::lp);//default is lp
}

BOOST_AUTO_TEST_CASE(test_dd_type) {

  ring_type ring(3, 0, false);
  ring_type empty(0);
  ring.activate();
  ring_type defaultr;
  ring.setVariableName(0, "x");
  ring.setVariableName(1, "y");
  ring.setVariableName(2, "z");
  ring.setVariableName(3, "v");
  output_test_stream output;

  BOOST_TEST_MESSAGE( "variable" );
  output << ring.variable(0);
  BOOST_CHECK(output.is_equal("{{x}}"));
  output << ring.variable(1);
  BOOST_CHECK(output.is_equal("{{y}}"));
  output << ring.variable(2);
  BOOST_CHECK(output.is_equal("{{z}}"));
  BOOST_CHECK_THROW(ring.variable(3), PBoRiError);
  BOOST_CHECK_THROW(ring.variable(-1), PBoRiError);
  output << defaultr.variable(0);
  BOOST_CHECK(output.is_equal("{{x}}"));
  output << defaultr.variable(1);
  BOOST_CHECK(output.is_equal("{{y}}"));
  output << defaultr.variable(2);
  BOOST_CHECK(output.is_equal("{{z}}"));
  BOOST_CHECK_THROW(defaultr.variable(3), PBoRiError);
  BOOST_CHECK_THROW(defaultr.variable(-1), PBoRiError);
  BOOST_CHECK_THROW(empty.variable(-1), PBoRiError);
  BOOST_CHECK_THROW(empty.variable(0), PBoRiError);
  BOOST_CHECK_THROW(empty.variable(-1), PBoRiError);

  BOOST_TEST_MESSAGE( "zero" );
  output << ring.zero();
  BOOST_CHECK(output.is_equal("{}"));
  output << defaultr.zero();
  BOOST_CHECK(output.is_equal("{}"));
  output << empty.zero();
  BOOST_CHECK(output.is_equal("{}"));

  BOOST_TEST_MESSAGE( "one" );
  output << ring.one();
  BOOST_CHECK(output.is_equal("{{}}"));
  output << defaultr.one();
  BOOST_CHECK(output.is_equal("{{}}"));
  output << empty.one();
  BOOST_CHECK(output.is_equal("{{}}"));

  BOOST_TEST_MESSAGE( "constant" );
  output << ring.constant(true);
  BOOST_CHECK(output.is_equal("{{}}"));
  BOOST_CHECK_EQUAL(ring.constant(true), ring.one());
  output << ring.constant(false);
  BOOST_CHECK(output.is_equal("{}"));
  BOOST_CHECK_EQUAL(ring.constant(false), ring.zero());
  output << defaultr.constant(true);
  BOOST_CHECK(output.is_equal("{{}}"));
  BOOST_CHECK_EQUAL(defaultr.constant(true), defaultr.one());
  output << defaultr.constant(false);
  BOOST_CHECK(output.is_equal("{}"));
  BOOST_CHECK_EQUAL(defaultr.constant(false), defaultr.zero());
  output << empty.constant(true);
  BOOST_CHECK(output.is_equal("{{}}"));
  BOOST_CHECK_EQUAL(empty.constant(true), empty.one());
  output << empty.constant(false);
  BOOST_CHECK(output.is_equal("{}"));
  BOOST_CHECK_EQUAL(empty.constant(false), empty.zero());
}

BOOST_AUTO_TEST_SUITE_END()
