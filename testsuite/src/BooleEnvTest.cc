// -*- c++ -*-
//*****************************************************************************
/** @file BooleEnvTest.cc
 *
 * @author Ket Kalda, Alexander Dreyer
 * @date 2010-11-02
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
#include "BooleEnv.h"

USING_NAMESPACE_PBORI

struct Fenv {
  Fenv() { BOOST_TEST_MESSAGE( "setup fixture" ); }
  ~Fenv() { BOOST_TEST_MESSAGE( "teardown fixture" ); }
};

BOOST_FIXTURE_TEST_SUITE(BooleEnvTestSuite, Fenv )

BOOST_AUTO_TEST_CASE(test_constructors) { }

BOOST_AUTO_TEST_SUITE_END()
