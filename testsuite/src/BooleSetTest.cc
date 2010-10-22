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
#include "BooleExponent.h"
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
      poly = x*y*z + v*z - x*v + y;
    }

    ~Fset() { BOOST_TEST_MESSAGE( "teardown fixture" ); }

    BoolePolyRing ring;
    BooleVariable x, y, z, v, w;
    BoolePolynomial poly;
};

BOOST_FIXTURE_TEST_SUITE(BooleSetTestSuite, Fset )

BOOST_AUTO_TEST_CASE(test_constructors) {
  set_type set;
  output_test_stream output;
  output << set;
  BOOST_CHECK(output.is_equal("{}"));
  set = set.add(x);
  set_type set2;
  BOOST_CHECK_THROW(set_type(0,set,set2), PBoRiGenericError<CTypes::invalid_ite>);
  set_type set1;
  set1 = set1.add(y);
  set_type set3 = set_type(0,set1,set2);
  output << set3;
  BOOST_CHECK(output.is_equal("{{x,y}}"));
  set = poly.set();
  output << set;
  BOOST_CHECK(output.is_equal("{{x,y,z}, {x,v}, {y}, {z,v}}"));
}

BOOST_AUTO_TEST_CASE(test_variables) {

  output_test_stream output;
  set_type set = poly.set();
  set_type empty;

  BOOST_TEST_MESSAGE( "usedVariables, usedVariablesExp" ); // BooleMonomial is used as a BooleVariable set here
  BOOST_CHECK_EQUAL(set.usedVariables(),x*y*z*v);
  BOOST_CHECK_EQUAL(empty.usedVariables(),BooleMonomial());
  set_type added_set = set.add(BooleMonomial());
  BOOST_CHECK_EQUAL(added_set.usedVariables(),x*y*z*v);
  BooleExponent exp = set.usedVariablesExp();
  output << exp;
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3)"));
  exp = empty.usedVariablesExp();
  output << exp;
  BOOST_CHECK(output.is_equal("()"));

  BOOST_TEST_MESSAGE( "lastLexicographicalTerm" );
  BOOST_CHECK_EQUAL(set.lastLexicographicalTerm(), BooleMonomial(v*z));
  BOOST_CHECK_THROW(empty.lastLexicographicalTerm(),  PBoRiError);

  set_type simple = empty.add(BooleMonomial());
  BOOST_CHECK_EQUAL(simple.lastLexicographicalTerm(), BooleMonomial());

  simple = empty.add(x);
  BOOST_CHECK_EQUAL(simple.lastLexicographicalTerm(), x);

  BOOST_TEST_MESSAGE( "minimalElements" );
  set_type set2 = set.minimalElements();
  output << set2;
  BOOST_CHECK(output.is_equal("{{x,v}, {y}, {z,v}}"));
  set2 = empty.minimalElements();
  output << set2;
  BOOST_CHECK(output.is_equal("{}"));
  BOOST_CHECK_EQUAL(set2,empty);

  BOOST_TEST_MESSAGE( "hasTermOfVariables" );// Strange? Does this do what it is supposed to?
  BOOST_CHECK(set.hasTermOfVariables(x*v));
  BOOST_CHECK(set.hasTermOfVariables(x*y*z));
  BOOST_CHECK(set.hasTermOfVariables(x*z*y));
  BOOST_CHECK(set.hasTermOfVariables(x*z*v));
  BOOST_CHECK(set.hasTermOfVariables(y));
  BOOST_CHECK(set.hasTermOfVariables(z*v));
  BOOST_CHECK(!set.hasTermOfVariables(x));
  BOOST_CHECK(!set.hasTermOfVariables(z));
  BOOST_CHECK(!set.hasTermOfVariables(v));
  BOOST_CHECK(!set.hasTermOfVariables(w));
  BOOST_CHECK(set.hasTermOfVariables(x*y));
  BOOST_CHECK(set.hasTermOfVariables(y*x));
  BOOST_CHECK(set.hasTermOfVariables(y*z));
  BOOST_CHECK(set.hasTermOfVariables(z*y));
  BOOST_CHECK(!set.hasTermOfVariables(BooleMonomial()));
  BOOST_CHECK(!empty.hasTermOfVariables(y));
  BOOST_CHECK(!empty.hasTermOfVariables(BooleMonomial()));

  BOOST_TEST_MESSAGE( "contains" );

  BOOST_CHECK(set.contains(empty));
  BOOST_CHECK(set.contains(empty));


  BOOST_CHECK(set.contains(set));
  BOOST_CHECK(!empty.contains(ring.one()));
  BOOST_CHECK(!set.contains(ring.one()));

  set2 = empty.add(x*y*z);
 
  BOOST_CHECK(set2.contains(set) == false);
  BOOST_CHECK(set.contains(set2));

  BOOST_CHECK(set.contains(BoolePolynomial(y).set()));

  BOOST_TEST_MESSAGE( "emptyElement" );
  set_type emptyEl = empty.emptyElement();
  output << emptyEl;
  BOOST_CHECK(output.is_equal("{}"));
  BOOST_CHECK_EQUAL(emptyEl, empty);
  emptyEl = set.emptyElement();
  output << emptyEl;
  BOOST_CHECK(output.is_equal("{}"));
  BOOST_CHECK_EQUAL(emptyEl, empty);

  BOOST_TEST_MESSAGE( "countIndex" );
  BOOST_CHECK_EQUAL(set.countIndex(0),2);
  BOOST_CHECK_EQUAL(set.countIndex(1),2);
  BOOST_CHECK_EQUAL(set.countIndex(2),2);
  BOOST_CHECK_EQUAL(set.countIndex(3),2);
  BOOST_CHECK_EQUAL(set.countIndex(4),0);
  BOOST_CHECK_EQUAL(set.countIndex(5),0);
  BOOST_CHECK_EQUAL(empty.countIndex(0),0);
  BOOST_CHECK_EQUAL(empty.countIndex(1),0);
  BOOST_CHECK_EQUAL(empty.countIndex(2),0);
  BOOST_CHECK_EQUAL(empty.countIndex(3),0);
  BOOST_CHECK_EQUAL(empty.countIndex(4),0);
  BOOST_CHECK_EQUAL(empty.countIndex(5),0);

  BOOST_TEST_MESSAGE( "countIndexDouble" );
  BOOST_CHECK_EQUAL(set.countIndexDouble(0),2);
  BOOST_CHECK_EQUAL(set.countIndexDouble(1),2);
  BOOST_CHECK_EQUAL(set.countIndexDouble(2),2);
  BOOST_CHECK_EQUAL(set.countIndexDouble(3),2);
  BOOST_CHECK_EQUAL(set.countIndexDouble(4),0);
  BOOST_CHECK_EQUAL(set.countIndexDouble(5),0);
  BOOST_CHECK_EQUAL(empty.countIndexDouble(0),0);
  BOOST_CHECK_EQUAL(empty.countIndexDouble(1),0);
  BOOST_CHECK_EQUAL(empty.countIndexDouble(2),0);
  BOOST_CHECK_EQUAL(empty.countIndexDouble(3),0);
  BOOST_CHECK_EQUAL(empty.countIndexDouble(4),0);
  BOOST_CHECK_EQUAL(empty.countIndexDouble(5),0);

  BOOST_TEST_MESSAGE( "containsDivisorsOfDecDeg" );
  BOOST_CHECK(!set.containsDivisorsOfDecDeg(x*v));
  BOOST_CHECK(!set.containsDivisorsOfDecDeg(x*y*z));
  BOOST_CHECK(!set.containsDivisorsOfDecDeg(x*z*y));
  BOOST_CHECK(!set.containsDivisorsOfDecDeg(y));
  BOOST_CHECK(!set.containsDivisorsOfDecDeg(z*v));
  BOOST_CHECK(!set.containsDivisorsOfDecDeg(x));
  BOOST_CHECK(!set.containsDivisorsOfDecDeg(z));
  BOOST_CHECK(!set.containsDivisorsOfDecDeg(v));
  BOOST_CHECK(!set.containsDivisorsOfDecDeg(w));
  BOOST_CHECK(!set.containsDivisorsOfDecDeg(x*y));
  BOOST_CHECK(!set.containsDivisorsOfDecDeg(y*x));
  BOOST_CHECK(!set.containsDivisorsOfDecDeg(y*z));
  BOOST_CHECK(!set.containsDivisorsOfDecDeg(z*y));
  BOOST_CHECK(set.containsDivisorsOfDecDeg(BooleMonomial()));
  BOOST_CHECK(!empty.containsDivisorsOfDecDeg(x));
  BOOST_CHECK(!empty.containsDivisorsOfDecDeg(x*y*z));
  BOOST_CHECK(empty.containsDivisorsOfDecDeg(BooleMonomial()));

  set_type set1;
  set1 = set1.add(x*y*z);
  set1 = set1.add(x*y);
  set1 = set1.add(x*z);
  set1 = set1.add(y*z);
  BOOST_CHECK(set1.containsDivisorsOfDecDeg(x*y*z));
  BOOST_CHECK(!set1.containsDivisorsOfDecDeg(x*y));
  BOOST_CHECK(!set1.containsDivisorsOfDecDeg(y*z));
  BOOST_CHECK(!set1.containsDivisorsOfDecDeg(x*z));
  set1 = set1.add(x);
  set1 = set1.add(y);
  set1 = set1.add(z);
  BOOST_CHECK(set1.containsDivisorsOfDecDeg(x*y));
  BOOST_CHECK(set1.containsDivisorsOfDecDeg(y*z));
  BOOST_CHECK(set1.containsDivisorsOfDecDeg(x*z));
  BOOST_CHECK(!set1.containsDivisorsOfDecDeg(x));
  BOOST_CHECK(!set1.containsDivisorsOfDecDeg(y));
  BOOST_CHECK(!set1.containsDivisorsOfDecDeg(z));
  set1 = set1.add(BooleMonomial());
  BOOST_CHECK(set1.containsDivisorsOfDecDeg(x));
  BOOST_CHECK(set1.containsDivisorsOfDecDeg(y));
  BOOST_CHECK(set1.containsDivisorsOfDecDeg(z));
  BOOST_CHECK(set1.containsDivisorsOfDecDeg(BooleMonomial()));
}

