/*
 *  strategy_wrapper.cc
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 21.04.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */
#include <boost/python.hpp>
#include "strategy_wrapper.h"
#include <iostream>
#include <vector>
#include "nf.h"
#include <groebner_alg.h>
#include <BoolePolyRing.h>
#include "slimgb_wrapper.h"
//#include <iostream>
//#include "polybori.h"
//#include "pbori_defs.h"
using namespace boost::python;
using namespace std;

USING_NAMESPACE_PBORIGB
USING_NAMESPACE_PBORI
static int npairs(const GroebnerStrategy& strat){
  return strat.pairs.queue.size();
}
static int nGenerators(const GroebnerStrategy& strat){
  return strat.generators.size();
}
static int pairs_top_sugar(const GroebnerStrategy& strat){
  if (strat.pairs.pairSetEmpty())
    return -1;
  else
    return (strat.pairs.queue.top().sugar);
}
static void cleanTopByChainCriterion(GroebnerStrategy & strat){
  strat.pairs.cleanTopByChainCriterion();
}
static void printGenerators(GroebnerStrategy& strat){
  int i;
  for (i=0;i<strat.generators.size();i++){
    std::cout<<(strat.generators[i].p)<<std::endl;
  }
}
static vector<Polynomial> nextDegreeSpolys(GroebnerStrategy& strat){
  vector<Polynomial> res;
  assert(!(strat.pairs.pairSetEmpty()));
  strat.pairs.cleanTopByChainCriterion();
  deg_type deg=strat.pairs.queue.top().sugar;
  
  while((!(strat.pairs.pairSetEmpty())) &&(strat.pairs.queue.top().sugar<=deg)){
    
    assert(strat.pairs.queue.top().sugar==deg);
    res.push_back(strat.nextSpoly());
    strat.pairs.cleanTopByChainCriterion();
  }
  return res;
  
}

static vector<Polynomial> someNextDegreeSpolys(GroebnerStrategy& strat, int n){
  vector<Polynomial> res;
  assert(!(strat.pairs.pairSetEmpty()));
  strat.pairs.cleanTopByChainCriterion();
  deg_type deg=strat.pairs.queue.top().sugar;
  
  while((!(strat.pairs.pairSetEmpty())) &&(strat.pairs.queue.top().sugar<=deg) && (res.size()<n)){
    
    assert(strat.pairs.queue.top().sugar==deg);
    res.push_back(strat.nextSpoly());
    strat.pairs.cleanTopByChainCriterion();
  }
  return res;
  
}
static vector<Polynomial> small_next_degree_spolys(GroebnerStrategy& strat, double f, int n){
  vector<Polynomial> res;
  assert(!(strat.pairs.pairSetEmpty()));
  strat.pairs.cleanTopByChainCriterion();
  deg_type deg=strat.pairs.queue.top().sugar;
  wlen_type wlen=strat.pairs.queue.top().wlen;
  while((!(strat.pairs.pairSetEmpty())) &&(strat.pairs.queue.top().sugar<=deg) && (strat.pairs.queue.top().wlen<=wlen*f+2)&& (res.size()<n)){
    
    assert(strat.pairs.queue.top().sugar==deg);
    res.push_back(strat.nextSpoly());
    strat.pairs.cleanTopByChainCriterion();
  }
  return res;
  
}
bool contains_one(const GroebnerStrategy& strat){
  int s=strat.generators.size();
  int i;
  for(i=0;i<s;i++){
    if (strat.generators[i].p.isOne()){
      return true;
    }
  }
  return false;
}
static void implications(GroebnerStrategy& strat, int i){
    strat.addNonTrivialImplicationsDelayed(strat.generators[i]);
    
}
int select_wrapped(const GroebnerStrategy & strat, const Monomial& m){
    return select1(strat,m);
}
static Polynomial get_ith_gen(const GroebnerStrategy& strat, int i){
    return strat.generators[i].p;
}
static void add_generator(GroebnerStrategy& strat, const Polynomial& p){
    strat.addGenerator(p);
}
void export_strategy(){
  export_slimgb();
  boost::python::class_<GroebnerStrategy>("GroebnerStrategy")
  .def(init<>())
  .def(init<const GroebnerStrategy&>())
  .def("suggestPluginVariable",&GroebnerStrategy::suggestPluginVariable)
  .def("addGenerator", add_generator)//&GroebnerStrategy::addGenerator)
  .def("addGeneratorDelayed", &GroebnerStrategy::addGeneratorDelayed)
  .def("llReduceAll", &GroebnerStrategy::llReduceAll)
  .def("addAsYouWish",&GroebnerStrategy::addAsYouWish)
  .def("implications",implications)
  .def("nextSpoly", &GroebnerStrategy::nextSpoly)
  .def("allSpolysInNextDegree", nextDegreeSpolys)
  .def("someSpolysInNextDegree", someNextDegreeSpolys)
  .def("smallSpolysInNextDegree",small_next_degree_spolys)
  .def("__len__",nGenerators)
  
  .def("__getitem__", get_ith_gen)
  .def("cleanTopByChainCriterion", cleanTopByChainCriterion)
#ifdef HAVE_NTL
  .def("noroStep", &GroebnerStrategy::noroStep)
  .def("faugereStepDense", &GroebnerStrategy::faugereStepDense)
#endif
  .def("toStdOut", printGenerators)
  .def("variableHasValue",&GroebnerStrategy::variableHasValue)
  .def_readonly("chainCriterions",&GroebnerStrategy::chainCriterions)
  .def_readonly("llReductor",&GroebnerStrategy::llReductor)
  .def_readonly("monomials",&GroebnerStrategy::monomials)
  .def_readwrite("optRedTail",&GroebnerStrategy::optRedTail)
  .def_readwrite("optLL",&GroebnerStrategy::optLL)
  .def_readwrite("optBrutalReductions",&GroebnerStrategy::optBrutalReductions)
  .def_readwrite("optLazy",&GroebnerStrategy::optLazy)
  .def_readwrite("optExchange",&GroebnerStrategy::optExchange)
  .def_readwrite("optAllowRecursion",&GroebnerStrategy::optAllowRecursion)
  .def_readwrite("enabledLog",&GroebnerStrategy::enabledLog)
  .def_readwrite("optRedTailDegGrowth",&GroebnerStrategy::optRedTailDegGrowth)
  .def_readonly("variableChainCriterions",&GroebnerStrategy::variableChainCriterions)
  .def_readonly("easyProductCriterions",&GroebnerStrategy::easyProductCriterions)
  .def_readonly("extendedProductCriterions",&GroebnerStrategy::extendedProductCriterions)
  .def("topSugar",pairs_top_sugar)
  .def("containsOne",contains_one)
  .def("minimalizeAndTailReduce",&GroebnerStrategy::minimalizeAndTailReduce)
  .def("minimalize",&GroebnerStrategy::minimalize)
	
  .def("select", select_wrapped)
  .def_readwrite("optRedByReduced",&GroebnerStrategy::reduceByTailReduced)
  .def("symmGB_F2",&GroebnerStrategy::symmGB_F2)
  .def("npairs", npairs);
  def("nf1",nf1);
  def("nf2",nf2);
  def("nf_delaying",nf_delaying);
  def("nf_delaying_exchanging", nf_delaying_exchanging);
  def("red_tail_self_tuning", red_tail_self_tuning);
  def("translate_indices",translate_indices);
  def("mult_fast_sim_C",mult_fast_sim);
  def("set_variable_name",&BoolePolyRing::setRingVariableName);
  def("mod_var_set",mod_var_set);
  def("contained_vars",contained_variables_cudd_style);
}
