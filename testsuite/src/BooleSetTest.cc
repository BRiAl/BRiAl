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

#include <polybori/pbori_defs.h>
#include <polybori/BooleVariable.h>
#include <polybori/BooleMonomial.h>
#include <polybori/BoolePolynomial.h>
#include <polybori/BooleExponent.h>
#include <polybori/BoolePolyRing.h>
#include <polybori/LexOrder.h>

USING_NAMESPACE_PBORI

struct Fset {
  typedef BooleSet set_type;
  Fset(const BoolePolyRing& input_ring = BoolePolyRing(5)):  
    ring(input_ring),
    x(0, input_ring), y(1, input_ring), z(2, input_ring),
    v(3, input_ring), w(4, input_ring), poly(input_ring) {

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
  set_type set(ring);
  output_test_stream output;
  output << set;
  BOOST_CHECK(output.is_equal("{}"));
  set = set.add(x);
  set_type set2(ring);
  BOOST_CHECK_THROW(set_type(0,set,set2), PBoRiGenericError<CTypes::invalid_ite>);
  set_type set1(ring);
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
  set_type empty(ring);

  BOOST_TEST_MESSAGE( "usedVariables, usedVariablesExp" ); // BooleMonomial is used as a BooleVariable set here
  BOOST_CHECK_EQUAL(set.usedVariables(),x*y*z*v);
  BOOST_CHECK_EQUAL(empty.usedVariables(),BooleMonomial(ring));
  set_type added_set = set.add(BooleMonomial(ring));
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

  set_type simple = empty.add(BooleMonomial(ring));
  BOOST_CHECK_EQUAL(simple.lastLexicographicalTerm(), BooleMonomial(ring));

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
  // set: {{x,y,z}, {x,v}, {y}, {z,v}}

  BOOST_TEST_MESSAGE( output.str() );
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
  BOOST_CHECK(!set.hasTermOfVariables(BooleMonomial(ring)));
  BOOST_CHECK(!empty.hasTermOfVariables(y));
  BOOST_CHECK(!empty.hasTermOfVariables(x*y*z));
  BOOST_CHECK(!empty.hasTermOfVariables(x*z*y));
  BOOST_CHECK(!empty.hasTermOfVariables(x*z*v));
  BOOST_CHECK(!empty.hasTermOfVariables(BooleMonomial(ring)));

  BOOST_TEST_MESSAGE( "contains" );
  BOOST_CHECK(set.contains(empty));
  BOOST_CHECK(set.contains(set));
  BOOST_CHECK(!empty.contains(set));
  BOOST_CHECK(empty.contains(empty));
  BOOST_CHECK(!empty.contains(ring.one()));
  BOOST_CHECK(!set.contains(ring.one()));
  set2 = empty.add(x*y*z);
  BOOST_CHECK(!set2.contains(set));
  BOOST_CHECK(set.contains(set2));
  set2 = empty.add(x*v);
  BOOST_CHECK(!set2.contains(set));
  BOOST_CHECK(set.contains(set2));
  BOOST_CHECK(set.contains(BoolePolynomial(y).set()));
  set2 = empty.add(z*v);
  BOOST_CHECK(!set2.contains(set));
  BOOST_CHECK(set.contains(set2));
  set2 = empty.add(x*y*z);
  set2 = set2.add(x*v);
  BOOST_CHECK(!set2.contains(set));
  BOOST_CHECK(set.contains(set2));
  set2 = empty.add(x*y*z);
  set2 = set2.add(y);
  BOOST_CHECK(!set2.contains(set));
  BOOST_CHECK(set.contains(set2));
  set2 = empty.add(x*y*z);
  set2 = set2.add(z*v);
  BOOST_CHECK(!set2.contains(set));
  BOOST_CHECK(set.contains(set2));
  set2 = empty.add(x*v);
  set2 = set2.add(y);
  BOOST_CHECK(!set2.contains(set));
  BOOST_CHECK(set.contains(set2));
  set2 = empty.add(x*v);
  set2 = set2.add(z*v);
  BOOST_CHECK(!set2.contains(set));
  BOOST_CHECK(set.contains(set2));
  set2 = empty.add(y);
  set2 = set2.add(z*v);
  BOOST_CHECK(!set2.contains(set));
  BOOST_CHECK(set.contains(set2));
  set2 = set2.add(x*v);
  BOOST_CHECK(!set2.contains(set));
  BOOST_CHECK(set.contains(set2));
  set2 = set2.add(x*y*z);
  BOOST_CHECK(set2.contains(set));
  BOOST_CHECK(set.contains(set2));
  set2 = set2.add(x*y*v);
  BOOST_CHECK(set2.contains(set));
  BOOST_CHECK(!set.contains(set2));

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
  BOOST_CHECK(set.containsDivisorsOfDecDeg(BooleMonomial(ring)));
  BOOST_CHECK(!empty.containsDivisorsOfDecDeg(x));
  BOOST_CHECK(!empty.containsDivisorsOfDecDeg(x*y*z));
  BOOST_CHECK(empty.containsDivisorsOfDecDeg(BooleMonomial(ring)));
  BOOST_CHECK(!set.containsDivisorsOfDecDeg(BooleExponent(x*v)));
  BOOST_CHECK(!set.containsDivisorsOfDecDeg(BooleExponent(x*y*z)));
  BOOST_CHECK(!set.containsDivisorsOfDecDeg(BooleExponent(x*z*y)));
  BOOST_CHECK(!set.containsDivisorsOfDecDeg(BooleExponent(y)));
  BOOST_CHECK(!set.containsDivisorsOfDecDeg(BooleExponent(z*v)));
  BOOST_CHECK(!set.containsDivisorsOfDecDeg(BooleExponent(x)));
  BOOST_CHECK(!set.containsDivisorsOfDecDeg(BooleExponent(z)));
  BOOST_CHECK(!set.containsDivisorsOfDecDeg(BooleExponent(v)));
  BOOST_CHECK(!set.containsDivisorsOfDecDeg(BooleExponent(w)));
  BOOST_CHECK(!set.containsDivisorsOfDecDeg(BooleExponent(x*y)));
  BOOST_CHECK(!set.containsDivisorsOfDecDeg(BooleExponent(y*x)));
  BOOST_CHECK(!set.containsDivisorsOfDecDeg(BooleExponent(y*z)));
  BOOST_CHECK(!set.containsDivisorsOfDecDeg(BooleExponent(z*y)));
  BOOST_CHECK(set.containsDivisorsOfDecDeg(BooleExponent()));
  BOOST_CHECK(!empty.containsDivisorsOfDecDeg(BooleExponent(x)));
  BOOST_CHECK(!empty.containsDivisorsOfDecDeg(BooleExponent(x*y*z)));
  BOOST_CHECK(empty.containsDivisorsOfDecDeg(BooleExponent()));

  set_type set1(ring);
  set1 = set1.add(x*y*z);
  set1 = set1.add(x*y);
  set1 = set1.add(x*z);
  set1 = set1.add(y*z);
  BOOST_CHECK(set1.containsDivisorsOfDecDeg(x*y*z));
  BOOST_CHECK(!set1.containsDivisorsOfDecDeg(x*y));
  BOOST_CHECK(!set1.containsDivisorsOfDecDeg(y*z));
  BOOST_CHECK(!set1.containsDivisorsOfDecDeg(x*z));
  BOOST_CHECK(set1.containsDivisorsOfDecDeg(BooleExponent(x*y*z)));
  BOOST_CHECK(!set1.containsDivisorsOfDecDeg(BooleExponent(x*y)));
  BOOST_CHECK(!set1.containsDivisorsOfDecDeg(BooleExponent(y*z)));
  BOOST_CHECK(!set1.containsDivisorsOfDecDeg(BooleExponent(x*z)));
  set1 = set1.add(x);
  set1 = set1.add(y);
  set1 = set1.add(z);
  BOOST_CHECK(set1.containsDivisorsOfDecDeg(x*y));
  BOOST_CHECK(set1.containsDivisorsOfDecDeg(y*z));
  BOOST_CHECK(set1.containsDivisorsOfDecDeg(x*z));
  BOOST_CHECK(!set1.containsDivisorsOfDecDeg(x));
  BOOST_CHECK(!set1.containsDivisorsOfDecDeg(y));
  BOOST_CHECK(!set1.containsDivisorsOfDecDeg(z));
  BOOST_CHECK(set1.containsDivisorsOfDecDeg(BooleExponent(x*y)));
  BOOST_CHECK(set1.containsDivisorsOfDecDeg(BooleExponent(y*z)));
  BOOST_CHECK(set1.containsDivisorsOfDecDeg(BooleExponent(x*z)));
  BOOST_CHECK(!set1.containsDivisorsOfDecDeg(BooleExponent(x)));
  BOOST_CHECK(!set1.containsDivisorsOfDecDeg(BooleExponent(y)));
  BOOST_CHECK(!set1.containsDivisorsOfDecDeg(BooleExponent(z)));
  set1 = set1.add(BooleMonomial(ring));
  BOOST_CHECK(set1.containsDivisorsOfDecDeg(x));
  BOOST_CHECK(set1.containsDivisorsOfDecDeg(y));
  BOOST_CHECK(set1.containsDivisorsOfDecDeg(z));
  BOOST_CHECK(set1.containsDivisorsOfDecDeg(BooleMonomial(ring)));
  BOOST_CHECK(set1.containsDivisorsOfDecDeg(BooleExponent(x)));
  BOOST_CHECK(set1.containsDivisorsOfDecDeg(BooleExponent(y)));
  BOOST_CHECK(set1.containsDivisorsOfDecDeg(BooleExponent(z)));
  BOOST_CHECK(set1.containsDivisorsOfDecDeg(BooleExponent()));
}

BOOST_AUTO_TEST_CASE(test_properties) {

  BOOST_TEST_MESSAGE( "isSingleton, isSingletonOrPair, isPair" );

  set_type set = poly.set();
  BOOST_CHECK(!set.isSingleton());
  BOOST_CHECK(!set.isSingletonOrPair());
  BOOST_CHECK(!set.isPair());

  set_type empty(ring);
  BOOST_CHECK(empty.isSingleton());
  BOOST_CHECK(empty.isSingletonOrPair());
  BOOST_CHECK(!empty.isPair());
  empty = empty.add(BooleMonomial(ring));
  BOOST_CHECK(empty.isSingleton());
  BOOST_CHECK(empty.isSingletonOrPair());
  BOOST_CHECK(!empty.isPair());

  set_type single(ring);
  single = single.add(x);
  BOOST_CHECK(single.isSingleton());
  BOOST_CHECK(single.isSingletonOrPair());
  BOOST_CHECK(!single.isPair());
  single = single.add(BooleMonomial(ring));
  BOOST_CHECK(!single.isSingleton());
  BOOST_CHECK(single.isSingletonOrPair());
  BOOST_CHECK(single.isPair());

  set_type pair(ring);
  pair = pair.add(x);
  pair = pair.add(y);
  BOOST_CHECK(!pair.isSingleton());
  BOOST_CHECK(pair.isSingletonOrPair());
  BOOST_CHECK(pair.isPair());
  pair = pair.add(BooleMonomial(ring));
  BOOST_CHECK(!pair.isSingleton());
  BOOST_CHECK(!pair.isSingletonOrPair());
  BOOST_CHECK(!pair.isPair());
}

BOOST_AUTO_TEST_CASE(test_add_own) {

  set_type set = poly.set();
  set_type empty(ring);
  BOOST_CHECK(set.owns(x*y*z));
  BOOST_CHECK(set.owns(v*z));
  BOOST_CHECK(set.owns(x*v));
  BOOST_CHECK(set.owns(y));
  BOOST_CHECK(!set.owns(x));
  BOOST_CHECK(!set.owns(z));
  BOOST_CHECK(!set.owns(v));
  BOOST_CHECK(!set.owns(w));
  BOOST_CHECK(!set.owns(x*v*z));
  BOOST_CHECK(!set.owns(BooleMonomial(ring)));
  BOOST_CHECK(!empty.owns(x));
  BOOST_CHECK(!empty.owns(y));
  BOOST_CHECK(!empty.owns(z));
  BOOST_CHECK(!empty.owns(v));
  BOOST_CHECK(!empty.owns(w));
  BOOST_CHECK(!empty.owns(BooleMonomial(ring)));
  BOOST_CHECK(set.owns(BooleExponent(x*y*z)));
  BOOST_CHECK(set.owns(BooleExponent(v*z)));
  BOOST_CHECK(set.owns(BooleExponent(x*v)));
  BOOST_CHECK(set.owns(BooleExponent(y)));
  BOOST_CHECK(!set.owns(BooleExponent(x)));
  BOOST_CHECK(!set.owns(BooleExponent(z)));
  BOOST_CHECK(!set.owns(BooleExponent(v)));
  BOOST_CHECK(!set.owns(BooleExponent(w)));
  BOOST_CHECK(!set.owns(BooleExponent(x*v*z)));
  BOOST_CHECK(!set.owns(BooleExponent()));
  BOOST_CHECK(!empty.owns(BooleExponent(x)));
  BOOST_CHECK(!empty.owns(BooleExponent(y)));
  BOOST_CHECK(!empty.owns(BooleExponent(z)));
  BOOST_CHECK(!empty.owns(BooleExponent(v)));
  BOOST_CHECK(!empty.owns(BooleExponent(w)));
  BOOST_CHECK(!empty.owns(BooleExponent()));

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
  BOOST_CHECK(addedset.owns(BooleExponent(x*v*z)));
  BOOST_CHECK(addedset.owns(BooleExponent(x*y*z)));
  BOOST_CHECK(addedset.owns(BooleExponent(v*z)));
  BOOST_CHECK(addedset.owns(BooleExponent(x*v)));
  BOOST_CHECK(addedset.owns(BooleExponent(y)));
  BOOST_CHECK(addedset.owns(BooleExponent(x)));
  BOOST_CHECK(!addedset.owns(BooleExponent(z)));
  BOOST_CHECK(!addedset.owns(BooleExponent(v)));
  BOOST_CHECK(!addedset.owns(BooleExponent(w)));
  BOOST_CHECK(addedset.owns(BooleExponent(x*v*z)));
  addedset = empty.add(x);
  addedset = addedset.add(y);
  addedset = addedset.add(v*w);
  BOOST_CHECK(addedset.owns(x));
  BOOST_CHECK(addedset.owns(y));
  BOOST_CHECK(addedset.owns(v*w));
  BOOST_CHECK(!addedset.owns(v));
  BOOST_CHECK(!addedset.owns(w));
  BOOST_CHECK(addedset.owns(BooleExponent(x)));
  BOOST_CHECK(addedset.owns(BooleExponent(y)));
  BOOST_CHECK(addedset.owns(BooleExponent(v*w)));
  BOOST_CHECK(!addedset.owns(BooleExponent(v)));
  BOOST_CHECK(!addedset.owns(BooleExponent(w)));

  set_type empty2(ring);
  empty2 = empty2.add(x);
  BOOST_CHECK(empty2.owns(x));
  BOOST_CHECK(empty2.owns(BooleExponent(x)));

  BOOST_CHECK(!set.ownsOne());
  addedset = set.add(BooleMonomial(ring));
  BOOST_CHECK(addedset.ownsOne());
  BOOST_CHECK(!empty.ownsOne());
  addedset = empty.add(BooleMonomial(ring));
  BOOST_CHECK(addedset.ownsOne());
}

BOOST_AUTO_TEST_CASE(test_division) {

  BOOST_TEST_MESSAGE( "divisors" );

  output_test_stream output;
  set_type set = poly.set();
  set_type result = set.divisorsOf(x);
  output << result;
  BOOST_CHECK(output.is_equal("{}"));
  result = set.divisorsOf(BooleExponent(x));
  output << result;
  BOOST_CHECK(output.is_equal("{}"));
  result = set.divisorsOf(y);
  output << result;
  BOOST_CHECK(output.is_equal("{{y}}"));
  result = set.divisorsOf(BooleExponent(y));
  output << result;
  BOOST_CHECK(output.is_equal("{{y}}"));
  result = set.divisorsOf(z);
  output << result;
  BOOST_CHECK(output.is_equal("{}"));
  result = set.divisorsOf(BooleExponent(z));
  output << result;
  BOOST_CHECK(output.is_equal("{}"));
  result = set.divisorsOf(v);
  output << result;
  BOOST_CHECK(output.is_equal("{}"));
  result = set.divisorsOf(BooleExponent(v));
  output << result;
  BOOST_CHECK(output.is_equal("{}"));
  result = set.divisorsOf(w);
  output << result;
  BOOST_CHECK(output.is_equal("{}"));
  result = set.divisorsOf(BooleExponent(w));
  output << result;
  BOOST_CHECK(output.is_equal("{}"));
  result = set.divisorsOf(x*y);
  output << result;
  BOOST_CHECK(output.is_equal("{{y}}"));
  result = set.divisorsOf(BooleExponent(x*y));
  output << result;
  BOOST_CHECK(output.is_equal("{{y}}"));

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
  result = set.divide(BooleMonomial(ring));
  output << result;
  BOOST_CHECK(output.is_equal("{{x,y,z}, {x,v}, {y}, {z,v}}"));
  set_type empty(ring);
  result = empty.divide(x*v*y);
  output << result;
  BOOST_CHECK(output.is_equal("{}"));
  result = empty.divide(BooleMonomial(ring));
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

  set_type empty(ring);
  set_type almost_empty(ring);
  almost_empty = almost_empty.add(BooleMonomial(ring));

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
  set_type empty(ring);
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
  output << set.multiplesOf(BooleMonomial(ring));
  BOOST_CHECK(output.is_equal("{{x,y,z}, {x,v}, {y}, {z,v}}"));

  output << empty.multiplesOf(x);
  BOOST_CHECK(output.is_equal("{}"));
  output << empty.multiplesOf(x*y);
  BOOST_CHECK(output.is_equal("{}"));
  output << empty.multiplesOf(BooleMonomial(ring));
  BOOST_CHECK(output.is_equal("{}"));

  BOOST_TEST_MESSAGE( "existAbstract" );
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
  output << set.existAbstract(BooleMonomial(ring));
  BOOST_CHECK(output.is_equal("{{x,y,z}, {x,v}, {y}, {z,v}}"));
  output << empty.existAbstract(x);
  BOOST_CHECK(output.is_equal("{}"));
  output << empty.existAbstract(x*v);
  BOOST_CHECK(output.is_equal("{}"));
  output << empty.existAbstract(BooleMonomial(ring));
  BOOST_CHECK(output.is_equal("{}"));

  BOOST_TEST_MESSAGE( "cartesianProduct" );
  set_type set1(ring);
  set1 = set1.add(x);
  set1 = set1.add(y);
  set_type set2(ring);
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
  set_type set2(ring);
  set2 = set2.add(x*y*z);
  set2 = set2.add(x*v);
  set2 = set2.add(y);
  set2 = set2.add(z*v);
  set_type set2cpy = set2;
  set_type empty(ring);
  set_type empty2(ring);
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
  empty2 = empty2.add(BooleMonomial(ring));
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
  empty2 = empty2.add(BooleMonomial(ring));
  BOOST_CHECK_NE(empty, empty2);
  BOOST_CHECK_NE(empty.stableHash(), empty2.stableHash());
}

BOOST_AUTO_TEST_CASE(test_change) {
  set_type set = poly.set();
  output_test_stream output;
  output << set;
  BOOST_CHECK(output.is_equal("{{x,y,z}, {x,v}, {y}, {z,v}}"));
  BOOST_CHECK_THROW(set.change(-1), PBoRiError);
  BOOST_CHECK_THROW(set.change(5), PBoRiError);
  set = set.change(4);
  output << set;
  BOOST_CHECK(output.is_equal("{{x,y,z,w}, {x,v,w}, {y,w}, {z,v,w}}"));
  set = set.change(0);
  output << set;
  BOOST_CHECK(output.is_equal("{{x,y,w}, {x,z,v,w}, {y,z,w}, {v,w}}"));
  set = set.change(1);
  output << set;
  BOOST_CHECK(output.is_equal("{{x,y,z,v,w}, {x,w}, {y,v,w}, {z,w}}"));
  set = set.change(2);
  output << set;
  BOOST_CHECK(output.is_equal("{{x,y,v,w}, {x,z,w}, {y,z,v,w}, {w}}"));
  set = set.change(3);
  output << set;
  BOOST_CHECK(output.is_equal("{{x,y,w}, {x,z,v,w}, {y,z,w}, {v,w}}"));
  set_type empty(ring);
  BOOST_CHECK_THROW(empty.change(-1), PBoRiError);
  BOOST_CHECK_THROW(empty.change(5), PBoRiError);
  empty = empty.change(0);
  output << empty;
  BOOST_CHECK(output.is_equal("{}"));
  empty = empty.add(BooleMonomial(ring));
  empty = empty.change(0);
  output << empty;
  BOOST_CHECK(output.is_equal("{{x}}"));
  empty = empty.add(BooleMonomial(ring));
  empty = empty.change(1);
  output << empty;
  BOOST_CHECK(output.is_equal("{{x,y}, {y}}"));
}

BOOST_AUTO_TEST_CASE(test_iter) {

  set_type set(ring);
  set = set.add(x*z);
  set = set.add(y);
  set_type empty(ring);
  output_test_stream output;

  BOOST_TEST_MESSAGE( "iterators" );
  BooleSet::const_reverse_iterator startr(set.rbegin()),finishr(set.rend());
  while (startr != finishr) {
    output << *startr <<", ";
    ++startr;
  }
  BOOST_CHECK(output.is_equal("y, x*z, "));
  startr = empty.rbegin();
  finishr = empty.rend();
  while (startr != finishr) {
    output << *startr <<", ";
    ++startr;
  }
  BOOST_CHECK(output.is_equal(""));

  BooleSet::const_iterator startc(set.begin()),finishc(set.end());
  while (startc != finishc) {
    output << *startc <<", ";
    ++startc;
  }
  BOOST_CHECK(output.is_equal("x*z, y, "));
  startc = empty.begin();
  finishc = empty.end();
  while (startc != finishc) {
    output << *startc <<", ";
    ++startc;
  }
  BOOST_CHECK(output.is_equal(""));

  BooleSet::exp_iterator starte(set.expBegin()),finishe(set.expEnd());
  while (starte != finishe) {
    output << *starte <<", ";
    ++starte;
  }
  BOOST_CHECK(output.is_equal("(0, 2), (1), "));
  starte = empty.expBegin();
  finishe = empty.expEnd();
  while (starte != finishe) {
    output << *starte <<", ";
    ++starte;
  }
  BOOST_CHECK(output.is_equal(""));
}

BOOST_AUTO_TEST_SUITE_END()
