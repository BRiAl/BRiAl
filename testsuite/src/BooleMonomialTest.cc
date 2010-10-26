// -*- c++ -*-
//*****************************************************************************
/** @file BooleMonomialTest.cc
 *
 * @author Ket Kalda, Alexander Dreyer
 * @date 2010-09-16
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
#include "BooleMonomial.h"
#include "BoolePolynomial.h"
#include "BoolePolyRing.h"
#include "BooleEnv.h"

USING_NAMESPACE_PBORI

struct Fmonom {
  typedef BooleMonomial monom_type;
  Fmonom():
    ring(5),
    bexp(BooleExponent().change(1)) {
    x = (BooleVariable(0));
    y = (BooleVariable(1));
    z = (BooleVariable(2));
    v = (BooleVariable(3));
    w = (BooleVariable(4));
    BOOST_TEST_MESSAGE( "setup fixture" );
    bset = BooleSet(1, ring.one(), ring.zero()); // something non-trivial
    ring.setVariableName(0, "x");
    ring.setVariableName(1, "y");
    ring.setVariableName(2, "z");
    ring.setVariableName(3, "v");
    ring.setVariableName(4, "w");
  }

  ~Fmonom() { BOOST_TEST_MESSAGE( "teardown fixture" ); }

  BoolePolyRing ring;
  BooleSet bset;
  BooleExponent bexp;
  BooleVariable x, y, z, v, w;
};


BOOST_FIXTURE_TEST_SUITE(BooleMonomialTestSuite, Fmonom )

BOOST_AUTO_TEST_CASE(test_constructors) {

  BOOST_TEST_MESSAGE( "Constant monomials..." );
  BOOST_CHECK_EQUAL(monom_type(), BooleConstant(true));

  BOOST_TEST_MESSAGE( "Polynomials from other types (variables, exponents, rings)..." );
  BOOST_CHECK_EQUAL(monom_type(x), x);
  BOOST_CHECK_EQUAL(monom_type(bexp,ring), y);
  BOOST_CHECK_EQUAL(monom_type(BooleExponent(),ring), BooleMonomial());
  BOOST_CHECK_EQUAL(monom_type(ring), BooleConstant(true));

  BooleExponent exponent;
  exponent.insert(4);
  exponent.insert(0);
  BooleMonomial monom(exponent, ring);
  output_test_stream output;
  output << monom;
  BOOST_CHECK(output.is_equal("x*w"));

  exponent.insert(5);
  BOOST_CHECK_THROW(BooleMonomial(exponent, ring), PBoRiError);

}

BOOST_AUTO_TEST_CASE(test_assigning_operators) {

  BOOST_TEST_MESSAGE( "Assignments..." );

  output_test_stream output;

  BOOST_TEST_MESSAGE( "*=" );
  output << ((((monom_type(x) *= y) *= z) *= v)*= w);
  BOOST_CHECK(output.is_equal("x*y*z*v*w"));
  output << ((((monom_type(x) *= x) *= x) *= x)*= x);
  BOOST_CHECK(output.is_equal("x"));
  BOOST_CHECK_EQUAL(monom_type(x*y)*=monom_type(),monom_type(x*y));
  BOOST_CHECK_EQUAL(monom_type()*=monom_type(x*z),monom_type(x*z));
  BOOST_CHECK_EQUAL(monom_type()*=monom_type(),monom_type());

  BOOST_TEST_MESSAGE( "/=" );
  output << ((monom_type(x*y*z*v*w)/= x)/= v);
  BOOST_CHECK(output.is_equal("y*z*w"));
  output << ((monom_type(x*y*z*v*w)/= x)/= v);
  BOOST_CHECK(output.is_equal("y*z*w"));
  output << (monom_type(x*y*z*v*w)/= monom_type());
  BOOST_CHECK(output.is_equal("x*y*z*v*w"));
  BOOST_CHECK_THROW(monom_type()/monom_type(x), PBoRiError);
  BOOST_CHECK_THROW(monom_type(y)/monom_type(x), PBoRiError);
  BOOST_CHECK_THROW(monom_type(z*y)/monom_type(x), PBoRiError);
}

BOOST_AUTO_TEST_CASE(test_properties) {

  BOOST_TEST_MESSAGE( "isOne, isConstant" );
  BOOST_CHECK_EQUAL(monom_type().isOne(), true);
  BOOST_CHECK_EQUAL(monom_type().isConstant(), true);
  BOOST_CHECK_EQUAL(monom_type(x).isOne(), false);
  BOOST_CHECK_EQUAL(monom_type(x).isConstant(), false);
  BOOST_CHECK_EQUAL(monom_type(ring).isOne(), true);
  BOOST_CHECK_EQUAL(monom_type(ring).isConstant(), true);
}

BOOST_AUTO_TEST_CASE(test_methods) {

  BOOST_TEST_MESSAGE( "Methods..." );

  output_test_stream output;
  monom_type monom;
  monom_type monom2;

  BOOST_TEST_MESSAGE( "firstVariable and firstIndex" );
  BOOST_CHECK(monom_type(v*w*y*z*x).firstVariable() == x);
  output << monom_type(v*w*y*z*x).firstIndex();
  BOOST_CHECK(output.is_equal("0"));

  BOOST_TEST_MESSAGE( "popFirst" );
  monom = monom_type(y*x);
  output << monom.popFirst();
  BOOST_CHECK(output.is_equal("y"));
  BOOST_CHECK(monom == monom_type(y));
  output << monom.popFirst();
  BOOST_CHECK(output.is_equal("1"));
  BOOST_CHECK(monom == monom_type());
  output << monom.popFirst();
  BOOST_CHECK(output.is_equal("1"));

  BOOST_TEST_MESSAGE( "deg and size" );
  monom = monom_type(v*y*x*x*z);
  output << monom.deg();
  BOOST_CHECK(output.is_equal("4"));
  output << monom.size();
  BOOST_CHECK(output.is_equal("4"));
  monom = monom_type();
  output << monom.deg();
  BOOST_CHECK(output.is_equal("0"));
  output << monom.size();
  BOOST_CHECK(output.is_equal("0"));
  monom = monom_type(x*w);
  output << monom.deg();
  BOOST_CHECK(output.is_equal("2"));
  output << monom.size();
  BOOST_CHECK(output.is_equal("2"));

  BOOST_TEST_MESSAGE( "exp" );
  monom = monom_type(x*w*z);
  output << monom.exp();
  BOOST_CHECK(output.is_equal("(0, 2, 4)"));
  monom = monom_type();
  output << monom.exp();
  BOOST_CHECK(output.is_equal("()"));

  BOOST_TEST_MESSAGE( "multiples" );
  monom = monom_type(x*w*z);
  monom2 = monom_type(x*y*z*v*w);
  output << monom.multiples(monom2);
  BOOST_CHECK(output.is_equal("{{x,y,z,v,w}, {x,y,z,w}, {x,z,v,w}, {x,z,w}}"));
  output << monom.multiples(monom_type());
  BOOST_CHECK(output.is_equal("{{x,z,w}}"));
  monom = monom_type();
  monom2 = monom_type(x*y);
  output << monom.multiples(monom2);
  BOOST_CHECK(output.is_equal("{{x,y}, {x}, {y}, {}}"));
  monom = monom_type();
  monom2 = monom_type();
  output << monom.multiples(monom2);
  BOOST_CHECK(output.is_equal("{{}}"));
}

BOOST_AUTO_TEST_CASE(test_logical_operators) {

  BOOST_TEST_MESSAGE( "Logical operators..." );

  output_test_stream output;
  monom_type monom1;
  monom_type monom2;

  BOOST_TEST_MESSAGE( "== and !=" );
  BOOST_CHECK_EQUAL(monom_type() == monom_type(), true);
  BOOST_CHECK_EQUAL(monom_type() != monom_type(), false);
  BOOST_CHECK_EQUAL(monom_type() == BooleConstant(true), true);
  BOOST_CHECK_EQUAL(monom_type() != BooleConstant(true), false);
  BOOST_CHECK_EQUAL(monom_type(x) == x, true);
  BOOST_CHECK_EQUAL(monom_type(x) != x, false);

  BOOST_TEST_MESSAGE( "compare as method of one monomial" );

  monom1 = monom_type();
  monom2 = monom_type(x*y*z*v*w);
  output << monom1.compare(monom2);
  BOOST_CHECK(output.is_equal("-1"));

  monom1 = monom_type(x);
  monom2 = monom_type(x*y);
  output << monom1.compare(monom2);
  BOOST_CHECK(output.is_equal("-1"));

  monom1 = monom_type(x*y*z);
  monom2 = monom_type(x*z);
  output << monom1.compare(monom2);
  BOOST_CHECK(output.is_equal("1"));

  monom1 = monom_type(x*y*z);
  monom2 = monom_type(x*y*z);
  output << monom1.compare(monom2);
  BOOST_CHECK(output.is_equal("0"));

  BOOST_TEST_MESSAGE( ">, <, >= and <=" );

  monom1 = monom_type();
  monom2 = monom_type(x*y*z*v*w);
  BOOST_CHECK_EQUAL(monom1 < monom2, true);
  BOOST_CHECK_EQUAL(monom1 <= monom2, true);
  BOOST_CHECK_EQUAL(monom1 > monom2, false);
  BOOST_CHECK_EQUAL(monom1 >= monom2, false);

  monom1 = monom_type(x);
  monom2 = monom_type(x*y);
  BOOST_CHECK_EQUAL(monom1 < monom2, true);
  BOOST_CHECK_EQUAL(monom1 <= monom2, true);
  BOOST_CHECK_EQUAL(monom1 > monom2, false);
  BOOST_CHECK_EQUAL(monom1 >= monom2, false);

  monom1 = monom_type(x*y*z);
  monom2 = monom_type(x*z);
  BOOST_CHECK_EQUAL(monom1 < monom2, false);
  BOOST_CHECK_EQUAL(monom1 <= monom2, false);
  BOOST_CHECK_EQUAL(monom1 > monom2, true);
  BOOST_CHECK_EQUAL(monom1 >= monom2, true);

  monom1 = monom_type(x*y*z);
  monom2 = monom_type(x*y*z);
  BOOST_CHECK_EQUAL(monom1 < monom2, false);
  BOOST_CHECK_EQUAL(monom1 <= monom2, true);
  BOOST_CHECK_EQUAL(monom1 > monom2, false);
  BOOST_CHECK_EQUAL(monom1 >= monom2, true);
}

BOOST_AUTO_TEST_CASE(test_LCM) {

  output_test_stream output;
  monom_type monom1;
  monom_type monom2;

  BOOST_TEST_MESSAGE( "LCM of two monomials" );

  monom1 = monom_type(x*x*y);
  monom2 = monom_type(z*v*w);
  output << LCM(monom1, monom2);
  BOOST_CHECK(output.is_equal("x*y*z*v*w"));
  BOOST_CHECK(monom1 == monom_type(x*y));
  BOOST_CHECK(monom2 == monom_type(z*v*w));

  monom1 = monom_type(x*y*z*v*w);
  monom2 = monom_type(x*y*z*v*w);
  output << LCM(monom1, monom2);
  BOOST_CHECK(output.is_equal("x*y*z*v*w"));
  BOOST_CHECK(monom1 == monom_type(x*y*z*v*w));
  BOOST_CHECK(monom2 == monom_type(x*y*z*v*w));

  monom1 = monom_type(x*y*z*v*w);
  monom2 = monom_type();
  output << LCM(monom1, monom2);
  BOOST_CHECK(output.is_equal("x*y*z*v*w"));
  BOOST_CHECK(monom1 == monom_type(x*y*z*v*w));
  BOOST_CHECK(monom2 == monom_type());

  monom1 = monom_type();
  monom2 = monom_type(x*y*z*v*w);
  output << LCM(monom1, monom2);
  BOOST_CHECK(output.is_equal("x*y*z*v*w"));
  BOOST_CHECK(monom1 == monom_type());
  BOOST_CHECK(monom2 == monom_type(x*y*z*v*w));

  monom1 = monom_type();
  monom2 = monom_type();
  output << LCM(monom1, monom2);
  BOOST_CHECK(output.is_equal("1"));
  BOOST_CHECK(monom1 == monom_type());
  BOOST_CHECK(monom2 == monom_type());

  BOOST_TEST_MESSAGE( "LCM as method of one monomial" );

  monom1 = monom_type(x*x*y);
  monom2 = monom_type(z*v*w);
  output << monom1.LCM(monom2);
  BOOST_CHECK(output.is_equal("x*y*z*v*w"));
  BOOST_CHECK(monom1 == monom_type(x*y));
  BOOST_CHECK(monom2 == monom_type(z*v*w));

  monom1 = monom_type(x*y*z*v*w);
  monom2 = monom_type(x*y*z*v*w);
  output << monom1.LCM(monom2);
  BOOST_CHECK(output.is_equal("x*y*z*v*w"));
  BOOST_CHECK(monom1 == monom_type(x*y*z*v*w));
  BOOST_CHECK(monom2 == monom_type(x*y*z*v*w));

  monom1 = monom_type(x*y*z*v*w);
  monom2 = monom_type();
  output << monom1.LCM(monom2);
  BOOST_CHECK(output.is_equal("x*y*z*v*w"));
  BOOST_CHECK(monom1 == monom_type(x*y*z*v*w));
  BOOST_CHECK(monom2 == monom_type());

  monom1 = monom_type();
  monom2 = monom_type(x*y*z*v*w);
  output << monom1.LCM(monom2);
  BOOST_CHECK(output.is_equal("x*y*z*v*w"));
  BOOST_CHECK(monom1 == monom_type());
  BOOST_CHECK(monom2 == monom_type(x*y*z*v*w));

  monom1 = monom_type();
  monom2 = monom_type();
  output << monom1.LCM(monom2);
  BOOST_CHECK(output.is_equal("1"));
  BOOST_CHECK(monom1 == monom_type());
  BOOST_CHECK(monom2 == monom_type());

  BOOST_TEST_MESSAGE( "LCM as method of one monomial with assignment" );

  monom1 = monom_type(x*x*y);
  monom2 = monom_type(z*v*w);
  output << monom1.LCMAssign(monom2);
  BOOST_CHECK(output.is_equal("x*y*z*v*w"));
  BOOST_CHECK(monom1 == monom_type(x*y*z*v*w));
  BOOST_CHECK(monom2 == monom_type(z*v*w));

  monom1 = monom_type(x*y*z*v*w);
  monom2 = monom_type(x*y*z*v*w);
  output << monom1.LCMAssign(monom2);
  BOOST_CHECK(output.is_equal("x*y*z*v*w"));
  BOOST_CHECK(monom1 == monom_type(x*y*z*v*w));
  BOOST_CHECK(monom2 == monom_type(x*y*z*v*w));

  monom1 = monom_type(x*y*z*v*w);
  monom2 = monom_type();
  output << monom1.LCMAssign(monom2);
  BOOST_CHECK(output.is_equal("x*y*z*v*w"));
  BOOST_CHECK(monom1 == monom_type(x*y*z*v*w));
  BOOST_CHECK(monom2 == monom_type());

  monom1 = monom_type();
  monom2 = monom_type(x*y*z*v*w);
  output << monom1.LCMAssign(monom2);
  BOOST_CHECK(output.is_equal("x*y*z*v*w"));
  BOOST_CHECK(monom1 == monom_type(x*y*z*v*w));
  BOOST_CHECK(monom2 == monom_type(x*y*z*v*w));

  monom1 = monom_type();
  monom2 = monom_type();
  output << monom1.LCMAssign(monom2);
  BOOST_CHECK(output.is_equal("1"));
  BOOST_CHECK(monom1 == monom_type());
  BOOST_CHECK(monom2 == monom_type());

  BOOST_TEST_MESSAGE( "LCM degree as method of one monomial" );

  monom1 = monom_type(x*x*y);
  monom2 = monom_type(z*v*w);
  output << monom1.LCMDeg(monom2);
  BOOST_CHECK(output.is_equal("5"));
  BOOST_CHECK(monom1 == monom_type(x*y));
  BOOST_CHECK(monom2 == monom_type(z*v*w));

  monom1 = monom_type(x*y*z*v*w);
  monom2 = monom_type(x*y*z*v*w);
  output << monom1.LCMDeg(monom2);
  BOOST_CHECK(output.is_equal("5"));
  BOOST_CHECK(monom1 == monom_type(x*y*z*v*w));
  BOOST_CHECK(monom2 == monom_type(x*y*z*v*w));

  monom1 = monom_type(x*y*z*v*w);
  monom2 = monom_type();
  output << monom1.LCMDeg(monom2);
  BOOST_CHECK(output.is_equal("5"));
  BOOST_CHECK(monom1 == monom_type(x*y*z*v*w));
  BOOST_CHECK(monom2 == monom_type());

  monom1 = monom_type();
  monom2 = monom_type(x*y*z*v*w);
  output << monom1.LCMDeg(monom2);
  BOOST_CHECK(output.is_equal("5"));
  BOOST_CHECK(monom1 == monom_type());
  BOOST_CHECK(monom2 == monom_type(x*y*z*v*w));

  monom1 = monom_type();
  monom2 = monom_type();
  output << monom1.LCMDeg(monom2);
  BOOST_CHECK(output.is_equal("0"));
  BOOST_CHECK(monom1 == monom_type());
  BOOST_CHECK(monom2 == monom_type());

  monom1 = monom_type(x);
  monom2 = monom_type(x);
  output << monom1.LCMDeg(monom2);
  BOOST_CHECK(output.is_equal("1"));
  BOOST_CHECK(monom1 == monom_type(x));
  BOOST_CHECK(monom2 == monom_type(x));

  monom1 = monom_type(x);
  monom2 = monom_type(y);
  output << monom1.LCMDeg(monom2);
  BOOST_CHECK(output.is_equal("2"));
  BOOST_CHECK(monom1 == monom_type(x));
  BOOST_CHECK(monom2 == monom_type(y));
}

BOOST_AUTO_TEST_CASE(test_GCD) {
  output_test_stream output;
  monom_type monom1;
  monom_type monom2;

  BOOST_TEST_MESSAGE( "GCD of two monomials" );

  monom1 = monom_type(x*x*y);
  monom2 = monom_type(z*v*w);
  output << GCD(monom1, monom2);
  BOOST_CHECK(output.is_equal("1"));
  BOOST_CHECK(monom1 == monom_type(x*y));
  BOOST_CHECK(monom2 == monom_type(z*v*w));

  monom1 = monom_type(x*y*z*v*w);
  monom2 = monom_type(x*y*z*v*w);
  output << GCD(monom1, monom2);
  BOOST_CHECK(output.is_equal("x*y*z*v*w"));
  BOOST_CHECK(monom1 == monom_type(x*y*z*v*w));
  BOOST_CHECK(monom2 == monom_type(x*y*z*v*w));

  monom1 = monom_type(x*y*z*v*w);
  monom2 = monom_type();
  output << GCD(monom1, monom2);
  BOOST_CHECK(output.is_equal("1"));
  BOOST_CHECK(monom1 == monom_type(x*y*z*v*w));
  BOOST_CHECK(monom2 == monom_type());

  monom1 = monom_type();
  monom2 = monom_type(x*y*z*v*w);
  output << GCD(monom1, monom2);
  BOOST_CHECK(output.is_equal("1"));
  BOOST_CHECK(monom1 == monom_type());
  BOOST_CHECK(monom2 == monom_type(x*y*z*v*w));

  monom1 = monom_type();
  monom2 = monom_type();
  output << GCD(monom1, monom2);
  BOOST_CHECK(output.is_equal("1"));
  BOOST_CHECK(monom1 == monom_type());
  BOOST_CHECK(monom2 == monom_type());

  BOOST_TEST_MESSAGE( "GCD as method of one monomial" );

  monom1 = monom_type(x*x*y);
  monom2 = monom_type(z*v*w);
  output << monom1.GCD(monom2);
  BOOST_CHECK(output.is_equal("1"));
  BOOST_CHECK(monom1 == monom_type(x*y));
  BOOST_CHECK(monom2 == monom_type(z*v*w));

  monom1 = monom_type(x*y*z*v*w);
  monom2 = monom_type(x*y*z*v*w);
  output << monom1.GCD(monom2);
  BOOST_CHECK(output.is_equal("x*y*z*v*w"));
  BOOST_CHECK(monom1 == monom_type(x*y*z*v*w));
  BOOST_CHECK(monom2 == monom_type(x*y*z*v*w));

  monom1 = monom_type(x*y*z*v*w);
  monom2 = monom_type();
  output << monom1.GCD(monom2);
  BOOST_CHECK(output.is_equal("1"));
  BOOST_CHECK(monom1 == monom_type(x*y*z*v*w));
  BOOST_CHECK(monom2 == monom_type());

  monom1 = monom_type();
  monom2 = monom_type(x*y*z*v*w);
  output << monom1.GCD(monom2);
  BOOST_CHECK(output.is_equal("1"));
  BOOST_CHECK(monom1 == monom_type());
  BOOST_CHECK(monom2 == monom_type(x*y*z*v*w));

  monom1 = monom_type();
  monom2 = monom_type();
  output << monom1.GCD(monom2);
  BOOST_CHECK(output.is_equal("1"));
  BOOST_CHECK(monom1 == monom_type());
  BOOST_CHECK(monom2 == monom_type());

  BOOST_TEST_MESSAGE( "GCD as method of one monomial with assignment" );

  monom1 = monom_type(x*x*y);
  monom2 = monom_type(z*v*w);
  output << monom1.GCDAssign(monom2);
  BOOST_CHECK(output.is_equal("1"));
  BOOST_CHECK(monom1 == monom_type());
  BOOST_CHECK(monom2 == monom_type(z*v*w));

  monom1 = monom_type(x*y*z*v*w);
  monom2 = monom_type(x*y*z*v*w);
  output << monom1.GCDAssign(monom2);
  BOOST_CHECK(output.is_equal("x*y*z*v*w"));
  BOOST_CHECK(monom1 == monom_type(x*y*z*v*w));
  BOOST_CHECK(monom2 == monom_type(x*y*z*v*w));

  monom1 = monom_type(x*y*z*v*w);
  monom2 = monom_type();
  output << monom1.GCDAssign(monom2);
  BOOST_CHECK(output.is_equal("1"));
  BOOST_CHECK(monom1 == monom_type());
  BOOST_CHECK(monom2 == monom_type());

  monom1 = monom_type();
  monom2 = monom_type(x*y*z*v*w);
  output << monom1.GCDAssign(monom2);
  BOOST_CHECK(output.is_equal("1"));
  BOOST_CHECK(monom1 == monom_type());
  BOOST_CHECK(monom2 == monom_type(x*y*z*v*w));

  monom1 = monom_type();
  monom2 = monom_type();
  output << monom1.GCDAssign(monom2);
  BOOST_CHECK(output.is_equal("1"));
  BOOST_CHECK(monom1 == monom_type());
  BOOST_CHECK(monom2 == monom_type());

}

BOOST_AUTO_TEST_SUITE_END()
