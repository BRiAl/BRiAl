// -*- c++ -*-
//*****************************************************************************
/** @file BlockDegLexOrderTest.cc
 *
 * @author Ket Shcherbakova, Alexander Dreyer
 * @date 2010-12-14
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

#include "BlockDegLexOrder.h"

USING_NAMESPACE_PBORI

struct Fblockdeglex {
  typedef BlockDegLexOrder order_type;
  Fblockdeglex():
      ring(5,COrderEnums::lp) {
      x = BooleVariable(0);
      y = BooleVariable(1);
      z = BooleVariable(2);
      v = BooleVariable(3);
      w = BooleVariable(4);
      BOOST_TEST_MESSAGE( "setup fixture" );
      ring.setVariableName(0, "x");
      ring.setVariableName(1, "y");
      ring.setVariableName(2, "z");
      ring.setVariableName(3, "v");
      ring.setVariableName(4, "w");
  }
  ~Fblockdeglex() { BOOST_TEST_MESSAGE( "teardown fixture" ); }

  BoolePolyRing ring;
  BooleVariable x, y, z, v, w;
};

BOOST_FIXTURE_TEST_SUITE(BlockDegLexOrderTestSuite, Fblockdeglex )

BOOST_AUTO_TEST_CASE(test_properties) {

  order_type order;
  BOOST_TEST_MESSAGE( "isLexicographical, isSymmetric, isDegreeOrder, isBlockOrder, isTotalDegreeOrder, isDegreeReverseLexicographical, ascendingVariables, descendingVariables, orderedStandardIteration" );
  BOOST_CHECK(!order.isLexicographical());
  BOOST_CHECK(!order.isSymmetric());
  BOOST_CHECK(!order.isDegreeOrder());
  BOOST_CHECK(order.isBlockOrder());
  BOOST_CHECK(!order.isTotalDegreeOrder());
  BOOST_CHECK(!order.isDegreeReverseLexicographical());
  BOOST_CHECK(!order.ascendingVariables());
  BOOST_CHECK(order.descendingVariables());
  BOOST_CHECK(!order.orderedStandardIteration());
}

BOOST_AUTO_TEST_CASE(test_getters) {

  order_type order;
  BOOST_TEST_MESSAGE( "getOrderCode, getBaseOrderCode" );
  BOOST_CHECK_EQUAL(order.getOrderCode(), COrderEnums::block_dlex);
  BOOST_CHECK_EQUAL(order.getBaseOrderCode(), COrderEnums::dlex);
}

BOOST_AUTO_TEST_CASE(test_compare) {

  order_type order;
  BOOST_TEST_MESSAGE( "compare" );
  BooleMonomial monom1 = x;
  BooleMonomial monom2 = x*x;
  BOOST_CHECK(order.compare(monom1, monom2) == CTypes::equality);
  monom1 = x*y;
  monom2 = x*z*v;
  BOOST_CHECK(order.compare(monom1, monom2) == CTypes::less_than);
  monom1 = v*y;
  monom2 = x;
  BOOST_CHECK(order.compare(monom1, monom2) == CTypes::greater_than);
  monom1 = BooleMonomial();
  monom2 = w;
  BOOST_CHECK(order.compare(monom1, monom2) == CTypes::less_than);
  monom1 = BooleMonomial();
  monom2 = BooleMonomial();
  BOOST_CHECK(order.compare(monom1, monom2) == CTypes::equality);
  BooleExponent exp1(x);
  BooleExponent exp2(x*x);
  BOOST_CHECK(order.compare(exp1, exp2) == CTypes::equality);
  exp1 = BooleExponent(w*x);
  exp2 = BooleExponent(v*x);
  BOOST_CHECK(order.compare(exp1, exp2) == CTypes::less_than);
  exp1 = BooleExponent(x*y*z*v*w);
  exp2 = BooleExponent(x*y*z*v);
  BOOST_CHECK(order.compare(exp1, exp2) == CTypes::greater_than);
  BOOST_CHECK(order.compare(0,1) == CTypes::greater_than);
  BOOST_CHECK(order.compare(2,1) == CTypes::less_than);
  BOOST_CHECK(order.compare(-1,-1) == CTypes::equality);
  BOOST_CHECK(order.compare(1,-1) == CTypes::greater_than);
}

BOOST_AUTO_TEST_CASE(test_lead) {

  BOOST_TEST_MESSAGE( "lead, leadExp, leadFirst" );
  order_type order;
  BoolePolynomial poly = x*y*z*w + x*x + x*y + y*z*v*w + 1;
  BOOST_CHECK(order.lead(poly)    == BooleMonomial(x*y*z*w));
  BOOST_CHECK(order.lead(poly,1)  == BooleMonomial(x*y*z*w));
  BOOST_CHECK(order.lead(poly,-1) == BooleMonomial(x*y*z*w));
  BOOST_CHECK(order.leadExp(poly)    == BooleExponent(x*y*z*w));
  BOOST_CHECK(order.leadExp(poly,1)  == BooleExponent(x*y*z*w));
  BOOST_CHECK(order.leadExp(poly,-1) == BooleExponent(x*y*z*w));
  BOOST_CHECK(order.leadFirst(poly)  == BoolePolynomial(x*y*z*w));
  poly = BoolePolynomial();
  output_test_stream output;
  output << order.lead(poly);
  BOOST_CHECK(output.is_equal("0"));///@todo How is BooleMonomial 0?
  BOOST_CHECK(order.leadExp(poly) == BooleExponent());
  BOOST_CHECK(order.leadFirst(poly) == poly);
  poly = x*w + x*z + w*v*y;
  output << order.lead(poly,0);
  BOOST_CHECK(order.lead(poly, 0) == BooleMonomial(w*v*y));///@todo How is poly a BooleMonomial?
  BOOST_CHECK(order.leadExp(poly, 0) == BooleExponent(w*v*y));
}

BOOST_AUTO_TEST_CASE(test_blocks) {

  BOOST_TEST_MESSAGE( "blockBegin, blockEnd, appendBlock, clearBlocks, lastBlockStart, lieInSameBlock" );
  order_type order;
  output_test_stream output;
  output_test_stream output_test;
  int intmax = std::numeric_limits<int>::max();
  BoolePolyRing::block_iterator start(order.blockBegin()),finish(order.blockEnd());
  order.clearBlocks();
  start = order.blockBegin();
  finish = order.blockEnd();
  while (start != finish) {
    output << *start <<", ";
    ++start;
  }
  output_test.str("");
  output_test << intmax << ", ";
  BOOST_CHECK(output.is_equal(output_test.str()));
  //BOOST_CHECK_THROW(order.appendBlock(-1), std::runtime_error);
  order.appendBlock(0);
  order.appendBlock(2);
  order.appendBlock(6);
  start = order.blockBegin();
  finish = order.blockEnd();
  while (start != finish) {
    output << *start <<", ";
    ++start;
  }
  output_test.str("");
  output_test << "0, 2, 6, " << intmax << ", ";
  BOOST_CHECK(output.is_equal(output_test.str()));
  BOOST_CHECK(order.lieInSameBlock(0,1));
  BOOST_CHECK(!order.lieInSameBlock(1,2));
  BOOST_CHECK(order.lieInSameBlock(3,2));
  BOOST_CHECK(order.lieInSameBlock(4,3));
  BOOST_CHECK(order.lieInSameBlock(4,5));
  BOOST_CHECK(!order.lieInSameBlock(5,6));
  BOOST_CHECK(order.lieInSameBlock(6,7));
  BOOST_CHECK(!order.lieInSameBlock(7,intmax));
  BOOST_CHECK_THROW(!order.lieInSameBlock(-1,3), std::runtime_error);
  BOOST_CHECK_THROW(!order.lieInSameBlock(-1,intmax), std::runtime_error);
  BOOST_CHECK_THROW(!order.lieInSameBlock(-1,-1), std::runtime_error);
  BOOST_CHECK_EQUAL(order.lastBlockStart(), 6);
}

BOOST_AUTO_TEST_CASE(test_compare_blocks) {

  BOOST_TEST_MESSAGE( "compare with appended blocks" );
  order_type blockorder;//(x,y)(z,v,w)(5-intmax)
  order_type order;
  blockorder.appendBlock(2);
  blockorder.appendBlock(6);
  // Variable comparison -> variables in different blocks dont follow the ascending order
  BooleMonomial monom1 = x;
  BooleMonomial monom2 = y;
  BooleExponent exp1(monom1);
  BooleExponent exp2(monom2);
  BOOST_CHECK(blockorder.compare(monom1, monom2) == CTypes::greater_than);
  BOOST_CHECK(order.compare(monom1, monom2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(blockorder.compare(exp1, exp2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(order.compare(exp1, exp2) == blockorder.compare(exp1, exp2));
  monom1 = x;
  monom2 = v;
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK(blockorder.compare(monom1, monom2) == CTypes::greater_than);
  BOOST_CHECK(order.compare(monom1, monom2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(blockorder.compare(exp1, exp2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(order.compare(exp1, exp2) == blockorder.compare(exp1, exp2));
  monom1 = z;
  monom2 = v;
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK(blockorder.compare(monom1, monom2) == CTypes::greater_than);
  BOOST_CHECK(order.compare(monom1, monom2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(blockorder.compare(exp1, exp2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(order.compare(exp1, exp2) == blockorder.compare(exp1, exp2));
  monom1 = x;
  monom2 = x*x;
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK(blockorder.compare(monom1, monom2) == CTypes::equality);
  BOOST_CHECK(order.compare(monom1, monom2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(blockorder.compare(exp1, exp2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(order.compare(exp1, exp2) == blockorder.compare(exp1, exp2));
  monom1 = z;
  monom2 = z*z;
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK(blockorder.compare(monom1, monom2) == CTypes::equality);
  BOOST_CHECK(order.compare(monom1, monom2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(blockorder.compare(exp1, exp2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(order.compare(exp1, exp2) == blockorder.compare(exp1, exp2));
  monom1 = x;
  monom2 = BooleMonomial();
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK(blockorder.compare(monom1, monom2) == CTypes::greater_than);
  BOOST_CHECK(order.compare(monom1, monom2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(blockorder.compare(exp1, exp2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(order.compare(exp1, exp2) == blockorder.compare(exp1, exp2));
  monom1 = BooleMonomial();
  monom2 = BooleMonomial();
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK(blockorder.compare(monom1, monom2) == CTypes::equality);
  BOOST_CHECK(order.compare(monom1, monom2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(blockorder.compare(exp1, exp2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(order.compare(exp1, exp2) == blockorder.compare(exp1, exp2));
  // Different first blocks -> blockorder result may differ from order 
  monom1 = x*y;
  monom2 = y*z;
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK(blockorder.compare(monom1, monom2) == CTypes::greater_than);
  BOOST_CHECK(order.compare(monom1, monom2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(blockorder.compare(exp1, exp2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(order.compare(exp1, exp2) == blockorder.compare(exp1, exp2));
  monom1 = x*y;
  monom2 = y*z*v*w;
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK(blockorder.compare(monom1, monom2) == CTypes::greater_than);
  BOOST_CHECK(order.compare(monom1, monom2) != blockorder.compare(monom1, monom2));
  BOOST_CHECK(blockorder.compare(exp1, exp2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(order.compare(exp1, exp2) != blockorder.compare(exp1, exp2));
  monom1 = y*v*w;
  monom2 = x*v*w;
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK(blockorder.compare(monom1, monom2) == CTypes::less_than);
  BOOST_CHECK(order.compare(monom1, monom2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(blockorder.compare(exp1, exp2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(order.compare(exp1, exp2) == blockorder.compare(exp1, exp2));
  monom1 = x*y*v*w;
  monom2 = y*v*w;
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK(blockorder.compare(monom1, monom2) == CTypes::greater_than);
  BOOST_CHECK(order.compare(monom1, monom2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(blockorder.compare(exp1, exp2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(order.compare(exp1, exp2) == blockorder.compare(exp1, exp2));
  monom1 = BooleMonomial();
  monom2 = y*v*w;
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK(blockorder.compare(monom1, monom2) == CTypes::less_than);
  BOOST_CHECK(order.compare(monom1, monom2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(blockorder.compare(exp1, exp2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(order.compare(exp1, exp2) == blockorder.compare(exp1, exp2));
  // Same first blocks -> blockorder=order
  monom1 = x*v;
  monom2 = x*w;
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK(blockorder.compare(monom1, monom2) == CTypes::greater_than);
  BOOST_CHECK(order.compare(monom1, monom2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(blockorder.compare(exp1, exp2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(order.compare(exp1, exp2) == blockorder.compare(exp1, exp2));
  monom1 = x*z*v;
  monom2 = x*w;
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK(blockorder.compare(monom1, monom2) == CTypes::greater_than);
  BOOST_CHECK(order.compare(monom1, monom2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(blockorder.compare(exp1, exp2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(order.compare(exp1, exp2) == blockorder.compare(exp1, exp2));
  monom1 = y*v;
  monom2 = y*z*v;
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK(blockorder.compare(monom1, monom2) == CTypes::less_than);
  BOOST_CHECK(order.compare(monom1, monom2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(blockorder.compare(exp1, exp2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(order.compare(exp1, exp2) == blockorder.compare(exp1, exp2));
  monom1 = x*y*v;
  monom2 = x*y*w;
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK(blockorder.compare(monom1, monom2) == CTypes::greater_than);
  BOOST_CHECK(order.compare(monom1, monom2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(blockorder.compare(exp1, exp2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(order.compare(exp1, exp2) == blockorder.compare(exp1, exp2));
  monom1 = x*y;
  monom2 = x*y*w;
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK(blockorder.compare(monom1, monom2) == CTypes::less_than);
  BOOST_CHECK(order.compare(monom1, monom2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(blockorder.compare(exp1, exp2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(order.compare(exp1, exp2) == blockorder.compare(exp1, exp2));
  monom1 = x*y*w;
  monom2 = x*y*w;
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK(blockorder.compare(monom1, monom2) == CTypes::equality);
  BOOST_CHECK(order.compare(monom1, monom2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(blockorder.compare(exp1, exp2) == blockorder.compare(monom1, monom2));
  BOOST_CHECK(order.compare(exp1, exp2) == blockorder.compare(exp1, exp2));
  BOOST_CHECK(blockorder.compare(0,1) == CTypes::greater_than);
  BOOST_CHECK(order.compare(0,1) == blockorder.compare(0,1));
  BOOST_CHECK(blockorder.compare(1,2) == CTypes::greater_than);
  BOOST_CHECK(order.compare(1,2) == blockorder.compare(1,2));
  BOOST_CHECK(blockorder.compare(2,3) == CTypes::greater_than);
  BOOST_CHECK(order.compare(2,3) == blockorder.compare(2,3));
  BOOST_CHECK(blockorder.compare(3,4) == CTypes::greater_than);
  BOOST_CHECK(order.compare(3,4) == blockorder.compare(3,4));
  BOOST_CHECK(blockorder.compare(4,5) == CTypes::greater_than);
  BOOST_CHECK(order.compare(4,5) == blockorder.compare(4,5));
  BOOST_CHECK(blockorder.compare(5,6) == CTypes::greater_than);
  BOOST_CHECK(order.compare(5,6) == blockorder.compare(5,6));
  BOOST_CHECK(blockorder.compare(6,7) == CTypes::greater_than);
  BOOST_CHECK(order.compare(6,7) == blockorder.compare(6,7));
  BOOST_CHECK(blockorder.compare(7,-1) == CTypes::greater_than);
  BOOST_CHECK(order.compare(7,-1) == blockorder.compare(7,-1));
  BOOST_CHECK(blockorder.compare(1,1) == CTypes::equality);
  BOOST_CHECK(order.compare(1,1) == blockorder.compare(1,1));
  BOOST_CHECK(blockorder.compare(3,3) == CTypes::equality);
  BOOST_CHECK(order.compare(3,3) == blockorder.compare(3,3));
  BOOST_CHECK(blockorder.compare(7,7) == CTypes::equality);
  BOOST_CHECK(order.compare(7,7) == blockorder.compare(7,7));
  BOOST_CHECK(blockorder.compare(-1,-1) == CTypes::equality);
  BOOST_CHECK(order.compare(-1,-1) == blockorder.compare(-1,-1));
}

BOOST_AUTO_TEST_CASE(test_lead_blocks) {

  BOOST_TEST_MESSAGE( "lead with appended blocks" );
  order_type blockorder;//(x,y)(z,v,w)(5-intmax)
  order_type order;
  blockorder.appendBlock(2);
  blockorder.appendBlock(6);

  BoolePolynomial poly = BoolePolynomial();
  output_test_stream output;
  output << blockorder.lead(poly);
  BOOST_CHECK(output.is_equal("0"));/// How is BooleMonomial 0?
  BOOST_CHECK_EQUAL(blockorder.lead(poly), order.lead(poly));
  BOOST_CHECK(blockorder.leadExp(poly) == BooleExponent());
  BOOST_CHECK_EQUAL(blockorder.leadExp(poly), order.leadExp(poly));
  BOOST_CHECK(blockorder.leadFirst(poly) == poly);
  BOOST_CHECK_EQUAL(blockorder.leadFirst(poly), order.leadFirst(poly));
  ///@todo Calculating the lead with no blocks makes the lead with blocks wrong
  poly = y*x + y*z*v*w + v*w + z*w + w*w;/// But y*x > y*z*v*w!
  std::cout << order.lead(poly) << std::endl;/// Comment this out to get correct lead!
  BOOST_CHECK(blockorder.lead(poly, 0) == BooleMonomial(y*z*v*w));
  BOOST_CHECK_EQUAL(blockorder.lead(poly, 0), order.lead(poly, 0));
  BOOST_CHECK(blockorder.leadExp(poly, 0) == BooleExponent(y*z*v*w));
  BOOST_CHECK_EQUAL(blockorder.leadExp(poly, 0), order.leadExp(poly, 0));
}
BOOST_AUTO_TEST_SUITE_END()