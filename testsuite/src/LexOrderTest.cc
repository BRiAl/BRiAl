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
  typedef LexOrder order_type;
  Flex(): ring(5) { BOOST_TEST_MESSAGE( "setup fixture" ); }
  ~Flex() { BOOST_TEST_MESSAGE( "teardown fixture" ); }

  BoolePolyRing ring;
};

BOOST_FIXTURE_TEST_SUITE(LexOrderTestSuite, Flex )

BOOST_AUTO_TEST_CASE(test) {

  BOOST_TEST_MESSAGE( "test" );
  order_type order();
}

BOOST_AUTO_TEST_CASE(test_properties) {

  order_type order;
  BOOST_TEST_MESSAGE( "isLexicographical, isSymmetric, isDegreeOrder, isBlockOrder, isTotalDegreeOrder, isDegreeReverseLexicograpical, ascendingVariables, descendingVariables, orderedStandardIteration" );
  BOOST_CHECK(order.isLexicographical());
  BOOST_CHECK(order.isSymmetric());
  BOOST_CHECK(!order.isDegreeOrder());
  BOOST_CHECK(!order.isBlockOrder());
  BOOST_CHECK(!order.isTotalDegreeOrder());
  BOOST_CHECK(!order.isDegreeReverseLexicograpical());///Typo in the function name
  BOOST_CHECK(!order.ascendingVariables());
  BOOST_CHECK(order.descendingVariables());
  BOOST_CHECK(order.orderedStandardIteration());
}

BOOST_AUTO_TEST_CASE(test_getters) {

  order_type order;
  BOOST_TEST_MESSAGE( "getOrderCode, getBaseOrderCode" );
  BOOST_CHECK_EQUAL(order.getOrderCode(), COrderEnums::lp);
  BOOST_CHECK_EQUAL(order.getBaseOrderCode(), COrderEnums::lp);
}

BOOST_AUTO_TEST_SUITE_END()