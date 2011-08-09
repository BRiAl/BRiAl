// -*- c++ -*-
//*****************************************************************************
/** @file FGLMStrategyTest.cc
 *
 * @author Alexander Dreyer
 * @date 2011-08-08
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
#include <polybori/groebner/FGLMStrategy.h>
#include <polybori/BoolePolyRing.h>

USING_NAMESPACE_PBORI
USING_NAMESPACE_PBORIGB

struct Ffglm {
  Ffglm(const BoolePolyRing& input_ring = BoolePolyRing(50)):
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

  ~Ffglm() { BOOST_TEST_MESSAGE( "teardown fixture" ); }

  BoolePolyRing ring;
  BooleVariable x, y, z, v, w;
};


BOOST_FIXTURE_TEST_SUITE(FGLMStrategyTest, Ffglm)

BOOST_AUTO_TEST_CASE(test_constructors) {

  BoolePolyRing other(ring.clone());
  other.changeOrdering(CTypes::dp_asc);

  PolynomialVector ideal;
  ideal.push_back(x+z);
  ideal.push_back(y+z);

  FGLMStrategy strat(ring, other, ideal);

  ideal = strat.main();

  output_test_stream output;

  std::copy(ideal.begin(), ideal.end(), 
	    std::ostream_iterator<BoolePolynomial>(output, ", ") );
  BOOST_CHECK(output.is_equal("y + x, z + x, "));

}

/// @note It is intended, that the following two tests do not compile!
#ifdef FGLMStrategyTest_compile_all_
BOOST_AUTO_TEST_CASE(test_copy_constructor) {
  BoolePolyRing other(ring.clone());
  other.changeOrdering(CTypes::dp_asc);

  PolynomialVector ideal;
  ideal.push_back(x+z);
  ideal.push_back(y+z);

  FGLMStrategy strat0(ring, other, ideal);
  FGLMStrategy strat(strat0);
  ideal = strat.main();

  output_test_stream output;

  std::copy(ideal.begin(), ideal.end(), 
	    std::ostream_iterator<BoolePolynomial>(output, ", ") );
  BOOST_CHECK(output.is_equal("y + x, z + x, "));
}

BOOST_AUTO_TEST_CASE(test_assign) {
  BoolePolyRing empty(1);
  FGLMStrategy strat(empty, empty, PolynomialVector());

  BoolePolyRing other(ring.clone());
  other.changeOrdering(CTypes::dp_asc);

  PolynomialVector ideal;
  ideal.push_back(x+z);
  ideal.push_back(y+z);

  strat = FGLMStrategy(ring, other, ideal);

  ideal = strat.main();

  output_test_stream output;

  std::copy(ideal.begin(), ideal.end(), 
	    std::ostream_iterator<BoolePolynomial>(output, ", ") );
  BOOST_CHECK(output.is_equal("y + x, z + x, "));
}
#endif

BOOST_AUTO_TEST_SUITE_END()
