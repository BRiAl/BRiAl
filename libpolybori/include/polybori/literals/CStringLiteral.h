// -*- c++ -*-
//*****************************************************************************
/** @file CStringLiteral.h
 *
 * @author Alexander Dreyer
 * @date 2006-07-04
 *
 * This file contains a template class for accessing predefined string literals.
 *
 * @par Copyright:
 *   (c) 2006 by The PolyBoRi Team
**/
//*****************************************************************************

#ifndef polybori_literals_CStringLiteral_h_
#define polybori_literals_CStringLiteral_h_

// include basic definitions
#include <polybori/pbori_defs.h>

// get standard string capability
#include <string>

// get literal codes (enumeration)
#include "CLiteralCodes.h"

BEGIN_NAMESPACE_PBORI

/** @class CStringLiteral
 * @brief This template class defines string literals corresponding to the codes
 * enumerated in the class CLiteralCodes.
 *
 **/
template <unsigned LitCode = CLiteralCodes::first_code>
class CStringLiteral {
public:

  /// Type for string literals
  typedef std::string literal_type;

  /// Default constructor
  CStringLiteral() {};

  /// Destructor
  ~CStringLiteral() {};

  /// Return string literal depending on LitCode
  literal_type operator()() const;
};


#define GENERATE_CSTRINGLITERAL(CODE, STRING) \
template <> inline CStringLiteral<>::literal_type \
CStringLiteral<CLiteralCodes::CODE>::operator()() const{ return STRING; }

GENERATE_CSTRINGLITERAL(empty, "")
GENERATE_CSTRINGLITERAL(space, " ")
GENERATE_CSTRINGLITERAL(plus, "+")
GENERATE_CSTRINGLITERAL(term_separator, " + ")
GENERATE_CSTRINGLITERAL(times, "*")
GENERATE_CSTRINGLITERAL(comma, ",")
GENERATE_CSTRINGLITERAL(list_separator, ", ")
GENERATE_CSTRINGLITERAL(default_variable_name, "x")
GENERATE_CSTRINGLITERAL(variable_head, "(")
GENERATE_CSTRINGLITERAL(variable_tail, ")")
GENERATE_CSTRINGLITERAL(between_list_separator, "}, {")

#undef GENERATE_CSTRINGLITERAL

END_NAMESPACE_PBORI

#endif
