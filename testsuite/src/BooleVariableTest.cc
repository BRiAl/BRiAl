// -*- c++ -*-
//*****************************************************************************
/** @file BooleVariableTest.cc
 *
 * @author Ket Kalda, Alexander Dreyer
 * @date 2010-10-05
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

#include <polybori/pbori_defs.h>
#include <polybori/BooleVariable.h>
#include <polybori/BooleMonomial.h>
#include <polybori/BoolePolynomial.h>
#include <polybori/BoolePolyRing.h>

USING_NAMESPACE_PBORI

struct Fvar {
  typedef BooleVariable var_type;
  Fvar(const BoolePolyRing& input_ring = BoolePolyRing(3)):
    ring(input_ring), 
    x(0, input_ring), y(1, input_ring), z(2, input_ring),
    bset(1, input_ring.one(), input_ring.zero()) {

    BOOST_TEST_MESSAGE( "setup fixture" );
    ring.setVariableName(0, "x");
    ring.setVariableName(1, "y");
    ring.setVariableName(2, "z");
   }

  ~Fvar() { BOOST_TEST_MESSAGE( "teardown fixture" ); }
  
  BoolePolyRing ring;
  BooleVariable x, y, z;
  BooleSet bset;
};

BOOST_FIXTURE_TEST_SUITE(BooleVariableTestSuite, Fvar )

BOOST_AUTO_TEST_CASE(test_constructors) {

  BOOST_TEST_MESSAGE( "Constructors..." );
  BOOST_CHECK_EQUAL(var_type(ring), var_type(0, ring));
  BOOST_CHECK_EQUAL(var_type(0, ring), x);
  BOOST_CHECK_EQUAL(var_type(1, ring), y);
  BOOST_CHECK_EQUAL(var_type(z), z);
  BOOST_CHECK_THROW(var_type(3, ring), PBoRiError);
  BOOST_CHECK_THROW(var_type(-1, ring), PBoRiError);
}

BOOST_AUTO_TEST_CASE(test_methods) {

  BOOST_TEST_MESSAGE( "Casting to poly_type" );
  BOOST_CHECK_EQUAL((BoolePolynomial)x, BoolePolynomial(x));
  BOOST_CHECK_EQUAL((BoolePolynomial)var_type(ring), BoolePolynomial(x));
  BOOST_TEST_MESSAGE( "index" );
  BOOST_CHECK_EQUAL(var_type(ring).index(), 0);
  BOOST_CHECK_EQUAL(var_type(1, ring).index(), 1);
  BOOST_CHECK_EQUAL(var_type(z).index(), 2);
  BOOST_CHECK_EQUAL(z.index(), 2);

  BOOST_CHECK_THROW(var_type(3, ring).index(), PBoRiError);
  BOOST_TEST_MESSAGE( "ring" );
  BOOST_CHECK_EQUAL(x.ring().nVariables(), ring.nVariables());
  BOOST_CHECK_EQUAL(x.ring().getVariableName(0), ring.getVariableName(0));
  BOOST_CHECK_EQUAL(x.ring().getVariableName(1), ring.getVariableName(1));
  BOOST_CHECK_EQUAL(x.ring().getVariableName(2), ring.getVariableName(2));
  BOOST_CHECK_EQUAL(x.ring().getVariableName(3), ring.getVariableName(3));
  BOOST_TEST_MESSAGE( "set" );
  BOOST_CHECK_EQUAL(y.set(),bset);
  BOOST_CHECK_EQUAL(var_type(1, ring).set(),bset);
  BOOST_CHECK_NE(x.set(),bset);
  BOOST_CHECK_THROW(var_type(3, ring).set(), PBoRiError);
}

BOOST_AUTO_TEST_CASE(test_logical_operators) {
  BOOST_TEST_MESSAGE( "== and !=" );
  BOOST_CHECK_EQUAL(x, x);
  BOOST_CHECK_EQUAL(x, var_type(ring));
  BOOST_CHECK_EQUAL(var_type(ring), x);
  BOOST_CHECK_NE(x, y);
  BOOST_CHECK_NE(y, x);
}

BOOST_AUTO_TEST_CASE(test_assigning_operators) {

  BOOST_TEST_MESSAGE( "Assignments..." );

  BOOST_TEST_MESSAGE( "/" );
  BOOST_CHECK_EQUAL(x / y, BoolePolynomial(0, ring));
  BOOST_CHECK_EQUAL((BoolePolynomial)x / (BoolePolynomial)y, BoolePolynomial(0, ring));
  BOOST_CHECK_EQUAL(BoolePolynomial(x*y) / x, BoolePolynomial(y));
  BOOST_CHECK_EQUAL(BoolePolynomial(x) / y, BoolePolynomial(0, ring));
  BOOST_CHECK_EQUAL(BoolePolynomial(x) / x, BoolePolynomial(1, ring));

  BOOST_CHECK_EQUAL((BoolePolynomial(x*y) /= x), BoolePolynomial(y));
  BOOST_CHECK_EQUAL((BoolePolynomial(x) /= y), BoolePolynomial(0, ring));
  BOOST_CHECK_EQUAL((BoolePolynomial(x) /= x), BoolePolynomial(1, ring));
  BOOST_CHECK_EQUAL(BooleMonomial(x*y)/y, BooleMonomial(x));
  BOOST_CHECK_EQUAL(BooleMonomial(x)/x, BooleMonomial(ring));
  BOOST_CHECK_THROW(BooleMonomial(x)/y, PBoRiError);
  BOOST_CHECK_EQUAL(x / x, BoolePolynomial(1, ring));
  BOOST_CHECK_EQUAL(var_type(ring) / x, BoolePolynomial(1, ring));
  BOOST_CHECK_EQUAL(var_type(ring) / var_type(ring), BoolePolynomial(1, ring));

  BOOST_TEST_MESSAGE( "*" );
  BOOST_CHECK_EQUAL(x*y, BooleMonomial(x*y));
  BOOST_CHECK_EQUAL(x*x, BooleMonomial(x*x));
  BOOST_CHECK_EQUAL(BooleMonomial(x)*y, BooleMonomial(x*y));
  BOOST_CHECK_EQUAL(BooleMonomial(x)*x, BooleMonomial(x*x));
  BOOST_CHECK_EQUAL(x*BooleMonomial(y), BooleMonomial(x*y));
  BOOST_CHECK_EQUAL(x*BooleMonomial(x), BooleMonomial(x*x));
  BOOST_CHECK_EQUAL(x*BooleMonomial(ring), BooleMonomial(x));
  BOOST_CHECK_EQUAL(BooleMonomial(ring)*x, BooleMonomial(x));
  BOOST_CHECK_EQUAL(x*1, BoolePolynomial(x));
  BOOST_CHECK_EQUAL(1*x, BoolePolynomial(x));
  BOOST_CHECK_EQUAL(0*1, BoolePolynomial(ring));
  BOOST_CHECK_EQUAL(1*0, BoolePolynomial(0, ring));
  BOOST_CHECK_EQUAL(x*BoolePolynomial(y), BoolePolynomial(x*y));
  BOOST_CHECK_EQUAL(x*BoolePolynomial(x), BoolePolynomial(x*x));
  BOOST_CHECK_EQUAL(x*BoolePolynomial(0, ring), BoolePolynomial(0, ring));
  BOOST_CHECK_EQUAL(x*BoolePolynomial(1, ring), BoolePolynomial(x));
  BOOST_CHECK_EQUAL(BoolePolynomial(x)*x, BoolePolynomial(x*x));
  BOOST_CHECK_EQUAL(BoolePolynomial(0, ring)*x, BoolePolynomial(0, ring));
  BOOST_CHECK_EQUAL(BoolePolynomial(1, ring)*x, BoolePolynomial(x));
  BOOST_CHECK_EQUAL(BoolePolynomial(x)*y, BoolePolynomial(x*y));
  //BOOST_CHECK_EQUAL((BoolePolynomial(x)*=x), BoolePolynomial(x*x)); // Does not FIND correct operator
  //BOOST_CHECK_EQUAL((BoolePolynomial(0)*=x), BoolePolynomial(0)); // Does not FIND correct operator
  //BOOST_CHECK_EQUAL((BoolePolynomial(1)*=x), BoolePolynomial(x)); // Does not FIND correct operator
  //BOOST_CHECK_EQUAL((BoolePolynomial(x)*=y), BoolePolynomial(x*y)); // Does not FIND correct operator

  BOOST_TEST_MESSAGE( "%" );
  BOOST_CHECK_EQUAL(BoolePolynomial(x)%y, BoolePolynomial(x));
  BOOST_CHECK_EQUAL(BoolePolynomial(x*y)%y, BoolePolynomial(ring));
  BOOST_CHECK_EQUAL(BoolePolynomial(ring)%y, BoolePolynomial(ring));
  //BOOST_CHECK_EQUAL((BoolePolynomial(x)%=y), BoolePolynomial(x)); // Does not FIND correct operator
  //BOOST_CHECK_EQUAL((BoolePolynomial(x*y)%=y), BoolePolynomial()); // Does not FIND correct operator
  //BOOST_CHECK_EQUAL((BoolePolynomial()%=y), BoolePolynomial()); // Does not FIND correct operator
  BOOST_CHECK_EQUAL(BooleMonomial(x)%y, BooleMonomial(x));
  BOOST_CHECK_EQUAL(BooleMonomial(x*y)%y, BoolePolynomial(ring));
  BOOST_CHECK_EQUAL(BooleMonomial(ring)%y, BooleMonomial(ring));
  BOOST_CHECK_EQUAL(x%y, BooleMonomial(x));
  BOOST_CHECK_EQUAL((x*y)%y, BoolePolynomial(ring));
}

BOOST_AUTO_TEST_SUITE_END()
