// -*- c++ -*-
//*****************************************************************************
/** @file LexOrderTest.cc
 *
 * @author Ket Shcherbakova, Alexander Dreyer
 * @date 2010-11-30
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

#include "LexOrder.h"

USING_NAMESPACE_PBORI

struct Flex {
  Flex(): ring(5) { BOOST_TEST_MESSAGE( "setup fixture" ); }
  ~Flex() { BOOST_TEST_MESSAGE( "teardown fixture" ); }

  BoolePolyRing ring;
};

BOOST_FIXTURE_TEST_SUITE(LexOrderTestSuite, Flex )

BOOST_AUTO_TEST_CASE(test) {

  BOOST_TEST_MESSAGE( "test" );
}

BOOST_AUTO_TEST_SUITE_END()