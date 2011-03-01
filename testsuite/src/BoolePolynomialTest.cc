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

#include <boost/test/unit_test.hpp>
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
#include "DegLexOrder.h"
#include "DegRevLexAscOrder.h"
#include "BlockDegLexOrder.h"
#include "BlockDegRevLexAscOrder.h"

#include <iterators/CExpIter.h>

USING_NAMESPACE_PBORI


struct Fpoly {
  typedef BoolePolynomial poly_type;
  Fpoly():
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

  ~Fpoly()  { BOOST_TEST_MESSAGE( "teardown fixture" ); }

  BoolePolyRing ring;
  BooleSet bset;
  BooleExponent bexp;
  BooleMonomial x, y, z, v, w; 
  output_test_stream output;
};


BOOST_FIXTURE_TEST_SUITE(BoolePolynomialTestSuite, Fpoly )

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
  
  BOOST_CHECK_EQUAL(BoolePolynomial() *= BooleExponent(x*y), 0);
  BOOST_CHECK_EQUAL(BoolePolynomial(z*w) *= BooleExponent(x*y), x*y*z*w);
  
  BOOST_CHECK_EQUAL(BoolePolynomial(x) += x, 0);
  BOOST_CHECK_EQUAL( (BoolePolynomial(x) += y ) += x, y);
  
  output_test_stream output;
  output << ((poly_type(x) *= y) += z);
  BOOST_CHECK(output.is_equal("x*y + z"));
  
  output << (poly_type(x) += BooleConstant(0));
  BOOST_CHECK(output.is_equal("x"));
  
  output << (poly_type(x) *= BooleConstant(0));
  BOOST_CHECK(output.is_equal("0"));
   
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


  BOOST_CHECK_EQUAL((x*z + y) *= (x*y).exp() ,  x*y*z +x*y);
  BOOST_CHECK_EQUAL((x*z + z) *= (x*y).exp() ,  0);

  BOOST_CHECK_EQUAL((x*y) * (x*z + y), x*y*z +x*y);
  BOOST_CHECK_EQUAL((x*z + z) *= x*y ,  0);

  BOOST_CHECK_EQUAL(poly_type(0) *= (x*y).exp(),  0);
  BOOST_CHECK_EQUAL((x*y) * poly_type(0), 0);  

  BOOST_CHECK_EQUAL(poly_type(1) *= (x*y).exp(),  x*y);
  BOOST_CHECK_EQUAL((x*y) * poly_type(1), x*y);  

  BOOST_CHECK_EQUAL(poly_type(x + y) *= v*w, x*v*w + y*v*w);
  BOOST_CHECK_EQUAL(poly_type(x + y) *= (v*w).exp(), x*v*w + y*v*w);

  BOOST_CHECK_EQUAL(poly_type(v + w) *= (x*y).exp(), x*y*v + x*y*w);
  BOOST_CHECK_EQUAL(poly_type(v + w) *= (x*v).exp(), x*v + x*v*w);

  BOOST_TEST_MESSAGE( "/= and %=" );
  
  BOOST_CHECK_THROW(BoolePolynomial(x) /= BoolePolynomial(), PBoRiGenericError<CTypes::division_by_zero>);
  BOOST_CHECK_THROW(BoolePolynomial(x) /= (false), PBoRiGenericError<CTypes::division_by_zero>);
  
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
  
  output << (poly = (poly_type(0) /= (x*y)));
  BOOST_CHECK_EQUAL(poly, 0);
  BOOST_CHECK(output.is_equal("0"));

  output << (poly = (poly_type(1) /= (x*y)));
  BOOST_CHECK_EQUAL(poly, 0);
  BOOST_CHECK(output.is_equal("0"));

  output << (poly = (poly_type(0) /= (x*y).exp()));
  BOOST_CHECK_EQUAL(poly, 0);
  BOOST_CHECK(output.is_equal("0"));

  output << (poly = (poly_type(1) /= (x*y).exp()));
  BOOST_CHECK_EQUAL(poly, 0);
  BOOST_CHECK(output.is_equal("0"));

  output << (poly = (poly_type(v+w) /= (x*y).exp()));
  BOOST_CHECK_EQUAL(poly, 0);
  BOOST_CHECK(output.is_equal("0"));

  output << (poly = (poly_type(v+w) /= (x*y)));
  BOOST_CHECK_EQUAL(poly, 0);
  BOOST_CHECK(output.is_equal("0"));

  output << (poly = (poly_type(x*v + x*w) /= (x*y).exp()));
  BOOST_CHECK_EQUAL(poly, 0);
  BOOST_CHECK(output.is_equal("0"));

  output << (poly = (poly_type(x*v + x*w) /= (x*y)));
  BOOST_CHECK_EQUAL(poly, 0);
  BOOST_CHECK(output.is_equal("0"));


  output << (poly = (poly_type(x*y*z + x*z*w +v) %= BooleMonomial(x*z)));
  BOOST_CHECK_EQUAL(poly, v);
  BOOST_CHECK(output.is_equal("v"));

  output << (poly = (poly_type(x+x*y*z + y*z*w +v) %= BooleMonomial(y*z)));
  BOOST_CHECK_EQUAL(poly, x+v);
  BOOST_CHECK(output.is_equal("x + v"));

  output << (poly = (poly_type(x+x*y*z + y*z*w +v) %= BooleVariable(1)));
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
  BOOST_CHECK_EQUAL(poly_type().lexLeadDeg(), -1);

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

  BOOST_CHECK_EQUAL(poly_type().leadTotalDeg(), -1);
  BOOST_CHECK_EQUAL(poly_type(0).leadTotalDeg(), -1);
  BOOST_CHECK_EQUAL(poly_type(1).leadTotalDeg(), 0);
  BOOST_CHECK_EQUAL(poly_type(x).leadTotalDeg(), 1);
  BOOST_CHECK_EQUAL(poly_type(x*y).leadTotalDeg(), 2);
  BOOST_CHECK_EQUAL(poly_type(x*y + z).leadTotalDeg(), 2);
  BOOST_CHECK_EQUAL(poly_type(x*y + z + 1).leadTotalDeg(), 2);
  BOOST_CHECK_EQUAL(poly_type(x*y + x + z).leadTotalDeg(), 2);
  BOOST_CHECK_EQUAL(poly_type(x*y + x + z + 1).leadTotalDeg(), 2);
  BOOST_CHECK_EQUAL(poly_type(x + y*z + 1).leadTotalDeg(), 1);
  BOOST_CHECK_EQUAL(poly_type(x + y*z + z).leadTotalDeg(), 1);
  BOOST_CHECK_EQUAL(poly_type(x + y*z + z*v*w ).leadTotalDeg(), 1);

  BOOST_CHECK(poly_type(x + y + y*v*w + v*w).firstReducibleBy(poly_type(x+y)));

  BOOST_CHECK(!poly_type(x + y + y*v*w + v*w).firstReducibleBy(poly_type(v+w)));

  BOOST_CHECK(!poly_type().firstReducibleBy(poly_type(x + y + y*v*w + v*w)));

  BOOST_CHECK(poly_type(x + y + y*v*w + v*w).firstReducibleBy(poly_type(1)));

  poly_type poly1(1), poly2(1);
  std::size_t len(1);
  BooleMonomial monom;
  for (std::size_t idx = 0; idx < ring.nVariables(); ++idx, (len <<= 1)) {
    monom = monom.change(idx);
    BOOST_CHECK_EQUAL(poly1.length(), len);
    poly1 *= (poly_type(ring.variable(idx)) + 1);
    BOOST_CHECK_EQUAL(poly1.deg(), idx + 1);
    BOOST_CHECK_EQUAL(poly1.totalDeg(), idx + 1);
    BOOST_CHECK_EQUAL(poly1.nNodes(), idx + 1);
    BOOST_CHECK_EQUAL(poly1.nUsedVariables(), idx + 1);
    BOOST_CHECK_EQUAL(poly1.usedVariables(), monom);
    BOOST_CHECK_EQUAL(poly1.usedVariablesExp(), monom.exp());

    if (idx > 0) {
      poly2 *= (ring.variable(idx) + ring.variable(0));
      BOOST_CHECK_EQUAL(poly2.deg(), idx);
      BOOST_CHECK_EQUAL(poly2.totalDeg(), idx);
      BOOST_CHECK_EQUAL(poly2.length(), len);
      BOOST_CHECK_EQUAL(poly2.nNodes(), 3*idx-1);
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

  BooleMonomial termsref[] = {x, y*z, z*v*w, BooleMonomial()};

  BOOST_CHECK_EQUAL_COLLECTIONS(terms.begin(), terms.end(), 
                                termsref, termsref + 3);
  terms = (x + y*z + z*v*w).terms();
  BOOST_CHECK_EQUAL_COLLECTIONS(terms.begin(), terms.end(), 
                                termsref, termsref + 3);
  poly_type poly = (x + y*z + z*v*w);
  poly_type::deg_type degs[] = {1, 2, 3, 0};
  poly_type::exp_type 
    exps[] = {BooleExponent().change(0), 
              BooleExponent().change(1).change(2),
              BooleExponent().change(2).change(3).change(4),
              BooleExponent()};
  
  BOOST_CHECK_EQUAL_COLLECTIONS(poly.begin(), poly.end(), 
                                termsref, termsref + 3);

  BOOST_CHECK_EQUAL_COLLECTIONS(poly.degBegin(), poly.degEnd(), 
                                degs, degs + 3);
  BOOST_CHECK_EQUAL_COLLECTIONS(poly.expBegin(), poly.expEnd(), 
                                exps, exps + 3);

  BOOST_CHECK_EQUAL(poly_type(x*y + z*v + v*w).firstTerm(), x*y);


  poly = (x + y*z + z*v*w + 1); // same as the latter, but  + 1
  BOOST_CHECK_EQUAL_COLLECTIONS(poly.begin(), poly.end(), 
                                termsref, termsref + 4);

  BOOST_CHECK_EQUAL_COLLECTIONS(poly.degBegin(), poly.degEnd(), 
                                degs, degs + 4);
  BOOST_CHECK_EQUAL_COLLECTIONS(poly.expBegin(), poly.expEnd(), 
                                exps, exps + 4);

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
  // repeat for covering sucessfull cache-calls
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

  BOOST_CHECK_EQUAL(poly_type().leadDeg(), -1);
  BOOST_CHECK_EQUAL(poly1.leadDeg(), 1);
  BOOST_CHECK_EQUAL(poly2.leadDeg(), 1);

  output << poly_type(a*b*d*f + a*b + a + g).leadDivisors();
  BOOST_CHECK(output.is_equal("{{a,b,d,f}, {a,b,d}, {a,b,f}, {a,b}, {a,d,f}, "
                              "{a,d}, {a,f}, {a}, {b,d,f}, {b,d}, {b,f}, {b}, "
                              "{d,f}, {d}, {f}, {}}"));

  BOOST_CHECK_EQUAL(poly_type(1).leadStableHash(), 4801919416);
  BOOST_CHECK_EQUAL(poly_type(a).leadStableHash(), 173100285919);
  BOOST_CHECK_EQUAL(poly_type(a*b).leadStableHash(), 11091674931773);
  BOOST_CHECK_EQUAL(poly_type(c*d +c + e + 1).leadStableHash(),
                    11091674972829);

  BOOST_CHECK_EQUAL(poly1.leadStableHash(), 173100285919);
  BOOST_CHECK_EQUAL(poly2.leadStableHash(), 173100285919);
  // naturally lexicographicsl

  BOOST_CHECK_EQUAL_COLLECTIONS(poly1.begin(), poly1.end(), 
                                poly1.orderedBegin(), poly1.orderedEnd());

  BOOST_CHECK_EQUAL_COLLECTIONS(poly2.begin(), poly2.end(), 
                                poly2.orderedBegin(), poly2.orderedEnd());

  BooleMonomial terms1[] = {a, b, c*d*e, c*e*f, g*h};
  BooleMonomial terms2[] = {a, b, c*d, e*f, g*h};

  BooleExponent exps1[] = {terms1[0].exp(), terms1[1].exp(), terms1[2].exp(),
                           terms1[3].exp(), terms1[4].exp()};
  BooleExponent exps2[] = {terms2[0].exp(), terms2[1].exp(), terms2[2].exp(),
                           terms2[3].exp(), terms2[4].exp()};

  BOOST_CHECK_EQUAL_COLLECTIONS(poly1.orderedBegin(), poly1.orderedEnd(),
                                terms1, terms1 + 5);

  BOOST_CHECK_EQUAL_COLLECTIONS(poly2.orderedBegin(), poly2.orderedEnd(),
                                terms2, terms2 + 5);

  BOOST_CHECK_EQUAL_COLLECTIONS(poly1.orderedExpBegin(), poly1.orderedExpEnd(),
                                exps1, exps1 + 5);

  BOOST_CHECK_EQUAL_COLLECTIONS(poly2.orderedExpBegin(), poly2.orderedExpEnd(),
                                exps2, exps2 + 5);




  BOOST_CHECK_EQUAL_COLLECTIONS(poly1.genericBegin(lex_tag()), poly1.genericEnd(lex_tag()),
                                terms1, terms1 + 5);

  BOOST_CHECK_EQUAL_COLLECTIONS(poly2.genericBegin(lex_tag()), poly2.genericEnd(lex_tag()),
                                terms2, terms2 + 5);


  BOOST_CHECK_EQUAL_COLLECTIONS(poly1.genericExpBegin(lex_tag()), poly1.genericExpEnd(lex_tag()),
                                exps1, exps1 + 5);

  BOOST_CHECK_EQUAL_COLLECTIONS(poly2.genericExpBegin(lex_tag()), poly2.genericExpEnd(lex_tag()),
                                exps2, exps2 + 5);

  BOOST_CHECK_EQUAL(poly_type().eliminationLength(), 0);
  BOOST_CHECK_EQUAL(poly1.eliminationLength(), 10);
  BOOST_CHECK_EQUAL(poly2.eliminationLength(), 8);
  BOOST_CHECK_EQUAL(poly1.eliminationLengthWithDegBound(3), 10);
  BOOST_CHECK_EQUAL(poly2.eliminationLengthWithDegBound(2), 8);

  BOOST_CHECK_EQUAL(poly1.compare(poly1), CTypes::equality);
  BOOST_CHECK_EQUAL(poly1.compare(poly2), CTypes::greater_than);
  BOOST_CHECK_EQUAL(poly2.compare(poly1), CTypes::less_than);
  BOOST_CHECK_EQUAL(poly1.compare(poly_type()), CTypes::greater_than);
  BOOST_CHECK_EQUAL(poly_type().compare(poly2), CTypes::less_than);
}

BOOST_FIXTURE_TEST_CASE(test_ordering_dlex, OrderGenFix<BoolePolyRing::dlex>) {

  BOOST_CHECK_THROW(poly_type().lead(), PBoRiError);
  BOOST_CHECK_THROW(poly_type(0).lead(), PBoRiError);

  BOOST_CHECK_EQUAL(poly1.lead(), c*d*e);
  BOOST_CHECK_EQUAL(poly2.lead(), c*d);

  BOOST_CHECK_EQUAL(poly1.leadExp(), (c*d*e).exp());
  BOOST_CHECK_EQUAL(poly2.leadExp(), (c*d).exp());

  BOOST_CHECK_EQUAL(poly1.boundedLead(3), c*d*e);
  BOOST_CHECK_EQUAL(poly2.boundedLead(3), c*d);

  BOOST_CHECK_EQUAL(poly1.boundedLeadExp(3), (c*d*e).exp());
  BOOST_CHECK_EQUAL(poly2.boundedLeadExp(3), (c*d).exp());


  BOOST_CHECK_EQUAL(poly1.leadDeg(), 3);
  BOOST_CHECK_EQUAL(poly2.leadDeg(), 2);

  output << poly_type(a*b*d*f + a*b + a + g).leadDivisors();
  BOOST_CHECK(output.is_equal("{{a,b,d,f}, {a,b,d}, {a,b,f}, {a,b}, {a,d,f}, "
                              "{a,d}, {a,f}, {a}, {b,d,f}, {b,d}, {b,f}, {b}, "
                              "{d,f}, {d}, {f}, {}}"));

  BOOST_CHECK_EQUAL(poly_type(1).leadStableHash(), 4801919416);
  BOOST_CHECK_EQUAL(poly_type(a).leadStableHash(), 173100285919);
  BOOST_CHECK_EQUAL(poly_type(a*b).leadStableHash(), 11091674931773);
  BOOST_CHECK_EQUAL(poly_type(c*d +c + e + 1).leadStableHash(),
                    11091674972829);

  BOOST_CHECK_EQUAL(poly1.leadStableHash(), 706244161997016);
  BOOST_CHECK_EQUAL(poly2.leadStableHash(),11091674972829); 


  BooleMonomial terms1[] = {c*d*e, c*e*f, g*h, a, b};
  BooleMonomial terms2[] = {c*d, e*f, g*h, a, b};

  BooleExponent exps1[] = {terms1[0].exp(), terms1[1].exp(), terms1[2].exp(),
                           terms1[3].exp(), terms1[4].exp()};
  BooleExponent exps2[] = {terms2[0].exp(), terms2[1].exp(), terms2[2].exp(),
                           terms2[3].exp(), terms2[4].exp()};

  BOOST_CHECK_EQUAL_COLLECTIONS(poly1.orderedBegin(), poly1.orderedEnd(),
                                terms1, terms1 + 5);

  BOOST_CHECK_EQUAL_COLLECTIONS(poly2.orderedBegin(), poly2.orderedEnd(),
                                terms2, terms2 + 5);

  BOOST_CHECK_EQUAL_COLLECTIONS(poly1.orderedExpBegin(), poly1.orderedExpEnd(),
                                exps1, exps1 + 5);

  BOOST_CHECK_EQUAL_COLLECTIONS(poly2.orderedExpBegin(), poly2.orderedExpEnd(),
                                exps2, exps2 + 5);




  BOOST_CHECK_EQUAL_COLLECTIONS(poly1.genericBegin(dlex_tag()), poly1.genericEnd(dlex_tag()),
                                terms1, terms1 + 5);

  BOOST_CHECK_EQUAL_COLLECTIONS(poly2.genericBegin(dlex_tag()), poly2.genericEnd(dlex_tag()),
                                terms2, terms2 + 5);


  BOOST_CHECK_EQUAL_COLLECTIONS(poly1.genericExpBegin(dlex_tag()), poly1.genericExpEnd(dlex_tag()),
                                exps1, exps1 + 5);

  BOOST_CHECK_EQUAL_COLLECTIONS(poly2.genericExpBegin(dlex_tag()), poly2.genericExpEnd(dlex_tag()),
                                exps2, exps2 + 5);

  BOOST_CHECK_EQUAL(poly_type().eliminationLength(), 0);
  BOOST_CHECK_EQUAL(poly1.eliminationLength(), 5);
  BOOST_CHECK_EQUAL(poly2.eliminationLength(), 5);
  BOOST_CHECK_EQUAL(poly1.eliminationLengthWithDegBound(3), 5);
  BOOST_CHECK_EQUAL(poly2.eliminationLengthWithDegBound(2), 5);

  BOOST_CHECK_EQUAL(poly1.compare(poly1), CTypes::equality);
  BOOST_CHECK_EQUAL(poly1.compare(poly2), CTypes::greater_than);
  BOOST_CHECK_EQUAL(poly2.compare(poly1), CTypes::less_than);
  BOOST_CHECK_EQUAL(poly1.compare(poly_type()), CTypes::greater_than);
  BOOST_CHECK_EQUAL(poly_type().compare(poly2), CTypes::less_than);
}

BOOST_FIXTURE_TEST_CASE(test_ordering_dp_asc, OrderGenFix<BoolePolyRing::dp_asc>) {

  BOOST_CHECK_THROW(poly_type().lead(), PBoRiError);
  BOOST_CHECK_THROW(poly_type(0).lead(), PBoRiError);

  BOOST_CHECK_EQUAL(poly1.lead(), c*e*f);
  BOOST_CHECK_EQUAL(poly2.lead(), g*h);

  BOOST_CHECK_EQUAL(poly1.leadExp(), poly1.lead().exp());
  BOOST_CHECK_EQUAL(poly2.leadExp(), poly2.lead().exp());

  BOOST_CHECK_EQUAL(poly1.boundedLead(3), poly1.lead());
  BOOST_CHECK_EQUAL(poly2.boundedLead(2), poly2.lead());

  BOOST_CHECK_EQUAL(poly1.boundedLeadExp(3), poly1.boundedLead(3).exp());
  BOOST_CHECK_EQUAL(poly2.boundedLeadExp(2), poly2.boundedLead(2).exp());

  BOOST_CHECK_EQUAL(poly1.leadDeg(), 3);
  BOOST_CHECK_EQUAL(poly2.leadDeg(), 2);


  output << poly_type(a*b*d*f + a*b + a + g).leadDivisors();
  BOOST_CHECK(output.is_equal("{{a,b,d,f}, {a,b,d}, {a,b,f}, {a,b}, {a,d,f}, "
                              "{a,d}, {a,f}, {a}, {b,d,f}, {b,d}, {b,f}, {b}, "
                              "{d,f}, {d}, {f}, {}}"));

  BOOST_CHECK_EQUAL(poly_type(1).leadStableHash(), 4801919416);
  BOOST_CHECK_EQUAL(poly_type(a).leadStableHash(), 173100285919);
  BOOST_CHECK_EQUAL(poly_type(a*b).leadStableHash(), 11091674931773);
  BOOST_CHECK_EQUAL(poly_type(c*d +c + e + 1).leadStableHash(),
                    11091674972829);

  BOOST_CHECK_EQUAL(poly1.leadStableHash(), 706244162000938);
  BOOST_CHECK_EQUAL(poly2.leadStableHash(), 11091674956572);


  BooleMonomial terms1[] = {c*e*f, c*d*e, g*h, b, a};
  BooleMonomial terms2[] = {g*h, e*f, c*d, b, a};

  BooleExponent exps1[] = {terms1[0].exp(), terms1[1].exp(), terms1[2].exp(),
                           terms1[3].exp(), terms1[4].exp()};
  BooleExponent exps2[] = {terms2[0].exp(), terms2[1].exp(), terms2[2].exp(),
                           terms2[3].exp(), terms2[4].exp()};

  BOOST_CHECK_EQUAL_COLLECTIONS(poly1.orderedBegin(), poly1.orderedEnd(),
                                terms1, terms1 + 5);

  BOOST_CHECK_EQUAL_COLLECTIONS(poly2.orderedBegin(), poly2.orderedEnd(),
                                terms2, terms2 + 5);

  BOOST_CHECK_EQUAL_COLLECTIONS(poly1.orderedExpBegin(), poly1.orderedExpEnd(),
                                exps1, exps1 + 5);

  BOOST_CHECK_EQUAL_COLLECTIONS(poly2.orderedExpBegin(), poly2.orderedExpEnd(),
                                exps2, exps2 + 5);




  BOOST_CHECK_EQUAL_COLLECTIONS(poly1.genericBegin(dp_asc_tag()), poly1.genericEnd(dp_asc_tag()),
                                terms1, terms1 + 5);

  BOOST_CHECK_EQUAL_COLLECTIONS(poly2.genericBegin(dp_asc_tag()), poly2.genericEnd(dp_asc_tag()),
                                terms2, terms2 + 5);


  BOOST_CHECK_EQUAL_COLLECTIONS(poly1.genericExpBegin(dp_asc_tag()), poly1.genericExpEnd(dp_asc_tag()),
                                exps1, exps1 + 5);

  BOOST_CHECK_EQUAL_COLLECTIONS(poly2.genericExpBegin(dp_asc_tag()), poly2.genericExpEnd(dp_asc_tag()),
                                exps2, exps2 + 5);

  BOOST_CHECK_EQUAL(poly_type().eliminationLength(), 0);
  BOOST_CHECK_EQUAL(poly1.eliminationLength(), 5);
  BOOST_CHECK_EQUAL(poly2.eliminationLength(), 5);
  BOOST_CHECK_EQUAL(poly1.eliminationLengthWithDegBound(3), 5);
  BOOST_CHECK_EQUAL(poly2.eliminationLengthWithDegBound(2), 5);

  BOOST_CHECK_EQUAL(poly1.compare(poly1), CTypes::equality);
  BOOST_CHECK_EQUAL(poly1.compare(poly2), CTypes::greater_than);
  BOOST_CHECK_EQUAL(poly2.compare(poly1), CTypes::less_than);
  BOOST_CHECK_EQUAL(poly1.compare(poly_type()), CTypes::greater_than);
  BOOST_CHECK_EQUAL(poly_type().compare(poly2), CTypes::less_than);
}

BOOST_FIXTURE_TEST_CASE(test_ordering_block_dlex, OrderGenFix<BoolePolyRing::block_dlex>) {


  ring.ordering().appendBlock(3);
  BOOST_CHECK_THROW(poly_type().lead(), PBoRiError);
  BOOST_CHECK_THROW(poly_type(0).lead(), PBoRiError);

  BOOST_CHECK_EQUAL(poly1.lead(), a);
  BOOST_CHECK_EQUAL(poly2.lead(), a);

  BOOST_CHECK_EQUAL(poly1.leadExp(), BooleExponent().change(0));
  BOOST_CHECK_EQUAL(poly2.leadExp(), BooleExponent().change(0));

  BOOST_CHECK_EQUAL(poly1.boundedLead(3), a);
  BOOST_CHECK_EQUAL(poly2.boundedLead(3), a);

  BOOST_CHECK_EQUAL(poly1.boundedLeadExp(3), BooleExponent().change(0));
  BOOST_CHECK_EQUAL(poly2.boundedLeadExp(3), BooleExponent().change(0));


  BOOST_CHECK_EQUAL(poly1.leadDeg(), 1);
  BOOST_CHECK_EQUAL(poly2.leadDeg(), 1);

  output << poly_type(a*b*d*f + a*b + a + g).leadDivisors();
  BOOST_CHECK(output.is_equal("{{a,b,d,f}, {a,b,d}, {a,b,f}, {a,b}, {a,d,f}, "
                              "{a,d}, {a,f}, {a}, {b,d,f}, {b,d}, {b,f}, {b}, "
                              "{d,f}, {d}, {f}, {}}"));

  BOOST_CHECK_EQUAL(poly_type(1).leadStableHash(), 4801919416);
  BOOST_CHECK_EQUAL(poly_type(a).leadStableHash(), 173100285919);
  BOOST_CHECK_EQUAL(poly_type(a*b).leadStableHash(), 11091674931773);
  BOOST_CHECK_EQUAL(poly_type(c*d +c + e + 1).leadStableHash(),
                    11091674972829);

  BOOST_CHECK_EQUAL(poly1.leadStableHash(), 173100285919);
  BOOST_CHECK_EQUAL(poly2.leadStableHash(), 173100285919); 


  BooleMonomial terms1[] = {a, b, c*d*e, c*e*f, g*h};
  BooleMonomial terms2[] = {a, b, c*d, e*f, g*h};

  BooleExponent exps1[] = {terms1[0].exp(), terms1[1].exp(), terms1[2].exp(),
                           terms1[3].exp(), terms1[4].exp()};
  BooleExponent exps2[] = {terms2[0].exp(), terms2[1].exp(), terms2[2].exp(),
                           terms2[3].exp(), terms2[4].exp()};

  BOOST_CHECK_EQUAL_COLLECTIONS(poly1.orderedBegin(), poly1.orderedEnd(),
                                terms1, terms1 + 5);

  BOOST_CHECK_EQUAL_COLLECTIONS(poly2.orderedBegin(), poly2.orderedEnd(),
                                terms2, terms2 + 5);

  BOOST_CHECK_EQUAL_COLLECTIONS(poly1.orderedExpBegin(), poly1.orderedExpEnd(),
                                exps1, exps1 + 5);

  BOOST_CHECK_EQUAL_COLLECTIONS(poly2.orderedExpBegin(), poly2.orderedExpEnd(),
                                exps2, exps2 + 5);




  BOOST_CHECK_EQUAL_COLLECTIONS(poly1.genericBegin(block_dlex_tag()), poly1.genericEnd(block_dlex_tag()),
                                terms1, terms1 + 5);

  BOOST_CHECK_EQUAL_COLLECTIONS(poly2.genericBegin(block_dlex_tag()), poly2.genericEnd(block_dlex_tag()),
                                terms2, terms2 + 5);


  BOOST_CHECK_EQUAL_COLLECTIONS(poly1.genericExpBegin(block_dlex_tag()), poly1.genericExpEnd(block_dlex_tag()),
                                exps1, exps1 + 5);

  BOOST_CHECK_EQUAL_COLLECTIONS(poly2.genericExpBegin(block_dlex_tag()), poly2.genericExpEnd(block_dlex_tag()),
                                exps2, exps2 + 5);

  BOOST_CHECK_EQUAL(poly_type().eliminationLength(), 0);
  BOOST_CHECK_EQUAL(poly1.eliminationLength(), 10);
  BOOST_CHECK_EQUAL(poly2.eliminationLength(), 8);
  BOOST_CHECK_EQUAL(poly1.eliminationLengthWithDegBound(3), 10);
  BOOST_CHECK_EQUAL(poly2.eliminationLengthWithDegBound(2), 8);

  BOOST_CHECK_EQUAL(poly1.compare(poly1), CTypes::equality);
  BOOST_CHECK_EQUAL(poly1.compare(poly2), CTypes::greater_than);
  BOOST_CHECK_EQUAL(poly2.compare(poly1), CTypes::less_than);
  BOOST_CHECK_EQUAL(poly1.compare(poly_type()), CTypes::greater_than);
  BOOST_CHECK_EQUAL(poly_type().compare(poly2), CTypes::less_than);
}

BOOST_FIXTURE_TEST_CASE(test_ordering_block_dp_asc, OrderGenFix<BoolePolyRing::block_dp_asc>) {

  ring.ordering().appendBlock(3);

  BOOST_CHECK_THROW(poly_type().lead(), PBoRiError);
  BOOST_CHECK_THROW(poly_type(0).lead(), PBoRiError);

  BOOST_CHECK_EQUAL(poly1.lead(), c*e*f);
  BOOST_CHECK_EQUAL(poly2.lead(), c*d);

  BOOST_CHECK_EQUAL(poly1.leadExp(), poly1.lead().exp());
  BOOST_CHECK_EQUAL(poly2.leadExp(), poly2.lead().exp());

  BOOST_CHECK_EQUAL(poly1.boundedLead(3), poly1.lead());
  BOOST_CHECK_EQUAL(poly2.boundedLead(2), poly2.lead());

  BOOST_CHECK_EQUAL(poly1.boundedLeadExp(3), poly1.boundedLead(3).exp());
  BOOST_CHECK_EQUAL(poly2.boundedLeadExp(2), poly2.boundedLead(2).exp());

  BOOST_CHECK_EQUAL(poly1.leadDeg(), 3);
  BOOST_CHECK_EQUAL(poly2.leadDeg(), 2);

  output << poly_type(a*b*d*f + a*b + a + g).leadDivisors();
  BOOST_CHECK(output.is_equal("{{a,b,d,f}, {a,b,d}, {a,b,f}, {a,b}, {a,d,f}, "
                              "{a,d}, {a,f}, {a}, {b,d,f}, {b,d}, {b,f}, {b}, "
                              "{d,f}, {d}, {f}, {}}"));

  BOOST_CHECK_EQUAL(poly_type(1).leadStableHash(), 4801919416);
  BOOST_CHECK_EQUAL(poly_type(a).leadStableHash(), 173100285919);
  BOOST_CHECK_EQUAL(poly_type(a*b).leadStableHash(), 11091674931773);
  BOOST_CHECK_EQUAL(poly_type(c*d +c + e + 1).leadStableHash(),
                    11091674972829);

  BOOST_CHECK_EQUAL(poly1.leadStableHash(), 706244162000938);
  BOOST_CHECK_EQUAL(poly2.leadStableHash(), 11091674972829);


  BooleMonomial terms1[] = {c*e*f, c*d*e, b, a, g*h};
  BooleMonomial terms2[] = {c*d, b, a, g*h, e*f};

  BooleExponent exps1[] = {terms1[0].exp(), terms1[1].exp(), terms1[2].exp(),
                           terms1[3].exp(), terms1[4].exp()};
  BooleExponent exps2[] = {terms2[0].exp(), terms2[1].exp(), terms2[2].exp(),
                           terms2[3].exp(), terms2[4].exp()};

  BOOST_CHECK_EQUAL_COLLECTIONS(poly1.orderedBegin(), poly1.orderedEnd(),
                                terms1, terms1 + 5);

  BOOST_CHECK_EQUAL_COLLECTIONS(poly2.orderedBegin(), poly2.orderedEnd(),
                                terms2, terms2 + 5);

  BOOST_CHECK_EQUAL_COLLECTIONS(poly1.orderedExpBegin(), poly1.orderedExpEnd(),
                                exps1, exps1 + 5);

  BOOST_CHECK_EQUAL_COLLECTIONS(poly2.orderedExpBegin(), poly2.orderedExpEnd(),
                                exps2, exps2 + 5);




  BOOST_CHECK_EQUAL_COLLECTIONS(poly1.genericBegin(block_dp_asc_tag()), poly1.genericEnd(block_dp_asc_tag()),
                                terms1, terms1 + 5);

  BOOST_CHECK_EQUAL_COLLECTIONS(poly2.genericBegin(block_dp_asc_tag()), poly2.genericEnd(block_dp_asc_tag()),
                                terms2, terms2 + 5);


  BOOST_CHECK_EQUAL_COLLECTIONS(poly1.genericExpBegin(block_dp_asc_tag()), poly1.genericExpEnd(block_dp_asc_tag()),
                                exps1, exps1 + 5);

  BOOST_CHECK_EQUAL_COLLECTIONS(poly2.genericExpBegin(block_dp_asc_tag()), poly2.genericExpEnd(block_dp_asc_tag()),
                                exps2, exps2 + 5);

  BOOST_CHECK_EQUAL(poly_type().eliminationLength(), 0);
  BOOST_CHECK_EQUAL(poly1.eliminationLength(), 5);
  BOOST_CHECK_EQUAL(poly2.eliminationLength(), 5);
  BOOST_CHECK_EQUAL(poly1.eliminationLengthWithDegBound(3), 5);
  BOOST_CHECK_EQUAL(poly2.eliminationLengthWithDegBound(2), 5);

  BOOST_CHECK_EQUAL(poly1.compare(poly1), CTypes::equality);
  BOOST_CHECK_EQUAL(poly1.compare(poly2), CTypes::greater_than);
  BOOST_CHECK_EQUAL(poly2.compare(poly1), CTypes::less_than);
  BOOST_CHECK_EQUAL(poly1.compare(poly_type()), CTypes::greater_than);
  BOOST_CHECK_EQUAL(poly_type().compare(poly2), CTypes::less_than);
}



BOOST_AUTO_TEST_CASE(test_incompatible) {

  BOOST_TEST_MESSAGE( "Incompatible operands..." ); 
  BoolePolyRing other_ring(5, CTypes::lp, false);

  BoolePolynomial other_poly = other_ring.variable(2);

  BOOST_CHECK_THROW((x*y + z) +other_poly, std::exception);

}



BOOST_AUTO_TEST_SUITE_END()
