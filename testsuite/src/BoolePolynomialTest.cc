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
#include "LexOrder.h"
#include "CExpIter.h"

USING_NAMESPACE_PBORI

struct F {
  typedef BoolePolynomial poly_type;
  F():
    ring(100, BoolePolyRing::lp, true), 
    bset(), 
    bexp(BooleExponent().change(1)) {
    x = (BooleVariable(0));
    y = (BooleVariable(1));
    z = (BooleVariable(2));
    v = (BooleVariable(3));
    w = (BooleVariable(4));

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

BOOST_FIXTURE_TEST_SUITE(BoolePolynomialTestSuite, F )

BOOST_AUTO_TEST_CASE(test_constructors) {

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

BOOST_AUTO_TEST_CASE(test_assigning_operators) {

  poly_type poly;
  BOOST_TEST_MESSAGE( "Assignments..." ); 

  BOOST_CHECK_EQUAL(poly = true, BooleEnv::ring().one());
  BOOST_CHECK_EQUAL(poly = false, BooleEnv::ring().zero());
  BOOST_CHECK_EQUAL(poly = bset, bset);
  
  BOOST_TEST_MESSAGE( "+=, -= and *=" );
  
  BOOST_CHECK_EQUAL(BoolePolynomial(x) += x, 0);
  BOOST_CHECK_EQUAL( (BoolePolynomial(x) += y ) += x, y);
  
  output_test_stream output;
  output << ((poly_type(x) *= y) += z);
  BOOST_CHECK(output.is_equal("x*y + z"));
  
   
  output << (poly =  (((poly_type(x) *= v) += (poly_type(y) *= v) ) += z));
  BOOST_CHECK(output.is_equal("x*v + y*v + z"));

  BOOST_CHECK_EQUAL(BoolePolynomial(x) -= x, 0);
  BOOST_CHECK_EQUAL( (BoolePolynomial(x) -= y ) -= x, y);
  
  output << ((poly_type(x) *= y) -= z);
  BOOST_CHECK(output.is_equal("x*y + z"));
  
   
  output << (poly =  (((poly_type(x) *= v) -= (poly_type(y) *= v) ) -= z));
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


  BOOST_CHECK_EQUAL(-(x*y*z + x*z*w +v), (x*y*z + x*z*w +v));
}




BOOST_AUTO_TEST_CASE(test_logical_operators) {

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


BOOST_AUTO_TEST_CASE(test_properties) {

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

BOOST_AUTO_TEST_CASE(test_ordering_independent) {

  BOOST_TEST_MESSAGE( "lexLead");

  BOOST_CHECK_THROW(poly_type().lexLead(), PBoRiError);
  BOOST_CHECK_THROW(poly_type(0).lexLead(), PBoRiError);

  BOOST_CHECK_EQUAL(poly_type(1).lexLead(), BooleMonomial());
  BOOST_CHECK_EQUAL(poly_type(x).lexLead(), x);
  BOOST_CHECK_EQUAL(poly_type(x*y).lexLead(), x*y);
  BOOST_CHECK_EQUAL(poly_type(x*y + z).lexLead(), x*y);
  BOOST_CHECK_EQUAL(poly_type(x*y + z + 1).lexLead(), x*y);
  BOOST_CHECK_EQUAL(poly_type(x*y + x + z).lexLead(), x*y);
  BOOST_CHECK_EQUAL(poly_type(x*y + x + z + 1).lexLead(), x*y);

  BOOST_TEST_MESSAGE( "stableHash");
  BOOST_CHECK_EQUAL(poly_type(1).stableHash(), 4801919416);
  BOOST_CHECK_EQUAL(poly_type(x).stableHash(), 173100285919);
  BOOST_CHECK_EQUAL(poly_type(x*y).stableHash(), 11091674931773);
  BOOST_CHECK_EQUAL(poly_type(x*y + z).stableHash(), 46073228251744210);
  BOOST_CHECK_EQUAL(poly_type(x*y + z + 1).stableHash(), 3005992135566960638);
  BOOST_CHECK_EQUAL(poly_type(x*y + x + z).stableHash(), 3006002441743652495);
  BOOST_CHECK_EQUAL(poly_type(x*y + x + z + 1).stableHash(), 5907816585472828820);

  BOOST_TEST_MESSAGE( "deg, TotalDeg, length, nNodes, nUsedVariables, usedVariables");
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

  // Should be the same up to now
  BOOST_CHECK_EQUAL(poly_type().totalDeg(), -1);
  BOOST_CHECK_EQUAL(poly_type(0).totalDeg(), -1);
  BOOST_CHECK_EQUAL(poly_type(1).totalDeg(), 0);
  BOOST_CHECK_EQUAL(poly_type(x).totalDeg(), 1);
  BOOST_CHECK_EQUAL(poly_type(x*y).totalDeg(), 2);
  BOOST_CHECK_EQUAL(poly_type(x*y + z).totalDeg(), 2);
  BOOST_CHECK_EQUAL(poly_type(x*y + z + 1).totalDeg(), 2);
  BOOST_CHECK_EQUAL(poly_type(x*y + x + z).totalDeg(), 2);
  BOOST_CHECK_EQUAL(poly_type(x*y + x + z + 1).totalDeg(), 2);
  BOOST_CHECK_EQUAL(poly_type(x + y*z + 1).totalDeg(), 2);
  BOOST_CHECK_EQUAL(poly_type(x + y*z + z).totalDeg(), 2);
  BOOST_CHECK_EQUAL(poly_type(x + y*z + z*v*w ).totalDeg(), 3);

  poly_type poly1(1), poly2(1);
  std::size_t len(1);
  BooleMonomial monom;
  for (std::size_t idx = 0; idx < ring.nVariables(); ++idx, (len <<= 1)) {
    monom = monom.change(idx);
    BOOST_CHECK_EQUAL(poly1.length(), len);
    poly1 *= (poly_type(ring.variable(idx)) + 1);
    BOOST_CHECK_EQUAL(poly1.deg(), idx + 1);
    BOOST_CHECK_EQUAL(poly1.totalDeg(), idx + 1);
    BOOST_CHECK_EQUAL(poly1.nNodes(), idx + 2);
    BOOST_CHECK_EQUAL(poly1.nUsedVariables(), idx + 1);
    BOOST_CHECK_EQUAL(poly1.usedVariables(), monom);
    BOOST_CHECK_EQUAL(poly1.usedVariablesExp(), monom.exp());

    if (idx > 0) {
      poly2 *= (ring.variable(idx) + ring.variable(0));
      BOOST_CHECK_EQUAL(poly2.deg(), idx);
      BOOST_CHECK_EQUAL(poly2.totalDeg(), idx);
      BOOST_CHECK_EQUAL(poly2.length(), len);
      BOOST_CHECK_EQUAL(poly2.nNodes(), 3*idx+1);
      BOOST_CHECK_EQUAL(poly2.nUsedVariables(), idx + 1); 
      BOOST_CHECK_EQUAL(poly2.usedVariables(), monom);
      BOOST_CHECK_EQUAL(poly2.usedVariablesExp(), monom.exp());
    }
  }

  BOOST_TEST_MESSAGE( "hash, terms, begin*, end*, navigation");
  BOOST_CHECK_NE(poly1.hash(), poly2.hash());
  BOOST_CHECK_EQUAL((x + y*z + z*v*w).hash(),(x + y*z + x + z*v*w + x).hash());

  poly_type::termlist_type terms;
  (x + y*z + z*v*w).fetchTerms(terms);

  BooleMonomial termsref[] = {x, y*z, z*v*w};

  BOOST_CHECK_EQUAL_COLLECTIONS(terms.begin(), terms.end(), 
                                termsref, termsref + 3);
  terms = (x + y*z + z*v*w).terms();
  BOOST_CHECK_EQUAL_COLLECTIONS(terms.begin(), terms.end(), 
                                termsref, termsref + 3);
  poly_type poly = (x + y*z + z*v*w);
  poly_type::deg_type degs[] = {1, 2, 3};
  poly_type::exp_type 
    exps[] = {BooleExponent().change(0), 
              BooleExponent().change(1).change(2),
              BooleExponent().change(2).change(3).change(4)};
  
  BOOST_CHECK_EQUAL_COLLECTIONS(poly.begin(), poly.end(), 
                                termsref, termsref + 3);

  BOOST_CHECK_EQUAL_COLLECTIONS(poly.degBegin(), poly.degEnd(), 
                                degs, degs + 3);
  BOOST_CHECK_EQUAL_COLLECTIONS(poly.expBegin(), poly.expEnd(), 
                                exps, exps + 3);

  BOOST_CHECK_EQUAL(poly_type(x*y + z*v + v*w).firstTerm(), x*y);

  poly = (x*y + z*v + v*w);
  poly_type::idx_type idxs[] = {0, 1};
  BOOST_CHECK_EQUAL_COLLECTIONS(poly.firstBegin(), poly.firstEnd(), 
                                idxs, idxs + 2);

  
  BOOST_CHECK_EQUAL(*poly.navigation(), 0);
  BOOST_CHECK_EQUAL(*(poly.navigation().thenBranch()), 1);
  BOOST_CHECK_EQUAL(*(poly.navigation().elseBranch()), 2);
  BOOST_CHECK_EQUAL(*(poly.navigation().elseBranch().thenBranch()), 3);
  BOOST_CHECK_EQUAL(*(poly.navigation().elseBranch().elseBranch()), 3);
  BOOST_CHECK_EQUAL(*(poly.navigation().elseBranch().elseBranch().thenBranch()),
                    4);


  BOOST_TEST_MESSAGE( "gradedPArt, set, print" );
  BOOST_CHECK_EQUAL(poly_type(x + z*v + z*v*w + 1).gradedPart(0), 1);
  BOOST_CHECK_EQUAL(poly_type(x + z*v + z*v*w + 1).gradedPart(1), x);
  BOOST_CHECK_EQUAL(poly_type(x + z*v + z*v*w + 1).gradedPart(2), z*v);
  BOOST_CHECK_EQUAL(poly_type(x + z*v + z*v*w + 1).gradedPart(3), z*v*w);

  output << poly_type(x + z*v + z*v*w + 1).set();
  BOOST_CHECK(output.is_equal("{{x}, {z,v,w}, {z,v}, {}}"));

  output << poly_type(x + z*v + z*v*w + 1);
  BOOST_CHECK(output.is_equal("x + z*v*w + z*v + 1"));
}

template <int order>
struct OrderGenFix {
  typedef BoolePolynomial poly_type;
  OrderGenFix():
    ring(100, order, true) {

    ring.setVariableName(0, "a");
    ring.setVariableName(1, "b");
    ring.setVariableName(2, "c");
    ring.setVariableName(3, "d");
    ring.setVariableName(4, "e");
    ring.setVariableName(5, "f");
    ring.setVariableName(6, "g");
    ring.setVariableName(7, "h");
 
    a = BooleVariable(0, ring);
    b = BooleVariable(1, ring);
    c = BooleVariable(2, ring);
    d = BooleVariable(3, ring);
    e = BooleVariable(4, ring);
    f = BooleVariable(5, ring);
    g = BooleVariable(6, ring);
    h = BooleVariable(7, ring);


    poly1 = a + b + c*d*e + c*e*f + g*h;
    poly2 = a + b + c*d + e*f + g*h;



  }
  ~OrderGenFix()  { BOOST_TEST_MESSAGE( "teardown fixture" ); }

  BoolePolyRing ring;
  BooleMonomial a, b, c, d, e, f, g, h;
  output_test_stream output;
  poly_type poly1, poly2;
};


BOOST_FIXTURE_TEST_CASE(test_ordering_lp, OrderGenFix<BoolePolyRing::lp>) {

  BOOST_CHECK_THROW(poly_type().lead(), PBoRiError);
  BOOST_CHECK_THROW(poly_type(0).lead(), PBoRiError);

  BOOST_CHECK_EQUAL(poly1.lead(), a);
  BOOST_CHECK_EQUAL(poly2.lead(), a);

  BOOST_CHECK_EQUAL(poly1.leadExp(), BooleExponent().change(0));
  BOOST_CHECK_EQUAL(poly2.leadExp(), BooleExponent().change(0));

  BOOST_CHECK_EQUAL(poly1.boundedLead(1), a);
  BOOST_CHECK_EQUAL(poly2.boundedLead(1), a);

  BOOST_CHECK_EQUAL(poly1.boundedLeadExp(1), BooleExponent().change(0));
  BOOST_CHECK_EQUAL(poly2.boundedLeadExp(1), BooleExponent().change(0));

  BOOST_CHECK_EQUAL(poly1.leadDeg(), 1);
  BOOST_CHECK_EQUAL(poly2.leadDeg(), 1);

  BOOST_CHECK(poly1.reducibleBy(poly_type(a+b)));

#if 0
  set_type leadDivisors() const { return leadFirst().firstDivisors(); };
  hash_type leadStableHash() const;

  /// Start of ordering respecting iterator
  ordered_iterator orderedBegin() const; 

  /// Finish of ordering respecting iterator
  ordered_iterator orderedEnd() const;

   /// Start of ordering respecting exponent iterator
  ordered_exp_iterator orderedExpBegin() const; 

  /// Finish of ordering respecting exponent iterator
  ordered_exp_iterator orderedExpEnd() const;

  /// @name Compile-time access to generic iterators
  //@{
  lex_iterator genericBegin(lex_tag) const;
  lex_iterator genericEnd(lex_tag) const;
  dlex_iterator genericBegin(dlex_tag) const;
  dlex_iterator genericEnd(dlex_tag) const;
  dp_asc_iterator genericBegin(dp_asc_tag) const;
  dp_asc_iterator genericEnd(dp_asc_tag) const;
  block_dlex_iterator genericBegin(block_dlex_tag) const;
  block_dlex_iterator genericEnd(block_dlex_tag) const;
  block_dp_asc_iterator genericBegin(block_dp_asc_tag) const;
  block_dp_asc_iterator genericEnd(block_dp_asc_tag) const;


  lex_exp_iterator genericExpBegin(lex_tag) const;
  lex_exp_iterator genericExpEnd(lex_tag) const;
  dlex_exp_iterator genericExpBegin(dlex_tag) const;
  dlex_exp_iterator genericExpEnd(dlex_tag) const;
  dp_asc_exp_iterator genericExpBegin(dp_asc_tag) const;
  dp_asc_exp_iterator genericExpEnd(dp_asc_tag) const;
  block_dlex_exp_iterator genericExpBegin(block_dlex_tag) const;
  block_dlex_exp_iterator genericExpEnd(block_dlex_tag) const;
  block_dp_asc_exp_iterator genericExpBegin(block_dp_asc_tag) const;
  block_dp_asc_exp_iterator genericExpEnd(block_dp_asc_tag) const; 
 size_type eliminationLength() const;
  size_type eliminationLengthWithDegBound(deg_type garantied_deg_bound) const;
  //@}
#endif
}


BOOST_AUTO_TEST_SUITE_END()
