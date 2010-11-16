// -*- c++ -*-
//*****************************************************************************
/** @file CCuddDDFacadeTest.cc
 *
 * @author Ket Kalda, Alexander Dreyer
 * @date 2010-11-09
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

#include "pbori_defs.h"
#include "BooleVariable.h"
#include "BooleMonomial.h"
#include "BoolePolynomial.h"
#include "BooleExponent.h"
#include "BoolePolyRing.h"
#include <vector>

USING_NAMESPACE_PBORI

struct Fdd {
  typedef BooleSet set_type;
  typedef CCuddDDFacade<BoolePolyRing, set_type> dd_type;
  Fdd():
    ring(5) {
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
      poly = x*y*z + v*z - x*v + y + 1;
    }

  ~Fdd() { BOOST_TEST_MESSAGE( "teardown fixture" ); }

  BoolePolyRing ring;
  BooleVariable x, y, z, v, w;
  BoolePolynomial poly;
};

BOOST_FIXTURE_TEST_SUITE(CCuddDDFacadeTestSuite, Fdd )

BOOST_AUTO_TEST_CASE(test_facade) {
  //dd_type facade(poly.set());
  //facade.printIntern(std::cout);
}

BOOST_AUTO_TEST_CASE(test_size) {

  dd_type diagram(poly.set());
  set_type one_set;
  one_set = one_set.add(BooleMonomial());
  dd_type one(one_set);

  BOOST_TEST_MESSAGE( "count, countDouble, nNodes, rootIndex, nSupport" );
  BOOST_CHECK_EQUAL(diagram.count(), 5);
  BOOST_CHECK_EQUAL(one.count(), 1);
  BOOST_CHECK_EQUAL(diagram.countDouble(), 5);
  BOOST_CHECK_EQUAL(one.countDouble(), 1);
  BOOST_CHECK_EQUAL(diagram.nNodes(), 8); // use simpler example to track correctness
  BOOST_CHECK_EQUAL(one.nNodes(), 1);
  BOOST_CHECK_EQUAL(diagram.rootIndex(), 0);
  BOOST_CHECK_EQUAL(one.rootIndex(), std::numeric_limits<int>::max());
  BOOST_CHECK_EQUAL(diagram.nSupport(), 4);
  BOOST_CHECK_EQUAL(one.nSupport(), 0);
}

BOOST_AUTO_TEST_CASE(test_properties) {

  dd_type diagram(poly.set());
  set_type one_set;
  one_set = one_set.add(BooleMonomial());
  dd_type one(one_set);
  // emptyness, blankness not tested as they are to be removed
  BOOST_TEST_MESSAGE( "isZero, isOne, isConstant" );
  BOOST_CHECK(!diagram.isZero());
  BOOST_CHECK(!one.isZero());
  BOOST_CHECK(!diagram.isOne());
  BOOST_CHECK(one.isOne());
  BOOST_CHECK(!diagram.isConstant());
  BOOST_CHECK(one.isConstant());
}

BOOST_AUTO_TEST_CASE(test_indices) {

  dd_type diagram(poly.set());
  set_type one_set;
  one_set = one_set.add(BooleMonomial());
  dd_type one(one_set);
  output_test_stream output;
  // What does usedIndices do?
  BOOST_TEST_MESSAGE( "usedIndices" );
  std::vector<int> indices;
  diagram.usedIndices(indices);
  for(unsigned i=0;i<indices.size();i++)
    output<< indices[i] << " ";
  BOOST_CHECK(output.is_equal("0 1 2 3 "));

  one.usedIndices(indices);
  for(unsigned i=0;i<indices.size();i++)
    output<< indices[i] << " ";
  BOOST_CHECK(output.is_equal(""));
  BOOST_CHECK_EQUAL(*diagram.usedIndices(), 1);
  BOOST_CHECK_EQUAL(*one.usedIndices(), 0);

  BOOST_TEST_MESSAGE( "firstBegin, firstEnd, lastBegin, lastEnd" );
  BOOST_CHECK_EQUAL(*diagram.firstBegin(), 0);
  dd_type::first_iterator start = diagram.firstBegin();
  dd_type::first_iterator finish = diagram.firstEnd();
  while (start != finish) {
    std::cout << *start <<", ";
    ++start;
  }
  dd_type::last_iterator last_start = diagram.lastBegin();
  dd_type::last_iterator last_finish = diagram.lastEnd();
  while (last_start != last_finish) {
    std::cout << *last_start <<", ";
    ++last_start;
  }
}

BOOST_AUTO_TEST_CASE(test_divide) {
  dd_type diagram(poly.set());
  BoolePolynomial poly_small = x;
  dd_type diagram_small(poly_small.set());
  set_type one_set;
  one_set = one_set.add(BooleMonomial());
  dd_type one(one_set);
  std::cout << diagram.divideFirst(diagram_small);
  std::cout << diagram.divideFirst(one);
}

BOOST_AUTO_TEST_SUITE_END()
