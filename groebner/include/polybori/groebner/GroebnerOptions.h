// -*- c++ -*-
//*****************************************************************************
/** @file GroebnerOptions.h 
 *
 * @author Michael Brickenstein (original), Alexander Dreyer (refactored)
 * @date 2012-02-03
 *
 * This file includes the definition of the class @c GroebnerOptions.
 *
 * @par Copyright:
 *   (c) 2006-2012 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_GroebnerOptions_h_
#define polybori_GroebnerOptions_h_

// include basic definitions
#include "groebner_defs.h"

#include <string>

BEGIN_NAMESPACE_PBORIGB

/** @class GroebnerOptions
 * @brief This class defines options settings for @c GroebnerStrategy.
 *
 * @note: For most option values we give fixed defaults, but @c
 * optRedTailInLastBlock and @c optLazy are problem dependent.
 **/
class GroebnerOptions {

public:
  /// Constructor with default settings
  GroebnerOptions(bool redTailInLastBlock = false, bool lazy = false):
    optRedTailInLastBlock(redTailInLastBlock), optLazy(lazy),
    matrixPrefix("mat"), optDrawMatrices(false),
    optModifiedLinearAlgebra(false), optDelayNonMinimals(true),
    enabledLog(false), optExchange(true), optHFE(false), optStepBounded(false),
    optAllowRecursion(true), optLinearAlgebraInLastBlock(true),
    reduceByTailReduced(false) { }

  bool optRedTailInLastBlock;
  bool optLazy;

  std::string matrixPrefix;
  bool optDrawMatrices;
 
  bool optModifiedLinearAlgebra;
  bool optDelayNonMinimals;
  bool enabledLog; 

  bool optExchange;
  bool optHFE;
  bool optStepBounded;
  bool optAllowRecursion;
  bool optLinearAlgebraInLastBlock;

  bool reduceByTailReduced;
};

END_NAMESPACE_PBORIGB

#endif /* polybori_GroebnerOptions_h_ */
