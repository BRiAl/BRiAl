// -*- c++ -*-
//*****************************************************************************
/** @file BooleSetTest.cc
 *
 * @author Ket Kalda, Alexander Dreyer
 * @date 2010-10-07
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
#include "BoolePolyRing.h"

USING_NAMESPACE_PBORI

struct Fset {
  typedef BooleSet set_type;
  Fset():
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
    }

    ~Fset() { BOOST_TEST_MESSAGE( "teardown fixture" ); }

    BoolePolyRing ring;
    BooleVariable x, y, z, v, w;
};

BOOST_FIXTURE_TEST_SUITE(BooleSetTestSuite, Fset )

BOOST_AUTO_TEST_CASE(test_constructors) {
  set_type set;
  output_test_stream output;
  output << set;
  BOOST_CHECK(output.is_equal("{}"));
  set.add(x);
  set_type set2;
  set_type set3 = set_type(0,set,set2);
  output << set3;
  BOOST_CHECK(output.is_equal("{}"));
  BoolePolynomial poly = x*y*z + v*z*w + x;
  set = poly.set();
  output << set;
  BOOST_CHECK(output.is_equal("{{x,y,z}, {x}, {z,v,w}}"));
}

BOOST_AUTO_TEST_CASE(test_accessors) {

  
}

BOOST_AUTO_TEST_CASE(test_add_own) {

  BoolePolynomial poly = x*y*z + v*z - x*v + y;
  set_type set = poly.set();
  BOOST_CHECK(set.owns(x*y*z));
  BOOST_CHECK(set.owns(v*z));
  BOOST_CHECK(set.owns(x*v));
  BOOST_CHECK(set.owns(y));
  BOOST_CHECK(!set.owns(x));
  BOOST_CHECK(!set.owns(z));
  BOOST_CHECK(!set.owns(v));
  BOOST_CHECK(!set.owns(w));
  BOOST_CHECK(!set.owns(x*v*z));
  BOOST_CHECK(!set.owns(BooleMonomial()));

  set_type addedset = set.add(x);
  addedset = addedset.add(x*v*z);
  BOOST_CHECK(addedset.owns(x*y*z));
  BOOST_CHECK(addedset.owns(v*z));
  BOOST_CHECK(addedset.owns(x*v));
  BOOST_CHECK(addedset.owns(y));
  BOOST_CHECK(addedset.owns(x));
  BOOST_CHECK(!addedset.owns(z));
  BOOST_CHECK(!addedset.owns(v));
  BOOST_CHECK(!addedset.owns(w));
  BOOST_CHECK(addedset.owns(x*v*z));
}

BOOST_AUTO_TEST_CASE(test_division) {

  BOOST_TEST_MESSAGE( "divisors" );

  output_test_stream output;
  BoolePolynomial poly = x*y*z + v*z - x*v + y;
  set_type set = poly.set();
  set_type result = set.divisorsOf(x);
  output << result;
  BOOST_CHECK(output.is_equal("{}"));
  result = set.divisorsOf(y);
  output << result;
  BOOST_CHECK(output.is_equal("{{y}}"));
  result = set.divisorsOf(z);
  output << result;
  BOOST_CHECK(output.is_equal("{}"));
  result = set.divisorsOf(v);
  output << result;
  BOOST_CHECK(output.is_equal("{}"));
  result = set.divisorsOf(w);
  output << result;
  BOOST_CHECK(output.is_equal("{}"));

  BOOST_TEST_MESSAGE( "divide" );

  result = set.divide(x);
  output << result;
  BOOST_CHECK(output.is_equal("{{y,z}, {v}}"));
  result = set.divide(y);
  output << result;
  BOOST_CHECK(output.is_equal("{{x,z}, {}}"));
  result = set.divide(z);
  output << result;
  BOOST_CHECK(output.is_equal("{{x,y}, {v}}"));
  result = set.divide(v);
  output << result;
  BOOST_CHECK(output.is_equal("{{x}, {z}}"));
  result = set.divide(w);
  output << result;
  BOOST_CHECK(output.is_equal("{}"));
  result = set.divide(x*v);
  output << result;
  BOOST_CHECK(output.is_equal("{{}}"));
  result = set.divide(x*y);
  output << result;
  BOOST_CHECK(output.is_equal("{{z}}"));
  result = set.divide(x*v*y);
  output << result;
  BOOST_CHECK(output.is_equal("{}"));
  result = set.divide(BooleMonomial());
  output << result;
  BOOST_CHECK(output.is_equal("{{x,y,z}, {x,v}, {y}, {z,v}}"));
  set_type empty;
  result = empty.divide(x*v*y);
  output << result;
  BOOST_CHECK(output.is_equal("{}"));
  result = empty.divide(BooleMonomial());
  output << result;
  BOOST_CHECK(output.is_equal("{}"));

  BOOST_TEST_MESSAGE( "firstDivisorsOf" );
  BoolePolynomial poly2 = x*y*z + x;
  set_type set2 = poly2.set();
  result = set.firstDivisorsOf(set2);
  output << result;
  BOOST_CHECK(output.is_equal("{{x,y,z}, {y}}"));
}

BOOST_AUTO_TEST_SUITE_END()
