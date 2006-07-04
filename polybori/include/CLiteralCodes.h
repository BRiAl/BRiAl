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
 *   (c) 2006 by
 *   Dep. of Mathematics, Kaiserslautern University of Technology and @n
 *   Fraunhofer Institute for Industrial Mathematics (ITWM)
 *   D-67663 Kaiserslautern, Germany
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.1  2006/07/04 14:11:03  dreyer
 * ADD: Generic and handy treatment of string literals
 *
 * @endverbatim
**/
//*****************************************************************************

// include basic definitions
#include "pbori_defs.h"

#ifndef CLiteralCodes_h_
#define CLiteralCodes_h_

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
    last_code
  };

};

END_NAMESPACE_PBORI

#endif
