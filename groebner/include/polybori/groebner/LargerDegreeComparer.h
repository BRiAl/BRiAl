// -*- c++ -*-
//*****************************************************************************
/** @file LargerDegreeComparer.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-29 
 *
 * This file includes the definition of the class @c LargerDegreeComparer.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_LargerDegreeComparer_h_
#define polybori_groebner_LargerDegreeComparer_h_

// include basic definitions
#include "groebner_defs.h"

BEGIN_NAMESPACE_PBORIGB

/** @class LargerDegreeComparer
 * @brief This class defines LargerDegreeComparer.
 *
 **/
class LargerDegreeComparer{
public:
  bool operator() (const Monomial& a , const Monomial& b){
      return a.deg() > b.deg(); 
    
  }
  bool operator() (const Exponent& a , const Exponent& b){
      return a.deg()>b.deg();
    
  }
};

END_NAMESPACE_PBORIGB

#endif /* polybori_LargerDegreeComparer_h_ */
