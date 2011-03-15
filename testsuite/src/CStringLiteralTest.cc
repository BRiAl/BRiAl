// -*- c++ -*-
//*****************************************************************************
/** @file CStringLiteralTest.cc
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

# include <polybori/literals/CStringLiteral.h>

USING_NAMESPACE_PBORI

template <unsigned NUM = CLiteralCodes::first_code>
class print_lit {
public:
  void operator()(std::ostream& os) const {
    os << '"'<< CStringLiteral<NUM>()() <<'"' <<std::endl;
    print_lit<NUM+1>()(os);
  };
};

template <>
class print_lit <CLiteralCodes::last_code>{
public:
  void operator()(std::ostream& os) const {  };
};

struct Fliteral {
  Fliteral() { BOOST_TEST_MESSAGE( "setup fixture" ); }
  ~Fliteral() { BOOST_TEST_MESSAGE( "teardown fixture" ); }
};

BOOST_FIXTURE_TEST_SUITE(CStringLiteralTestSuite, Fliteral )

BOOST_AUTO_TEST_CASE(test_lit) {

  BOOST_TEST_MESSAGE("literals");
  // Test all valid template arguments
  output_test_stream output;
  print_lit<>()(output);
  output_test_stream output2;
  output2 << '"'<< "" <<'"' <<std::endl;
  output2 << '"'<< " " <<'"' <<std::endl;
  output2 << '"'<< "+" <<'"' <<std::endl;
  output2 << '"'<< " + " <<'"' <<std::endl;
  output2 << '"'<< "*" <<'"' <<std::endl;
  output2 << '"'<< "," <<'"' <<std::endl;
  output2 << '"'<< ", " <<'"' <<std::endl;
  output2 << '"'<< "x" <<'"' <<std::endl;
  output2 << '"'<< "(" <<'"' <<std::endl;
  output2 << '"'<< ")" <<'"' <<std::endl;
  output2 << '"'<< "}, {" <<'"' <<std::endl;
  BOOST_CHECK(output.is_equal(output2.str()));
  // Test default template argument
  output2.flush();
  output << CStringLiteral<>()();
  output2 << CStringLiteral<CLiteralCodes::first_code>()();
  BOOST_CHECK(output.is_equal(output2.str()));
}
BOOST_AUTO_TEST_SUITE_END()
