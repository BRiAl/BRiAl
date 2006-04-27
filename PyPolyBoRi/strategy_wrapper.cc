/*
 *  strategy_wrapper.cc
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 21.04.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "strategy_wrapper.h"
#include <boost/python.hpp>
#include "nf.h"
#include <groebner_alg.h>

//#include <iostream>
//#include "polybori.h"
//#include "pbori_defs.h"
using namespace boost::python;
using namespace std;

USING_NAMESPACE_PBORIGB
static int npairs(const GroebnerStrategy& strat){
  return strat.pairs.queue.size();
}
static int nGenerators(const GroebnerStrategy& strat){
  return strat.generators.size();
}
static void cleanTopByChainCriterion(GroebnerStrategy & strat){
  strat.pairs.cleanTopByChainCriterion();
}
void export_strategy(){
  boost::python::class_<GroebnerStrategy>("GroebnerStrategy")
  .def(init<>())
  .def("addGenerator", &GroebnerStrategy::addGenerator)
  .def("addGeneratorDelayed", &GroebnerStrategy::addGeneratorDelayed)
  .def("nextSpoly", &GroebnerStrategy::nextSpoly)
  .def("__len__",nGenerators)
  .def("cleanTopByChainCriterion", cleanTopByChainCriterion)
  .def("npairs", npairs);
  def("nf1",nf1);
  def("nf2",nf2);
}
