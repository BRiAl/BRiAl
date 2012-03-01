// -*- c++ -*-
//*****************************************************************************
/** @file PseudoLongProductTest.cc
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

#include <polybori/groebner/PseudoLongProduct.h>
#include <polybori/groebner/PseudoLongLong.h>
#include <polybori/groebner/Long64From32BitsPair.h>
USING_NAMESPACE_PBORI
USING_NAMESPACE_PBORIGB

struct Fdelayedlongprod {
  typedef unsigned long long_type;
  unsigned nhalf;
  unsigned nbits;
  Fdelayedlongprod(): nhalf(sizeof(long_type)*4), nbits(sizeof(long_type)*8) {

    BOOST_TEST_MESSAGE( "setup fixture" );
  }

  ~Fdelayedlongprod() { BOOST_TEST_MESSAGE( "teardown fixture" ); }
};


BOOST_FIXTURE_TEST_SUITE(PseudoLongProductTest, Fdelayedlongprod)

BOOST_AUTO_TEST_CASE(test_less) {

  BOOST_TEST_MESSAGE( "operator<..." );

  BOOST_CHECK_EQUAL((PseudoLongProduct(long_type(-1), 
					long_type(-1)) >
		     PseudoLongLong<long_type(-1), long_type(-1)>()), false);

  BOOST_CHECK_EQUAL((PseudoLongProduct(long_type(-1), 
					long_type(-1)) >
		     PseudoLongLong<long_type(-1), 0>()), false);


  BOOST_CHECK_EQUAL((PseudoLongProduct(long_type(-1), 
					long_type(-1)) >
		     PseudoLongLong<long_type(-1)/2, 
		     long_type(-1)>()), true);

  BOOST_CHECK_EQUAL((PseudoLongProduct(long_type(2)<< nhalf,
					long_type(2)<< nhalf) >
		     PseudoLongLong<3,long_type(-1)>()), true);

  BOOST_CHECK_EQUAL((PseudoLongProduct(long_type(2)<< nhalf ,
					long_type(2)<< nhalf ) >
		    PseudoLongLong<4,0>()), false);

  BOOST_CHECK_EQUAL((PseudoLongProduct(long_type(2)<< nhalf,
					long_type(2)<< nhalf )>
		    PseudoLongLong<4,1>()), false);

  BOOST_CHECK_EQUAL((PseudoLongProduct(5, 7) > 34), true);
  BOOST_CHECK_EQUAL((PseudoLongProduct(5, 7) > 35), false);
  BOOST_CHECK_EQUAL((PseudoLongProduct(5, 7) > 36), false);

  BOOST_CHECK_EQUAL((PseudoLongProduct(long_type(5)<<nhalf, 7) >
		     long_type(34)<<nhalf), true);
  BOOST_CHECK_EQUAL((PseudoLongProduct(long_type(5)<<nhalf, 7) >
		     long_type(35)<<nhalf), false);
  BOOST_CHECK_EQUAL((PseudoLongProduct(long_type(5)<<nhalf, 7) >
		     long_type(36)<<nhalf), false);

  BOOST_CHECK_EQUAL((PseudoLongProduct(long_type(5)<<(nhalf - 4), long_type(7)<<(nhalf-4)) >
		     long_type(34)<<(nbits - 8)), true);
  BOOST_CHECK_EQUAL((PseudoLongProduct(long_type(5)<<(nhalf - 4), long_type(7)<<(nhalf-4)) >
		     long_type(35)<<(nbits - 8)), false);
  BOOST_CHECK_EQUAL((PseudoLongProduct(long_type(5)<<(nhalf - 4), long_type(7)<<(nhalf-4)) >
		     long_type(36)<<(nbits - 8)), false);


  BOOST_CHECK_EQUAL((PseudoLongProduct(long_type(3)<<16, long_type(1)<<16) >
		     Long64From32BitsPair<4, 0>::get()), false);
  BOOST_CHECK_EQUAL((PseudoLongProduct(long_type(4)<<16, long_type(1)<<16) >
		     Long64From32BitsPair<4, 0>::get()), false);
  BOOST_CHECK_EQUAL((PseudoLongProduct(long_type(5)<<16, long_type(1)<<16) >
		     Long64From32BitsPair<4, 0>::get()), true);


}

BOOST_AUTO_TEST_SUITE_END()
