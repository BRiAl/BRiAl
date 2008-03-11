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

void changeAssign(BooleSet& c, BooleSet::idx_type idx){
  c.changeAssign(idx);
}

static BooleSet if_then_else(idx_type i,const BooleSet& a, const BooleSet& b){
  return BooleSet(i,a,b);
}
static BooleSet if_then_else_var(const BooleVariable & v,const BooleSet& a, const BooleSet& b){
  idx_type i=v.index();
  return BooleSet(i,a,b);
}
BooleSet the_set_itself(const BooleSet &s){
    return s;
}
BooleSet navi_to_set(const BooleSet::navigator& navi, const BooleSet::ring_type& ring){
  return BooleSet(navi, ring);
}

void export_bset(){

  typedef bool (BooleSet::*owns_func_type)(const BooleSet::term_type &) const;
  typedef BooleSet (BooleSet::*divisors_func_type)(const BooleSet::term_type &) const;
  boost::python::class_<BooleSet>("BooleSet", "Represents set of Boolean terms")
    .def(boost::python::init <const BooleSet&>())
    .def(boost::python::init <const BooleSet::navigator&,
         const BooleSet::ring_type&>())
    .def(boost::python::init <>())
    .def(self==self)
    .def("__repr__", streamable_as_str<BooleSet>)
    .def("__str__", streamable_as_str<BooleSet>)
    .def("__len__", &BooleSet::length)
    .def("__iter__", range(&BooleSet::begin, &BooleSet::end))
    .def("__hash__", &BooleSet::hash, "Fast hash code, based on the \
pointer to the underlying ZDD node. \nIt may vary from runtime to runtime.")
    .def("stableHash", &BooleSet::stableHash, "Reproducible hash code")
    .def("cartesianProduct", &BooleSet::cartesianProduct, "Cartesian product")
    .def("diff", &BooleSet::diff, "Set difference")
    .def("__mod__", mod_mon_set)
    .def("divide", &BooleSet::divide, "Set division by a given term")
    .def("set", the_set_itself)
    .def("subset1",&BooleSet::subset1, "Returns BooleSet, where the nodes \
corresponding to Variables of given index is replaced by its then-branch" )
    .def("subset0",&BooleSet::subset0, "Returns BooleSet, where the nodes \
corresponding to Variables of given index is replaced by its else-branch")
    .def("change", &BooleSet::change, 
         "Substitute variable of given index by its complement")
    .def("empty", &BooleSet::emptiness, "Check, whether BooleSet is empty")
    .def("nNodes", &BooleSet::nNodes, "Number of nodes of the underlying ZDD")
    .def("nSupport", &BooleSet::nSupport)
    .def("union",&BooleSet::unite, "Set union")
    .def("vars",&BooleSet::usedVariables, "Variables occurring in BooleSet")
    .def("navigation", &BooleSet::navigation,
         "Navigate through underlying ZDD structure")
    .def("ring", &BoolePolynomial::ring, "Get corresponding ring")
    .def("includeDivisors",include_divisors)
    .def("minimalElements",minimal_elements, 
         "Get minimal elements with respect to inclusion")
    .def("__contains__", (owns_func_type) &BooleSet::owns, 
         "Check whether a term is included in BooleSet")
    .def("multiplesOf",(divisors_func_type) &BooleSet::multiplesOf, 
         "Compute intersection with multiples of the right-hand side term")
    .def("divisorsOf",(divisors_func_type) &BooleSet::divisorsOf, 
         "Compute intersection with divisors of the right-hand side term")
    .def("existAbstract",&BooleSet::existAbstract, "a.existsAbstract(m) returns\
 a BooleSet, where every term t is included, where exists n such that n*m=t*m\
 and n is element of a")
    .def("sizeDouble",&BooleSet::sizeDouble, 
         "Number of elements as double floating point number.")
    .def("intersect", &BooleSet::intersect, "Set intersection");
  def("if_then_else",if_then_else, "if-then else operator");
  def("if_then_else",if_then_else_var, "if-then else operator");
}
