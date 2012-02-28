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
#include "DelayedLongLong.h"
#include "LongLongConstant.h"

BEGIN_NAMESPACE_PBORIGB

/** @class Long64From32BitsPairBase
 * @brief This class defines an auxiliary class for @c Long64From32BitsPair.
 *
 **/

template <DelayedLongLong::long_type High,
          DelayedLongLong::long_type Low, unsigned SizeOfLong>
class Long64From32BitsPairBase;


template <DelayedLongLong::long_type High,
          DelayedLongLong::long_type Low>
class Long64From32BitsPairBase<High, Low, 4> {
public:
  static LongLongConstant<High, Low> get() { 
    return LongLongConstant<High, Low>();
  }
};


template <DelayedLongLong::long_type High,
          DelayedLongLong::long_type Low>
class Long64From32BitsPairBase<High, Low, 8> {
public:
  typedef typename DelayedLongLong::long_type long_type;
  static long_type get() { return (High << sizeof(long_type)*4) + Low; }
};

/** @class Long64From32BitsPair
 * @brief This class defines Long64From32BitsPair.
 *
 **/
template <DelayedLongLong::long_type High,
          DelayedLongLong::long_type Low>
class Long64From32BitsPair:
  public Long64From32BitsPairBase<High, Low,
				  sizeof(DelayedLongLong::long_type)> {
public:
  typedef typename DelayedLongLong::long_type long_type;

};


END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_Long64From32BitsPair_h_ */
