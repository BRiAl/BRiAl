// -*- c++ -*-
//*****************************************************************************
/** @file BoolePolyRingTest.cc
 *
 * @author Ket Kalda, Alexander Dreyer
 * @date 2010-10-27
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
#include "BoolePolyRing.h"
#include "BooleVariable.h"

USING_NAMESPACE_PBORI

struct Fring {
  typedef BoolePolyRing ring_type;
  Fring() {
    x = (BooleVariable(0));
    y = (BooleVariable(1));
    z = (BooleVariable(2));
    v = (BooleVariable(3));
    w = (BooleVariable(4));
    BOOST_TEST_MESSAGE( "setup fixture" );
  }

  ~Fring() { BOOST_TEST_MESSAGE( "teardown fixture" ); }

  BooleVariable x, y, z, v, w;
};

BOOST_FIXTURE_TEST_SUITE(BoolePolyRingTestSuite, Fring )

BOOST_AUTO_TEST_CASE(test_constructors) {

  BOOST_TEST_MESSAGE( "Constant rings..." );
}


BOOST_AUTO_TEST_SUITE_END()
