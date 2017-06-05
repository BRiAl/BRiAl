// -*- c++ -*-
//*****************************************************************************
/** @file MonomialFactoryTest.cc
 *
 * @author Alexander Dreyer
 * @date 2011-05-05
 *
 * boost/test-driven unit test
 * 
 * @par Copyright:
 *   (c) 2011 by The PolyBoRi Team
 *
 **/
//*****************************************************************************


#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp> 
using boost::test_tools::output_test_stream;

#include <polybori/pbori_defs.h>
#include <polybori/factories/MonomialFactory.h>
#include <polybori/BooleExponent.h>
#include <polybori/BoolePolyRing.h>

USING_NAMESPACE_PBORI

struct Fmonfac {
  Fmonfac(const BoolePolyRing& input_ring = BoolePolyRing(5)):
    ring(input_ring),
    bexp(BooleExponent().change(1)),
    x(0, input_ring), y(1, input_ring), z(2, input_ring),
    v(3, input_ring), w(4, input_ring) {

    BOOST_TEST_MESSAGE( "setup fixture" );

    ring.setVariableName(0, "x");
    ring.setVariableName(1, "y");
    ring.setVariableName(2, "z");
    ring.setVariableName(3, "v");
    ring.setVariableName(4, "w");
  }

  ~Fmonfac() { BOOST_TEST_MESSAGE( "teardown fixture" ); }

  BoolePolyRing ring;
  BooleExponent bexp;
  BooleVariable x, y, z, v, w;
};


BOOST_FIXTURE_TEST_SUITE(MonomialFactoryTest, Fmonfac )

BOOST_AUTO_TEST_CASE(test_constructors) {

  MonomialFactory factory(ring);
  MonomialFactory factory_copy(factory);

  BOOST_CHECK_EQUAL(factory.parent().hash(), ring.hash());
  BOOST_CHECK_EQUAL(factory_copy.parent().hash(), ring.hash());
}

BOOST_AUTO_TEST_CASE(test_pseudo_constructors) {

  MonomialFactory factory(ring);

  BOOST_TEST_MESSAGE( "Constant monomials..." );
  BOOST_CHECK_EQUAL(factory(), BooleConstant(true));

  BOOST_TEST_MESSAGE( "Polynomials from other types (variables, exponents, rings)..." );
  BOOST_CHECK_EQUAL(factory(x), x);
  BOOST_CHECK_EQUAL(factory(bexp, ring), y);
  BOOST_CHECK_EQUAL(factory(bexp), y);

  BOOST_CHECK_EQUAL(factory(BooleExponent(),ring), BooleMonomial(ring));
  BOOST_CHECK_EQUAL(factory(BooleExponent()), BooleMonomial(ring));

  BOOST_CHECK_EQUAL(factory(ring), BooleConstant(true));
  BOOST_CHECK_EQUAL(factory(), BooleConstant(true));

  BooleExponent exponent;
  exponent.insert(4);
  exponent.insert(0);

  BooleMonomial monom = factory(exponent, ring);
  output_test_stream output;
  output << monom;
  BOOST_CHECK(output.is_equal("x*w"));

  monom = factory(exponent);
  output << monom;
  BOOST_CHECK(output.is_equal("x*w"));

  exponent.insert(5);
  BOOST_CHECK_THROW(factory(exponent, ring), PBoRiError);
  BOOST_CHECK_THROW(factory(exponent), PBoRiError);
}

BOOST_AUTO_TEST_SUITE_END()
