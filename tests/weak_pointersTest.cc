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
#include <polybori.h>

USING_NAMESPACE_PBORI

struct FWeakptrFac {
   FWeakptrFac() { BOOST_TEST_MESSAGE( "setup fixture" ); }
  ~FWeakptrFac() { BOOST_TEST_MESSAGE( "teardown fixture" ); }

};


class uses_weakptr :
  public CWeakPtrFacade<uses_weakptr> {
public:
  uses_weakptr(int data = 17): CWeakPtrFacade<uses_weakptr>(), m_data(data) {}

  int value() const { return m_data; }

  uses_weakptr* internal_ptr() const {
    return *CWeakPtrFacade<uses_weakptr>::m_data;
  }
private:
  int m_data;
};


BOOST_FIXTURE_TEST_SUITE(CWeakPtrFacadeTestSuite, FWeakptrFac)


BOOST_AUTO_TEST_CASE(test_construction) {

  BOOST_TEST_MESSAGE( "constructors/destructors..." );
  uses_weakptr* ptrinstance = new uses_weakptr(11);
  CWeakPtr<uses_weakptr> ptrbase(*ptrinstance);

  BOOST_CHECK_EQUAL(ptrinstance->internal_ptr(), ptrinstance);
  BOOST_CHECK_EQUAL(ptrbase.operator->(), ptrinstance);

  CWeakPtr<uses_weakptr> outercopy(ptrbase);
  BOOST_CHECK_EQUAL(outercopy.operator->(), ptrinstance);

  uses_weakptr innercopy(*ptrinstance);
  BOOST_CHECK_EQUAL(innercopy.internal_ptr(), &innercopy);
  BOOST_CHECK_NE(innercopy.internal_ptr(), ptrinstance->internal_ptr());

  delete ptrinstance;
  BOOST_CHECK_EQUAL(ptrbase, false);
  BOOST_CHECK_EQUAL(outercopy, false);
  BOOST_CHECK_THROW(ptrbase.operator->(), std::exception);
  BOOST_CHECK_THROW(outercopy.operator->(), std::exception);
}

BOOST_AUTO_TEST_CASE(test_operations) {

  BOOST_TEST_MESSAGE( "pointer/deref/bool operation..." );

  uses_weakptr ptrinstance(19);
  BOOST_CHECK_EQUAL(ptrinstance.value(), 19);  //  check whether class does
                                               //  something useful

  CWeakPtr<uses_weakptr> ptrbase(ptrinstance);

  BOOST_CHECK_EQUAL(ptrbase.operator->(), &ptrinstance);

  uses_weakptr* ptrinst1 = new uses_weakptr(13);
  CWeakPtr<uses_weakptr> ptrbase1(*ptrinst1);
  BOOST_CHECK_EQUAL(ptrbase1.operator->(), ptrinst1);
  BOOST_CHECK_EQUAL(ptrbase1->value(), 13);
  BOOST_CHECK_EQUAL(&(*ptrbase1), ptrinst1);
  BOOST_CHECK_EQUAL((*ptrbase1).value(), 13);

  delete ptrinst1;
  BOOST_CHECK_EQUAL(ptrbase1, false);
  BOOST_CHECK_THROW(ptrbase1.operator->(), std::exception);
  BOOST_CHECK_THROW(*ptrbase1, std::exception);

  ptrinst1 = new uses_weakptr(23);
  ptrbase1 =  CWeakPtr<uses_weakptr>(*ptrinst1);

  BOOST_CHECK_EQUAL(ptrbase1.operator->(), ptrinst1);

  uses_weakptr newinst(*ptrinst1);
  delete ptrinst1;


  BOOST_CHECK_EQUAL(newinst.value(), 23);


}


BOOST_AUTO_TEST_CASE(test_ring) {

  BOOST_TEST_MESSAGE( "Primary usability check..." );

  BoolePolyRing ring(17);
  WeakRingPtr weakptr(ring);
  BoolePolyRing ringnew(*weakptr);
  BOOST_CHECK_EQUAL(ring.id(), ringnew.id());


  BOOST_TEST_MESSAGE( "we can get a strong reference..." );
  BoolePolyRing* pring = new BoolePolyRing(17);

  weakptr = WeakRingPtr(*pring);
  ringnew = BoolePolyRing(*weakptr);
  long store_id = pring->id();
  BOOST_CHECK_EQUAL(store_id, ringnew.id());
  delete pring;
  BOOST_CHECK_EQUAL(store_id, ringnew.id());

  BOOST_TEST_MESSAGE( "a weak reference throws if invalid..." );
  pring = new BoolePolyRing(17);
  weakptr = WeakRingPtr(*pring);
  store_id = pring->id();

  BOOST_CHECK_EQUAL(store_id, BoolePolyRing(*weakptr).id());
  delete pring;

  BOOST_CHECK_EQUAL(weakptr, false);
  BOOST_CHECK_THROW((*weakptr), std::exception);
}


BOOST_AUTO_TEST_SUITE_END()



