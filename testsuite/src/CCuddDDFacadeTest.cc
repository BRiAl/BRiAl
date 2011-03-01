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
#include <orderings/pbori_order.h>
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

// Testing environment needs correct stream operator
inline std::ostream&
operator<<(std::ostream& os, Fdd::dd_type diagram) {
  return diagram.printIntern(os);
}



BOOST_FIXTURE_TEST_SUITE(CCuddDDFacadeTestSuite, Fdd )

BOOST_AUTO_TEST_CASE(test_constructors) {

  dd_type diagram(poly.set());

  BOOST_TEST_MESSAGE( "constructors" );

  BOOST_CHECK_EQUAL(set_type(diagram), poly.set());

  BoolePolynomial poly1 = y*z + v;
  BoolePolynomial poly2 = v*z + y + 1;
  dd_type diagram1(poly1.set());
  dd_type diagram2(poly2.set());
  output_test_stream output;
  diagram = dd_type(0,diagram1,diagram2);
  output << set_type(diagram);
  BOOST_CHECK(output.is_equal("{{x,y,z}, {x,v}, {y}, {z,v}, {}}"));
  BOOST_CHECK_THROW(dd_type(1,diagram1,diagram2), PBoRiGenericError<CTypes::invalid_ite>);
  BOOST_CHECK_THROW(dd_type(2,diagram1,diagram2), PBoRiGenericError<CTypes::invalid_ite>);

  BOOST_CHECK_THROW(dd_type(-1,diagram1,diagram2), std::exception);
  poly1 = 0;
  poly2 = y*z + v;
  diagram1 = dd_type(poly1.set());
  diagram2 = dd_type(poly2.set());
  diagram = dd_type(0,diagram1,diagram2);
  output << set_type(diagram);
  BOOST_CHECK(output.is_equal("{{y,z}, {v}}"));
  poly1 = 1;
  poly2 = y*z + v;
  diagram1 = dd_type(poly1.set());
  diagram2 = dd_type(poly2.set());
  diagram = dd_type(0,diagram1,diagram2);
  output << set_type(diagram);
  BOOST_CHECK(output.is_equal("{{x}, {y,z}, {v}}"));
  poly1 = 1;
  poly2 = 0;
  diagram1 = dd_type(poly1.set());
  diagram2 = dd_type(poly2.set());
  diagram = dd_type(4,diagram1,diagram2);
  output << set_type(diagram);
  BOOST_CHECK(output.is_equal("{{w}}"));
  diagram = dd_type(5,diagram1,diagram2);
  output << set_type(diagram);
  BOOST_CHECK(output.is_equal("{{UNDEF}}"));/// Wanted behaviour?
  BOOST_CHECK_THROW(dd_type(ring, NULL), std::runtime_error);
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
    output<<  *start <<", ";
    ++start;
  }
  BOOST_CHECK(output.is_equal("0, 1, 2, "));

  diagram = BoolePolynomial(x*y*z + x*y).set();
  dd_type::last_iterator last_start = diagram.lastBegin();
  dd_type::last_iterator last_finish = diagram.lastEnd();
  while (last_start != last_finish) {
    output << *last_start <<", ";
    ++last_start;
  }
  BOOST_CHECK(output.is_equal("0, 1, "));
  last_start = diagram.lastBegin();
  last_finish = diagram.lastEnd();
  while (last_start != last_finish) {
    output << *last_start <<", ";
    last_start++;
  }
  BOOST_CHECK(output.is_equal("0, 1, "));
  last_start = one.lastBegin();
  last_finish = one.lastEnd();
  while (last_start != last_finish) {
    output << *last_start <<", ";
    last_start++;
  }
  BOOST_CHECK(output.is_equal(""));

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

  poly_small = 0; // Assuming rhs to be monomial, so this is not a valid input.
                  // Anyway, the result must be sane, be behaving like 1 is fine
  diagram_small = dd_type(poly_small.set());
  BOOST_CHECK_EQUAL(diagram_large.divideFirst(diagram_small), diagram_large);
  output << diagram_large.divideFirst(diagram_small);
  BOOST_CHECK(output.is_equal("{{x,y,z}, {x,v}, {y}, {z,v}, {}}"));

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
  BOOST_CHECK(output.is_equal("{{x,y,z}}"));

  multipliers[0]= 2;
  multipliers[1]= 0;
  multipliers[2]= 1;
  output << diagram.firstMultiples(multipliers);
  BOOST_CHECK(output.is_equal("{{x,y,z}}"));

  multipliers = std::vector<dd_type::idx_type>(2);
  multipliers[0]= 3;
  multipliers[1]= 4;
  output << diagram.firstMultiples(multipliers);
  BOOST_CHECK(output.is_equal("{{x,y,z,v,w}, {x,y,z,v}, {x,y,z,w}, {x,y,z}}"));

  multipliers[0]= 4;
  multipliers[1]= 3;
  output << diagram.firstMultiples(multipliers);
  BOOST_CHECK(output.is_equal("{{x,y,z,v,w}, {x,y,z,v}, {x,y,z,w}, {x,y,z}}"));

  multipliers[0]= 5;
  multipliers[1]= 6;
  output << diagram.firstMultiples(multipliers); ///@todo UNDEFs - maybe should throw
  BOOST_CHECK(output.is_equal("{{x,y,z,UNDEF,UNDEF}, {x,y,z,UNDEF}, {x,y,z,UNDEF}, {x,y,z}}"));

  multipliers[0]= 3;
  multipliers[1]= 3;
  output << diagram.firstMultiples(multipliers);
  BOOST_CHECK(output.is_equal("{{x,y,z,v}, {x,y,z}}"));

  multipliers[0]= 0;
  multipliers[1]= 0;

  output << diagram.firstMultiples(multipliers);
  BOOST_CHECK(output.is_equal("{{x,y,z}}"));
  multipliers = std::vector<dd_type::idx_type>(3);
  multipliers[0]= 0;
  multipliers[1]= 0;
  multipliers[2]= 0;

  output << diagram.firstMultiples(multipliers);
  BOOST_CHECK(output.is_equal("{{x,y,z}}"));
  multipliers = std::vector<dd_type::idx_type>(0);
  output << diagram.firstMultiples(multipliers);
  BOOST_CHECK(output.is_equal("{{x,y,z}}"));
  multipliers = std::vector<dd_type::idx_type>(1);
  multipliers[0]= -1;
  output << diagram.firstMultiples(multipliers); 
  BOOST_CHECK(output.is_equal("{{x,y,z}}"));
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

  BOOST_TEST_MESSAGE( "implies" );
  //diagram = x*y*z + v*z - x*v + y + 1
  poly_small = 1;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(diagram_small.implies(diagram));
  poly_small = x*y*z;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(diagram_small.implies(diagram));
  poly_small = x*v;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(diagram_small.implies(diagram));
  poly_small = y;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(diagram_small.implies(diagram));
  poly_small = z*v;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(diagram_small.implies(diagram));
  poly_small = x*y*z + y;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(diagram_small.implies(diagram));
  poly_small = x*y*z + v*z;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(diagram_small.implies(diagram));
  poly_small = x*y*z + x*v;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(diagram_small.implies(diagram));
  poly_small = x*y*z + 1;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(diagram_small.implies(diagram));
  poly_small = v*z - x*v;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(diagram_small.implies(diagram));
  poly_small = v*z - y;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(diagram_small.implies(diagram));
  poly_small = v*z - 1;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(diagram_small.implies(diagram));
  poly_small = x*v - y;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(diagram_small.implies(diagram));
  poly_small = x*v - 1;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(diagram_small.implies(diagram));
  poly_small = y - 1;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(diagram_small.implies(diagram));
  poly_small = x*y*z + v*z - x*v;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(diagram_small.implies(diagram));
  poly_small = x*y*z + v*z + y;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(diagram_small.implies(diagram));
  poly_small = x*y*z + v*z + 1;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(diagram_small.implies(diagram));
  poly_small = x*y*z - x*v + y;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(diagram_small.implies(diagram));
  poly_small = x*y*z + x*v + 1;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(diagram_small.implies(diagram));
  poly_small = x*y*z  + y + 1;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(diagram_small.implies(diagram));
  poly_small = v*z - x*v + y;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(diagram_small.implies(diagram));
  poly_small = v*z - x*v + 1;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(diagram_small.implies(diagram));
  poly_small = v*z + y + 1;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(diagram_small.implies(diagram));
  poly_small = x*v + y + 1;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(diagram_small.implies(diagram));
  poly_small = x*y*z + v*z - x*v + y;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(diagram_small.implies(diagram));
  poly_small = x*y*z + v*z - x*v + 1;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(diagram_small.implies(diagram));
  poly_small = x*y*z + v*z + y + 1;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(diagram_small.implies(diagram));
  poly_small = x*y*z - x*v + y + 1;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(diagram_small.implies(diagram));
  poly_small = v*z - x*v + y + 1;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(diagram_small.implies(diagram));
  BOOST_CHECK(diagram.implies(diagram));
  poly_small = 0;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(diagram_small.implies(diagram));
  poly_small = x*w;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(!diagram_small.implies(diagram));
  poly_small = w;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(!diagram_small.implies(diagram));
  poly_small = w + x*y*z;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(!diagram_small.implies(diagram));
  poly_small = x*v*z;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(!diagram_small.implies(diagram));
  poly_small = v;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(!diagram_small.implies(diagram));
  BoolePolynomial poly_large = x;
  dd_type diagram_large = dd_type( poly_large.set() );
  poly_small = x*y + z;
  diagram_small = dd_type( poly_small.set() );
  BOOST_CHECK(!diagram_small.implies(diagram_large));
  poly_large = 0;
  diagram_large = dd_type(poly_large.set());
  BOOST_CHECK(!diagram_small.implies(diagram_large));
  BOOST_CHECK(diagram_large.implies(diagram_large));
  poly_large = w;
  diagram_large = dd_type(poly_large.set());
  BOOST_CHECK(!diagram_small.implies(diagram_large));
  poly_large = x*y;
  diagram_large = dd_type(poly_large.set());
  BOOST_CHECK(!diagram_small.implies(diagram_large));

  BOOST_TEST_MESSAGE( "==, !=" );
  BoolePolynomial poly1(0);
  BoolePolynomial poly2(0);
  dd_type diag1(poly1.set());
  dd_type diag2(poly2.set());
  BOOST_CHECK(diag1 == diag2);
  BOOST_CHECK_EQUAL(diag1, diag2);
  BOOST_CHECK(!(diag1 != diag2));
  poly1 = 1;
  diag1 = dd_type(poly1.set());
  BOOST_CHECK_NE(diag1, diag2);
  BOOST_CHECK(diag1 != diag2);
  BOOST_CHECK(!(diag1 == diag2));
  poly1 = x;
  poly2 = x;
  diag1 = dd_type(poly1.set());
  diag2 = dd_type(poly2.set());
  BOOST_CHECK_EQUAL(diag1, diag2);
  BOOST_CHECK(diag1 == diag2);
  BOOST_CHECK(!(diag1 != diag2));
  poly1 = x*y;
  poly2 = y*x;
  diag1 = dd_type(poly1.set());
  diag2 = dd_type(poly2.set());
  BOOST_CHECK_EQUAL(diag1, diag2);
  BOOST_CHECK(diag1 == diag2);
  BOOST_CHECK(!(diag1 != diag2));
  poly1 = x + 1;
  poly2 = x;
  diag1 = dd_type(poly1.set());
  diag2 = dd_type(poly2.set());
  BOOST_CHECK_NE(diag1, diag2);
  BOOST_CHECK(diag1 != diag2);
  BOOST_CHECK(!(diag1 == diag2));
}

