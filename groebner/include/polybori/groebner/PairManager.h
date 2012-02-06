// -*- c++ -*-
//*****************************************************************************
/** @file PairManager.h 
 *
 * @author Michael Brickenstein 
 * @date 2011-06-29 
 *
 * This file includes the definition of the class @c PairManager.
 *
 * @par Copyright:
 *   (c) 2006-2010 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_PairManager_h_
#define polybori_groebner_PairManager_h_

#include "PairStatusSet.h"


// include basic definitions
#include "groebner_defs.h"
#include "pairs.h"

#include <utility>

BEGIN_NAMESPACE_PBORIGB

// forward declaration
class GroebnerStrategy;
class ReductionStrategy;

/** @class PairManager
 * @brief This class defines PairManager.
 *
 **/
class PairManager{
public:
  PairStatusSet status;
  GroebnerStrategy* strat;
  PairManager(GroebnerStrategy & strat, 
	      const BoolePolyRing& ring): queue(ring)  {
    this->strat=&strat;
  }

  void appendHiddenGenerators(std::vector<Polynomial>& vec);
  typedef std::priority_queue<Pair,std::vector<PairE>, PairECompare> queue_type;
  queue_type queue;
  void introducePair(const Pair& p);
  Polynomial nextSpoly(ReductionStrategy& gen);
  bool pairSetEmpty() const;
  void cleanTopByChainCriterion();
protected:
  void replacePair(const ReductionStrategy& gen, int& i, int & j);
};

END_NAMESPACE_PBORIGB

#endif /* polybori_PairManager_h_ */
