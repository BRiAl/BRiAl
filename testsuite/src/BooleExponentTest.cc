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

#include "pbori_defs.h"
#include "BooleExponent.h"

USING_NAMESPACE_PBORI

struct Fexp {
  typedef BooleExponent exp_type;
  Fexp():
    ring(5) {
    x = (BooleVariable(0));
    y = (BooleVariable(1));
    z = (BooleVariable(2));
    v = (BooleVariable(3));
    w = (BooleVariable(4));
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
  BOOST_CHECK_EQUAL(exp_type(), exp_type(true));
  BOOST_CHECK_EQUAL(exp_type(), exp_type(false));
  BOOST_CHECK_EQUAL(exp_type(), exp_type().get(BooleMonomial()));

  BOOST_TEST_MESSAGE( "Exponents from monomials" );
  output_test_stream output;
  output << exp_type().get(x*y*z*z);
  BOOST_CHECK(output.is_equal("(0, 1, 2)"));
  output << exp_type().get(x);
  BOOST_CHECK(output.is_equal("(0)"));
  output << exp_type().get(BooleMonomial());
  BOOST_CHECK(output.is_equal("()"));
  BOOST_CHECK_EQUAL(exp_type().get(BooleMonomial(exp_type(), ring)), exp_type());
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
  output << exponent.divisors();
  BOOST_CHECK(output.is_equal("{{x,y,z}, {x,y}, {x,z}, {x}, {y,z}, {y}, {z}, {}}"));
  output << empty.divisors();
  BOOST_CHECK(output.is_equal("{{}}"));

  BOOST_TEST_MESSAGE( "divide" );
  output << exponent.divide(exp_type().get(y*z));
  BOOST_CHECK(output.is_equal("(0)"));
  output << exponent.divide(exp_type());
  BOOST_CHECK(output.is_equal("(0, 1, 2)"));
  output << exponent.divide(0);
  BOOST_CHECK(output.is_equal("(1, 2)"));
  output << exponent.divide(4);
  BOOST_CHECK(output.is_equal("()"));// Difference from BooleMonomial?!
  output << exponent.divide(5);
  BOOST_CHECK(output.is_equal("()"));// Difference from BooleMonomial?!
  output << exponent.divide(BooleVariable(y));
  BOOST_CHECK(output.is_equal("(0, 2)"));
  output << exponent.divide(BooleVariable(4));
  BOOST_CHECK(output.is_equal("()"));
  output << exponent.divide(BooleVariable());
  BOOST_CHECK(output.is_equal("(1, 2)"));
  //output << exponent.divide(BooleVariable(5));// memory access violation at address: 0x00000044
  //BOOST_CHECK(output.is_equal("()"));
  output << exponent.divide(x*y*z);
  BOOST_CHECK(output.is_equal("()"));
  output << exponent.divide(BooleMonomial());
  BOOST_CHECK(output.is_equal("(0, 1, 2)"));
  output << empty.divide(exp_type().get(y*z));// Difference from BooleMonomial for all empty.divide?!
  BOOST_CHECK(output.is_equal("()"));
  output << empty.divide(exp_type());
  BOOST_CHECK(output.is_equal("()"));
  output << empty.divide(0);
  BOOST_CHECK(output.is_equal("()"));
  output << empty.divide(4);
  BOOST_CHECK(output.is_equal("()"));
  output << empty.divide(5);
  BOOST_CHECK(output.is_equal("()"));
  output << empty.divide(BooleVariable(y));
  BOOST_CHECK(output.is_equal("()"));
  output << empty.divide(BooleVariable(4));
  BOOST_CHECK(output.is_equal("()"));
  output << empty.divide(BooleVariable());
  BOOST_CHECK(output.is_equal("()"));
  //output << empty.divide(BooleVariable(5));// memory access violation at address: 0x00000044
  //BOOST_CHECK(output.is_equal("()"));
  output << empty.divide(x*y*z);
  BOOST_CHECK(output.is_equal("()"));
  output << empty.divide(BooleMonomial());
  BOOST_CHECK(output.is_equal("()"));
}

BOOST_AUTO_TEST_CASE(test_multiply) {
  exp_type exponent = exp_type().get(x*y*z*z);
  exp_type empty = exp_type();
  output_test_stream output;

  BOOST_TEST_MESSAGE( "multiples" );
  output << exponent.multiples(exp_type().get(z*v*w));
  BOOST_CHECK(output.is_equal("{{x,y,z,v,w}, {x,y,z,v}, {x,y,z,w}, {x,y,z}}"));
  output << exponent.multiples(exp_type().get(v));
  BOOST_CHECK(output.is_equal("{{x,y,z,v}, {x,y,z}}"));
  output << exponent.multiples(exp_type().get(x));
  BOOST_CHECK(output.is_equal("{{x,y,z}}"));
  output << exponent.multiples(exp_type());
  BOOST_CHECK(output.is_equal("{{x,y,z}}"));
  output << empty.multiples(exp_type().get(z*v*w));
  BOOST_CHECK(output.is_equal("{{z,v,w}, {z,v}, {z,w}, {z}, {v,w}, {v}, {w}, {}}"));
  output << empty.multiples(exp_type().get(v));
  BOOST_CHECK(output.is_equal("{{v}, {}}"));
  output << empty.multiples(exp_type().get(x));
  BOOST_CHECK(output.is_equal("{{x}, {}}"));
  output << empty.multiples(exp_type());
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
  output << exponent.multiply(BooleVariable(v));
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3)"));
  output << exponent.multiply(BooleVariable(4));
  BOOST_CHECK(output.is_equal("(0, 1, 2, 4)"));
  output << exponent.multiply(BooleVariable());
  BOOST_CHECK(output.is_equal("(0, 1, 2)"));
  //output << exponent.multiply(BooleVariable(5));// memory access violation at address: 0x00000034
  //BOOST_CHECK(output.is_equal("()"));
  output << exponent.multiply(v*w);
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4)"));
  output << exponent.multiply(BooleMonomial());
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
  output << empty.multiply(BooleVariable(v));
  BOOST_CHECK(output.is_equal("(3)"));
  output << empty.multiply(BooleVariable(4));
  BOOST_CHECK(output.is_equal("(4)"));
  output << empty.multiply(BooleVariable());
  BOOST_CHECK(output.is_equal("(0)"));
  //output << exponent.multiply(BooleVariable(5));// memory access violation at address: 0x00000034
  //BOOST_CHECK(output.is_equal("()"));
  output << empty.multiply(v*w);
  BOOST_CHECK(output.is_equal("(3, 4)"));
  output << empty.multiply(BooleMonomial());
  BOOST_CHECK(output.is_equal("()"));

  BOOST_TEST_MESSAGE( "multiplyFirst" );
  BooleSet set;
  output << exponent.multiplyFirst(set);
  BOOST_CHECK(output.is_equal("(0, 1, 2)"));
  set = set.add(BooleMonomial());
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
  set = BooleSet();
  output << empty.multiplyFirst(set);
  BOOST_CHECK(output.is_equal("()"));
  set = set.add(BooleMonomial());
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
  BOOST_CHECK(exp1 == exp_type().get(x*y));
  BOOST_CHECK(exp2 == exp_type().get(z*v*w));

  exp1 = exp_type().get(x*x);
  exp2 = exp_type().get(z*v);
  output << LCM(exp1, exp2);
  BOOST_CHECK(output.is_equal("(0, 2, 3)"));
  BOOST_CHECK(exp1 == exp_type().get(x));
  BOOST_CHECK(exp2 == exp_type().get(z*v));

  exp1 = exp_type().get(x*y*z*v*w);
  exp2 = exp_type().get(x*y*z*v*w);
  output << LCM(exp1, exp2);
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4)"));
  BOOST_CHECK(exp1 == exp_type().get(x*y*z*v*w));
  BOOST_CHECK(exp2 == exp_type().get(x*y*z*v*w));

  exp1 = exp_type().get(x*y*z*v*w);
  exp2 = exp_type();
  output << LCM(exp1, exp2);
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4)"));
  BOOST_CHECK(exp1 == exp_type().get(x*y*z*v*w));
  BOOST_CHECK(exp2 == exp_type());

  exp1 = exp_type();
  exp2 = exp_type().get(x*y*z*v*w);
  output << LCM(exp1, exp2);
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4)"));
  BOOST_CHECK(exp1 == exp_type());
  BOOST_CHECK(exp2 == exp_type().get(x*y*z*v*w));

  exp1 = exp_type();
  exp2 = exp_type();
  output << LCM(exp1, exp2);
  BOOST_CHECK(output.is_equal("()"));
  BOOST_CHECK(exp1 == exp_type());
  BOOST_CHECK(exp2 == exp_type());

  BOOST_TEST_MESSAGE( "LCM as method of one monomial" );

  exp1 = exp_type().get(x*x*y);
  exp2 = exp_type().get(z*v*w);
  output << exp1.LCM(exp2);
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4)"));
  BOOST_CHECK(exp1 == exp_type().get(x*y));
  BOOST_CHECK(exp2 == exp_type().get(z*v*w));

  exp1 = exp_type().get(x*x);
  exp2 = exp_type().get(z*v);
  output << exp1.LCM(exp2);
  BOOST_CHECK(output.is_equal("(0, 2, 3)"));
  BOOST_CHECK(exp1 == exp_type().get(x));
  BOOST_CHECK(exp2 == exp_type().get(z*v));

  exp1 = exp_type().get(x*y*z*v*w);
  exp2 = exp_type().get(x*y*z*v*w);
  output << exp1.LCM(exp2);
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4)"));
  BOOST_CHECK(exp1 == exp_type().get(x*y*z*v*w));
  BOOST_CHECK(exp2 == exp_type().get(x*y*z*v*w));

  exp1 = exp_type().get(x*y*z*v*w);
  exp2 = exp_type();
  output << exp1.LCM(exp2);
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4)"));
  BOOST_CHECK(exp1 == exp_type().get(x*y*z*v*w));
  BOOST_CHECK(exp2 == exp_type());

  exp1 = exp_type();
  exp2 = exp_type().get(x*y*z*v*w);
  output << exp1.LCM(exp2);
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4)"));
  BOOST_CHECK(exp1 == exp_type());
  BOOST_CHECK(exp2 == exp_type().get(x*y*z*v*w));

  exp1 = exp_type();
  exp2 = exp_type();
  output << exp1.LCM(exp2);
  BOOST_CHECK(output.is_equal("()"));
  BOOST_CHECK(exp1 == exp_type());
  BOOST_CHECK(exp2 == exp_type());

  BOOST_TEST_MESSAGE( "LCMDeg as method of one monomial" );

  exp1 = exp_type().get(x*x*y);
  exp2 = exp_type().get(z*v*w);
  output << exp1.LCMDeg(exp2);
  BOOST_CHECK(output.is_equal("5"));
  BOOST_CHECK(exp1 == exp_type().get(x*y));
  BOOST_CHECK(exp2 == exp_type().get(z*v*w));

  exp1 = exp_type().get(x*x);
  exp2 = exp_type().get(z*v);
  output << exp1.LCMDeg(exp2);
  BOOST_CHECK(output.is_equal("3"));
  BOOST_CHECK(exp1 == exp_type().get(x));
  BOOST_CHECK(exp2 == exp_type().get(z*v));

  exp1 = exp_type().get(x*y*z*v*w);
  exp2 = exp_type().get(x*y*z*v*w);
  output << exp1.LCMDeg(exp2);
  BOOST_CHECK(output.is_equal("5"));
  BOOST_CHECK(exp1 == exp_type().get(x*y*z*v*w));
  BOOST_CHECK(exp2 == exp_type().get(x*y*z*v*w));

  exp1 = exp_type().get(x*y*z*v*w);
  exp2 = exp_type();
  output << exp1.LCMDeg(exp2);
  BOOST_CHECK(output.is_equal("5"));
  BOOST_CHECK(exp1 == exp_type().get(x*y*z*v*w));
  BOOST_CHECK(exp2 == exp_type());

  exp1 = exp_type();
  exp2 = exp_type().get(x*y*z*v*w);
  output << exp1.LCMDeg(exp2);
  BOOST_CHECK(output.is_equal("5"));
  BOOST_CHECK(exp1 == exp_type());
  BOOST_CHECK(exp2 == exp_type().get(x*y*z*v*w));

  exp1 = exp_type();
  exp2 = exp_type();
  output << exp1.LCMDeg(exp2);
  BOOST_CHECK(output.is_equal("0"));
  BOOST_CHECK(exp1 == exp_type());
  BOOST_CHECK(exp2 == exp_type());

  exp1 = exp_type().get(x);
  exp2 = exp_type().get(x);
  output << exp1.LCMDeg(exp2);
  BOOST_CHECK(output.is_equal("1"));
  BOOST_CHECK(exp1 == exp_type().get(x));
  BOOST_CHECK(exp2 == exp_type().get(x));

  exp1 = exp_type().get(x);
  exp2 = exp_type().get(y);
  output << exp1.LCMDeg(exp2);
  BOOST_CHECK(output.is_equal("2"));
  BOOST_CHECK(exp1 == exp_type().get(x));
  BOOST_CHECK(exp2 == exp_type().get(y));
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
  BOOST_CHECK(exp1 == exp_type().get(x*y));
  BOOST_CHECK(exp2 == exp_type().get(z*v*w));

  exp1 = exp_type().get(x*y*z*v*w);
  exp2 = exp_type().get(x*y*z*v*w);
  output << GCD(exp1, exp2);
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4)"));
  BOOST_CHECK(exp1 == exp_type().get(x*y*z*v*w));
  BOOST_CHECK(exp2 == exp_type().get(x*y*z*v*w));

  exp1 = exp_type().get(x*y*z*v*w);
  exp2 = exp_type();
  output << GCD(exp1, exp2);
  BOOST_CHECK(output.is_equal("()"));
  BOOST_CHECK(exp1 == exp_type().get(x*y*z*v*w));
  BOOST_CHECK(exp2 == exp_type());

  exp1 = exp_type();
  exp2 = exp_type().get(x*y*z*v*w);
  output << GCD(exp1, exp2);
  BOOST_CHECK(output.is_equal("()"));
  BOOST_CHECK(exp1 == exp_type());
  BOOST_CHECK(exp2 == exp_type().get(x*y*z*v*w));

  exp1 = exp_type();
  exp2 = exp_type();
  output << GCD(exp1, exp2);
  BOOST_CHECK(output.is_equal("()"));
  BOOST_CHECK(exp1 == exp_type());
  BOOST_CHECK(exp2 == exp_type());

  BOOST_TEST_MESSAGE( "GCD as method of one monomial" );

  exp1 = exp_type().get(x*x*y);
  exp2 = exp_type().get(z*v*w);
  output << exp1.GCD(exp2);
  BOOST_CHECK(output.is_equal("()"));
  BOOST_CHECK(exp1 == exp_type().get(x*y));
  BOOST_CHECK(exp2 == exp_type().get(z*v*w));

  exp1 = exp_type().get(x*y*z*v*w);
  exp2 = exp_type().get(x*y*z*v*w);
  output << exp1.GCD(exp2);
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4)"));
  BOOST_CHECK(exp1 == exp_type().get(x*y*z*v*w));
  BOOST_CHECK(exp2 == exp_type().get(x*y*z*v*w));

  exp1 = exp_type().get(x*y*z*v*w);
  exp2 = exp_type();
  output << exp1.GCD(exp2);
  BOOST_CHECK(output.is_equal("()"));
  BOOST_CHECK(exp1 == exp_type().get(x*y*z*v*w));
  BOOST_CHECK(exp2 == exp_type());

  exp1 = exp_type();
  exp2 = exp_type().get(x*y*z*v*w);
  output << exp1.GCD(exp2);
  BOOST_CHECK(output.is_equal("()"));
  BOOST_CHECK(exp1 == exp_type());
  BOOST_CHECK(exp2 == exp_type().get(x*y*z*v*w));

  exp1 = exp_type();
  exp2 = exp_type();
  output << exp1.GCD(exp2);
  BOOST_CHECK(output.is_equal("()"));
  BOOST_CHECK(exp1 == exp_type());
  BOOST_CHECK(exp2 == exp_type());
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
  BOOST_CHECK_NE(exponent2, exponent);
  exponent2 = exponent;
  exponent2.push_back(0);
  BOOST_CHECK_EQUAL(exponent2, exp_type().get(x*y*z));
  exponent2.push_back(4);
  BOOST_CHECK_EQUAL(exponent2, exp_type().get(x*y*z*w));
  exponent2.push_back(3);
  BOOST_CHECK_EQUAL(exponent2, exp_type().get(x*y*z*v*w));
  exponent2.push_back(5);
  output << exponent2;
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4, 5)"));
  BOOST_CHECK_NE(exponent2, exponent);
  exponent2 = exponent.insertConst(0);
  BOOST_CHECK_EQUAL(exponent2, exp_type().get(x*y*z));
  exponent2 = exponent2.insertConst(4);
  BOOST_CHECK_EQUAL(exponent2, exp_type().get(x*y*z*w));
  exponent2 = exponent2.insertConst(3);
  BOOST_CHECK_EQUAL(exponent2, exp_type().get(x*y*z*v*w));
  exponent2 = exponent2.insertConst(5);
  output << exponent2;
  BOOST_CHECK(output.is_equal("(0, 1, 2, 3, 4, 5)"));
  BOOST_CHECK_NE(exponent2, exponent);

  BOOST_TEST_MESSAGE( "remove, removeConst" );
}

BOOST_AUTO_TEST_SUITE_END()
