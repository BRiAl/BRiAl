// -*- c++ -*-
//*****************************************************************************
/** @file term_accumulateTest.cc
 *
 * @author Ket Kalda, Alexander Dreyer
 * @date 2011-03-15
 *
 * boost/test-driven unit test
 * 
 * @par Copyright:
 *   (c) 2011 by The PolyBoRi Team
 *
 **/
//*****************************************************************************


#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp> 
using boost::test_tools::output_test_stream;

#include <polybori.h>

USING_NAMESPACE_PBORI

struct Ftermaccu {
  Ftermaccu(): 
    ring(5), x(ring), y(ring), z(ring), v(ring), w(ring) {
    BOOST_TEST_MESSAGE( "setup fixture" );
    x = (BooleVariable(0, ring));
    y = (BooleVariable(1, ring));
    z = (BooleVariable(2, ring));
    v = (BooleVariable(3, ring));
    w = (BooleVariable(4, ring));
  }
  ~Ftermaccu() { BOOST_TEST_MESSAGE( "teardown fixture" ); }

  BoolePolyRing ring;
  BooleMonomial x, y, z, v, w;
};

BOOST_FIXTURE_TEST_SUITE(term_accumulateTestSuite, Ftermaccu )

BOOST_AUTO_TEST_CASE(test_termaccu) {

  BOOST_TEST_MESSAGE("term_accumulate");

  BoolePolynomial poly = x*y +z +x*z +y*z;
  BoolePolynomial result = term_accumulate(poly.begin(), poly.end(),
                                          BoolePolynomial(ring));
  BOOST_CHECK_EQUAL(poly, result);
  // Only meant to work for lexicographical ordering, so same result
  result = term_accumulate(poly.orderedBegin(), poly.orderedEnd(),
                          BoolePolynomial(ring));
  BOOST_CHECK_EQUAL(poly, result);

  // Sophisticated accumulation
  BoolePolynomial::ordered_iterator start(poly.orderedBegin()),
                                    start2(poly.orderedBegin());
  ++start;
  start2 = start;++start2; ++start2;
  result = std::accumulate(start, start2, BoolePolynomial(0, ring));
  BoolePolynomial result2 = term_accumulate(start, start2, BoolePolynomial(0, ring));
  BOOST_CHECK_EQUAL(result, result2);
  result2 = term_accumulate(start.begin(), start.end(), start.navigation(),
                      start2.begin(), start2.end(), BoolePolynomial(0, ring));
  BOOST_CHECK_EQUAL(result, result2);

  // Lower summing 1)
  poly = x*z + x*w+ y*z + y*w + y;
  start = poly.orderedBegin();++start;
  start2 = start;++start2;++start2;

  result = lower_term_accumulate(start.navigation(),
                      start2.begin(), start2.end(), BoolePolynomial(0, ring));
  BOOST_CHECK_EQUAL(result, x*z + x*w+ y*z);
  result = term_accumulate(start, start2, BoolePolynomial(0, ring));
  BOOST_CHECK_EQUAL(result, x*w+ y*z);

  // Lower summing 2)
  poly = x*y +x*z +y*z + 1;
  start = poly.orderedBegin();++start;
  start2 = start;++start2;++start2;
  result = term_accumulate(start, start2, BoolePolynomial(0, ring));
  BOOST_CHECK_EQUAL(result, x*z +y*z);
  ++start2;
  result = term_accumulate(start, start2, BoolePolynomial(0, ring));
  BOOST_CHECK_EQUAL(result, x*z +y*z + 1);

}


BOOST_AUTO_TEST_CASE(test_termaccu_other) {

  BOOST_TEST_MESSAGE("term_accumulate other orderings");

  BoolePolynomial poly = x*y +z +x*z +y*z;

  for (unsigned ordernum = 1; ordernum <= 4; ++ordernum) {

    BoolePolyRing ring_other = ring.clone();
    //    ring_other.activate();
    ring_other.changeOrdering(ordernum);
    BoolePolynomial init(ring_other.zero());

    BoolePolynomial poly_other = ring_other.coerce(poly);
    BoolePolynomial result = term_accumulate(poly_other.begin(), poly_other.end(),
                                             init);
    
    BOOST_CHECK_EQUAL(poly_other, result);
    
    // Sophisticated accumulation
    BoolePolynomial::ordered_iterator start_other(poly_other.orderedBegin()),
      last_other(poly_other.orderedBegin());
    ++start_other;
    last_other = start_other;
    ++last_other; ++last_other;

    BoolePolynomial::ordered_iterator start(poly.orderedBegin()),
      last(poly.orderedBegin());
    
    while ((*start) != (*start_other) )
      ++start;
    
    while ((*last) != (*last_other) )
      ++last;
    

    result = term_accumulate(start_other, last_other, init);
    
    /// Term accumulate sums up in a lexicographical way
    BOOST_CHECK_EQUAL(result,  std::accumulate(start, last, init));
  }

}




BOOST_AUTO_TEST_SUITE_END()
