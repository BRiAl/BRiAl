// -*- c++ -*-
//*****************************************************************************
/** @file CWeakPtrFacadeTest.cc
 *
 * @author Alexander Dreyer
 * @date 2011-06-14
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

#include <polybori/pbori_defs.h>
#include <polybori/common/CWeakPtrFacade.h>

USING_NAMESPACE_PBORI

struct FWeakptrFac {
   FWeakptrFac() { BOOST_TEST_MESSAGE( "setup fixture" ); }
  ~FWeakptrFac() { BOOST_TEST_MESSAGE( "teardown fixture" ); }

};

BOOST_FIXTURE_TEST_SUITE(CWeakPtrFacadeTestSuite, FWeakptrFac)

BOOST_AUTO_TEST_CASE(test_construction) {

  BOOST_TEST_MESSAGE( "constructors/destructors..." );

  CWeakPtrFacade ptrbase;
  BOOST_CHECK_EQUAL(*ptrbase.get(), &ptrbase);

  CWeakPtrFacade* ptrbase1 = new CWeakPtrFacade;
  CWeakPtrFacade::ptr_type p_data = ptrbase1->get();

  BOOST_CHECK_EQUAL(*p_data, ptrbase1);
  delete ptrbase1;

  BOOST_CHECK_EQUAL(*p_data, (CWeakPtrFacade*)NULL);
}

BOOST_AUTO_TEST_SUITE_END()



