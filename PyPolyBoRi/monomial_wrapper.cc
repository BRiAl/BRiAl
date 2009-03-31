/*
 *  monomial_wrapper.cc
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 21.04.06.
 *  Copyright 2006 The PolyBoRi Team. See LICENSE file.
 *
 */

#include <boost/python.hpp>

#include "monomial_wrapper.h"


#include <boost/python/tuple.hpp>
#include <boost/python/iterator.hpp>
#include <iostream>
#include <vector>
#include "polybori.h"
#include "pbori_defs.h"
#include "out_helper.h"
using namespace boost::python;
using namespace std;
USING_NAMESPACE_PBORI
#include "Poly_wrapper.h"
static BooleMonomial mon_power(const BooleMonomial& p, int n){
  if (n==0) return BooleMonomial(p.ring());
  return p;
}
//static void print_monomial(const BooleMonomial & p){
//  p.print(cout);
//}
//static void plot(const BooleMonomial& p, const char* c){
//  p.prettyPrint(c);
//}
static boost::python::tuple mon2tuple(const BooleMonomial& m ){
  return tuple(m);
}
static BooleMonomial used_var(const BooleMonomial& m){
    return m;
}
static int one(const BooleMonomial& m){
    return 1;
}
static BoolePolynomial::navigator nav(const BoolePolynomial& p){
    return p.navigation();
}
BOOST_PYTHON_FUNCTION_OVERLOADS(red_overloads, &BooleMonomial::reducibleBy, 1, 1)

void export_monomial(){
  BooleMonomial::set_type  (BooleMonomial::*set)(void) const = &BooleMonomial::set;
 // bool (BooleMonomial::*redv)(void) = &BooleMonomial::diagram;

  bool  (BooleMonomial::*reducibleBy)(const BooleMonomial&) const = &BooleMonomial::reducibleBy;
  boost::python::class_<BooleMonomial>("Monomial", "Boolean monomial")
  .def(init<>("Construct Boolean monomial"))
  .def(init<const BooleMonomial &>())
  .def(init<const BooleVariable &>())
  .def("variables", range(&BooleMonomial::variableBegin, &BooleMonomial::variableEnd))
  //.def("__iter__", boost::python::iterator<BooleMonomial>())
  .def("__hash__", &BooleMonomial::hash, "Fast hash code, based on the \
pointer to the underlying ZDD node. \nIt may vary from runtime to runtime.")
  .def("stable_hash", &BooleMonomial::stableHash, "Reproducible hash code")
  //.def("__len__", &BooleMonomial::length)
  //.def(self+=self)
  //.def(self*=self)
  .def(self!=int())
  .def(self==int())
  .def(int()*self)
  .def(self*int())
  .def(self/self)
  .def(self/int())
  .def("navigation", nav)
  .def("vars_as_monomial",&used_var, 
       "Variables occurring in Polynomial")
    //  .def(self/BooleVariable())
  .def(self/=self)
  //.def("tuple",mon2tuple)
  //.def(self+=BooleVariable())
  //.def(self*=BooleVariable())
  //.def(self* BooleVariable())
  //.def(self+BooleVariable())
  .def(self*self)
  .def(self*int())
  .def(int()*self)
  .def(self==self)
  .def(self==bool())
  .def(self!=self)
  .def(self!=bool())
  .def(self+self)
  .def(self<self)
  .def(self>self)
  .def(self<=self)
  .def(self>=self)
  .def("gcd",&BooleMonomial::GCD)
  .def(self+int())
  .def(int()+self)
  .def(int()-self)
  .def(self+ BooleVariable())
  .def(self- BooleVariable())
  .def(BooleVariable()+self)
  .def(BooleVariable()-self) 
  //.def("isOne", &BooleMonomial::isOne)
  
    .def("deg", &BooleMonomial::deg, "Degree of the monomial")
  .def("__len__", one, "Constant one function")
  .def("divisors", &BooleMonomial::divisors, 
       "Return Boolean set consisting of all divisors of the monomial")
  .def("multiples", &BooleMonomial::multiples, 
       "Return Boolean set consisting of all multiples of the monomial")
  //.def("reducibleBy", &BooleMonomial::reducibleBy)
  .def("__str__", streamable_as_str<BooleMonomial>)
  .def("__repr__", streamable_as_str<BooleMonomial>)
  .def("__pow__",mon_power)
    //  .def("divisors", &BooleMonomial::divisors)
  //.def("lmDeg", &BooleMonomial::lmDeg)

  //.def("totalDegree", &BooleMonomial::totalDeg)
  //.def("diagram", diagram,return_internal_reference<1>());
    .def("set", set, "Convert to BooleSet")
    .def("reducible_by", reducibleBy, "Test for reducibility")
    .def("ring", &BooleMonomial::ring, "Get corresponding ring");
  //wrap usedVariables
  //.def("toStdOut", &print_monomial);

  
}
