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
  BoolePolynomial poly_large = x*y*z + v*z - x*v + y + 1;
  BoolePolynomial poly_small = x;
  dd_type diagram_large(poly_large.set());
  dd_type diagram_small(poly_small.set());
  set_type one_set;
  one_set = one_set.add(BooleMonomial());
  dd_type one(one_set);
  output_test_stream output;

  BOOST_TEST_MESSAGE( "divideFirst" );
  output << diagram_large.divideFirst(diagram_small);
  BOOST_CHECK(output.is_equal("{{y,z}, {v}}"));
  poly_small = y;
  diagram_small =dd_type(poly_small.set());
  output << diagram_large.divideFirst(diagram_small);
  BOOST_CHECK(output.is_equal("{{x,z}, {}}"));
  poly_small = z;
  diagram_small =dd_type(poly_small.set());
  output << diagram_large.divideFirst(diagram_small);
  BOOST_CHECK(output.is_equal("{{x,y}, {v}}"));
  poly_small = v;
  diagram_small =dd_type(poly_small.set());
  output << diagram_large.divideFirst(diagram_small);
  BOOST_CHECK(output.is_equal("{{x}, {z}}"));
  poly_small = x*y;
  diagram_small =dd_type(poly_small.set());
  output << diagram_large.divideFirst(diagram_small);
  BOOST_CHECK(output.is_equal("{{z}}"));
  poly_small = x*v;
  diagram_small =dd_type(poly_small.set());
  output << diagram_large.divideFirst(diagram_small);
  BOOST_CHECK(output.is_equal("{{}}"));
  poly_small = z*v;
  diagram_small =dd_type(poly_small.set());
  output << diagram_large.divideFirst(diagram_small);
  BOOST_CHECK(output.is_equal("{{}}"));
  poly_small = x*z;
  diagram_small =dd_type(poly_small.set());
  output << diagram_large.divideFirst(diagram_small);
  BOOST_CHECK(output.is_equal("{{y}}"));
  poly_small = y*z;
  diagram_small =dd_type(poly_small.set());
  output << diagram_large.divideFirst(diagram_small);
  BOOST_CHECK(output.is_equal("{{x}}"));
  poly_small = z*y*x;
  diagram_small =dd_type(poly_small.set());
  output << diagram_large.divideFirst(diagram_small);
  BOOST_CHECK(output.is_equal("{{}}"));
  poly_small = 1;
  diagram_small =dd_type(poly_small.set());
  output << diagram_large.divideFirst(diagram_small);
  BOOST_CHECK(output.is_equal("{{x,y,z}, {x,v}, {y}, {z,v}, {}}"));
  poly_small = 0;
  diagram_small =dd_type(poly_small.set());
  output << diagram_large.divideFirst(diagram_small);
  BOOST_CHECK(output.is_equal("{{x,y,z}, {x,v}, {y}, {z,v}, {}}"));/// Why all not nothing?
  poly_small = w;// nonexisting variable in poly_large
  diagram_small =dd_type(poly_small.set());
  output << diagram_large.divideFirst(diagram_small);
  BOOST_CHECK(output.is_equal("{}"));
  poly_small = x*w;// nonexisting variable combination in poly_large
  diagram_small =dd_type(poly_small.set());
  output << diagram_large.divideFirst(diagram_small);
  BOOST_CHECK(output.is_equal("{}"));
  poly_small = y*v;// nonexisting combination in poly_large
  diagram_small =dd_type(poly_small.set());
  output << diagram_large.divideFirst(diagram_small);
  BOOST_CHECK(output.is_equal("{}"));

  BOOST_TEST_MESSAGE( "firstDivisors" );
  output << diagram_large.firstDivisors();
  BOOST_CHECK(output.is_equal("{{x,y,z}, {x,y}, {x,z}, {x}, {y,z}, {y}, {z}, {}}"));
  poly_small = x*z + x*y;
  diagram_small =dd_type(poly_small.set());
  output << diagram_small.firstDivisors();
  BOOST_CHECK(output.is_equal("{{x,y}, {x}, {y}, {}}"));
  poly_small = z + x -1;
  diagram_small =dd_type(poly_small.set());
  output << diagram_small.firstDivisors();
  BOOST_CHECK(output.is_equal("{{x}, {}}"));
  poly_small = -1;
  diagram_small =dd_type(poly_small.set());
  output << diagram_small.firstDivisors();
  BOOST_CHECK(output.is_equal("{{}}"));
  poly_small = 0;
  diagram_small =dd_type(poly_small.set());
  output << diagram_small.firstDivisors();
  BOOST_CHECK(output.is_equal("{{}}"));// one is always a divisor
}

