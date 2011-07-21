// -*- c++ -*-
//*****************************************************************************
/** @file LexOrderTest.cc
 *
 * @author Ket Shcherbakova, Alexander Dreyer
 * @date 2010-11-30
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

#include <polybori/LexOrder.h>

USING_NAMESPACE_PBORI


struct Flex {
  typedef LexOrder order_type;
  Flex(const BoolePolyRing& input_ring = 
       BoolePolyRing(1000, COrderEnums::dlex)):  
    ring(input_ring),
    x(0, input_ring), y(1, input_ring), z(2, input_ring),
    v(3, input_ring), w(4, input_ring) {

    BOOST_TEST_MESSAGE( "setup fixture" );
    ring.setVariableName(0, "x");
    ring.setVariableName(1, "y");
    ring.setVariableName(2, "z");
    ring.setVariableName(3, "v");
    ring.setVariableName(4, "w");
  }
  ~Flex() { BOOST_TEST_MESSAGE( "teardown fixture" ); }

  BoolePolyRing ring;
  BooleVariable x, y, z, v, w;
};

BOOST_FIXTURE_TEST_SUITE(LexOrderTestSuite, Flex )

BOOST_AUTO_TEST_CASE(test_properties) {

  order_type order;
  BOOST_TEST_MESSAGE( "isLexicographical, isSymmetric, isDegreeOrder, isBlockOrder, isTotalDegreeOrder, isDegreeReverseLexicographical, ascendingVariables, descendingVariables, orderedStandardIteration" );
  BOOST_CHECK(order.isLexicographical());
  BOOST_CHECK(order.isSymmetric());
  BOOST_CHECK(!order.isDegreeOrder());
  BOOST_CHECK(!order.isBlockOrder());
  BOOST_CHECK(!order.isTotalDegreeOrder());
  BOOST_CHECK(!order.isDegreeReverseLexicographical());
  BOOST_CHECK(!order.ascendingVariables());
  BOOST_CHECK(order.descendingVariables());
  BOOST_CHECK(order.orderedStandardIteration());
}

BOOST_AUTO_TEST_CASE(test_getters) {

  order_type order;
  BOOST_TEST_MESSAGE( "getOrderCode, getBaseOrderCode" );
  BOOST_CHECK_EQUAL(order.getOrderCode(), COrderEnums::lp);
  BOOST_CHECK_EQUAL(order.getBaseOrderCode(), COrderEnums::lp);
}

BOOST_AUTO_TEST_CASE(test_compare) {

  order_type order;
  BOOST_TEST_MESSAGE( "compare" );
  BooleMonomial monom1 = x;
  BooleMonomial monom2 = x*x;
  BOOST_CHECK_EQUAL(order.compare(monom1, monom2) , CTypes::equality);
  monom1 = y*z*v;
  monom2 = x*y;
  BOOST_CHECK_EQUAL(order.compare(monom1, monom2) , CTypes::less_than);
  monom1 = x*y*z;
  monom2 = x*y;
  BOOST_CHECK_EQUAL(order.compare(monom1, monom2) , CTypes::greater_than);
  monom1 = BooleMonomial(ring);
  monom2 = w;
  BOOST_CHECK_EQUAL(order.compare(monom1, monom2) , CTypes::less_than);
  monom1 = BooleMonomial(ring);
  monom2 = BooleMonomial(ring);
  BOOST_CHECK_EQUAL(order.compare(monom1, monom2) , CTypes::equality);
  BooleExponent exp1(x);
  BooleExponent exp2(x*x);
  BOOST_CHECK_EQUAL(order.compare(exp1, exp2) , CTypes::equality);
  exp1 = BooleExponent(w*x);
  exp2 = BooleExponent(v*x);
  BOOST_CHECK_EQUAL(order.compare(exp1, exp2) , CTypes::less_than);
  exp1 = BooleExponent(x*y*z*v*w);
  exp2 = BooleExponent(x*y*z*v);
  BOOST_CHECK_EQUAL(order.compare(exp1, exp2) , CTypes::greater_than);
  BOOST_CHECK_EQUAL(order.compare(0,1) , CTypes::greater_than);
  BOOST_CHECK_EQUAL(order.compare(2,1) , CTypes::less_than);
  BOOST_CHECK_EQUAL(order.compare(-1,-1) , CTypes::equality);
  BOOST_CHECK_EQUAL(order.compare(1,-1) , CTypes::less_than);
}

BOOST_AUTO_TEST_CASE(test_lead) {

  BOOST_TEST_MESSAGE( "lead, leadExp, leadFirst" );
  order_type order;
  BoolePolynomial poly = x*x + x*y + y*z*v*w + 1;
  BOOST_CHECK_EQUAL(order.lead(poly)    , BooleMonomial(x*y));
  BOOST_CHECK_EQUAL(order.lead(poly,1)  , BooleMonomial(x*y));
  BOOST_CHECK_EQUAL(order.lead(poly,-1) , BooleMonomial(x*y));
  BOOST_CHECK_EQUAL(order.leadExp(poly)    , BooleExponent(x*y));
  BOOST_CHECK_EQUAL(order.leadExp(poly,1)  , BooleExponent(x*y));
  BOOST_CHECK_EQUAL(order.leadExp(poly,-1) , BooleExponent(x*y));
  BOOST_CHECK_EQUAL(order.leadFirst(poly)  , poly);
  poly = BoolePolynomial(ring);
  BOOST_CHECK_THROW(order.lead(poly), PBoRiGenericError<CTypes::illegal_on_zero>);
  BOOST_CHECK_THROW(order.lead(poly,1),PBoRiGenericError<CTypes::illegal_on_zero>);
  BOOST_CHECK_THROW(order.lead(poly),PBoRiGenericError<CTypes::illegal_on_zero>);
  BOOST_CHECK_EQUAL(order.leadExp(poly,1), BooleExponent());
  BOOST_CHECK_EQUAL(order.leadExp(poly), BooleExponent());
  BOOST_CHECK_EQUAL(order.leadFirst(poly), poly);
  poly = 1;
  BOOST_CHECK_EQUAL(order.lead(poly, 1), BooleMonomial(ring));
  BOOST_CHECK_EQUAL(order.lead(poly), BooleMonomial(ring));
  BOOST_CHECK_EQUAL(order.leadExp(poly, 1), BooleExponent());
  BOOST_CHECK_EQUAL(order.leadExp(poly), BooleExponent());
  BOOST_CHECK_EQUAL(order.leadFirst(poly), poly);
}

BOOST_AUTO_TEST_CASE(test_lead_iter) {

  order_type order;
  BoolePolynomial poly = x*x + x*y + y*z*v*w + 1;

  BOOST_TEST_MESSAGE( "leadIteratorBegin" );
  order_type::ordered_iterator start(order.leadIteratorBegin(poly)), finish(order.leadIteratorEnd(poly));
  start.increment();
}

BOOST_AUTO_TEST_CASE(test_blocks) {

  BOOST_TEST_MESSAGE( "blockBegin, blockEnd, appendBlock, clearBlocks, lastBlockStart, lieInSameBlock" );
  order_type order;
  output_test_stream output;
  BoolePolyRing::block_iterator start(order.blockBegin()),finish(order.blockEnd());

  BOOST_CHECK(start == finish);
  BOOST_CHECK_THROW(order.appendBlock(-1), std::exception);
  order.appendBlock(0);
  order.appendBlock(2);
  order.appendBlock(6);
  start = order.blockBegin();
  finish = order.blockEnd();
  BOOST_CHECK(start == finish);

  BOOST_CHECK(order.lieInSameBlock(0,1));
  BOOST_CHECK(order.lieInSameBlock(-1,4));
  BOOST_CHECK_EQUAL(order.lastBlockStart(), CUDD_MAXINDEX);
  order.clearBlocks();
  start = order.blockBegin();
  finish = order.blockEnd();
  BOOST_CHECK(start==finish);
}



BOOST_AUTO_TEST_CASE(test_cover_constructors_and_destructors) {
  int order_code = CTypes::lp;
  BoolePolyRing block_ring(5, order_code);

  BOOST_CHECK_EQUAL(block_ring.ordering().getOrderCode(), order_code);

  class Inherited: public order_type {};

  Inherited dummy;
  BOOST_CHECK_EQUAL(dummy.getOrderCode(), order_code);

  order_type self;
  BOOST_CHECK_EQUAL(self.getOrderCode(), order_code);

  order_type* pSelf = new order_type;
  BOOST_CHECK_EQUAL(pSelf->getOrderCode(), order_code);
  delete pSelf;

  pSelf = new Inherited;
  BOOST_CHECK_EQUAL(pSelf->getOrderCode(), order_code);
  delete pSelf;

  Inherited* pDummy = new Inherited;
  BOOST_CHECK_EQUAL(pDummy->getOrderCode(), order_code);
  delete pDummy;
}

BOOST_AUTO_TEST_SUITE_END()
