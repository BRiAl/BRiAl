/*
 *  dd_wrapper.cpp
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 04.04.06.
 *  Copyright 2006 The PolyBoRi Team. All rights reserved.
 *
 */
#include <boost/python.hpp>
#include <iostream>
#include "CDDInterface.h"
#include "polybori.h"
#include "interpolate.h"
#include "groebner_alg.h"
#include "nf.h"
#include "out_helper.h"

using namespace boost::python;
using namespace std;
USING_NAMESPACE_PBORI
USING_NAMESPACE_PBORIGB

#include "set_wrapper.h"

class ITEIndexException{
    
};

void changeAssign(BooleSet& c, BooleSet::idx_type idx){
  c.changeAssign(idx);
}
static void translator(ITEIndexException const& x) {
    PyErr_SetString( PyExc_ValueError, "node index must be smaller than top indices in then and else branch");
}
static BooleSet if_then_else(idx_type i,const BooleSet& a, const BooleSet& b){
    if ((i>=*a.navigation())|| (i>=*b.navigation())){
        throw ITEIndexException();
    }
    return BooleSet(i,a,b);
}
BooleSet the_set_itself(const BooleSet &s){
    return s;
}
void export_bset(){

  typedef bool (BooleSet::*owns_func_type)(const BooleSet::term_type &) const;
  typedef BooleSet (BooleSet::*divisors_func_type)(const BooleSet::term_type &) const;
  boost::python::class_<BooleSet>("BooleSet")
  .def(boost::python::init <const BooleSet&>())
  .def(boost::python::init <const BooleSet::navigator& >())
  .def(boost::python::init <>())
  .def(self==self)
  .def("__repr__", streamable_as_str<BooleSet>)
  .def("__str__", streamable_as_str<BooleSet>)
  .def("__len__", &BooleSet::length)
  .def("__iter__", range(&BooleSet::begin, &BooleSet::end))
  .def("__hash__", &BooleSet::hash)
  .def("stableHash", &BooleSet::stableHash)
  .def("cartesianProduct", &BooleSet::cartesianProduct)
  .def("diff", &BooleSet::diff)
  .def("__mod__", mod_mon_set)
  .def("divide", &BooleSet::divide)
  .def("set", the_set_itself)
  .def("subset1",&BooleSet::subset1)
  .def("subset0",&BooleSet::subset0)
  .def("change", &BooleSet::change)
  .def("empty", &BooleSet::emptiness)
  .def("nNodes", &BooleSet::nNodes)
  .def("nSupport", &BooleSet::nSupport)
  .def("union",&BooleSet::unite)
  .def("vars",&BooleSet::usedVariables)
  .def("navigation", &BooleSet::navigation)
  .def("includeDivisors",include_divisors)
  .def("minimalElements",minimal_elements)
  .def("__contains__", (owns_func_type) &BooleSet::owns)
  .def("multiplesOf",(divisors_func_type) &BooleSet::multiplesOf)
  .def("divisorsOf",(divisors_func_type) &BooleSet::divisorsOf)
  .def("existAbstract",&BooleSet::existAbstract, "a.existsAbstract(m) returns a BooleSet, where every term t is included, where exists n such that n*m=t*m and n is element of a")
  .def("sizeDouble",&BooleSet::sizeDouble)
  .def("intersect", &BooleSet::intersect);
  def("if_then_else",if_then_else);
  boost::python::register_exception_translator<
            ITEIndexException>(translator);
}
