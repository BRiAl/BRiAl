// -*- c++ -*-
//*****************************************************************************
/** @file embedTest.cc
 *
 * @author Alexander Dreyer
 * @date 2012-08-06
 *
 * boost/test-driven unit test for testing embdedding via boost/python
 * 
 * @par Copyright:
 *   (c) 2012 by The PolyBoRi Team
 *
 **/
//*****************************************************************************


#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp> 
using boost::test_tools::output_test_stream;

// in case of a custom python installation: define this before including
#define PBORI_PYTHONPATH "pyroot"

#include <polybori/embed.h>

USING_NAMESPACE_PBORI

struct Fembed {
  Fembed() { BOOST_TEST_MESSAGE( "setup fixture" ); }
  ~Fembed() { BOOST_TEST_MESSAGE( "teardown fixture" ); }
};

BOOST_FIXTURE_TEST_SUITE(embedTestSuite, Fembed )

BOOST_AUTO_TEST_CASE(test_python) {

  BOOST_TEST_MESSAGE( "Embedding..." );

  BEGIN_PBORI_EMBED();
  // Execute python code and extract results
  exec("num = 17+4");
  int num = eval("num");
  BOOST_CHECK_EQUAL(num, 21);

  // remove unused stuff in python (most of the time not necessary)
  exec("del num, r, x");

  Interpreter::globals()["myobj"] = 17;
  exec("num = myobj");
  num = eval("num");
  BOOST_CHECK_EQUAL(num, 17);
  END_PBORI_EMBED();
}

BOOST_AUTO_TEST_CASE(test_sequence) {

  BEGIN_PBORI_EMBED();
  // Execute python code and extract results
  std::vector<int> ivec = eval("xrange(10)");

  BOOST_CHECK_EQUAL(ivec.size(), 10);
  for (int i = 0; i < ivec.size(); ++i)
    BOOST_CHECK_EQUAL(ivec[i], i);

  // Execute python code and extract results
  std::list<int> ilist = eval("xrange(10)");

  BOOST_CHECK_EQUAL(ilist.size(), 10);
  std::list<int>::const_iterator iter(ilist.begin());
  for (int j = 0; iter != ilist.end();  ++iter, ++j)
    BOOST_CHECK_EQUAL(*iter, j);


  std::vector<std::vector<int> > ivecvec = eval("[xrange(10)]");

  BOOST_CHECK_EQUAL(ivecvec.size(), 1);
  BOOST_CHECK_EQUAL(ivecvec[0].size(), 10);

  for (int k = 0; k < 10; ++k)
    BOOST_CHECK_EQUAL(ivecvec[0][k], k);


  std::list<std::vector<int> > iveclist = eval("[xrange(10)]");

  BOOST_CHECK_EQUAL(iveclist.size(), 1);
  BOOST_CHECK_EQUAL(iveclist.begin()->size(), 10);

  for (int k = 0; k < 10; ++k)
    BOOST_CHECK_EQUAL((*iveclist.begin())[k], k);

  END_PBORI_EMBED();
}

BOOST_AUTO_TEST_CASE(test_polybori) {

  BEGIN_PBORI_EMBED();

  BOOST_TEST_MESSAGE( "Interact with PolyBoRi...");
  Ring ring(10,  dp_asc);
  Polynomial poly = ring.variable(1);
  std::vector<Polynomial> ideal;
  ideal.push_back(poly);
  ideal.push_back(poly + 1);
        
  // Lookup and call PolyBoRi function
  object groebner_basis = eval("groebner_basis");
  std::vector<Polynomial> gb1 = groebner_basis(ideal);
    
  // Function with options
  dict kw;
  kw["prot"] = false;
  kw["faugere"] = true;
  std::vector<Polynomial> gb2 = groebner_basis(ideal, **kw);
    
  BOOST_CHECK(gb1 == gb2);
  
  /// Access vector by indices
  for (unsigned int idx = 0; idx < gb1.size(); ++idx) {
    BOOST_CHECK(gb1[idx].isOne());
  }
    
  /// Access vector element by element (works also for lists and sets)
  for(std::vector<Polynomial>::const_iterator start(gb2.begin()),
        finish(gb2.end());  start != finish; ++start) {
    BOOST_CHECK(start->isOne());
  }

  END_PBORI_EMBED();
}

BOOST_AUTO_TEST_SUITE_END()
