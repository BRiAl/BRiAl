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
  Fexp() {
    BOOST_TEST_MESSAGE( "setup fixture" );
  }

  ~Fexp() { BOOST_TEST_MESSAGE( "teardown fixture" ); }
};

BOOST_FIXTURE_TEST_SUITE(BooleExponentTestSuite, Fexp )

BOOST_AUTO_TEST_CASE(test_constructors) {

  BOOST_TEST_MESSAGE( "Constant exponents..." );
}

BOOST_AUTO_TEST_SUITE_END()
