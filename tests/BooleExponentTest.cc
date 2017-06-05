
// -*- c++ -*-
//*****************************************************************************
/** @file BooleExponentTest.cc
 *
 * @author Ket Kalda, Alexander Dreyer
 * @date 2010-10-19
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
#include <polybori/BooleExponent.h>

USING_NAMESPACE_PBORI

struct Fexp {
  typedef BooleExponent exp_type;
  typedef BooleMonomial monom_type;
  Fexp(const BoolePolyRing& input_ring = BoolePolyRing(5)):  
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

  ~Fexp() { BOOST_TEST_MESSAGE( "teardown fixture" ); }

  BoolePolyRing ring;
  BooleVariable x, y, z, v, w;
};

BOOST_FIXTURE_TEST_SUITE(BooleExponentTestSuite, Fexp )

BOOST_AUTO_TEST_CASE(test_constructors) {

  BOOST_TEST_MESSAGE( "Constant exponents..." );

  // removed, because this constructor is useless and confusing
  //    BOOST_CHECK_EQUAL(exp_type(), exp_type(true));
  //   BOOST_CHECK_EQUAL(exp_type(), exp_type(false));
  BOOST_CHECK_EQUAL(exp_type(), exp_type().get(BooleMonomial(ring)));

  BOOST_TEST_MESSAGE( "Exponents from monomials" );
  output_test_stream output;
  output << exp_type(x*y*z*z);
  BOOST_CHECK(output.is_equal("(0, 1, 2)"));
  output << exp_type().get(x);
  BOOST_CHECK(output.is_equal("(0)"));
  output << exp_type().get(BooleMonomial(ring));
  BOOST_CHECK(output.is_equal("()"));
  BOOST_CHECK_EQUAL(exp_type().get(BooleMonomial(exp_type(), ring)), exp_type());
  BOOST_CHECK_EQUAL(exp_type(x*y*z), exp_type().get(x*y*z));
  BOOST_CHECK_EQUAL(exp_type(x), exp_type().get(x));
  BOOST_CHECK_EQUAL(exp_type(BooleMonomial(ring)), exp_type().get(BooleMonomial(ring)));
  BOOST_CHECK_EQUAL(exp_type(), exp_type().get(BooleMonomial(ring)));
  BOOST_CHECK_EQUAL(exp_type(x*y*z).get(v*w), exp_type().get(v*w));
}

BOOST_AUTO_TEST_CASE(test_size_values) {
  exp_type exponent = exp_type().get(x*y*z*z);
  exp_type empty = exp_type();

  BOOST_TEST_MESSAGE( "size" );
  BOOST_CHECK_EQUAL(exponent.size(),3);
  BOOST_CHECK_EQUAL(empty.size(),0);

  BOOST_TEST_MESSAGE( "deg" );
  BOOST_CHECK_EQUAL(exponent.deg(),3);
  BOOST_CHECK_EQUAL(empty.deg(),0);
}

BOOST_AUTO_TEST_CASE(test_divide) {
  exp_type exponent = exp_type().get(x*y*z*z);
  exp_type empty = exp_type();
  output_test_stream output;

  BOOST_TEST_MESSAGE( "divisors" );
  output << exponent.divisors(ring);
  BOOST_CHECK(output.is_equal("{{x,y,z}, {x,y}, {x,z}, {x}, {y,z}, {y}, {z}, {}}"));
  output << empty.divisors(ring);
  BOOST_CHECK(output.is_equal("{{}}"));

  BOOST_TEST_MESSAGE( "divide" );
  output << exponent.divide(exp_type().get(y*z));

  BOOST_CHECK(output.is_equal("(0)"));
  output << exponent.divide(exp_type());
  BOOST_CHECK(output.is_equal("(0, 1, 2)"));
  output << exponent.divideByIndex(0);
  BOOST_CHECK(output.is_equal("(1, 2)"));
  output << exponent.divideByIndex(4);
  BOOST_CHECK(output.is_equal("()"));// Note: Difference from BooleMonomial
  output << exponent.divideByIndex(5);
  BOOST_CHECK(output.is_equal("()"));// Difference from BooleMonomial
  output << exponent.divideByIndex(-5);
  BOOST_CHECK(output.is_equal("()"));// Difference from BooleMonomial
  output << exponent.divide(BooleVariable(y));
  BOOST_CHECK(output.is_equal("(0, 2)"));
  output << exponent.divide(BooleVariable(4, ring));
  BOOST_CHECK(output.is_equal("()"));
  output << exponent.divide(BooleVariable(ring));
  BOOST_CHECK(output.is_equal("(1, 2)"));
  output << exponent.divide(x*y*z);
  BOOST_CHECK(output.is_equal("()"));
  output << exponent.divide(BooleMonomial(ring));
  BOOST_CHECK(output.is_equal("(0, 1, 2)"));
  output << empty.divide(exp_type().get(y*z));// Difference from BooleMonomial for all empty.divide
  BOOST_CHECK(output.is_equal("()"));
  output << empty.divide(exp_type());
  BOOST_CHECK(output.is_equal("()"));
  output << empty.divideByIndex(0);
  BOOST_CHECK(output.is_equal("()"));
  output << empty.divideByIndex(4);
  BOOST_CHECK(output.is_equal("()"));
  output << empty.divideByIndex(5);
  BOOST_CHECK(output.is_equal("()"));
  output << empty.divideByIndex(-5);
  BOOST_CHECK(output.is_equal("()"));
  output << empty.divide(BooleVariable(y));
  BOOST_CHECK(output.is_equal("()"));
  output << empty.divide(BooleVariable(4, ring));
  BOOST_CHECK(output.is_equal("()"));
  output << empty.divide(BooleVariable(ring));
  BOOST_CHECK(output.is_equal("()"));
  output << empty.divide(x*y*z);
  BOOST_CHECK(output.is_equal("()"));
  output << empty.divide(BooleMonomial(ring));
  BOOST_CHECK(output.is_equal("()"));
}

BOOST_AUTO_TEST_CASE(test_multiply) {
  exp_type exponent = exp_type().get(x*y*z*z);
  exp_type empty = exp_type();
  output_test_stream output;

  BOOST_TEST_MESSAGE( "multiples" );
  output << exponent.multiples(exp_type().get(z*v*w), ring);
  BOOST_CHECK(output.is_equal("{{x,y,z,v,w}, {x,y,z,v}, {x,y,z,w}, {x,y,z}}"));
  output << exponent.multiples(exp_type().get(v), ring);
  BOOST_CHECK(output.is_equal("{{x,y,z,v}, {x,y,z}}"));
  output << exponent.multiples(exp_type().get(x), ring);
  BOOST_CHECK(output.is_equal("{{x,y,z}}"));
  output << exponent.multiples(exp_type(), ring);
  BOOST_CHECK(output.is_equal("{{x,y,z}}"));
  output << empty.multiples(exp_type().get(z*v*w), ring);
  BOOST_CHECK(output.is_equal("{{z,v,w}, {z,v}, {z,w}, {z}, {v,w}, {v}, {w}, {}}"));
  output << empty.multiples(exp_type().get(v), ring);
  BOOST_CHECK(output.is_equal("{{v}, {}}"));
  output << empty.multiples(exp_type().get(x), ring);
  BOOST_CHECK(output.is_equal("{{x}, {}}"));
  output << empty.multiples(exp_type(), ring);
  BOOST_CHECK(output.is_equal("{{}}"));

  output << exponent.multiples(z*v*w);
  BOOST_CHECK(output.is_equal("{{x,y,z,v,w}, {x,y,z,v}, {x,y,z,w}, {x,y,z}}"));
  output << exponent.multiples(v);
  BOOST_CHECK(output.is_equal("{{x,y,z,v}, {x,y,z}}"));
  output << exponent.multiples(x);
  BOOST_CHECK(output.is_equal("{{x,y,z}}"));
  output << exponent.multiples(monom_type(ring));
  BOOST_CHECK(output.is_equal("{{x,y,z}}"));
  output << empty.multiples(z*v*w);
  BOOST_CHECK(output.is_equal("{{z,v,w}, {z,v}, {z,w}, {z}, {v,w}, {v}, {w}, {}}"));
  output << empty.multiples(v);
  BOOST_CHECK(output.is_equal("{{v}, {}}"));
  output << empty.multiples(x);
  BOOST_CHECK(output.is_equal("{{x}, {}}"));
  output << empty.multiples(monom_type(ring));
  BOOST_CHECK(output.is_equal("{{}}"));

  BOOST_TEST_MESSAGE( "multiply" );
  output << exponent.multiply(exp_type().get(y*z*v*w));
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4)"));
  output << exponent.multiply(exp_type());
  BOOST_CHECK(output.is_equal("(0, 1, 2)"));
  output << exponent.multiply(0);
  BOOST_CHECK(output.is_equal("(0, 1, 2)"));
  output << exponent.multiply(4);
  BOOST_CHECK(output.is_equal("(0, 1, 2, 4)"));
  output << exponent.multiply(5);
  BOOST_CHECK(output.is_equal("(0, 1, 2, 5)"));
  output << exponent.multiply(100);

  BOOST_CHECK(output.is_equal("(0, 1, 2, 100)"));


  output << exponent.multiply(BooleVariable(v));
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3)"));
  output << exponent.multiply(BooleVariable(4, ring));
  BOOST_CHECK(output.is_equal("(0, 1, 2, 4)"));
  output << exponent.multiply(BooleVariable(ring));
  BOOST_CHECK(output.is_equal("(0, 1, 2)"));
  output << exponent.multiply(v*w);
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4)"));
  output << exponent.multiply(BooleMonomial(ring));
  BOOST_CHECK(output.is_equal("(0, 1, 2)"));
  output << empty.multiply(exp_type().get(y*z*v*w));
  BOOST_CHECK(output.is_equal("(1, 2, 3, 4)"));
  output << empty.multiply(exp_type());
  BOOST_CHECK(output.is_equal("()"));
  output << empty.multiply(0);
  BOOST_CHECK(output.is_equal("(0)"));
  output << empty.multiply(4);
  BOOST_CHECK(output.is_equal("(4)"));
  output << empty.multiply(5);
  BOOST_CHECK(output.is_equal("(5)"));
  output << empty.multiply(100);
  BOOST_CHECK(output.is_equal("(100)"));
  output << empty.multiply(BooleVariable(v));
  BOOST_CHECK(output.is_equal("(3)"));
  output << empty.multiply(BooleVariable(4, ring));
  BOOST_CHECK(output.is_equal("(4)"));
  output << empty.multiply(BooleVariable(ring));
  BOOST_CHECK(output.is_equal("(0)"));
  output << empty.multiply(v*w);
  BOOST_CHECK(output.is_equal("(3, 4)"));
  output << empty.multiply(BooleMonomial(ring));
  BOOST_CHECK(output.is_equal("()"));

  BOOST_TEST_MESSAGE( "multiplyFirst" );
  BooleSet set(ring);
  output << exponent.multiplyFirst(set);
  BOOST_CHECK(output.is_equal("(0, 1, 2)"));
  set = set.add(BooleMonomial(ring));
  output << exponent.multiplyFirst(set);
  BOOST_CHECK(output.is_equal("(0, 1, 2)"));
  set = set.add(v);
  output << exponent.multiplyFirst(set);
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3)"));
  set = set.add(v*w);
  output << exponent.multiplyFirst(set);
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4)"));
  set = set.add(w);
  output << exponent.multiplyFirst(set);
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4)"));
  set = set.add(x);
  output << exponent.multiplyFirst(set);
  BOOST_CHECK(output.is_equal("(0, 1, 2)"));
  set = BooleSet(ring);
  output << empty.multiplyFirst(set);
  BOOST_CHECK(output.is_equal("()"));
  set = set.add(BooleMonomial(ring));
  output << empty.multiplyFirst(set);
  BOOST_CHECK(output.is_equal("()"));
  set = set.add(v);
  output << empty.multiplyFirst(set);
  BOOST_CHECK(output.is_equal("(3)"));
  set = set.add(v*w);
  output << empty.multiplyFirst(set);
  BOOST_CHECK(output.is_equal("(3, 4)"));
  set = set.add(w);
  output << empty.multiplyFirst(set);
  BOOST_CHECK(output.is_equal("(3, 4)"));
  set = set.add(x);
  output << empty.multiplyFirst(set);
  BOOST_CHECK(output.is_equal("(0)"));
}

BOOST_AUTO_TEST_CASE(test_LCM) {

  output_test_stream output;
  exp_type exp1;
  exp_type exp2;

  BOOST_TEST_MESSAGE( "LCM of two monomials" );

  exp1 = exp_type().get(x*x*y);
  exp2 = exp_type().get(z*v*w);
  output << LCM(exp1, exp2);
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4)"));
  BOOST_CHECK_EQUAL(exp1, exp_type().get(x*y));
  BOOST_CHECK_EQUAL(exp2, exp_type().get(z*v*w));

  exp1 = exp_type().get(x*x);
  exp2 = exp_type().get(z*v);
  output << LCM(exp1, exp2);
  BOOST_CHECK(output.is_equal("(0, 2, 3)"));
  BOOST_CHECK_EQUAL(exp1, exp_type().get(x));
  BOOST_CHECK_EQUAL(exp2, exp_type().get(z*v));

  exp1 = exp_type().get(x*y*z*v*w);
  exp2 = exp_type().get(x*y*z*v*w);
  output << LCM(exp1, exp2);
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4)"));
  BOOST_CHECK_EQUAL(exp1, exp_type().get(x*y*z*v*w));
  BOOST_CHECK_EQUAL(exp2, exp_type().get(x*y*z*v*w));

  exp1 = exp_type().get(x*y*z*v*w);
  exp2 = exp_type();
  output << LCM(exp1, exp2);
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4)"));
  BOOST_CHECK_EQUAL(exp1, exp_type().get(x*y*z*v*w));
  BOOST_CHECK_EQUAL(exp2, exp_type());

  exp1 = exp_type();
  exp2 = exp_type().get(x*y*z*v*w);
  output << LCM(exp1, exp2);
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4)"));
  BOOST_CHECK_EQUAL(exp1, exp_type());
  BOOST_CHECK_EQUAL(exp2, exp_type().get(x*y*z*v*w));

  exp1 = exp_type();
  exp2 = exp_type();
  output << LCM(exp1, exp2);
  BOOST_CHECK(output.is_equal("()"));
  BOOST_CHECK_EQUAL(exp1, exp_type());
  BOOST_CHECK_EQUAL(exp2, exp_type());

  BOOST_TEST_MESSAGE( "LCM as method of one monomial" );

  exp1 = exp_type().get(x*x*y);
  exp2 = exp_type().get(z*v*w);
  output << exp1.LCM(exp2);
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4)"));
  BOOST_CHECK_EQUAL(exp1, exp_type().get(x*y));
  BOOST_CHECK_EQUAL(exp2, exp_type().get(z*v*w));

  exp1 = exp_type().get(x*x);
  exp2 = exp_type().get(z*v);
  output << exp1.LCM(exp2);
  BOOST_CHECK(output.is_equal("(0, 2, 3)"));
  BOOST_CHECK_EQUAL(exp1, exp_type().get(x));
  BOOST_CHECK_EQUAL(exp2, exp_type().get(z*v));

  exp1 = exp_type().get(x*y*z*v*w);
  exp2 = exp_type().get(x*y*z*v*w);
  output << exp1.LCM(exp2);
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4)"));
  BOOST_CHECK_EQUAL(exp1, exp_type().get(x*y*z*v*w));
  BOOST_CHECK_EQUAL(exp2, exp_type().get(x*y*z*v*w));

  exp1 = exp_type().get(x*y*z*v*w);
  exp2 = exp_type();
  output << exp1.LCM(exp2);
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4)"));
  BOOST_CHECK_EQUAL(exp1, exp_type().get(x*y*z*v*w));
  BOOST_CHECK_EQUAL(exp2, exp_type());

  exp1 = exp_type();
  exp2 = exp_type().get(x*y*z*v*w);
  output << exp1.LCM(exp2);
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4)"));
  BOOST_CHECK_EQUAL(exp1, exp_type());
  BOOST_CHECK_EQUAL(exp2, exp_type().get(x*y*z*v*w));

  exp1 = exp_type();
  exp2 = exp_type();
  output << exp1.LCM(exp2);
  BOOST_CHECK(output.is_equal("()"));
  BOOST_CHECK_EQUAL(exp1, exp_type());
  BOOST_CHECK_EQUAL(exp2, exp_type());

  BOOST_TEST_MESSAGE( "LCMDeg as method of one monomial" );

  exp1 = exp_type().get(x*x*y);
  exp2 = exp_type().get(z*v*w);
  output << exp1.LCMDeg(exp2);
  BOOST_CHECK(output.is_equal("5"));
  BOOST_CHECK_EQUAL(exp1, exp_type().get(x*y));
  BOOST_CHECK_EQUAL(exp2, exp_type().get(z*v*w));

  exp1 = exp_type().get(x*x);
  exp2 = exp_type().get(z*v);
  output << exp1.LCMDeg(exp2);
  BOOST_CHECK(output.is_equal("3"));
  BOOST_CHECK_EQUAL(exp1, exp_type().get(x));
  BOOST_CHECK_EQUAL(exp2, exp_type().get(z*v));

  exp1 = exp_type().get(x*y*z*v*w);
  exp2 = exp_type().get(x*y*z*v*w);
  output << exp1.LCMDeg(exp2);
  BOOST_CHECK(output.is_equal("5"));
  BOOST_CHECK_EQUAL(exp1, exp_type().get(x*y*z*v*w));
  BOOST_CHECK_EQUAL(exp2, exp_type().get(x*y*z*v*w));

  exp1 = exp_type().get(x*y*z*v*w);
  exp2 = exp_type();
  output << exp1.LCMDeg(exp2);
  BOOST_CHECK(output.is_equal("5"));
  BOOST_CHECK_EQUAL(exp1, exp_type().get(x*y*z*v*w));
  BOOST_CHECK_EQUAL(exp2, exp_type());

  exp1 = exp_type();
  exp2 = exp_type().get(x*y*z*v*w);
  output << exp1.LCMDeg(exp2);
  BOOST_CHECK(output.is_equal("5"));
  BOOST_CHECK_EQUAL(exp1, exp_type());
  BOOST_CHECK_EQUAL(exp2, exp_type().get(x*y*z*v*w));

  exp1 = exp_type();
  exp2 = exp_type();
  output << exp1.LCMDeg(exp2);
  BOOST_CHECK(output.is_equal("0"));
  BOOST_CHECK_EQUAL(exp1, exp_type());
  BOOST_CHECK_EQUAL(exp2, exp_type());

  exp1 = exp_type().get(x);
  exp2 = exp_type().get(x);
  output << exp1.LCMDeg(exp2);
  BOOST_CHECK(output.is_equal("1"));
  BOOST_CHECK_EQUAL(exp1, exp_type().get(x));
  BOOST_CHECK_EQUAL(exp2, exp_type().get(x));

  exp1 = exp_type().get(x);
  exp2 = exp_type().get(y);
  output << exp1.LCMDeg(exp2);
  BOOST_CHECK(output.is_equal("2"));
  BOOST_CHECK_EQUAL(exp1, exp_type().get(x));
  BOOST_CHECK_EQUAL(exp2, exp_type().get(y));
}

BOOST_AUTO_TEST_CASE(test_GCD) {

  output_test_stream output;
  exp_type exp1;
  exp_type exp2;

  BOOST_TEST_MESSAGE( "GCD of two monomials" );

  exp1 = exp_type().get(x*x*y);
  exp2 = exp_type().get(z*v*w);
  output << GCD(exp1, exp2);
  BOOST_CHECK(output.is_equal("()"));
  BOOST_CHECK_EQUAL(exp1, exp_type().get(x*y));
  BOOST_CHECK_EQUAL(exp2, exp_type().get(z*v*w));

  exp1 = exp_type().get(x*y*z*v*w);
  exp2 = exp_type().get(x*y*z*v*w);
  output << GCD(exp1, exp2);
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4)"));
  BOOST_CHECK_EQUAL(exp1, exp_type().get(x*y*z*v*w));
  BOOST_CHECK_EQUAL(exp2, exp_type().get(x*y*z*v*w));

  exp1 = exp_type().get(x*y*z*v*w);
  exp2 = exp_type();
  output << GCD(exp1, exp2);
  BOOST_CHECK(output.is_equal("()"));
  BOOST_CHECK_EQUAL(exp1, exp_type().get(x*y*z*v*w));
  BOOST_CHECK_EQUAL(exp2, exp_type());

  exp1 = exp_type();
  exp2 = exp_type().get(x*y*z*v*w);
  output << GCD(exp1, exp2);
  BOOST_CHECK(output.is_equal("()"));
  BOOST_CHECK_EQUAL(exp1, exp_type());
  BOOST_CHECK_EQUAL(exp2, exp_type().get(x*y*z*v*w));

  exp1 = exp_type();
  exp2 = exp_type();
  output << GCD(exp1, exp2);
  BOOST_CHECK(output.is_equal("()"));
  BOOST_CHECK_EQUAL(exp1, exp_type());
  BOOST_CHECK_EQUAL(exp2, exp_type());

  BOOST_TEST_MESSAGE( "GCD as method of one monomial" );

  exp1 = exp_type().get(x*x*y);
  exp2 = exp_type().get(z*v*w);
  output << exp1.GCD(exp2);
  BOOST_CHECK(output.is_equal("()"));
  BOOST_CHECK_EQUAL(exp1, exp_type().get(x*y));
  BOOST_CHECK_EQUAL(exp2, exp_type().get(z*v*w));

  exp1 = exp_type().get(x*y*z*v*w);
  exp2 = exp_type().get(x*y*z*v*w);
  output << exp1.GCD(exp2);
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4)"));
  BOOST_CHECK_EQUAL(exp1, exp_type().get(x*y*z*v*w));
  BOOST_CHECK_EQUAL(exp2, exp_type().get(x*y*z*v*w));

  exp1 = exp_type().get(x*y*z*v*w);
  exp2 = exp_type();
  output << exp1.GCD(exp2);
  BOOST_CHECK(output.is_equal("()"));
  BOOST_CHECK_EQUAL(exp1, exp_type().get(x*y*z*v*w));
  BOOST_CHECK_EQUAL(exp2, exp_type());

  exp1 = exp_type();
  exp2 = exp_type().get(x*y*z*v*w);
  output << exp1.GCD(exp2);
  BOOST_CHECK(output.is_equal("()"));
  BOOST_CHECK_EQUAL(exp1, exp_type());
  BOOST_CHECK_EQUAL(exp2, exp_type().get(x*y*z*v*w));

  exp1 = exp_type();
  exp2 = exp_type();
  output << exp1.GCD(exp2);
  BOOST_CHECK(output.is_equal("()"));
  BOOST_CHECK_EQUAL(exp1, exp_type());
  BOOST_CHECK_EQUAL(exp2, exp_type());
}

BOOST_AUTO_TEST_CASE(test_add_remove) {
  exp_type exponent = exp_type().get(x*y*z*z);
  exp_type empty = exp_type();
  output_test_stream output;

  BOOST_TEST_MESSAGE( "insert, push_back, insertConst" );
  exp_type exponent2 = exponent;
  exponent2.insert(0);
  BOOST_CHECK_EQUAL(exponent2, exp_type().get(x*y*z));
  exponent2.insert(4);
  BOOST_CHECK_EQUAL(exponent2, exp_type().get(x*y*z*w));
  exponent2.insert(3);
  BOOST_CHECK_EQUAL(exponent2, exp_type().get(x*y*z*v*w));
  exponent2.insert(5);
  output << exponent2;
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4, 5)"));
  exponent2.insert(99);
  output << exponent2;
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4, 5, 99)"));
  BOOST_CHECK_NE(exponent2, exponent);
  exp_type empty2;
  empty2.insert(5);
  output << empty2;
  BOOST_CHECK(output.is_equal("(5)"));
  empty2.insert(2);
  output << empty2;
  BOOST_CHECK(output.is_equal("(2, 5)"));
  empty2.insert(20);
  output << empty2;
  BOOST_CHECK(output.is_equal("(2, 5, 20)"));
  BOOST_CHECK_NE(empty2, empty);

  exponent2 = exponent;
  exponent2.push_back(0);
  BOOST_CHECK_EQUAL(exponent2, exp_type().get(x*y*z));
  exponent2.push_back(4);
  BOOST_CHECK_EQUAL(exponent2, exp_type().get(x*y*z*w));
  exponent2.push_back(4);
  BOOST_CHECK_EQUAL(exponent2, exp_type().get(x*y*z*w));
  exponent2.push_back(3);
  BOOST_CHECK_EQUAL(exponent2, exp_type().get(x*y*z*v*w));
  exponent2.push_back(5);
  output << exponent2;
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4, 5)"));
  exponent2.push_back(99);
  output << exponent2;
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4, 5, 99)"));
  BOOST_CHECK_NE(exponent2, exponent);
  empty2 = empty;
  empty2.push_back(5);
  output << empty2;
  BOOST_CHECK(output.is_equal("(5)"));
  empty2.push_back(2);
  output << empty2;
  BOOST_CHECK(output.is_equal("(2, 5)"));
  empty2.push_back(20);
  output << empty2;
  BOOST_CHECK(output.is_equal("(2, 5, 20)"));
  BOOST_CHECK_NE(empty2, empty);

  exponent2 = exponent.insertConst(0);
  BOOST_CHECK_EQUAL(exponent2, exp_type().get(x*y*z));
  exponent2 = exponent2.insertConst(4);
  BOOST_CHECK_EQUAL(exponent2, exp_type().get(x*y*z*w));
  exponent2 = exponent2.insertConst(3);
  BOOST_CHECK_EQUAL(exponent2, exp_type().get(x*y*z*v*w));
  exponent2 = exponent2.insertConst(5);
  output << exponent2;
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4, 5)"));
  exponent2 = exponent2.insertConst(99);
  output << exponent2;
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4, 5, 99)"));
  BOOST_CHECK_NE(exponent2, exponent);
  empty2 = empty;
  empty2 = empty.insertConst(5);
  output << empty2;
  BOOST_CHECK(output.is_equal("(5)"));
  empty2 = empty2.insertConst(2);
  output << empty2;
  BOOST_CHECK(output.is_equal("(2, 5)"));
  empty2 = empty2.insertConst(20);
  output << empty2;
  BOOST_CHECK(output.is_equal("(2, 5, 20)"));
  BOOST_CHECK_NE(empty2, empty);

  BOOST_TEST_MESSAGE( "remove, removeConst" );
  exp_type exponent3 = exponent2;
  exponent3.remove(0);
  output << exponent3;
  BOOST_CHECK(output.is_equal("(1, 2, 3, 4, 5, 99)"));
  exponent3.remove(4);
  output << exponent3;
  BOOST_CHECK(output.is_equal("(1, 2, 3, 5, 99)"));
  exponent3.remove(3);
  output << exponent3;
  BOOST_CHECK(output.is_equal("(1, 2, 5, 99)"));
  exponent3.remove(5);
  output << exponent3;
  BOOST_CHECK(output.is_equal("(1, 2, 99)"));
  exponent3.remove(99);
  output << exponent3;
  BOOST_CHECK(output.is_equal("(1, 2)"));
  BOOST_CHECK_NE(exponent3, exponent2);
  exponent3.remove(100);
  output << exponent3;
  BOOST_CHECK(output.is_equal("(1, 2)"));
  exp_type empty3 = empty2;
  empty3.remove(5);
  output << empty3;
  BOOST_CHECK(output.is_equal("(2, 20)"));
  empty3.remove(2);
  output << empty3;
  BOOST_CHECK(output.is_equal("(20)"));
  empty3.remove(20);
  output << empty3;
  BOOST_CHECK(output.is_equal("()"));
  empty3.remove(20);
  output << empty3;
  BOOST_CHECK(output.is_equal("()"));
  BOOST_CHECK_NE(empty3, empty2);

  exponent3 = exponent2;
  exponent3 = exponent2.removeConst(0);
  output << exponent3;
  BOOST_CHECK(output.is_equal("(1, 2, 3, 4, 5, 99)"));
  exponent3 = exponent3.removeConst(4);
  output << exponent3;
  BOOST_CHECK(output.is_equal("(1, 2, 3, 5, 99)"));
  exponent3 = exponent3.removeConst(3);
  output << exponent3;
  BOOST_CHECK(output.is_equal("(1, 2, 5, 99)"));
  exponent3 = exponent3.removeConst(5);
  output << exponent3;
  BOOST_CHECK(output.is_equal("(1, 2, 99)"));
  exponent3 = exponent3.removeConst(99);
  output << exponent3;
  BOOST_CHECK(output.is_equal("(1, 2)"));
  BOOST_CHECK_NE(exponent3, exponent2);
  exponent3 = exponent3.removeConst(100);
  output << exponent3;
  BOOST_CHECK(output.is_equal("(1, 2)"));
  empty3 = empty2;
  empty3 = empty2.removeConst(5);
  output << empty3;
  BOOST_CHECK(output.is_equal("(2, 20)"));
  empty3 = empty3.removeConst(2);
  output << empty3;
  BOOST_CHECK(output.is_equal("(20)"));
  empty3 = empty3.removeConst(20);
  output << empty3;
  BOOST_CHECK(output.is_equal("()"));
  empty3 = empty3.removeConst(100);
  output << empty3;
  BOOST_CHECK(output.is_equal("()"));
  BOOST_CHECK_NE(empty3, empty2);
}

BOOST_AUTO_TEST_CASE(test_change) {
  exp_type exponent = exp_type().get(x*y*z*z);
  exp_type empty = exp_type();
  output_test_stream output;

  BOOST_TEST_MESSAGE( "change, changeAssign" );
  output << exponent.change(1);
  BOOST_CHECK(output.is_equal("(0, 2)"));
  output << exponent.change(0);
  BOOST_CHECK(output.is_equal("(1, 2)"));
  output << exponent.change(2);
  BOOST_CHECK(output.is_equal("(0, 1)"));
  output << exponent.change(3);
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3)"));
  output << exponent.change(100);
  BOOST_CHECK(output.is_equal("(0, 1, 2, 100)"));
  output << empty.change(1);
  BOOST_CHECK(output.is_equal("(1)"));
  output << empty.change(99);
  BOOST_CHECK(output.is_equal("(99)"));
  output << exponent.changeAssign(1);
  BOOST_CHECK(output.is_equal("(0, 2)"));
  output << exponent.changeAssign(0);
  BOOST_CHECK(output.is_equal("(2)"));
  output << exponent.changeAssign(2);
  BOOST_CHECK(output.is_equal("()"));
  output << exponent.changeAssign(3);
  BOOST_CHECK(output.is_equal("(3)"));
  output << exponent.changeAssign(100);
  BOOST_CHECK(output.is_equal("(3, 100)"));
  output << empty.changeAssign(1);
  BOOST_CHECK(output.is_equal("(1)"));
  output << empty.changeAssign(99);
  BOOST_CHECK(output.is_equal("(1, 99)"));
}

BOOST_AUTO_TEST_CASE(test_logical_operators) {

  BOOST_TEST_MESSAGE( "Logical operators..." );

  output_test_stream output;
  exp_type exp1;
  exp_type exp2;

  BOOST_TEST_MESSAGE( "== and !=" );
  BOOST_CHECK_EQUAL(exp_type(), exp_type());
  BOOST_CHECK(!(exp_type() !=exp_type()));
}

BOOST_AUTO_TEST_CASE(test_reducible) {
  exp_type exponent = exp_type().get(x*y*z*z);
  exp_type empty = exp_type();
  output_test_stream output;

  BOOST_TEST_MESSAGE( "reducibleBy" );
  BOOST_CHECK(exponent.reducibleBy(exp_type().get(x)));
  BOOST_CHECK(exponent.reducibleBy(exp_type().get(y)));
  BOOST_CHECK(exponent.reducibleBy(exp_type().get(z)));
  BOOST_CHECK(exponent.reducibleBy(exp_type().get(x*y)));
  BOOST_CHECK(exponent.reducibleBy(exp_type().get(x*z)));
  BOOST_CHECK(exponent.reducibleBy(exp_type().get(y*z)));
  BOOST_CHECK(exponent.reducibleBy(exp_type().get(y*z*x)));
  BOOST_CHECK(exponent.reducibleBy(exp_type()));
  BOOST_CHECK(!exponent.reducibleBy(exp_type().get(v)));
  BOOST_CHECK(!exponent.reducibleBy(exp_type().get(x*v)));
  BOOST_CHECK(empty.reducibleBy(exp_type()));
  BOOST_CHECK(!empty.reducibleBy(exp_type().get(x)));
  BOOST_CHECK(!empty.reducibleBy(exp_type().get(x*y)));

  BOOST_CHECK(exponent.reducibleBy(BooleMonomial(x)));
  BOOST_CHECK(exponent.reducibleBy(BooleMonomial(y)));
  BOOST_CHECK(exponent.reducibleBy(BooleMonomial(z)));
  BOOST_CHECK(exponent.reducibleBy(x*y));
  BOOST_CHECK(exponent.reducibleBy(x*z));
  BOOST_CHECK(exponent.reducibleBy(y*z));
  BOOST_CHECK(exponent.reducibleBy(y*z*x));
  BOOST_CHECK(exponent.reducibleBy(BooleMonomial(ring)));
  BOOST_CHECK(!exponent.reducibleBy(BooleMonomial(v)));
  BOOST_CHECK(!exponent.reducibleBy(x*v));
  BOOST_CHECK(empty.reducibleBy(BooleMonomial(ring)));
  BOOST_CHECK(!empty.reducibleBy(BooleMonomial(x)));
  BOOST_CHECK(!empty.reducibleBy(x*y));

  BOOST_CHECK(exponent.reducibleBy(x));
  BOOST_CHECK(exponent.reducibleBy(y));
  BOOST_CHECK(exponent.reducibleBy(z));
  BOOST_CHECK(exponent.reducibleBy(BooleVariable(ring)));
  BOOST_CHECK(!exponent.reducibleBy(v));
  BOOST_CHECK(!empty.reducibleBy(BooleVariable(ring))); // as BooleVariable() = x
  BOOST_CHECK(!empty.reducibleBy(x));

  BOOST_CHECK(exponent.reducibleBy(0));
  BOOST_CHECK(exponent.reducibleBy(1));
  BOOST_CHECK(exponent.reducibleBy(2));
  BOOST_CHECK(!exponent.reducibleBy(3));
  BOOST_CHECK(!exponent.reducibleBy(5));
  BOOST_CHECK(!exponent.reducibleBy(-5));
  BOOST_CHECK(!empty.reducibleBy(0));
  BOOST_CHECK(!empty.reducibleBy(-5));
}

BOOST_AUTO_TEST_CASE(test_assigning_operators) {
  output_test_stream output;

  BOOST_TEST_MESSAGE( "Assignments..." );
  BOOST_TEST_MESSAGE( "+" );
  output << ((((exp_type().get(x) + y) + z) + v)+ w);
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4)"));
  output << ((((exp_type().get(x) + x) + x) + x)+ x);
  BOOST_CHECK(output.is_equal("(0)"));
  BOOST_CHECK_EQUAL(exp_type().get(x*y)+exp_type(),exp_type().get(x*y));
  BOOST_CHECK_EQUAL(exp_type()+exp_type().get(x*z),exp_type().get(x*z));
  BOOST_CHECK_EQUAL(exp_type()+exp_type(),exp_type());

  BOOST_TEST_MESSAGE( "-" );
  output << ((exp_type().get(x*y*z*v*w)- x)- v);
  BOOST_CHECK(output.is_equal("(1, 2, 4)"));
  output << (exp_type().get(x*y*z*v*w)- exp_type());
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4)"));

  // The following corresponds to division of monomials
  BOOST_CHECK_EQUAL(exp_type()-exp_type().get(x), exp_type());
  BOOST_CHECK_EQUAL(exp_type().get(y)-exp_type().get(x), exp_type());
  BOOST_CHECK_EQUAL(exp_type().get(z*y)-exp_type().get(x), exp_type());

  BOOST_TEST_MESSAGE( "=" );
  exp_type exponent;
  exponent = x*y*z;
  BOOST_CHECK_EQUAL(exponent, exp_type().get(x*y*z));
  exponent = x;
  BOOST_CHECK_EQUAL(exponent, exp_type().get(x));
  exponent = BooleMonomial(ring);
  BOOST_CHECK_EQUAL(exponent, exp_type());
  BooleExponent exponent2;
  exponent2.insert(5);
  exponent2.insert(11);
  exponent2.insert(0);
  output << exponent2;
  BOOST_CHECK(output.is_equal("(0, 5, 11)"));
}

BOOST_AUTO_TEST_CASE(test_popFirst) {
  output_test_stream output;
  exp_type exponent = exp_type().get(x*y*z*z);
  exp_type empty = exp_type();

  BOOST_TEST_MESSAGE( "popFirst" );
  BOOST_CHECK_EQUAL(exponent.popFirst(), exp_type().get(y*z));
  BOOST_CHECK_EQUAL(exponent.popFirst(), exp_type().get(z));
  BOOST_CHECK_EQUAL(exponent.popFirst(), exp_type());
  BOOST_CHECK_EQUAL(exponent.popFirst(), exp_type());
}

BOOST_AUTO_TEST_CASE(test_hash) {

  exp_type exp = exp_type().get(x*y*v);
  exp_type exp2;
  exp2.insert(0);
  exp2.insert(1);
  exp2.insert(3);
  exp_type exp2cpy = exp2;
  exp_type empty;
  exp_type empty2;
  exp_type empty2cpy = empty2;
  output_test_stream output;

  BOOST_TEST_MESSAGE( "hash" );
  BOOST_CHECK_EQUAL(exp, exp2);
  BOOST_CHECK_EQUAL(exp.hash(), exp2.hash());
  exp2.insert(2);
  BOOST_CHECK_NE(exp.hash(), exp2.hash());
  BOOST_CHECK_EQUAL(empty, empty2);
  BOOST_CHECK_EQUAL(empty.hash(), empty2.hash());
  empty2 = empty.insertConst(0);
  BOOST_CHECK_NE(empty.hash(), empty2.hash());
  empty2 = empty2cpy;
  empty2.insert(1);
  BOOST_CHECK_NE(empty, empty2);
  BOOST_CHECK_NE(empty.hash(), empty2.hash());

  exp2 = exp2cpy;
  empty2 = empty2cpy;
  BOOST_TEST_MESSAGE( "stableHash" );
  BOOST_CHECK_EQUAL(exp, exp2);
  BOOST_CHECK_EQUAL(exp.stableHash(), exp2.stableHash());
  exp2.insert(2);
  BOOST_CHECK_NE(exp.stableHash(), exp2.stableHash());
  BOOST_CHECK_EQUAL(empty, empty2);
  BOOST_CHECK_EQUAL(empty.stableHash(), empty2.stableHash());
  empty2 = empty.insertConst(0);
  BOOST_CHECK_NE(empty.stableHash(), empty2.stableHash());
  empty2 = empty2cpy;
  empty2.insert(1);
  BOOST_CHECK_NE(empty, empty2);
  BOOST_CHECK_NE(empty.stableHash(), empty2.stableHash());
}

BOOST_AUTO_TEST_SUITE_END()
