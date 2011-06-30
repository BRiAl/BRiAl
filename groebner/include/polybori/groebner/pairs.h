/*
 *  pairs.h
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 19.04.06.
 *  Copyright 2006 The PolyBoRi Team. See LICENSE file.
 *
 */

#ifndef PB_PAIR_H
#define PB_PAIR_H

#include "PolyEntry.h"
#include "PairData.h"
#include "IJPairData.h"
#include "PolyPairData.h"
#include "VariablePairData.h"
#include "PairLS.h"
#include "PairE.h"
#include "PairLSCompare.h"
#include "PairECompare.h"

#include <functional>
#include "groebner_defs.h"

#include <boost/shared_ptr.hpp>
#include <queue>
#include <algorithm>
#include <utility>
#include <set>

BEGIN_NAMESPACE_PBORIGB

//using std::less;





typedef PairE Pair;

 END_NAMESPACE_PBORIGB

#endif