BOOST_AUTO_TEST_CASE(test_refcount) {

  BoolePolynomial count1 = x*y + z - 1;
  dd_type count2(count1.set());

  BOOST_TEST_MESSAGE( "refCount" );
  BOOST_CHECK_EQUAL(count2.refCount(), 2);
  dd_type count3(count1.set());
  BOOST_CHECK_EQUAL(count2.refCount(), 3);
  BOOST_CHECK_EQUAL(count3.refCount(), 3);
  BOOST_CHECK(count2 == count3);
  BoolePolynomial count4 = count1;
  BOOST_CHECK_EQUAL(count2.refCount(), 4);
  BOOST_CHECK_EQUAL(count3.refCount(), 4);
  BoolePolynomial count5 = x*y + z - 1;
  BOOST_CHECK_EQUAL(count2.refCount(), 5);
  BOOST_CHECK_EQUAL(count3.refCount(), 5);
  count3 = count2;
  BOOST_CHECK_EQUAL(count2.refCount(), 5);
  BOOST_CHECK_EQUAL(count3.refCount(), 5);
  count1 = x*y + z;
  BOOST_CHECK_EQUAL(count2.refCount(), 4);
  BOOST_CHECK_EQUAL(count3.refCount(), 4);
  count4 = 1;
  BOOST_CHECK_EQUAL(count2.refCount(), 3);
  BOOST_CHECK_EQUAL(count3.refCount(), 3);
  count5 = z;
  BOOST_CHECK_EQUAL(count2.refCount(), 2);
  BOOST_CHECK_EQUAL(count3.refCount(), 2);
  BOOST_CHECK(count2 == count3);
  count3 = dd_type(count1.set());
  BOOST_CHECK_EQUAL(count2.refCount(), 1);
  BOOST_CHECK_EQUAL(count3.refCount(), 2);
  BOOST_CHECK(count2 != count3);
}

