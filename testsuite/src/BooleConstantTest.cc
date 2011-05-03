// -*- c++ -*-
//*****************************************************************************
/** @file BooleConstantTest.cc
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

#include <polybori/pbori_defs.h>
#include <polybori/BooleConstant.h>

USING_NAMESPACE_PBORI

struct Fconst {
  typedef BooleConstant const_type;
  Fconst() { BOOST_TEST_MESSAGE( "setup fixture" ); }
  ~Fconst() { BOOST_TEST_MESSAGE( "teardown fixture" ); }
};

BOOST_FIXTURE_TEST_SUITE(BooleConstantTestSuite, Fconst )

BOOST_AUTO_TEST_CASE(test_constructors) {

  BOOST_TEST_MESSAGE( "Constructors..." );
  BOOST_CHECK_EQUAL(const_type(), const_type(false));
  BOOST_CHECK_EQUAL((bool)const_type(false), false);
  BOOST_CHECK_EQUAL((bool)const_type(true), true);
  BOOST_CHECK_EQUAL(const_type(21), const_type(true));
  BOOST_CHECK_EQUAL(const_type(-4), const_type(false));
}

BOOST_AUTO_TEST_CASE(test_operators) {

  const_type c21(21);
  const_type c_4(-4);
  const_type ctrue(true);
  const_type cfalse(false);

  BOOST_TEST_MESSAGE( "bool, !, <<" );
  BOOST_CHECK_EQUAL((bool)c21, true);
  BOOST_CHECK_EQUAL((bool)c_4, false);
  BOOST_CHECK_EQUAL((bool)ctrue, true);
  BOOST_CHECK_EQUAL((bool)cfalse, false);
  BOOST_CHECK_EQUAL(!c21, false);
  BOOST_CHECK_EQUAL(!c_4, true);
  BOOST_CHECK_EQUAL(!ctrue, false);
  BOOST_CHECK_EQUAL(!cfalse, true);
  output_test_stream output;
  output << c21;
  BOOST_CHECK(output.is_equal("1"));
  output << c_4;
  BOOST_CHECK(output.is_equal("0"));
  output << ctrue;
  BOOST_CHECK(output.is_equal("1"));
  output << cfalse;
  BOOST_CHECK(output.is_equal("0"));
}

BOOST_AUTO_TEST_SUITE_END()
