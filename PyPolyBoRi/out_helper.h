/*  Copyright (c) 2005-2007 by The PolyBoRi Team */

#ifndef OUT_HELPER_H
#define OUT_HELPER_H
#include <sstream>
#include <iosfwd>
#include <boost/python.hpp>

template <class c> boost::python::str streamable_as_str(const c& f)
{
  using boost::python::str;
  std::basic_stringstream<char>  s;
  s<<f;
  return boost::python::str(s.str());
}

#endif
