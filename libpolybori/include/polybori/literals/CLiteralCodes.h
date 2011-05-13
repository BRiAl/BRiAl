// -*- c++ -*-
//*****************************************************************************
/** @file CLiteralCodes.h
 *
 * @author Alexander Dreyer
 * @date 2006-07-04
 *
 * This file contains a class for codes representing string literals.
 *
 * @par Copyright:
 *   (c) 2006 by The PolyBoRi Team
**/
//*****************************************************************************

#ifndef polybori_literals_CLiteralCodes_h_
#define polybori_literals_CLiteralCodes_h_

// include basic definitions
#include <polybori/pbori_defs.h>

BEGIN_NAMESPACE_PBORI

/** @class CLiteralCodes
 * @brief This template class defines enumerative codes for the string literals
 * defined by the CStringLiterals class.
 **/
class CLiteralCodes {
public:

  /// Define codes representing string literals
  enum literal_codes {
    first_code = 0, 
    empty = first_code,
    space,
    plus,
    term_separator,
    times,
    comma,
    list_separator,
    default_variable_name,
    variable_head,
    variable_tail,
    between_list_separator,
    last_code
  };

};

END_NAMESPACE_PBORI

#endif
