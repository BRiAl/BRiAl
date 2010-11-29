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
#include "COrderingBase.h"

USING_NAMESPACE_PBORI

struct Fring {
  typedef BoolePolyRing ring_type;
  Fring() { BOOST_TEST_MESSAGE( "setup fixture" ); }

  ~Fring() { BOOST_TEST_MESSAGE( "teardown fixture" ); }
};

BOOST_FIXTURE_TEST_SUITE(BoolePolyRingTestSuite, Fring )

BOOST_AUTO_TEST_CASE(test_constructors) {

  BOOST_TEST_MESSAGE( "Constructors..." );
  ring_type ring1(0,COrderEnums::lp,false);
  BOOST_CHECK_EQUAL(ring1.nVariables(), 0);
  BOOST_CHECK_EQUAL(ring1.ordering().getOrderCode(), COrderEnums::lp);
  ring_type defaultr;
  BOOST_CHECK_NE(ring1.hash(),defaultr.hash());
  ring_type ring2(1,COrderEnums::dlex,false);
  BOOST_CHECK_EQUAL(ring2.nVariables(), 1);
  BOOST_CHECK_EQUAL(ring2.ordering().getOrderCode(), COrderEnums::dlex);
  defaultr = ring_type();
  BOOST_CHECK_NE(ring2.hash(),defaultr.hash());
  ring_type ring3(2,COrderEnums::dp_asc,true);
  BOOST_CHECK_EQUAL(ring3.nVariables(), 2);
  BOOST_CHECK_EQUAL(ring3.ordering().getOrderCode(), COrderEnums::dp_asc);
  defaultr = ring_type();
  BOOST_CHECK_EQUAL(ring3.hash(),defaultr.hash());
  ring_type ring4(3,COrderEnums::block_dlex,false);
  BOOST_CHECK_EQUAL(ring4.nVariables(), 3);
  BOOST_CHECK_EQUAL(ring4.ordering().getOrderCode(), COrderEnums::block_dlex);
  defaultr = ring_type();
  BOOST_CHECK_NE(ring4.hash(),defaultr.hash());
  ring_type ring5(4,COrderEnums::block_dp_asc,false);
  BOOST_CHECK_EQUAL(ring5.nVariables(), 4);
  BOOST_CHECK_EQUAL(ring5.ordering().getOrderCode(), COrderEnums::block_dp_asc);
  defaultr = ring_type();
  BOOST_CHECK_NE(ring5.hash(),defaultr.hash());
  ring5.activate();
  defaultr = ring_type();
  BOOST_CHECK_EQUAL(ring5.hash(),defaultr.hash());
  
  ring_type ring11(0, get_ordering(COrderEnums::lp)); 
  BOOST_CHECK_EQUAL(ring11.nVariables(), 0);
  BOOST_CHECK_EQUAL(ring11.ordering().getOrderCode(), COrderEnums::lp);

  defaultr = ring_type();

  BOOST_CHECK_NE(ring11.hash(),defaultr.hash());
  ring_type ring12(1,get_ordering(COrderEnums::dlex));
  BOOST_CHECK_EQUAL(ring12.nVariables(), 1);
  BOOST_CHECK_EQUAL(ring12.ordering().getOrderCode(), COrderEnums::dlex);
  defaultr = ring_type();
  BOOST_CHECK_NE(ring12.hash(),defaultr.hash());

  ring_type ring13(2,get_ordering(COrderEnums::dp_asc));
  BOOST_CHECK_EQUAL(ring13.nVariables(), 2);
  BOOST_CHECK_EQUAL(ring13.ordering().getOrderCode(), COrderEnums::dp_asc);
  defaultr = ring_type();
  BOOST_CHECK_NE(ring13.hash(),defaultr.hash());
  ring_type ring14(3,get_ordering(COrderEnums::block_dlex));
  BOOST_CHECK_EQUAL(ring14.nVariables(), 3);
  BOOST_CHECK_EQUAL(ring14.ordering().getOrderCode(), COrderEnums::block_dlex);
  defaultr = ring_type();
  BOOST_CHECK_NE(ring14.hash(),defaultr.hash());
  ring_type ring15(4,get_ordering(COrderEnums::block_dp_asc));
  BOOST_CHECK_EQUAL(ring15.nVariables(), 4);
  BOOST_CHECK_EQUAL(ring15.ordering().getOrderCode(), COrderEnums::block_dp_asc);
  defaultr = ring_type();
  BOOST_CHECK_NE(ring15.hash(),defaultr.hash());
  ring15.activate();
  defaultr = ring_type();
  BOOST_CHECK_EQUAL(ring15.hash(),defaultr.hash());
 
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

  // note: setVariableName also sets names for unused variables
  ring.setVariableName(0, "x");
  defaultr.setVariableName(1, "y");
  ring.setVariableName(3, "v");
  BOOST_CHECK_THROW( defaultr.setVariableName(-1, "w"), std::bad_alloc);

  empty.setVariableName(0, "x");
  empty.setVariableName(1, "y");
  BOOST_CHECK_THROW(empty.setVariableName(-1, "z"), std::bad_alloc);

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

  // change empty - other ring stays the same
  empty.changeOrdering(COrderEnums::lp);
  BOOST_CHECK_EQUAL(ring.ordering().getOrderCode(), COrderEnums::lp);
  empty.changeOrdering(COrderEnums::dlex);
  BOOST_CHECK_EQUAL(empty.ordering().getOrderCode(), COrderEnums::dlex); 
  BOOST_CHECK_EQUAL(ring.ordering().getOrderCode(), COrderEnums::lp); 
  empty.changeOrdering(COrderEnums::dp_asc);
  BOOST_CHECK_EQUAL(empty.ordering().getOrderCode(), COrderEnums::dp_asc); 
  BOOST_CHECK_EQUAL(ring.ordering().getOrderCode(), COrderEnums::lp); 
  empty.changeOrdering(COrderEnums::block_dlex);
  BOOST_CHECK_EQUAL(empty.ordering().getOrderCode(), COrderEnums::block_dlex);
  BOOST_CHECK_EQUAL(ring.ordering().getOrderCode(), COrderEnums::lp);
  empty.changeOrdering(COrderEnums::block_dp_asc);
  BOOST_CHECK_EQUAL(empty.ordering().getOrderCode(), COrderEnums::block_dp_asc); 
  BOOST_CHECK_EQUAL(ring.ordering().getOrderCode(), COrderEnums::lp);

  empty.changeOrdering(-1);
  BOOST_CHECK_EQUAL(empty.ordering().getOrderCode(), COrderEnums::lp);//default is lp
  BOOST_CHECK_EQUAL(ring.ordering().getOrderCode(), COrderEnums::lp);
  empty.changeOrdering(COrderEnums::block_dp_asc);
  BOOST_CHECK_EQUAL(empty.ordering().getOrderCode(), COrderEnums::block_dp_asc); 
  BOOST_CHECK_EQUAL(ring.ordering().getOrderCode(), COrderEnums::lp);
  empty.changeOrdering(5);
  BOOST_CHECK_EQUAL(empty.ordering().getOrderCode(), COrderEnums::lp);//default
                                                                     //is lp
  BOOST_CHECK_EQUAL(ring.ordering().getOrderCode(), COrderEnums::lp);

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

BOOST_AUTO_TEST_CASE(test_coerce) {
  ring_type empty(0);
  ring_type small(2,0,false);
  ring_type ring(4, 0, false);
  ring.activate();
  ring_type defaultr;
  ring.setVariableName(0, "*");
  ring.setVariableName(1, "ö");
  ring.setVariableName(2, "-");
  ring.setVariableName(3, "v");
  ring.setVariableName(4, "a");
  output_test_stream output;
  BooleVariable x(0);
  BooleVariable y(1);
  BooleVariable z(2);
  BooleVariable v(3);
  BoolePolynomial poly = x*y*v + y*z*v + y*v;
  BooleMonomial monom = x*y*z;

  BOOST_TEST_MESSAGE( "coerce" );
  output << ring.coerce(poly);
  BOOST_CHECK(output.is_equal("**ö*v + ö*-*v + ö*v"));
  output << ring.coerce(monom);
  BOOST_CHECK(output.is_equal("**ö*-"));
  output << ring.coerce(x);
  BOOST_CHECK(output.is_equal("*"));
  output << ring.coerce(y);
  BOOST_CHECK(output.is_equal("ö"));
  output << ring.coerce(z);
  BOOST_CHECK(output.is_equal("-"));
  output << ring.coerce(v);
  BOOST_CHECK(output.is_equal("v"));
  output << defaultr.coerce(poly);
  BOOST_CHECK(output.is_equal("**ö*v + ö*-*v + ö*v"));
  output << defaultr.coerce(monom);
  BOOST_CHECK(output.is_equal("**ö*-"));
  output << defaultr.coerce(x);
  BOOST_CHECK(output.is_equal("*"));
  output << defaultr.coerce(y);
  BOOST_CHECK(output.is_equal("ö"));
  output << defaultr.coerce(z);
  BOOST_CHECK(output.is_equal("-"));
  output << defaultr.coerce(v);
  BOOST_CHECK(output.is_equal("v"));
  BOOST_CHECK_THROW(empty.coerce(poly), PBoRiError);
  BOOST_CHECK_THROW(empty.coerce(monom), PBoRiError);
  BOOST_CHECK_THROW(empty.coerce(x), PBoRiError);
  BOOST_CHECK_THROW(small.coerce(poly), PBoRiError);
  output << defaultr.coerce(monom);
  BOOST_CHECK(output.is_equal("**ö*-"));
  output << defaultr.coerce(x);
  BOOST_CHECK(output.is_equal("*"));
  output << defaultr.coerce(y);
  BOOST_CHECK(output.is_equal("ö"));
  output << defaultr.coerce(z);
  BOOST_CHECK(output.is_equal("-"));
  BOOST_CHECK_THROW(small.coerce(v), PBoRiError);
}

BOOST_AUTO_TEST_CASE(test_hash) {

  BOOST_TEST_MESSAGE( "hash" );
  ring_type ring1(3, 0, false);
  ring_type ring2(3, 0, false);
  BOOST_CHECK_NE(ring1.hash(), ring2.hash()); // Not equal because different
                                              // instances (but of likewise rings)
  ring_type defaultr;
  BOOST_CHECK_NE(ring1.hash(), defaultr.hash());
  ring1.activate();
  defaultr = ring_type();
  BOOST_CHECK_EQUAL(ring1.hash(), defaultr.hash());
  ring_type empty1(0, 0, false);
  ring_type empty2(0, 0, false);
  BOOST_CHECK_NE(empty1.hash(), empty2.hash());// Not equal because different
                                // instances (but of likewise rings)
}

BOOST_AUTO_TEST_SUITE_END()
