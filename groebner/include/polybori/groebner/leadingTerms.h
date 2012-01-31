// -*- c++ -*-
//*****************************************************************************
/** @file leadingTerms.h 
 *
 * @author Alexander Dreyer 
 * @date 2012-01-31 
 *
 * This file includes the definition of the class @c leadingTerms.
 *
 * @par Copyright:
 *   (c) 2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_leadingTerms_h_
#define polybori_groebner_leadingTerms_h_

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class leadingTerms
 * @brief This class defines leadingTerms.
 *
 **/

class leadingTerms {
  /// Type of *this
  typedef leadingTerms self;

public:
  /// Default constructor
  leadingTerms() {}

  /// Copy constructor
  leadingTerms(const self& rhs) {}

  /// Destructor
  ~leadingTerms() {}

};

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_leadingTerms_h_ */
