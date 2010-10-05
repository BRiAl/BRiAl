// -*- c++ -*-
//*****************************************************************************
/** @file BooleVariableTest.cc
 *
 * @author Ket Kalda, Alexander Dreyer
 * @date 2010-10-05
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

struct Fvar {
  typedef BooleVariable var_type;
  Fvar():
    ring(3) {
      x = var_type(0);
      y = var_type(1);
      z = var_type(2);
      BOOST_TEST_MESSAGE( "setup fixture" );
      ring.setVariableName(0, "x");
      ring.setVariableName(1, "y");
      ring.setVariableName(2, "z");
      bset = BooleSet(1, ring.one(), ring.zero());
    }

    ~Fvar() { BOOST_TEST_MESSAGE( "teardown fixture" ); }

    BoolePolyRing ring;
    BooleVariable x, y, z;
    BooleSet bset;
};

BOOST_FIXTURE_TEST_SUITE(BooleVariableTestSuite, Fvar )

BOOST_AUTO_TEST_CASE(test_constructors) {

  BOOST_TEST_MESSAGE( "Constructors..." );
  BOOST_CHECK_EQUAL(var_type(), var_type(0));
  BOOST_CHECK_EQUAL(var_type(0),x);
  BOOST_CHECK_EQUAL(var_type(1,ring),y);
  BOOST_CHECK_EQUAL(var_type(z),z);
}

BOOST_AUTO_TEST_CASE(test_methods) {

  BOOST_TEST_MESSAGE( "Casting to poly_type" );
  BOOST_CHECK_EQUAL((BoolePolynomial)x, BoolePolynomial(x));
  BOOST_CHECK_EQUAL((BoolePolynomial)var_type(), BoolePolynomial(x));
  BOOST_TEST_MESSAGE( "index" );
  BOOST_CHECK_EQUAL(var_type().index(), 0);
  BOOST_CHECK_EQUAL(var_type(1).index(), 1);
  BOOST_CHECK_EQUAL(var_type(z).index(), 2);
  BOOST_CHECK_EQUAL(z.index(), 2);
  //BOOST_CHECK_EQUAL(var_type(3).index(), 3);
  BOOST_TEST_MESSAGE( "ring" );
  //BOOST_CHECK_EQUAL(x.ring(), ring);
  BOOST_TEST_MESSAGE( "set" );
  BOOST_CHECK_EQUAL(y.set(),bset);
  BOOST_CHECK_EQUAL(var_type(1).set(),bset);
  BOOST_CHECK_NE(x.set(),bset);
}

BOOST_AUTO_TEST_CASE(test_logical_operators) {
  BOOST_TEST_MESSAGE( "== and !=" );
}

BOOST_AUTO_TEST_CASE(test_assigning_operators) {
  BOOST_TEST_MESSAGE( "/" );
  BOOST_CHECK_EQUAL(x / y, BoolePolynomial(0));
  BOOST_CHECK_EQUAL(BoolePolynomial(x) / BoolePolynomial(y), BoolePolynomial(0));
  BOOST_CHECK_EQUAL((BoolePolynomial)x / (BoolePolynomial)y, BoolePolynomial(0));
  BOOST_CHECK_EQUAL((BoolePolynomial)x / y, BoolePolynomial(0));
  BOOST_CHECK_THROW(BooleMonomial(x) / BooleMonomial(y), PBoRiError);
  BOOST_CHECK_EQUAL(x / x, BoolePolynomial(1));
  BOOST_CHECK_EQUAL(var_type() / x, BoolePolynomial(1));
  BOOST_CHECK_EQUAL(var_type() / var_type(), BoolePolynomial(1));
}

BOOST_AUTO_TEST_SUITE_END()
