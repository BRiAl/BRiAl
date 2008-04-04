/*
 *  strategy_wrapper.cc
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 21.04.06.
 *  Copyright 2006 The PolyBoRi Team. All rights reserved.
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
#include "interpolate.h"
//#include <iostream>
//#include "polybori.h"
//#include "pbori_defs.h"
using namespace boost::python;
using namespace std;
#include "variable_block.h"
USING_NAMESPACE_PBORIGB
USING_NAMESPACE_PBORI
class StrategyIndexException{
    
};
static void translator_gi(StrategyIndexException const& x) {
    PyErr_SetString( PyExc_IndexError, "Wrong Index access at strategy");
}
class StrategyIterator{
public:
    PolyEntryVector::const_iterator it;
    StrategyIterator(PolyEntryVector::const_iterator it){
        this->it=it;
    }
    bool operator==(const StrategyIterator other){
        return it==other.it;
    }
    const Polynomial& operator*(){
        return it->p;
    }
    const Polynomial* operator->(){
        return &(it->p);
    }
    StrategyIterator operator++(int){
        StrategyIterator cp(*this);
        ++it;
        return cp;
    }
    StrategyIterator& operator++(){
        ++it;
        return *this;
    }
    typedef std::input_iterator_tag iterator_category;
    typedef Polynomial value_type;
    typedef PolyEntryVector::const_iterator::difference_type difference_type;
    typedef const Polynomial* pointer;
    typedef const Polynomial reference;
};
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
static Polynomial get_gen_by_lead(const GroebnerStrategy& strat,  const Monomial& m){
    lm2Index_map_type::const_iterator it,end;
    end=strat.lm2Index.end();
    it=strat.lm2Index.find(m);
    if UNLIKELY(it==end){
        throw StrategyIndexException();
    }
    return strat.generators[it->second].p;
}
static Polynomial get_ith_gen(const GroebnerStrategy& strat, int i){
    if UNLIKELY((i<0)||(i>=strat.generators.size())){
        throw StrategyIndexException();
    }
    return strat.generators[i].p;
}
class DuplicateLeadException{};


static void translator_g(PolynomialIsZeroException const& x) {
    PyErr_SetString( PyExc_ValueError, "zero polynomial not allowed here");
}
static void translator_d(DuplicateLeadException const& x) {
    PyErr_SetString( PyExc_ValueError, "strategy contains already a polynomial with same lead");
}


static void add_generator(GroebnerStrategy& strat, const Polynomial& p){
    if UNLIKELY(p.isZero()) throw PolynomialIsZeroException();
    Monomial m=p.lead();
    if UNLIKELY(strat.leadingTerms.owns(m)) throw DuplicateLeadException();
    strat.addGenerator(p);
}
static StrategyIterator stratbegin(const GroebnerStrategy& strat){
    return StrategyIterator(strat.generators.begin());
}
static StrategyIterator stratend(const GroebnerStrategy& strat){
    return StrategyIterator(strat.generators.end());
}

static void add_as_you_wish(GroebnerStrategy& strat, const Polynomial& p){
    if UNLIKELY(p.isZero()) throw PolynomialIsZeroException();
    strat.addAsYouWish(p);
}

static void add_generator_delayed(GroebnerStrategy& strat, const Polynomial& p){
    if UNLIKELY(p.isZero()) throw PolynomialIsZeroException();
    strat.addGeneratorDelayed(p);
}

void export_strategy(){
  export_slimgb();
  boost::python::class_<GroebnerStrategy>("GroebnerStrategy")
  .def(init<>())
  .def(init<const GroebnerStrategy&>())
  .def("suggestPluginVariable",&GroebnerStrategy::suggestPluginVariable)
  .def("addGenerator", add_generator)//&GroebnerStrategy::addGenerator)
  .def("addGeneratorDelayed", add_generator_delayed)
  .def("llReduceAll", &GroebnerStrategy::llReduceAll)
  .def("addAsYouWish",add_as_you_wish)
  .def("implications",implications)
  .def("nextSpoly", &GroebnerStrategy::nextSpoly)
  .def("allSpolysInNextDegree", nextDegreeSpolys)
  .def("someSpolysInNextDegree", someNextDegreeSpolys)
  .def("smallSpolysInNextDegree",small_next_degree_spolys)
  .def("__len__",nGenerators)
  .def("__iter__",range(stratbegin, stratend))
  .def("__getitem__", get_ith_gen)
  .def("__getitem__", get_gen_by_lead)
  
  .def("cleanTopByChainCriterion", cleanTopByChainCriterion)
  .def("allGenerators", &GroebnerStrategy::allGenerators)
  
#ifdef HAVE_NTL
  .def("noroStep", &GroebnerStrategy::noroStep)
#endif
#if defined(HAVE_NTL) || defined(HAVE_M4RI)
  .def("faugereStepDense", &GroebnerStrategy::faugereStepDense)
#endif
  .def("toStdOut", printGenerators)
  .def("variableHasValue",&GroebnerStrategy::variableHasValue)
  .def_readonly("chainCriterions",&GroebnerStrategy::chainCriterions)
  .def_readonly("llReductor",&GroebnerStrategy::llReductor)
  .def_readonly("minimalLeadingTerms",&GroebnerStrategy::minimalLeadingTerms)
  .def_readonly("leadingTerms",&GroebnerStrategy::leadingTerms)
  .def_readonly("monomials",&GroebnerStrategy::monomials)
  .def_readwrite("optRedTail",&GroebnerStrategy::optRedTail)
  .def_readwrite("optLL",&GroebnerStrategy::optLL)
  .def_readwrite("optLinearAlgebraInLastBlock",&GroebnerStrategy::optLinearAlgebraInLastBlock)
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
  .def("nf",&GroebnerStrategy::nf)
  .def("npairs", npairs);
  def("translate_indices",translate_indices);
  def("mult_fast_sim_C",mult_fast_sim);
  def("set_variable_name",&BooleEnv::setVariableName);
  def("mod_var_set",mod_var_set);
  def("interpolate",interpolate);
  def("interpolate_smallest_lex",interpolate_smallest_lex);
  def("zeroes",zeroes);
  def("contained_vars",contained_variables_cudd_style);
  def("map_every_x_to_x_plus_one",map_every_x_to_x_plus_one);
  boost::python::register_exception_translator<
              PolynomialIsZeroException>(translator_g);
  boost::python::register_exception_translator<
            DuplicateLeadException>(translator_d);
        boost::python::register_exception_translator<
                    StrategyIndexException>(translator_gi);
}
