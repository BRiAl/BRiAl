// -*- c++ -*-
//*****************************************************************************
/** @file spolyTest.cc
 *
 * @author Ket Kalda, Alexander Dreyer
 * @date 2011-03-15
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

#include <polybori.h>

USING_NAMESPACE_PBORI

struct Fspoly {
  Fspoly(const BoolePolyRing& input_ring = BoolePolyRing(5)): 
    ring(input_ring),
    x(BooleVariable(0, input_ring)), y(BooleVariable(1, input_ring)),
    z(BooleVariable(2, input_ring)),
    v(BooleVariable(3, input_ring)), w(BooleVariable(4, input_ring)) {

    BOOST_TEST_MESSAGE( "setup fixture" );
  }
  ~Fspoly() { BOOST_TEST_MESSAGE( "teardown fixture" ); }

  BoolePolyRing ring;
  BooleMonomial x, y, z, v, w;
};

BOOST_FIXTURE_TEST_SUITE(spolyTestSuite, Fspoly )

BOOST_AUTO_TEST_CASE(test_spoly) {

  BOOST_TEST_MESSAGE("term_spoly");
  BoolePolynomial poly1 = x*y + x*z + y + w;
  BoolePolynomial poly2 = x*w + w;
  BoolePolynomial theSpoly = spoly(poly1, poly2);

  BOOST_CHECK_EQUAL(theSpoly, x*z*w + w);
  BOOST_CHECK_THROW(spoly(BoolePolynomial(ring), BoolePolynomial(ring)),
                    PBoRiGenericError<CTypes::illegal_on_zero>);
  BOOST_CHECK_EQUAL(spoly(BooleMonomial(ring), BooleMonomial(ring)),BoolePolynomial(ring));
}

BOOST_AUTO_TEST_SUITE_END()
