/*  Copyright (c) 2005-2007 by The PolyBoRi Team */

#include <boost/python.hpp>
#include <boost/python/default_call_policies.hpp>
#include <boost/python/return_value_policy.hpp>
#include "misc_wrapper.h"

#include <polybori.h>
#include <polybori/pbori_defs.h>
#include <iostream>
using namespace boost::python;
using namespace std;
#include <polybori/groebner/groebner_alg.h>
#include <polybori/groebner/randomset.h>
#include <polybori/groebner/interpolate.h>
#include <polybori/groebner/linear_algebra_step.h>
#include <polybori/groebner/nf.h>
USING_NAMESPACE_PBORI
USING_NAMESPACE_PBORIGB

double count_double(const MonomialSet &d){
    return d.sizeDouble();
}

void testvalidstrat(const GroebnerStrategy& strat){
    int s=strat.generators.size();
    int i;
    for(i=0;i<s;i++){
        PBORI_ASSERT(strat.generators[i].p.navigation().isValid());
        cout<<i<<":"<<strat.generators[i].lead<<endl;
    }
}

idx_type top_index(const BooleSet& s){
    return *(s.navigation());
}

void export_misc(){
  def("mapping", mapping<BoolePolynomial, BooleMonomial>);
  def("substitute_variables",
      substitute_variables<BoolePolynomial::ring_type, 
                           std::vector<BoolePolynomial>, BoolePolynomial> );
  def("testvalidstrat",testvalidstrat);
  def("count_double",count_double);
  def("random_set",random_set);
  def("set_random_seed",set_random_seed);
  def("variety_lex_leading_terms",variety_lex_leading_terms);
  def("variety_lex_groebner_basis",variety_lex_groebner_basis);
//   def("global_ring",&BooleEnv::ring, 
//       return_value_policy<copy_non_const_reference>());
  def("top_index",top_index);
  def("gauss_on_polys", gauss_on_polys);
  
}
