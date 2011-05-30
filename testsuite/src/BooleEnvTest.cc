// -*- c++ -*-
//*****************************************************************************
/** @file BooleEnvTest.cc
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

#if 0
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp> 
using boost::test_tools::output_test_stream;

#include <polybori/pbori_defs.h>
#include <polybori/BooleEnv.h>
#include <polybori/orderings/pbori_order.h>

USING_NAMESPACE_PBORI

struct Fenv {
   Fenv(): ring(5) { BOOST_TEST_MESSAGE( "setup fixture" ); }
  ~Fenv() { BOOST_TEST_MESSAGE( "teardown fixture" ); }

  BoolePolyRing ring;
};

BOOST_FIXTURE_TEST_SUITE(BooleEnvTestSuite, Fenv )

BOOST_AUTO_TEST_CASE(test_dd_type) {

  BOOST_TEST_MESSAGE( "variable, zero, one" );
  BOOST_CHECK_THROW(BooleEnv::variable(-1), PBoRiError);
  BOOST_CHECK_EQUAL(BooleEnv::variable(0), ring.variable(0));
  BOOST_CHECK_EQUAL(BooleEnv::variable(1), ring.variable(1));
  BOOST_CHECK_EQUAL(BooleEnv::variable(2), ring.variable(2));
  BOOST_CHECK_EQUAL(BooleEnv::variable(3), ring.variable(3));
  BOOST_CHECK_EQUAL(BooleEnv::variable(4), ring.variable(4));
  BOOST_CHECK_THROW(BooleEnv::variable(5), PBoRiError);
  BOOST_CHECK_EQUAL(BooleEnv::zero(), ring.zero());
  BOOST_CHECK_EQUAL(BooleEnv::one(), ring.one());
}

BOOST_AUTO_TEST_CASE(test_variables) {

  BOOST_TEST_MESSAGE( "nVariables, setVariableName, getVariableName" );
  BOOST_CHECK_EQUAL(BooleEnv::nVariables(), ring.nVariables());
  BooleEnv::setVariableName(0, "x");
  BOOST_CHECK_EQUAL(BooleEnv::getVariableName(0), ring.getVariableName(0));
  BOOST_CHECK_EQUAL(BooleEnv::getVariableName(0), "x");
  ring.setVariableName(1, "y");
  BOOST_CHECK_EQUAL(BooleEnv::getVariableName(1), ring.getVariableName(1));
  BOOST_CHECK_EQUAL(BooleEnv::getVariableName(1), "y");
  BooleEnv::setVariableName(2, "z");
  BOOST_CHECK_EQUAL(BooleEnv::getVariableName(2), ring.getVariableName(2));
  BOOST_CHECK_EQUAL(BooleEnv::getVariableName(2), "z");
  ring.setVariableName(3, "v");
  BOOST_CHECK_EQUAL(BooleEnv::getVariableName(3), ring.getVariableName(3));
  BOOST_CHECK_EQUAL(BooleEnv::getVariableName(3), "v");
  BooleEnv::setVariableName(4, "w");
  BOOST_CHECK_EQUAL(BooleEnv::getVariableName(4), ring.getVariableName(4));
  BOOST_CHECK_EQUAL(BooleEnv::getVariableName(4), "w");
  BooleEnv::setVariableName(5, "t");
  BOOST_CHECK_EQUAL(BooleEnv::getVariableName(5), ring.getVariableName(5));
  BOOST_CHECK_EQUAL(BooleEnv::getVariableName(5), "t");


  BOOST_CHECK_THROW( BooleEnv::setVariableName(-1, "r"), std::exception); 

  BOOST_CHECK_THROW(BooleEnv::getVariableName(-1), std::exception);
  BOOST_CHECK_THROW(BooleEnv::getVariableName(-1), std::exception);
}

BOOST_AUTO_TEST_CASE(test_ordering) {

  BOOST_TEST_MESSAGE( "ordering, getOrderCode, getBaseOrderCode, changeOrdering" );
  //  BOOST_CHECK_EQUAL(BooleEnv::ordering().getOrderCode(), ring.ordering().getOrderCode());
  BOOST_CHECK_EQUAL(BooleEnv::getOrderCode(), ring.ordering().getOrderCode());
  BOOST_CHECK_EQUAL(BooleEnv::getBaseOrderCode(), ring.ordering().getBaseOrderCode());
  BooleEnv::changeOrdering(COrderEnums::dlex);
  //  BOOST_CHECK_EQUAL(BooleEnv::ordering().getOrderCode(), COrderEnums::dlex);
  BOOST_CHECK_EQUAL(BooleEnv::getOrderCode(), ring.ordering().getOrderCode());
  ring.changeOrdering(COrderEnums::dp_asc);
  //  BOOST_CHECK_EQUAL(BooleEnv::ordering().getOrderCode(), COrderEnums::dp_asc);
  BOOST_CHECK_EQUAL(BooleEnv::getOrderCode(), ring.ordering().getOrderCode());
  BooleEnv::changeOrdering(COrderEnums::block_dlex);
  //  BOOST_CHECK_EQUAL(BooleEnv::ordering().getOrderCode(), COrderEnums::block_dlex);
  BOOST_CHECK_EQUAL(BooleEnv::getOrderCode(), ring.ordering().getOrderCode());
  ring.changeOrdering(COrderEnums::block_dp_asc);
  //  BOOST_CHECK_EQUAL(BooleEnv::ordering().getOrderCode(), COrderEnums::block_dp_asc);
  BOOST_CHECK_EQUAL(BooleEnv::getOrderCode(), ring.ordering().getOrderCode());
  BooleEnv::changeOrdering(-1);
  //  BOOST_CHECK_EQUAL(BooleEnv::ordering().getOrderCode(), COrderEnums::lp);//default is lp
  BOOST_CHECK_EQUAL(BooleEnv::getOrderCode(), ring.ordering().getOrderCode());
  BooleEnv::changeOrdering(5);
  //  BOOST_CHECK_EQUAL(BooleEnv::ordering().getOrderCode(), COrderEnums::lp);//default is lp
  BOOST_CHECK_EQUAL(BooleEnv::getOrderCode(), ring.ordering().getOrderCode());
}

BOOST_AUTO_TEST_CASE(test_ring) {
  ///@todo implement == for rings?
  BoolePolyRing notempty(3, 1, false);
  BoolePolyRing empty(0,2,false);

//   BOOST_TEST_MESSAGE( "ring, set" );
//   BOOST_CHECK_EQUAL(BooleEnv::ring().ordering().getOrderCode(), ring.ordering().getOrderCode());
//   BOOST_CHECK_EQUAL(BooleEnv::ring().nVariables(), ring.nVariables());
//   BooleEnv::set(notempty);
//   BOOST_CHECK_EQUAL(BooleEnv::ring().ordering().getOrderCode(), notempty.ordering().getOrderCode());
//   BOOST_CHECK_EQUAL(BooleEnv::ring().nVariables(), notempty.nVariables());
//   BooleEnv::set(empty);
//   BOOST_CHECK_EQUAL(BooleEnv::ring().ordering().getOrderCode(), empty.ordering().getOrderCode());
//   BOOST_CHECK_EQUAL(BooleEnv::ring().nVariables(), empty.nVariables());
//   BooleEnv::set(ring);
//   BOOST_CHECK_EQUAL(BooleEnv::ring().ordering().getOrderCode(), ring.ordering().getOrderCode());
//   BOOST_CHECK_EQUAL(BooleEnv::ring().nVariables(), ring.nVariables());
}

BOOST_AUTO_TEST_CASE(test_blocks) {

  BOOST_TEST_MESSAGE( "blockBegin, blockEnd, appendBlock, clearBlocks, lastBlockStart" );
  BOOST_CHECK_EQUAL(BooleEnv::lastBlockStart(), ring.ordering().lastBlockStart());
  BOOST_CHECK_EQUAL(BooleEnv::lastBlockStart(), CUDD_MAXINDEX);
  output_test_stream output, output_ring;
  BooleEnv::changeOrdering(COrderEnums::block_dlex);
  BOOST_CHECK_THROW(BooleEnv::appendBlock(0), std::exception);
  BooleEnv::appendBlock(2);
  BoolePolyRing::block_iterator start(BooleEnv::blockBegin()), 
                                finish(BooleEnv::blockEnd());
  while (start != finish) {
    output << *start <<", ";
    ++start;
  }
  start = ring.ordering().blockBegin();
  finish = ring.ordering().blockEnd();
  while (start != finish) {
    output_ring << *start <<", ";
    ++start;
  }
  BOOST_CHECK_EQUAL(output.str(),output_ring.str());
  output_ring.str("");
  output_ring << "2, " << CUDD_MAXINDEX << ", ";
  BOOST_CHECK_EQUAL(output.str(), output_ring.str());

  BooleEnv::clearBlocks();
  start = BooleEnv::blockBegin();
  finish = BooleEnv::blockEnd();
  output.str("");
  while (start != finish) {
    output << *start <<", ";
    ++start;
  }
  start = ring.ordering().blockBegin();
  finish = ring.ordering().blockEnd();
  output_ring.str("");
  while (start != finish) {
    output_ring << *start <<", ";
    ++start;
  }
  BOOST_CHECK_EQUAL(output.str(), output_ring.str());
  output_ring.str("");
  output_ring << CUDD_MAXINDEX << ", ";
  BOOST_CHECK_EQUAL(output.str(), output_ring.str());
}

BOOST_AUTO_TEST_SUITE_END()


#endif
