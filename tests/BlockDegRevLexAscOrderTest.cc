// -*- c++ -*-
//*****************************************************************************
/** @file BlockDegRevLexAscOrderTest.cc
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

#include <polybori/BlockDegRevLexAscOrder.h>
#include <polybori/DegRevLexAscOrder.h>

USING_NAMESPACE_PBORI

struct Fblockdegrevlex {
  typedef BlockDegRevLexAscOrder order_type;
  typedef DegRevLexAscOrder base_order_type;
  Fblockdegrevlex(const BoolePolyRing& input_ring = 
                  BoolePolyRing(5, COrderEnums::dlex)):  
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
  ~Fblockdegrevlex() { BOOST_TEST_MESSAGE( "teardown fixture" ); }

  BoolePolyRing ring;
  BooleVariable x, y, z, v, w;
};

BOOST_FIXTURE_TEST_SUITE(BlockDegRevLexOrderTestSuite, Fblockdegrevlex )

BOOST_AUTO_TEST_CASE(test_properties) {

  order_type order;
  BOOST_TEST_MESSAGE( "isLexicographical, isSymmetric, isDegreeOrder, isBlockOrder, isTotalDegreeOrder, isDegreeReverseLexicographical, ascendingVariables, descendingVariables, orderedStandardIteration" );
  BOOST_CHECK(!order.isLexicographical());
  BOOST_CHECK(!order.isSymmetric());
  BOOST_CHECK(!order.isDegreeOrder());
  BOOST_CHECK(order.isBlockOrder());
  BOOST_CHECK(!order.isTotalDegreeOrder());
  BOOST_CHECK(!order.isDegreeReverseLexicographical());
  BOOST_CHECK(order.ascendingVariables());
  BOOST_CHECK(!order.descendingVariables());
  BOOST_CHECK(!order.orderedStandardIteration());
}

BOOST_AUTO_TEST_CASE(test_getters) {

  order_type order;
  BOOST_TEST_MESSAGE( "getOrderCode, getBaseOrderCode" );
  BOOST_CHECK_EQUAL(order.getOrderCode(), COrderEnums::block_dp_asc);
  BOOST_CHECK_EQUAL(order.getBaseOrderCode(), COrderEnums::dp_asc);
}

BOOST_AUTO_TEST_CASE(test_compare) {

  order_type order;
  base_order_type baseorder;
  BOOST_TEST_MESSAGE( "compare" );
  BooleMonomial monom1 = x;
  BooleMonomial monom2 = x*x;
  BOOST_CHECK_EQUAL(order.compare(monom1, monom2), CTypes::equality);
  BOOST_CHECK_EQUAL(order.compare(monom1,monom2), baseorder.compare(monom1,monom2));
  monom1 = x*y;
  monom2 = x*z*v;
  BOOST_CHECK_EQUAL(order.compare(monom1, monom2), CTypes::less_than);
  BOOST_CHECK_EQUAL(order.compare(monom1,monom2), baseorder.compare(monom1,monom2));
  monom1 = v*y;
  monom2 = x;
  BOOST_CHECK_EQUAL(order.compare(monom1, monom2), CTypes::greater_than);
  BOOST_CHECK_EQUAL(order.compare(monom1,monom2), baseorder.compare(monom1,monom2));
  monom1 = BooleMonomial(ring);
  monom2 = w;
  BOOST_CHECK_EQUAL(order.compare(monom1, monom2), CTypes::less_than);
  BOOST_CHECK_EQUAL(order.compare(monom1,monom2), baseorder.compare(monom1,monom2));
  monom1 = BooleMonomial(ring);
  monom2 = BooleMonomial(ring);
  BOOST_CHECK_EQUAL(order.compare(monom1, monom2), CTypes::equality);
  BOOST_CHECK_EQUAL(order.compare(monom1,monom2), baseorder.compare(monom1,monom2));
  BooleExponent exp1(x);
  BooleExponent exp2(x*x);
  BOOST_CHECK_EQUAL(order.compare(exp1, exp2), CTypes::equality);
  BOOST_CHECK_EQUAL(order.compare(exp1,exp2), baseorder.compare(exp1,exp2));
  exp1 = BooleExponent(w*x);
  exp2 = BooleExponent(v*x);
  BOOST_CHECK_EQUAL(order.compare(exp1, exp2), CTypes::greater_than);
  BOOST_CHECK_EQUAL(order.compare(exp1,exp2), baseorder.compare(exp1,exp2));
  exp1 = BooleExponent(x*y*z*v*w);
  exp2 = BooleExponent(x*y*z*v);
  BOOST_CHECK_EQUAL(order.compare(exp1, exp2), CTypes::greater_than);
  BOOST_CHECK_EQUAL(order.compare(exp1,exp2), baseorder.compare(exp1,exp2));
  BOOST_CHECK_EQUAL(order.compare(0,1), CTypes::less_than);
  BOOST_CHECK_EQUAL(order.compare(0,1), baseorder.compare(0,1));
  BOOST_CHECK_EQUAL(order.compare(2,1), CTypes::greater_than);
  BOOST_CHECK_EQUAL(order.compare(2,1), baseorder.compare(2,1));
  BOOST_CHECK_EQUAL(order.compare(-1,-1), CTypes::equality);
  BOOST_CHECK_EQUAL(order.compare(-1,-1), baseorder.compare(-1,-1));
  BOOST_CHECK_EQUAL(order.compare(1,-1), CTypes::greater_than);
  BOOST_CHECK_EQUAL(order.compare(-1,-1), baseorder.compare(-1,-1));
}

BOOST_AUTO_TEST_CASE(test_lead) {

  BOOST_TEST_MESSAGE( "lead, leadExp, leadFirst" );
  order_type order;
  BoolePolynomial poly = x*y*z*w + x*x + x*y + y*z*v*w + 1;
  BOOST_CHECK_EQUAL(order.lead(poly)   , BooleMonomial(y*z*v*w));
  BOOST_CHECK_EQUAL(order.lead(poly,1) , BooleMonomial(y*z*v*w));
  BOOST_CHECK_EQUAL(order.lead(poly,-1), BooleMonomial(y*z*v*w));
  BOOST_CHECK_EQUAL(order.leadExp(poly)   , BooleExponent(y*z*v*w));
  BOOST_CHECK_EQUAL(order.leadExp(poly,1) , BooleExponent(y*z*v*w));
  BOOST_CHECK_EQUAL(order.leadExp(poly,-1), BooleExponent(y*z*v*w));
  BOOST_CHECK_EQUAL(order.leadFirst(poly) , BoolePolynomial(y*z*v*w));
  poly = BoolePolynomial(ring);
  BOOST_CHECK_THROW(order.lead(poly), PBoRiGenericError<CTypes::illegal_on_zero>);
  BOOST_CHECK_THROW(order.lead(poly,1),PBoRiGenericError<CTypes::illegal_on_zero>);
  BOOST_CHECK_THROW(order.lead(poly),PBoRiGenericError<CTypes::illegal_on_zero>);
  BOOST_CHECK_THROW(order.leadExp(poly,1),PBoRiGenericError<CTypes::illegal_on_zero>);
  BOOST_CHECK_THROW(order.leadExp(poly),PBoRiGenericError<CTypes::illegal_on_zero>);
  BOOST_CHECK_THROW(order.leadFirst(poly),PBoRiGenericError<CTypes::illegal_on_zero>);
  poly = 1;
  BOOST_CHECK_EQUAL(order.lead(poly, 1), BooleMonomial(ring));
  BOOST_CHECK_EQUAL(order.lead(poly), BooleMonomial(ring));
  BOOST_CHECK_EQUAL(order.leadExp(poly,1), BooleExponent());
  BOOST_CHECK_EQUAL(order.leadExp(poly), BooleExponent());
  BOOST_CHECK_EQUAL(order.leadFirst(poly), poly);
  poly = x*w + x*z + w*v*y;
  BOOST_CHECK_EQUAL(order.lead(poly, 0), BooleMonomial(w*v*y));
  BOOST_CHECK_EQUAL(order.leadExp(poly, 0), BooleExponent(w*v*y));
}

BOOST_AUTO_TEST_CASE(test_blocks) {

  BOOST_TEST_MESSAGE( "blockBegin, blockEnd, appendBlock, clearBlocks, lastBlockStart, lieInSameBlock" );
  order_type order;
  output_test_stream output;
  output_test_stream output_test;
  int intmax = CUDD_MAXINDEX;
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
  //  std::cout << "fin " << *finish << " fin-1 " << *(finish-1) << " fin-2 " << *(finish-2);;
  BOOST_CHECK_THROW(order.appendBlock(-1), std::exception);
  BOOST_CHECK_THROW(order.appendBlock(0), std::runtime_error);
  order.appendBlock(2);
  BOOST_CHECK_THROW(order.appendBlock(1), std::runtime_error);
  order.appendBlock(6);
  BOOST_CHECK_THROW(order.appendBlock(6), std::runtime_error);
  BOOST_CHECK_THROW(order.appendBlock(intmax), std::runtime_error);
  start = order.blockBegin();
  finish = order.blockEnd();
  while (start != finish) {
    output << *start <<", ";
    ++start;
  }
  output_test.str("");
  output_test << "2, 6, " << intmax << ", ";
  BOOST_CHECK(output.is_equal(output_test.str()));
  BOOST_CHECK(order.lieInSameBlock(0,1));
  BOOST_CHECK(!order.lieInSameBlock(1,2));
  BOOST_CHECK(order.lieInSameBlock(3,2));
  BOOST_CHECK(order.lieInSameBlock(4,3));
  BOOST_CHECK(order.lieInSameBlock(4,5));
  BOOST_CHECK(!order.lieInSameBlock(5,6));
  BOOST_CHECK(order.lieInSameBlock(6,7));
  BOOST_CHECK(!order.lieInSameBlock(7,intmax));
  BOOST_CHECK_THROW(order.lieInSameBlock(-1,3), std::runtime_error);
  BOOST_CHECK_THROW(order.lieInSameBlock(-1,intmax), std::runtime_error);
  BOOST_CHECK_THROW(order.lieInSameBlock(-1,-1), std::runtime_error);
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
  BOOST_CHECK_EQUAL(blockorder.compare(monom1, monom2), CTypes::less_than);
  BOOST_CHECK_EQUAL(order.compare(monom1, monom2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_EQUAL(blockorder.compare(exp1, exp2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_EQUAL(order.compare(exp1, exp2), blockorder.compare(exp1, exp2));
  monom1 = x;
  monom2 = v;
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK_EQUAL(blockorder.compare(monom1, monom2), CTypes::greater_than);
  BOOST_CHECK_NE(order.compare(monom1, monom2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_EQUAL(blockorder.compare(exp1, exp2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_NE(order.compare(exp1, exp2), blockorder.compare(exp1, exp2));
  monom1 = z;
  monom2 = v;
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK_EQUAL(blockorder.compare(monom1, monom2), CTypes::less_than);
  BOOST_CHECK_EQUAL(order.compare(monom1, monom2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_EQUAL(blockorder.compare(exp1, exp2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_EQUAL(order.compare(exp1, exp2), blockorder.compare(exp1, exp2));
  monom1 = x;
  monom2 = x*x;
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK_EQUAL(blockorder.compare(monom1, monom2), CTypes::equality);
  BOOST_CHECK_EQUAL(order.compare(monom1, monom2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_EQUAL(blockorder.compare(exp1, exp2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_EQUAL(order.compare(exp1, exp2), blockorder.compare(exp1, exp2));
  monom1 = z;
  monom2 = z*z;
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK_EQUAL(blockorder.compare(monom1, monom2), CTypes::equality);
  BOOST_CHECK_EQUAL(order.compare(monom1, monom2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_EQUAL(blockorder.compare(exp1, exp2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_EQUAL(order.compare(exp1, exp2), blockorder.compare(exp1, exp2));
  monom1 = x;
  monom2 = BooleMonomial(ring);
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK_EQUAL(blockorder.compare(monom1, monom2), CTypes::greater_than);
  BOOST_CHECK_EQUAL(order.compare(monom1, monom2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_EQUAL(blockorder.compare(exp1, exp2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_EQUAL(order.compare(exp1, exp2), blockorder.compare(exp1, exp2));
  monom1 = BooleMonomial(ring);
  monom2 = BooleMonomial(ring);
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK_EQUAL(blockorder.compare(monom1, monom2), CTypes::equality);
  BOOST_CHECK_EQUAL(order.compare(monom1, monom2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_EQUAL(blockorder.compare(exp1, exp2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_EQUAL(order.compare(exp1, exp2), blockorder.compare(exp1, exp2));
  // Different first blocks -> blockorder result may differ from order 
  monom1 = x*y;
  monom2 = y*z;
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK_EQUAL(blockorder.compare(monom1, monom2), CTypes::greater_than);
  BOOST_CHECK_NE(order.compare(monom1, monom2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_EQUAL(blockorder.compare(exp1, exp2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_NE(order.compare(exp1, exp2), blockorder.compare(exp1, exp2));
  monom1 = x*y;
  monom2 = y*z*v*w;
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK_EQUAL(blockorder.compare(monom1, monom2), CTypes::greater_than);
  BOOST_CHECK_NE(order.compare(monom1, monom2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_EQUAL(blockorder.compare(exp1, exp2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_NE(order.compare(exp1, exp2), blockorder.compare(exp1, exp2));
  monom1 = y*v*w;
  monom2 = x*v*w;
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK_EQUAL(blockorder.compare(monom1, monom2), CTypes::greater_than);
  BOOST_CHECK_EQUAL(order.compare(monom1, monom2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_EQUAL(blockorder.compare(exp1, exp2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_EQUAL(order.compare(exp1, exp2), blockorder.compare(exp1, exp2));
  monom1 = x*y*v*w;
  monom2 = y*v*w;
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK_EQUAL(blockorder.compare(monom1, monom2), CTypes::greater_than);
  BOOST_CHECK_EQUAL(order.compare(monom1, monom2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_EQUAL(blockorder.compare(exp1, exp2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_EQUAL(order.compare(exp1, exp2), blockorder.compare(exp1, exp2));
  monom1 = BooleMonomial(ring);
  monom2 = y*v*w;
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK_EQUAL(blockorder.compare(monom1, monom2), CTypes::less_than);
  BOOST_CHECK_EQUAL(order.compare(monom1, monom2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_EQUAL(blockorder.compare(exp1, exp2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_EQUAL(order.compare(exp1, exp2), blockorder.compare(exp1, exp2));
  // Same first blocks -> blockorder=order
  monom1 = x*v;
  monom2 = x*w;
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK_EQUAL(blockorder.compare(monom1, monom2), CTypes::less_than);
  BOOST_CHECK_EQUAL(order.compare(monom1, monom2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_EQUAL(blockorder.compare(exp1, exp2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_EQUAL(order.compare(exp1, exp2), blockorder.compare(exp1, exp2));
  monom1 = x*z*v;
  monom2 = x*w;
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK_EQUAL(blockorder.compare(monom1, monom2), CTypes::greater_than);
  BOOST_CHECK_EQUAL(order.compare(monom1, monom2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_EQUAL(blockorder.compare(exp1, exp2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_EQUAL(order.compare(exp1, exp2), blockorder.compare(exp1, exp2));
  monom1 = y*v;
  monom2 = y*z*v;
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK_EQUAL(blockorder.compare(monom1, monom2), CTypes::less_than);
  BOOST_CHECK_EQUAL(order.compare(monom1, monom2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_EQUAL(blockorder.compare(exp1, exp2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_EQUAL(order.compare(exp1, exp2), blockorder.compare(exp1, exp2));
  monom1 = x*y*v;
  monom2 = x*y*w;
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK_EQUAL(blockorder.compare(monom1, monom2), CTypes::less_than);
  BOOST_CHECK_EQUAL(order.compare(monom1, monom2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_EQUAL(blockorder.compare(exp1, exp2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_EQUAL(order.compare(exp1, exp2), blockorder.compare(exp1, exp2));
  monom1 = x*y;
  monom2 = x*y*w;
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK_EQUAL(blockorder.compare(monom1, monom2), CTypes::less_than);
  BOOST_CHECK_EQUAL(order.compare(monom1, monom2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_EQUAL(blockorder.compare(exp1, exp2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_EQUAL(order.compare(exp1, exp2), blockorder.compare(exp1, exp2));
  monom1 = x*y*w;
  monom2 = x*y*w;
  exp1 = monom1;
  exp2 = monom2;
  BOOST_CHECK_EQUAL(blockorder.compare(monom1, monom2), CTypes::equality);
  BOOST_CHECK_EQUAL(order.compare(monom1, monom2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_EQUAL(blockorder.compare(exp1, exp2), blockorder.compare(monom1, monom2));
  BOOST_CHECK_EQUAL(order.compare(exp1, exp2), blockorder.compare(exp1, exp2));
  BOOST_CHECK_EQUAL(blockorder.compare(0,1), CTypes::less_than);
  BOOST_CHECK_EQUAL(order.compare(0,1), blockorder.compare(0,1));
  BOOST_CHECK_EQUAL(blockorder.compare(1,2), CTypes::greater_than);
  BOOST_CHECK_NE(order.compare(1,2), blockorder.compare(1,2));
  BOOST_CHECK_EQUAL(blockorder.compare(2,3), CTypes::less_than);
  BOOST_CHECK_EQUAL(order.compare(2,3), blockorder.compare(2,3));
  BOOST_CHECK_EQUAL(blockorder.compare(3,4), CTypes::less_than);
  BOOST_CHECK_EQUAL(order.compare(3,4), blockorder.compare(3,4));
  BOOST_CHECK_EQUAL(blockorder.compare(4,5), CTypes::less_than);
  BOOST_CHECK_EQUAL(order.compare(4,5), blockorder.compare(4,5));
  BOOST_CHECK_EQUAL(blockorder.compare(5,6), CTypes::greater_than);
  BOOST_CHECK_NE(order.compare(5,6), blockorder.compare(5,6));
  BOOST_CHECK_EQUAL(blockorder.compare(6,7), CTypes::less_than);
  BOOST_CHECK_EQUAL(order.compare(6,7), blockorder.compare(6,7));
  BOOST_CHECK_EQUAL(blockorder.compare(7,-1), CTypes::less_than);
  // note: no need to have order.compare(7,-1) == blockorder.compare(7,-1)
  BOOST_CHECK_EQUAL(blockorder.compare(1,1), CTypes::equality);
  BOOST_CHECK_EQUAL(order.compare(1,1), blockorder.compare(1,1));
  BOOST_CHECK_EQUAL(blockorder.compare(3,3), CTypes::equality);
  BOOST_CHECK_EQUAL(order.compare(3,3), blockorder.compare(3,3));
  BOOST_CHECK_EQUAL(blockorder.compare(7,7), CTypes::equality);
  BOOST_CHECK_EQUAL(order.compare(7,7), blockorder.compare(7,7));
  BOOST_CHECK_EQUAL(blockorder.compare(-1,-1), CTypes::equality);
  BOOST_CHECK_EQUAL(order.compare(-1,-1), blockorder.compare(-1,-1));
}

BOOST_AUTO_TEST_CASE(test_lead_blocks) {

  BOOST_TEST_MESSAGE( "lead with appended blocks" );
  order_type blockorder;//(x,y)(z,v,w)(5-intmax)
  order_type order;
  blockorder.appendBlock(2);
  blockorder.appendBlock(6);

  BoolePolynomial poly = BoolePolynomial(ring);
  BOOST_CHECK_THROW(blockorder.lead(poly,1),PBoRiGenericError<CTypes::illegal_on_zero>);
  BOOST_CHECK_THROW(blockorder.lead(poly),PBoRiGenericError<CTypes::illegal_on_zero>);
  BOOST_CHECK_THROW(blockorder.leadExp(poly,1),PBoRiGenericError<CTypes::illegal_on_zero>);
  BOOST_CHECK_THROW(blockorder.leadExp(poly),PBoRiGenericError<CTypes::illegal_on_zero>);
  BOOST_CHECK_THROW(blockorder.leadFirst(poly),PBoRiGenericError<CTypes::illegal_on_zero>);
  poly = 1;
  BOOST_CHECK_EQUAL(blockorder.lead(poly, 1), BooleMonomial(ring));
  BOOST_CHECK_EQUAL(blockorder.lead(poly), BooleMonomial(ring));
  BOOST_CHECK_EQUAL(blockorder.leadExp(poly,1), BooleExponent());
  BOOST_CHECK_EQUAL(blockorder.leadExp(poly), BooleExponent());
  BOOST_CHECK_EQUAL(blockorder.leadFirst(poly), poly);
  poly = y*x + y*z*v*w + v*w + z*w + w*w; ///  y*x > y*z*v*w for blockorder
  BOOST_CHECK_EQUAL(order.lead(poly), BooleMonomial(y*z*v*w));
  BOOST_CHECK_EQUAL(blockorder.lead(poly, 0), BooleMonomial(x*y));
  BOOST_CHECK_NE(blockorder.lead(poly, 0), order.lead(poly, 0));
  BOOST_CHECK_EQUAL(blockorder.leadExp(poly, 0), BooleExponent(x*y));
  BOOST_CHECK_NE(blockorder.leadExp(poly, 0), order.leadExp(poly, 0));
}


BOOST_AUTO_TEST_CASE(test_cover_constructors_and_destructors) {
  int order_code = CTypes::block_dp_asc;
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
