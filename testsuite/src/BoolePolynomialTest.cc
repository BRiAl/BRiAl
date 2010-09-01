// -*- c++ -*-
//*****************************************************************************
/** @file BoolePolynomialTest.cc
 *
 * @author Alexander Dreyer
 * @date 2010-08-31
 *
 * boost/test-driven unit test
 * 
 * @par Copyright:
 *   (c) 2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#define BOOST_TEST_MODULE BoolePolynomialTest
#include <boost/test/included/unit_test.hpp>
#include <boost/test/output_test_stream.hpp> 
using boost::test_tools::output_test_stream;

#include "pbori_defs.h"
#include "BoolePolynomial.h"
#include "BoolePolyRing.h"
#include "BooleEnv.h"
#include "BooleSet.h"
#include "BooleExponent.h"
#include "BooleVariable.h"
#include "BooleMonomial.h"
#include "PBoRiError.h"
USING_NAMESPACE_PBORI

struct F {
  typedef BoolePolynomial poly_type;
  F():
    ring(100, BoolePolyRing::lp, true), 
    bset(), 
    bexp(BooleExponent().change(1)),
    x(BooleVariable(0)),
    y(BooleVariable(1)),
    z(BooleVariable(2)),
    v(BooleVariable(3)),
    w(BooleVariable(4)) { 

    BOOST_TEST_MESSAGE( "setup fixture" ); 
    bset = BooleSet(1, ring.one(), ring.zero()); // something non-trivial
    ring.setVariableName(0, "x");
    ring.setVariableName(1, "y");
    ring.setVariableName(2, "z");
    ring.setVariableName(3, "v");
    ring.setVariableName(4, "w");

  }

  ~F()  { BOOST_TEST_MESSAGE( "teardown fixture" ); }

  BoolePolyRing ring;
  BooleSet bset;
  BooleExponent bexp;
  BooleMonomial x, y, z, v, w; 
  output_test_stream output;
};

BOOST_FIXTURE_TEST_CASE( test_constructors, F ) {

  BOOST_TEST_MESSAGE( "Constant polynomials..." ); 
  BOOST_CHECK_EQUAL(BoolePolynomial(), BooleEnv::ring().zero());
  BOOST_CHECK_EQUAL(BoolePolynomial(true), BooleEnv::ring().one());
  BOOST_CHECK_EQUAL(BoolePolynomial(false), BooleEnv::ring().zero());
  BOOST_CHECK_EQUAL(BoolePolynomial(true, ring), ring.one());
  BOOST_CHECK_EQUAL(BoolePolynomial(false, ring), ring.zero());

  BOOST_TEST_MESSAGE( "Polynomials from other types (decision diagrams, exponents)..." ); 

  BOOST_CHECK_EQUAL(BoolePolynomial(bset).set(), bset); 
  BOOST_CHECK_EQUAL(BoolePolynomial(bset.navigation(), ring).set(), bset);
  BOOST_CHECK_EQUAL(BoolePolynomial(bexp, ring), bset);
}

BOOST_FIXTURE_TEST_CASE( test_assigning_operators, F ) {

  poly_type poly;
  BOOST_TEST_MESSAGE( "Assignments..." ); 

  BOOST_CHECK_EQUAL(poly = true, BooleEnv::ring().one());
  BOOST_CHECK_EQUAL(poly = false, BooleEnv::ring().zero());
  BOOST_CHECK_EQUAL(poly = bset, bset);
  
  BOOST_TEST_MESSAGE( "+= and *=" );
  
  BOOST_CHECK_EQUAL(BoolePolynomial(x) += x, 0);
  BOOST_CHECK_EQUAL( (BoolePolynomial(x) += y ) += x, y);
  
  output_test_stream output;
  output << ((poly_type(x) *= y) += z);
  BOOST_CHECK(output.is_equal("x*y + z"));
  
   
  output << (poly =  (((poly_type(x) *= v) += (poly_type(y) *= v) ) += z));
  BOOST_CHECK(output.is_equal("x*v + y*v + z"));
  
  output << ( poly *=(poly_type(v)*=y));
  BOOST_CHECK(output.is_equal("x*y*v + y*z*v + y*v"));
  
  BOOST_TEST_MESSAGE( "/= and %=" );
  output << (poly = (poly_type(x*y*z + x*z*w +v) /= BooleMonomial(x*z)));
  BOOST_CHECK_EQUAL(poly, (y + w));
  BOOST_CHECK(output.is_equal("y + w"));
  
  output << (poly = (poly_type(x*y*z + x*z*w +v) /= (((x*z)).exp())));
  BOOST_CHECK_EQUAL(poly, (y + w));
  BOOST_CHECK(output.is_equal("y + w"));
  
  output << (poly = (poly_type(x*y*z + x*z*w +v) /= (x*z + 1)));
  BOOST_CHECK_EQUAL(poly, (y + w));
  BOOST_CHECK(output.is_equal("y + w"));

  output << (poly = (poly_type(x*y*z + x*z*w +v) /= (true)));
  BOOST_CHECK_EQUAL(poly, (x*y*z + x*z*w + v));
  BOOST_CHECK(output.is_equal("x*y*z + x*z*w + v"));
  
  
  output << (poly = (poly_type(x*y*z + x*z*w +v) %= BooleMonomial(x*z)));
  BOOST_CHECK_EQUAL(poly, v);
  BOOST_CHECK(output.is_equal("v"));

  output << (poly = (poly_type(x+x*y*z + y*z*w +v) %= BooleMonomial(y*z)));
  BOOST_CHECK_EQUAL(poly, x+v);
  BOOST_CHECK(output.is_equal("x + v"));
}




BOOST_FIXTURE_TEST_CASE( test_logical_operators, F ) {

  BOOST_CHECK_EQUAL(BoolePolynomial(bset) == BoolePolynomial(bset), true);
  BOOST_CHECK_EQUAL(BoolePolynomial(bset) != BoolePolynomial(bset), false);
  BOOST_CHECK_EQUAL(BoolePolynomial(bset) == BoolePolynomial(), false);
  BOOST_CHECK_EQUAL(BoolePolynomial(bset) != BoolePolynomial(), true);

  BOOST_CHECK_EQUAL(BoolePolynomial(bset) == true, false);
  BOOST_CHECK_EQUAL(BoolePolynomial(bset) == false, false);

  BOOST_CHECK_EQUAL(BoolePolynomial(true) == true, true);
  BOOST_CHECK_EQUAL(BoolePolynomial(true) == false, false);

  BOOST_CHECK_EQUAL(BoolePolynomial(false) == true, false);
  BOOST_CHECK_EQUAL(BoolePolynomial(false) == false, true);


  BOOST_CHECK_EQUAL(true == BoolePolynomial(true), true);
  BOOST_CHECK_EQUAL(false == BoolePolynomial(true), false);
  BOOST_CHECK_EQUAL(true == BoolePolynomial(false), false);
  BOOST_CHECK_EQUAL(false ==BoolePolynomial(false), true);

}


BOOST_FIXTURE_TEST_CASE( test_properties, F ) {

  BOOST_TEST_MESSAGE( "isZero, isOne, isConstant, hasConstantPart" ); 
  BOOST_CHECK_EQUAL(poly_type().isZero(), true);
  BOOST_CHECK_EQUAL(poly_type(0).isZero(), true);
  BOOST_CHECK_EQUAL(poly_type(1).isZero(), false);
  BOOST_CHECK_EQUAL(poly_type(x).isZero(), false);
  BOOST_CHECK_EQUAL(poly_type(x*y).isZero(), false);
  BOOST_CHECK_EQUAL(poly_type(x*y + z).isZero(), false);
  BOOST_CHECK_EQUAL(poly_type(x*y + z + 1).isZero(), false);

  BOOST_CHECK_EQUAL(poly_type().isOne(), false);
  BOOST_CHECK_EQUAL(poly_type(0).isOne(), false);
  BOOST_CHECK_EQUAL(poly_type(1).isOne(), true);
  BOOST_CHECK_EQUAL(poly_type(x).isOne(), false);
  BOOST_CHECK_EQUAL(poly_type(x*y).isOne(), false);
  BOOST_CHECK_EQUAL(poly_type(x*y + z).isOne(), false);
  BOOST_CHECK_EQUAL(poly_type(x*y + z + 1).isOne(), false);

  BOOST_CHECK_EQUAL(poly_type().isConstant(), true);
  BOOST_CHECK_EQUAL(poly_type(0).isConstant(), true);
  BOOST_CHECK_EQUAL(poly_type(1).isConstant(), true);
  BOOST_CHECK_EQUAL(poly_type(x).isConstant(), false);
  BOOST_CHECK_EQUAL(poly_type(x*y).isConstant(), false);
  BOOST_CHECK_EQUAL(poly_type(x*y + z).isConstant(), false);
  BOOST_CHECK_EQUAL(poly_type(x*y + z + 1).isConstant(), false);

  BOOST_CHECK_EQUAL(poly_type().hasConstantPart(), false);
  BOOST_CHECK_EQUAL(poly_type(0).hasConstantPart(), false);
  BOOST_CHECK_EQUAL(poly_type(1).hasConstantPart(), true);
  BOOST_CHECK_EQUAL(poly_type(x).hasConstantPart(), false);
  BOOST_CHECK_EQUAL(poly_type(x*y).hasConstantPart(), false);
  BOOST_CHECK_EQUAL(poly_type(x*y + z).hasConstantPart(), false);
  BOOST_CHECK_EQUAL(poly_type(x*y + z + 1).hasConstantPart(), true);

  BOOST_TEST_MESSAGE( "isSingleton, isSingletonOrPair, isPair");

  BOOST_CHECK_EQUAL(poly_type().isSingleton(), true);
  BOOST_CHECK_EQUAL(poly_type(0).isSingleton(), true);
  BOOST_CHECK_EQUAL(poly_type(1).isSingleton(), true);
  BOOST_CHECK_EQUAL(poly_type(x).isSingleton(), true);
  BOOST_CHECK_EQUAL(poly_type(x*y).isSingleton(), true);
  BOOST_CHECK_EQUAL(poly_type(x*y + z).isSingleton(), false);
  BOOST_CHECK_EQUAL(poly_type(x*y + 1).isSingleton(), false);
  BOOST_CHECK_EQUAL(poly_type(x*y + z + 1).isSingleton(), false);

  BOOST_CHECK_EQUAL(poly_type().isSingletonOrPair(), true);
  BOOST_CHECK_EQUAL(poly_type(0).isSingletonOrPair(), true);
  BOOST_CHECK_EQUAL(poly_type(1).isSingletonOrPair(), true);
  BOOST_CHECK_EQUAL(poly_type(x).isSingletonOrPair(), true);
  BOOST_CHECK_EQUAL(poly_type(x*y).isSingletonOrPair(), true);
  BOOST_CHECK_EQUAL(poly_type(x*y + z).isSingletonOrPair(), true);
  BOOST_CHECK_EQUAL(poly_type(x*y + 1).isSingletonOrPair(), true);
  BOOST_CHECK_EQUAL(poly_type(x*y + z + 1).isSingletonOrPair(), false);

  BOOST_CHECK_EQUAL(poly_type().isPair(), false);
  BOOST_CHECK_EQUAL(poly_type(0).isPair(), false);
  BOOST_CHECK_EQUAL(poly_type(1).isPair(), false);
  BOOST_CHECK_EQUAL(poly_type(x).isPair(), false);
  BOOST_CHECK_EQUAL(poly_type(x*y).isPair(), false);
  BOOST_CHECK_EQUAL(poly_type(x*y + z).isPair(), true);
  BOOST_CHECK_EQUAL(poly_type(x*y + 1).isPair(), true);
  BOOST_CHECK_EQUAL(poly_type(x*y + z + 1).isPair(), false);
}

BOOST_FIXTURE_TEST_CASE( test_ordering_independent, F ) {

  BOOST_CHECK_THROW(poly_type().lexLead(), PBoRiError);
  BOOST_CHECK_THROW(poly_type(0).lexLead(), PBoRiError);

  BOOST_CHECK_EQUAL(poly_type(1).lexLead(), BooleMonomial());
  BOOST_CHECK_EQUAL(poly_type(x).lexLead(), x);
  BOOST_CHECK_EQUAL(poly_type(x*y).lexLead(), x*y);
  BOOST_CHECK_EQUAL(poly_type(x*y + z).lexLead(), x*y);
  BOOST_CHECK_EQUAL(poly_type(x*y + z + 1).lexLead(), x*y);
  BOOST_CHECK_EQUAL(poly_type(x*y + x + z).lexLead(), x*y);
  BOOST_CHECK_EQUAL(poly_type(x*y + x + z + 1).lexLead(), x*y);

  BOOST_CHECK_EQUAL(poly_type(1).stableHash(), 4801919416);
  BOOST_CHECK_EQUAL(poly_type(x).stableHash(), 173100285919);
  BOOST_CHECK_EQUAL(poly_type(x*y).stableHash(), 11091674931773);
  BOOST_CHECK_EQUAL(poly_type(x*y + z).stableHash(), 46073228251744210);
  BOOST_CHECK_EQUAL(poly_type(x*y + z + 1).stableHash(), 3005992135566960638);
  BOOST_CHECK_EQUAL(poly_type(x*y + x + z).stableHash(), 3006002441743652495);
  BOOST_CHECK_EQUAL(poly_type(x*y + x + z + 1).stableHash(), 5907816585472828820);

  BOOST_CHECK_EQUAL(poly_type().deg(), -1);
  BOOST_CHECK_EQUAL(poly_type(0).deg(), -1);
  BOOST_CHECK_EQUAL(poly_type(1).deg(), 0);
  BOOST_CHECK_EQUAL(poly_type(x).deg(), 1);
  BOOST_CHECK_EQUAL(poly_type(x*y).deg(), 2);
  BOOST_CHECK_EQUAL(poly_type(x*y + z).deg(), 2);
  BOOST_CHECK_EQUAL(poly_type(x*y + z + 1).deg(), 2);
  BOOST_CHECK_EQUAL(poly_type(x*y + x + z).deg(), 2);
  BOOST_CHECK_EQUAL(poly_type(x*y + x + z + 1).deg(), 2);
  BOOST_CHECK_EQUAL(poly_type(x + y*z + 1).deg(), 2);
  BOOST_CHECK_EQUAL(poly_type(x + y*z + z).deg(), 2);
  BOOST_CHECK_EQUAL(poly_type(x + y*z + z*v*w ).deg(), 3);

  poly_type poly1(1), poly2(1);
  std::size_t len(1);
    for (std::size_t idx = 0; idx < ring.nVariables(); ++idx, (len <<= 1)) {
    BOOST_CHECK_EQUAL(poly1.length(), len);
    poly1 *= (poly_type(ring.variable(idx)) + 1);
    BOOST_CHECK_EQUAL(poly1.deg(), idx + 1);
    if (idx > 0) {
      poly2 *= (ring.variable(idx) + ring.variable(0));
      BOOST_CHECK_EQUAL(poly2.deg(), idx);
      BOOST_CHECK_EQUAL(poly2.length(), len);
     
    }
  }

#if 0


/// Maximal degree of the polynomial
  deg_type deg() const;

  /// Total maximal degree of the polynomial
 /// Number of nodes in the decision diagram
  size_type nNodes() const;

  /// Number of variables of the polynomial
  size_type nUsedVariables() const;

  /// Set of variables of the polynomial
  monom_type usedVariables() const;

  /// Exponent vector of all of variables of the polynomial
  exp_type usedVariablesExp() const;

  /// Returns number of terms
  size_type length() const;

   deg_type totalDeg() const;
 /// Get list of all terms
  void fetchTerms(termlist_type&) const;

  /// Return of all terms
  termlist_type terms() const;

  hash_type hash() const { return m_dd.hash(); }
  /// Read-only access to int
#endif
}
