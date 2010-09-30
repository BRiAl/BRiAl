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
  BOOST_CHECK_EQUAL(monom_type(ring), BooleConstant(true));
}

BOOST_AUTO_TEST_CASE(test_assigning_operators) {

  output_test_stream output;

  BOOST_TEST_MESSAGE( "*=" );
  output << ((((monom_type(x) *= y) *= z) *= v)*= w);
  BOOST_CHECK(output.is_equal("x*y*z*v*w"));
  output << ((((monom_type(x) *= x) *= x) *= x)*= x);
  BOOST_CHECK(output.is_equal("x"));

  BOOST_TEST_MESSAGE( "/=" );
  output << ((monom_type(x*y*z*v*w)/= x)/= v);
  BOOST_CHECK(output.is_equal("y*z*w"));
  output << ((monom_type(x*y*z*v*w)/= x)/= v);
  BOOST_CHECK(output.is_equal("y*z*w"));
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

BOOST_AUTO_TEST_CASE(test_logical_operators) {
  BOOST_CHECK_EQUAL(monom_type() == monom_type(), true);
  BOOST_CHECK_EQUAL(monom_type() != monom_type(), false);
  BOOST_CHECK_EQUAL(monom_type() == BooleConstant(true), true);
  BOOST_CHECK_EQUAL(monom_type() != BooleConstant(true), false);
  BOOST_CHECK_EQUAL(monom_type(x) == x, true);
  BOOST_CHECK_EQUAL(monom_type(x) != x, false);
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
