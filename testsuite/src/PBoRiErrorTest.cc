// -*- c++ -*-
//*****************************************************************************
/** @file PBoRiErrorTest.cc
 *
 * @author Ket Kalda, Alexander Dreyer
 * @date 2011-02-08
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

#include <polybori/except/PBoRiGenericError.h>
#include <polybori/except/CErrorInfo.h>

USING_NAMESPACE_PBORI

struct Ferror {
   Ferror() { BOOST_TEST_MESSAGE( "setup fixture" ); }
   ~Ferror() { BOOST_TEST_MESSAGE( "teardown fixture" ); }
};

BOOST_FIXTURE_TEST_SUITE(PBoRiErrorTestSuite, Ferror )

BOOST_AUTO_TEST_CASE(test_text) {

  BOOST_TEST_MESSAGE("text");

  for(unsigned i=0; i<=CTypes::last_error; i++) {
    try { throw PBoRiError(i); }
    catch (PBoRiError e) {
      BOOST_CHECK_EQUAL(e.text(), CErrorInfo::text(i));
    }
  }

  try { throw PBoRiError(-1); }
  catch (PBoRiError e) {
    BOOST_CHECK_EQUAL(e.text(), CErrorInfo::text(-1));
  }
}

BOOST_AUTO_TEST_CASE(test_what) {

  BOOST_TEST_MESSAGE("what");

  for(unsigned i=0; i<=CTypes::last_error; i++) {
    try { throw PBoRiError(i); }
    catch (PBoRiError e) {
      BOOST_CHECK_EQUAL(e.what(), CErrorInfo::text(i));
    }
  }

  try { throw PBoRiError(-1); }
  catch (PBoRiError e) {
    BOOST_CHECK_EQUAL(e.what(), CErrorInfo::text(-1));
  }
}

BOOST_AUTO_TEST_CASE(test_code) {

  BOOST_TEST_MESSAGE("code");

  for(unsigned i=0; i<=CTypes::last_error; i++) {
    try { throw PBoRiError(i); }
    catch (PBoRiError e) {
      BOOST_CHECK_EQUAL(e.code(), i);
    }
  }

  try { throw PBoRiError(-1); }
  catch (PBoRiError e) {
    BOOST_CHECK_EQUAL(e.code(), -1);
  }
}
BOOST_AUTO_TEST_SUITE_END()
