// -*- c++ -*-
//*****************************************************************************
/** @file BooleEnvTest.cc
 *
 * @author Alexander Dreyer
 * @date 2011-05-05
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
#include <polybori/factories/CFactoryBase.h>

BEGIN_NAMESPACE_PBORI

// dummy class for testing inheritance
class CFactoryBaseInherited: 
  public CFactoryBase {
public:
  CFactoryBaseInherited(const CFactoryBase::parent_type& ring):
    CFactoryBase(ring) {}

  CFactoryBaseInherited(const CFactoryBaseInherited& rhs):
    CFactoryBase(rhs) {}

  bool operator==(const CFactoryBase::parent_type& ring) const {
    return parent().hash() == ring.hash();
  }
};

END_NAMESPACE_PBORI

USING_NAMESPACE_PBORI

struct Ffacbase {
  Ffacbase(): ring(5) { BOOST_TEST_MESSAGE( "setup fixture" ); }
  ~Ffacbase() { BOOST_TEST_MESSAGE( "teardown fixture" ); }

  BoolePolyRing ring;
};


BOOST_FIXTURE_TEST_SUITE(CFactoryBaseTest, Ffacbase )

BOOST_AUTO_TEST_CASE(test_constructors) {

  BOOST_TEST_MESSAGE( "CFactoryBase(...)" );

  CFactoryBaseInherited factory(ring);
  BOOST_CHECK(factory == ring);
  CFactoryBaseInherited factory_copy(factory);
  BOOST_CHECK(factory_copy == ring);
}

BOOST_AUTO_TEST_CASE(test_parent) {

  BOOST_TEST_MESSAGE( "parent()" );

  CFactoryBase factory(ring);
  BOOST_CHECK_EQUAL(factory.parent().hash(), ring.hash());
  CFactoryBase factory_copy(factory);
  BOOST_CHECK_EQUAL(factory_copy.parent().hash(), ring.hash());
}
BOOST_AUTO_TEST_SUITE_END()
