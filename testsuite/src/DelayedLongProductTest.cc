// -*- c++ -*-
//*****************************************************************************
/** @file DelayedLongProductTest.cc
 *
 * @author Alexander Dreyer
 * @date 2012-02-24
 *
 * boost/test-driven unit test
 * 
 * @par Copyright:
 *   (c) 2012 by The PolyBoRi Team
 *
 **/
//*****************************************************************************


#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp> 
using boost::test_tools::output_test_stream;

#include <polybori/groebner/DelayedLongProduct.h>
#include <polybori/groebner/PseudoLongLong.h>
#include <polybori/groebner/Long64From32BitsPair.h>
USING_NAMESPACE_PBORI
USING_NAMESPACE_PBORIGB

struct Fdelayedlongprod {
  typedef unsigned long long_type;
  unsigned nhalf;
  Fdelayedlongprod(): nhalf(sizeof(long_type)*4) {

    BOOST_TEST_MESSAGE( "setup fixture" );
  }

  ~Fdelayedlongprod() { BOOST_TEST_MESSAGE( "teardown fixture" ); }
};


BOOST_FIXTURE_TEST_SUITE(DelayedLongProductTest, Fdelayedlongprod)

BOOST_AUTO_TEST_CASE(test_less) {

  BOOST_TEST_MESSAGE( "operator<..." );

  BOOST_CHECK_EQUAL((DelayedLongProduct((unsigned long)(-1), 
					(unsigned long)(-1)) >
		     PseudoLongLong<(unsigned long)(-1), (unsigned long)(-1)>()), true);

  BOOST_CHECK_EQUAL((DelayedLongProduct((unsigned long)(-1), 
					(unsigned long)(-1)) >
		     PseudoLongLong<(unsigned long)(-1), 0>()), false);

  BOOST_CHECK_EQUAL((DelayedLongProduct((unsigned long)(-1), 
					(unsigned long)(-1)) >
		     PseudoLongLong<(unsigned long)(-1)/2, 
		     (unsigned long)(-1)>()), true);

  BOOST_CHECK_EQUAL((DelayedLongProduct((unsigned long)(2)<< nhalf,
					(unsigned long)(2)<< nhalf) >
		     PseudoLongLong<3,(unsigned long)(-1)>()), true);

  BOOST_CHECK_EQUAL((DelayedLongProduct((unsigned long)(2)<< nhalf ,
					(unsigned long)(2)<< nhalf ) >
		    PseudoLongLong<4,0>()), false);

  BOOST_CHECK_EQUAL((DelayedLongProduct((unsigned long)(2)<< nhalf,
					(unsigned long)(2)<< nhalf )>
		    PseudoLongLong<4,1>()), false);

  BOOST_CHECK_EQUAL((DelayedLongProduct(5, 7) > 34), true);
  BOOST_CHECK_EQUAL((DelayedLongProduct(5, 7) > 35), false);
  BOOST_CHECK_EQUAL((DelayedLongProduct(5, 7) > 36), false);

  BOOST_CHECK_EQUAL((DelayedLongProduct(long_type(5)<<nhalf, 7) >
		     long_type(34)<<nhalf), true);
  BOOST_CHECK_EQUAL((DelayedLongProduct(long_type(5)<<nhalf, 7) >
		     long_type(35)<<nhalf), false);
  BOOST_CHECK_EQUAL((DelayedLongProduct(long_type(5)<<nhalf, 7) >
		     long_type(36)<<nhalf), false);

  BOOST_CHECK_EQUAL((DelayedLongProduct(5, long_type(7)<<32) >
		     long_type(34)<<32), true);
  BOOST_CHECK_EQUAL((DelayedLongProduct(5, long_type(7)<<32) >
		     long_type(35)<<32), false);
  BOOST_CHECK_EQUAL((DelayedLongProduct(5, long_type(7)<<32) >
		     long_type(36)<<32), false);

  BOOST_CHECK_EQUAL((DelayedLongProduct(5, long_type(7)<<32) >
		     long_type(36)<<32), false);



}

BOOST_AUTO_TEST_SUITE_END()