BOOST_AUTO_TEST_CASE(test_properties) {

  BOOST_TEST_MESSAGE( "isSingleton, isSingletonOrPair, isPair" );

  set_type set = poly.set();
  BOOST_CHECK(!set.isSingleton());
  BOOST_CHECK(!set.isSingletonOrPair());
  BOOST_CHECK(!set.isPair());

  set_type empty;
  BOOST_CHECK(empty.isSingleton());
  BOOST_CHECK(empty.isSingletonOrPair());
  BOOST_CHECK(!empty.isPair());
  empty = empty.add(BooleMonomial());
  BOOST_CHECK(empty.isSingleton());
  BOOST_CHECK(empty.isSingletonOrPair());
  BOOST_CHECK(!empty.isPair());

  set_type single;
  single = single.add(x);
  BOOST_CHECK(single.isSingleton());
  BOOST_CHECK(single.isSingletonOrPair());
  BOOST_CHECK(!single.isPair());
  single = single.add(BooleMonomial());
  BOOST_CHECK(!single.isSingleton());
  BOOST_CHECK(single.isSingletonOrPair());
  BOOST_CHECK(single.isPair());

  set_type pair;
  pair = pair.add(x);
  pair = pair.add(y);
  BOOST_CHECK(!pair.isSingleton());
  BOOST_CHECK(pair.isSingletonOrPair());
  BOOST_CHECK(pair.isPair());
  pair = pair.add(BooleMonomial());
  BOOST_CHECK(!pair.isSingleton());
  BOOST_CHECK(!pair.isSingletonOrPair());
  BOOST_CHECK(!pair.isPair());
}

