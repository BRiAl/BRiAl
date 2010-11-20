// -*- c++ -*-
//*****************************************************************************
/** @file SkeletonTest.cc
 *
 * @author 
 * @date 
 *
 * boost/test-driven unit test
 * 
 * @par Copyright:
 *   (c) by The PolyBoRi Team
 *
**/
//*****************************************************************************

#define BOOST_TEST_MODULE SkeletonTest
#include <boost/test/included/unit_test.hpp>

#include "pbori_def.h"
USING_NAMESPACE_PBORI

struct F {
    F() : i( 0 ) { BOOST_TEST_MESSAGE( "setup fixture" ); }
    ~F()         { BOOST_TEST_MESSAGE( "teardown fixture" ); }

    int i;
};

BOOST_FIXTURE_TEST_CASE( test_case1, F )
{
    BOOST_CHECK( i == 1 );
    ++i;
}

BOOST_FIXTURE_TEST_CASE( test_case2, F )
{
    BOOST_CHECK_EQUAL( i, 1 );
}

BOOST_AUTO_TEST_CASE( test_case3 )
{
    BOOST_CHECK( true );
}