BOOST_AUTO_TEST_CASE(test_multiples) {

  dd_type diagram(poly.set());//x*y*z + v*z - x*v + y + 1;
  output_test_stream output;

  BOOST_TEST_MESSAGE( "firstmultiples" );
  std::vector<dd_type::idx_type> multipliers(3);
  multipliers[0]= 0;
  multipliers[1]= 1;
  multipliers[2]= 2;
  output << diagram.firstMultiples(multipliers);
  BOOST_CHECK(output.is_equal("{{x,y,z}}"));
  multipliers[0]= 2;
  multipliers[1]= 1;
  multipliers[2]= 0;
  output << diagram.firstMultiples(multipliers);
  BOOST_CHECK(output.is_equal("{{x}, {x,y}, {x,z}, {x,y,z}}")); ///WRONG
  multipliers[0]= 2;
  multipliers[1]= 0;
  multipliers[2]= 1;
  output << diagram.firstMultiples(multipliers);
  BOOST_CHECK(output.is_equal("{{x,y}, {x,y,z}}")); ///WRONG
  multipliers = std::vector<dd_type::idx_type>(2);
  multipliers[0]= 3;
  multipliers[1]= 4;
  output << diagram.firstMultiples(multipliers);
  BOOST_CHECK(output.is_equal("{{x,y,z,v,w}, {x,y,z,v}, {x,y,z,w}, {x,y,z}}"));
  multipliers[0]= 4;
  multipliers[1]= 3;
  output << diagram.firstMultiples(multipliers);
  BOOST_CHECK(output.is_equal("{{x,y,z,v,w}, {x,y,z,w}, {x,y,z,v}, {x,y,z}}")); ///WRONG ordering
  multipliers[0]= 5;
  multipliers[1]= 6;
  output << diagram.firstMultiples(multipliers);///UNDEF repetition - TODO should throw
  BOOST_CHECK(output.is_equal("{{x,y,z,UNDEF,UNDEF}, {x,y,z,UNDEF}, {x,y,z,UNDEF}, {x,y,z}}"));
  multipliers[0]= 3;
  multipliers[1]= 3;
  output << diagram.firstMultiples(multipliers);/// Repetition - TODO needs investigation
  BOOST_CHECK(output.is_equal("{{x,y,z}, {x,y,z,v}, {x,y,z,v}, {x,y,z}}"));
  multipliers[0]= 0;
  multipliers[1]= 0;
  output << diagram.firstMultiples(multipliers);/// How was x lost? - TODO needs investigation
  BOOST_CHECK(output.is_equal("{{y,z}, {x,y,z}}"));
  multipliers = std::vector<dd_type::idx_type>(3);
  multipliers[0]= 0;
  multipliers[1]= 0;
  multipliers[2]= 0;
  output << diagram.firstMultiples(multipliers);/// Repetition
  BOOST_CHECK(output.is_equal("{{x,y,z}, {y,z}, {y,z}, {x,y,z}}"));
  multipliers = std::vector<dd_type::idx_type>(0);
  output << diagram.firstMultiples(multipliers);
  BOOST_CHECK(output.is_equal("{{x,y,z}}"));
  multipliers = std::vector<dd_type::idx_type>(1);
  multipliers[0]= -1;
  //output << diagram.firstMultiples(multipliers); /// memory access violation at 0x7fdd1c00097c - TODO needs handling
  //BOOST_CHECK(output.is_equal("{{x,y,z}}"));
}

BOOST_AUTO_TEST_CASE(test_operators) {

  dd_type diagram(poly.set());//x*y*z + v*z - x*v + y + 1;
  BoolePolynomial poly_small = x*y*z + v*z - x*v + y + 1;
  dd_type diagram_small(poly_small.set());
  output_test_stream output;

  BOOST_TEST_MESSAGE( "Xor" );
  output << diagram.Xor(diagram_small);
  BOOST_CHECK(output.is_equal("{}"));
  poly_small = x*y*z + v*z - x*v + y;
  diagram_small = dd_type(poly_small.set());
  output << diagram.Xor(diagram_small);
  BOOST_CHECK(output.is_equal("{{}}"));
  poly_small = v*z - x*v + y + 1;
  diagram_small = dd_type(poly_small.set());
  output << diagram.Xor(diagram_small);
  BOOST_CHECK(output.is_equal("{{x,y,z}}"));
  poly_small = 1;
  diagram_small = dd_type(poly_small.set());
  output << diagram.Xor(diagram_small);
  BOOST_CHECK(output.is_equal("{{x,y,z}, {x,v}, {y}, {z,v}}"));
  poly_small = 0;
  diagram_small = dd_type(poly_small.set());
  output << diagram.Xor(diagram_small);
  BOOST_CHECK(output.is_equal("{{x,y,z}, {x,v}, {y}, {z,v}, {}}"));
  poly_small = x*y;
  diagram_small = dd_type(poly_small.set());
  output << diagram.Xor(diagram_small);
  BOOST_CHECK(output.is_equal("{{x,y,z}, {x,y}, {x,v}, {y}, {z,v}, {}}"));
  poly_small = w*v;
  diagram_small = dd_type(poly_small.set());
  output << diagram.Xor(diagram_small);
  BOOST_CHECK(output.is_equal("{{x,y,z}, {x,v}, {y}, {z,v}, {v,w}, {}}"));

  /// What does it do?
  BOOST_TEST_MESSAGE( "ite" ); /// Should it return self or diagram_type (like Xor)?
  BoolePolynomial poly_large = v;
  dd_type diagram_large = dd_type(poly_large.set());
  poly_small = y*z;
  BoolePolynomial poly_small2 = x*y;
  diagram_small = dd_type(poly_small.set());
  dd_type diagram_small2 = dd_type(poly_small2.set());
  output << set_type(diagram_large.ite(diagram_small, diagram_small2));
  BOOST_CHECK(output.is_equal(""));

  /// What does it do?
  BOOST_TEST_MESSAGE( "implies" );
}

BOOST_AUTO_TEST_CASE(test_print) {
  dd_type diagram(poly.set());
  diagram.printIntern(std::cout);/// Why public?
  diagram.PrintMinterm();/// Why public, why no input of stream?
  diagram.print(std::cout);/// Not used, no << operator either
}

BOOST_AUTO_TEST_SUITE_END()