BOOST_AUTO_TEST_CASE(test_add_own) {

  set_type set = poly.set();
  set_type empty;
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
  BOOST_CHECK(!empty.owns(x));
  BOOST_CHECK(!empty.owns(y));
  BOOST_CHECK(!empty.owns(z));
  BOOST_CHECK(!empty.owns(v));
  BOOST_CHECK(!empty.owns(w));
  BOOST_CHECK(!empty.owns(BooleMonomial()));

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
  addedset = empty.add(x);
  addedset = addedset.add(y);
  addedset = addedset.add(v*w);
  BOOST_CHECK(addedset.owns(x));
  BOOST_CHECK(addedset.owns(y));
  BOOST_CHECK(addedset.owns(v*w));
  BOOST_CHECK(!addedset.owns(v));
  BOOST_CHECK(!addedset.owns(w));

  set_type empty2;
  empty2 = empty2.add(x);
  BOOST_CHECK(empty2.owns(x));

  BOOST_CHECK(!set.ownsOne());
  addedset = set.add(BooleMonomial());
  BOOST_CHECK(addedset.ownsOne());
  BOOST_CHECK(!empty.ownsOne());
  addedset = empty.add(BooleMonomial());
  BOOST_CHECK(addedset.ownsOne());
}

BOOST_AUTO_TEST_CASE(test_division) {

  BOOST_TEST_MESSAGE( "divisors" );

  output_test_stream output;
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

BOOST_AUTO_TEST_CASE(test_size_values) {

  set_type set = poly.set();
  set = set.add(x);
  set = set.add(x*y);
  std::cout << set << std::endl;
  set_type empty;
  set_type almost_empty;
  almost_empty = almost_empty.add(BooleMonomial());

  BOOST_TEST_MESSAGE( "size" );
  BOOST_CHECK_EQUAL(set.size(),6);
  BOOST_CHECK_EQUAL(empty.size(),0);
  BOOST_CHECK_EQUAL(almost_empty.size(),1);

  BOOST_TEST_MESSAGE( "length" );
  BOOST_CHECK_EQUAL(set.length(),6);
  BOOST_CHECK_EQUAL(empty.length(),0);
  BOOST_CHECK_EQUAL(almost_empty.length(),1);

  BOOST_TEST_MESSAGE( "nVariables" );
  BOOST_CHECK_EQUAL(set.nVariables(),5);
  BOOST_CHECK_EQUAL(empty.nVariables(),5);
  BOOST_CHECK_EQUAL(almost_empty.nVariables(),5);

  BOOST_TEST_MESSAGE( "sizeDouble" );
  BOOST_CHECK_EQUAL(set.sizeDouble(),6);
  BOOST_CHECK_EQUAL(empty.sizeDouble(),0);
  BOOST_CHECK_EQUAL(almost_empty.sizeDouble(),1);
}

BOOST_AUTO_TEST_CASE(test_compute) {

  set_type set = poly.set();
  set_type empty;
  output_test_stream output;

  BOOST_TEST_MESSAGE( "multiplesOf" );
  output << set.multiplesOf(x);
  BOOST_CHECK(output.is_equal("{{x,y,z}, {x,v}}"));
  output << set.multiplesOf(y);
  BOOST_CHECK(output.is_equal("{{x,y,z}, {y}}"));
  output << set.multiplesOf(z);
  BOOST_CHECK(output.is_equal("{{x,y,z}, {z,v}}"));
  output << set.multiplesOf(v);
  BOOST_CHECK(output.is_equal("{{x,v}, {z,v}}"));
  output << set.multiplesOf(x*y);
  BOOST_CHECK(output.is_equal("{{x,y,z}}"));
  output << set.multiplesOf(x*y*z);
  BOOST_CHECK(output.is_equal("{{x,y,z}}"));
  output << set.multiplesOf(x*y);
  BOOST_CHECK(output.is_equal("{{x,y,z}}"));
  output << set.multiplesOf(z*v);
  BOOST_CHECK(output.is_equal("{{z,v}}"));
  output << set.multiplesOf(y*v);
  BOOST_CHECK(output.is_equal("{}"));
  output << set.multiplesOf(w);
  BOOST_CHECK(output.is_equal("{}"));
  output << set.multiplesOf(BooleMonomial());
  BOOST_CHECK(output.is_equal("{{x,y,z}, {x,v}, {y}, {z,v}}"));
  output << empty.multiplesOf(x);
  BOOST_CHECK(output.is_equal("{}"));
  output << empty.multiplesOf(x*y);
  BOOST_CHECK(output.is_equal("{}"));
  output << empty.multiplesOf(BooleMonomial());
  BOOST_CHECK(output.is_equal("{}"));

  BOOST_TEST_MESSAGE( "existAbstract" ); // It is not clear what this function is supposed to do
  output << set.existAbstract(x);
  BOOST_CHECK(output.is_equal("{{y,z}, {y}, {z,v}, {v}}"));
  output << set.existAbstract(y);
  BOOST_CHECK(output.is_equal("{{x,z}, {x,v}, {z,v}, {}}"));
  output << set.existAbstract(z);
  BOOST_CHECK(output.is_equal("{{x,y}, {x,v}, {y}, {v}}"));
  output << set.existAbstract(v);
  BOOST_CHECK(output.is_equal("{{x,y,z}, {x}, {y}, {z}}"));
  output << set.existAbstract(x*y);
  BOOST_CHECK(output.is_equal("{{z,v}, {z}, {v}, {}}"));
  output << set.existAbstract(x*z);
  BOOST_CHECK(output.is_equal("{{y}, {v}}"));
  output << set.existAbstract(x*v);
  BOOST_CHECK(output.is_equal("{{y,z}, {y}, {z}, {}}"));
  output << set.existAbstract(y*z);
  BOOST_CHECK(output.is_equal("{{x,v}, {x}, {v}, {}}"));
  output << set.existAbstract(y*v);
  BOOST_CHECK(output.is_equal("{{x,z}, {x}, {z}, {}}"));
  output << set.existAbstract(x*y*z);
  BOOST_CHECK(output.is_equal("{{v}, {}}"));
  output << set.existAbstract(x*y*v);
  BOOST_CHECK(output.is_equal("{{z}, {}}"));
  output << set.existAbstract(y*z*v);
  BOOST_CHECK(output.is_equal("{{x}, {}}"));
  output << set.existAbstract(x*y*z*v*w);
  BOOST_CHECK(output.is_equal("{{}}"));
  output << set.existAbstract(w);
  BOOST_CHECK(output.is_equal("{{x,y,z}, {x,v}, {y}, {z,v}}"));
  output << set.existAbstract(x*w);
  BOOST_CHECK(output.is_equal("{{y,z}, {y}, {z,v}, {v}}"));
  output << set.existAbstract(BooleMonomial());
  BOOST_CHECK(output.is_equal("{{x,y,z}, {x,v}, {y}, {z,v}}"));
  output << empty.existAbstract(x);
  BOOST_CHECK(output.is_equal("{}"));
  output << empty.existAbstract(x*v);
  BOOST_CHECK(output.is_equal("{}"));
  output << empty.existAbstract(BooleMonomial());
  BOOST_CHECK(output.is_equal("{}"));

  BOOST_TEST_MESSAGE( "cartesianProduct" );
  set_type set1;
  set1 = set1.add(x);
  set1 = set1.add(y);
  set_type set2;
  set2 = set2.add(v);
  set2 = set2.add(w);
  output << set1.cartesianProduct(set2);
  BOOST_CHECK(output.is_equal("{{x,v}, {x,w}, {y,v}, {y,w}}"));
  output << set1.cartesianProduct(empty);
  BOOST_CHECK(output.is_equal("{}"));
  output << empty.cartesianProduct(set1);
  BOOST_CHECK(output.is_equal("{}"));
  output << empty.cartesianProduct(empty);
  BOOST_CHECK(output.is_equal("{}"));
}

BOOST_AUTO_TEST_CASE(test_hash) {

  set_type set = poly.set();
  set_type set2;
  set2 = set2.add(x*y*z);
  set2 = set2.add(x*v);
  set2 = set2.add(y);
  set2 = set2.add(z*v);
  set_type set2cpy = set2;
  set_type empty;
  set_type empty2;
  set_type empty2cpy = empty2;
  output_test_stream output;

  BOOST_TEST_MESSAGE( "hash" );
  BOOST_CHECK_EQUAL(set, set2);
  BOOST_CHECK_EQUAL(set.hash(), set2.hash());
  set2 = set2.add(z);
  BOOST_CHECK_NE(set.hash(), set2.hash());
  BOOST_CHECK_EQUAL(empty, empty2);
  BOOST_CHECK_EQUAL(empty.hash(), empty2.hash());
  empty2 = empty.add(x);
  BOOST_CHECK_NE(empty.hash(), empty2.hash());
  empty2 = empty2cpy;
  empty2 = empty2.add(BooleMonomial());
  BOOST_CHECK_NE(empty, empty2);
  BOOST_CHECK_NE(empty.hash(), empty2.hash());

  set2 = set2cpy;
  empty2 = empty2cpy;
  BOOST_TEST_MESSAGE( "stableHash" );
  BOOST_CHECK_EQUAL(set, set2);
  BOOST_CHECK_EQUAL(set.stableHash(), set2.stableHash());
  set2 = set2.add(z);
  BOOST_CHECK_NE(set.stableHash(), set2.stableHash());
  BOOST_CHECK_EQUAL(empty, empty2);
  BOOST_CHECK_EQUAL(empty.stableHash(), empty2.stableHash());
  empty2 = empty.add(x);
  BOOST_CHECK_NE(empty.stableHash(), empty2.stableHash());
  empty2 = empty2cpy;
  empty2 = empty2.add(BooleMonomial());
  BOOST_CHECK_NE(empty, empty2);
  BOOST_CHECK_NE(empty.stableHash(), empty2.stableHash());
}

BOOST_AUTO_TEST_SUITE_END()