BOOST_AUTO_TEST_CASE(test_getters) {

  dd_type diagram(poly.set());
  BoolePolyRing other(3, 1, false);
  BoolePolyRing orig = BooleEnv::ring();
  output_test_stream output;

  BOOST_TEST_MESSAGE( "ring" );
  BOOST_CHECK_EQUAL(BooleEnv::ring().ordering().getOrderCode(), diagram.ring().ordering().getOrderCode());
  BOOST_CHECK_EQUAL(BooleEnv::ring().nVariables(), diagram.ring().nVariables());
  BooleEnv::set(other);
  BOOST_CHECK_NE(BooleEnv::ring().ordering().getOrderCode(), diagram.ring().ordering().getOrderCode());
  BOOST_CHECK_NE(BooleEnv::ring().nVariables(), diagram.ring().nVariables());
  BooleEnv::set(orig);
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
  BOOST_CHECK_EQUAL(diagram.nNodes(), 6);
  BOOST_CHECK_EQUAL(one.nNodes(), 0);
  BOOST_CHECK_EQUAL(diagram.rootIndex(), 0);
  BOOST_CHECK_EQUAL(one.rootIndex(), std::numeric_limits<int>::max());
  BOOST_CHECK_EQUAL(diagram.nSupport(), 4);
  BOOST_CHECK_EQUAL(one.nSupport(), 0);

  BoolePolynomial pol(1);
  diagram = dd_type(pol.set());
  BOOST_CHECK_EQUAL(diagram.nNodes(), 0);
  pol = 0;
  diagram = dd_type(pol.set());
  BOOST_CHECK_EQUAL(diagram.nNodes(), 0);
  pol = y + 1;
  diagram = dd_type(pol.set());
  BOOST_CHECK_EQUAL(diagram.nNodes(), 1);
  pol = x;
  diagram = dd_type(pol.set());
  BOOST_CHECK_EQUAL(diagram.nNodes(), 1);
  pol = y;
  diagram = dd_type(pol.set());
  BOOST_CHECK_EQUAL(diagram.nNodes(), 1);
  pol = x*y;
  diagram = dd_type(pol.set());
  BOOST_CHECK_EQUAL(diagram.nNodes(), 2);
  pol = x*y + z;
  diagram = dd_type(pol.set());
  BOOST_CHECK_EQUAL(diagram.nNodes(), 3);
}

BOOST_AUTO_TEST_SUITE_END()
