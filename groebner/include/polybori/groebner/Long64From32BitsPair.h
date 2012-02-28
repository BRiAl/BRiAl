// -*- c++ -*-
//*****************************************************************************
/** @file Long64From32BitsPair.h 
 *
 * @author Alexander Dreyer
 * @date 2012-02-23
 *
 * This file includes the definition of the class @c Long64From32BitsPair.
 *
 * @par Copyright:
 *   (c) 2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_Long64From32BitsPair_h_
#define polybori_groebner_Long64From32BitsPair_h_

// include basic definitions
#include "groebner_defs.h"
#include "PseudoLongLong.h"

BEGIN_NAMESPACE_PBORIGB

/** @class Long64From32BitsPairBase
 * @brief This class defines an auxiliary class for @c Long64From32BitsPair.
 *
 **/

template <unsigned long High, unsigned long Low, unsigned SizeOfLong>
class Long64From32BitsPairBase;


template <unsigned long High, unsigned long Low>
class Long64From32BitsPairBase<High, Low, 4> {
public:
  static PseudoLongLong<High, Low> get() { 
    return PseudoLongLong<High, Low>();
  }
};


template <unsigned long High,
          unsigned long Low>
class Long64From32BitsPairBase<High, Low, 8> {
public:
  typedef unsigned long long_type;
  static long_type get() { return (High << sizeof(long_type)*4) + Low; }
};

/** @class Long64From32BitsPair
 * @brief This class defines Long64From32BitsPair.
 *
 **/
template <unsigned long High, unsigned long Low>
class Long64From32BitsPair:
  public Long64From32BitsPairBase<High, Low,
				  sizeof(unsigned long)> {
public:
  typedef unsigned long long_type;

};


END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_Long64From32BitsPair_h_ */
