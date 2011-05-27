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
//

#include <boost/python/tuple.hpp>
#include <boost/python/iterator.hpp>
#include <iostream>
#include <vector>
#include <polybori.h>

#include "out_helper.h"
using namespace boost::python;
using namespace std;
USING_NAMESPACE_PBORI
#include "Poly_wrapper.h"

//#include "Poly_wrapper.h"
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
  return boost::python::tuple(m);
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
  BoolePolyRing dummy_ring(1);
  BooleVariable var(0, dummy_ring);

  BooleMonomial::set_type  (BooleMonomial::*set)(void) const = &BooleMonomial::set;
 // bool (BooleMonomial::*redv)(void) = &BooleMonomial::diagram;

  bool  (BooleMonomial::*reducibleBy)(const BooleMonomial&) const = &BooleMonomial::reducibleBy;

  boost::python::class_<BooleMonomial>("Monomial", "Boolean monomial",
       init<const BoolePolyRing&>("Construct Boolean monomial from ring"))
  .def(init<const BooleMonomial &>())
  .def(init<const BooleVariable &>())
  .def("is_one", &BooleMonomial::isOne)
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
  .def(self+ var)
  .def(self- var)
  .def(var+self)
  .def(var-self) 
  //.def("isOne", &BooleMonomial::isOne)
  .def("has_constant_part", has_constant_part_wrap)
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
  .def("lead", lead_wrap)
  .def("lex_lead", lex_lead_wrap)
  .def("lex_lead_deg", lex_lead_deg_wrap)
  .def("lead_deg", lead_deg_wrap)
  //.def("totalDegree", &BooleMonomial::totalDeg)
  //.def("diagram", diagram,return_internal_reference<1>());
    .def("set", set, "Convert to BooleSet")
    .def("reducible_by", reducibleBy, "Test for reducibility")
    .def("ring", &BooleMonomial::ring, 
        return_internal_reference<>(),
       "Get corresponding ring");
  //wrap usedVariables
  //.def("toStdOut", &print_monomial);

  typedef BooleMonomial value_type;
  typedef value_type::var_type var_type;
  typedef value_type::exp_type exp_type;
  typedef value_type::ring_type ring_type;

  boost::python::class_<MonomialFactory>("MonomialFactory",
     "Generator for Boolean monomials",
     init<const ring_type &>("Construct Generator for given ring") )
    .def("__call__",
         (value_type(MonomialFactory::*)() const)
         (&MonomialFactory::operator()))

    .def("__call__",
         (value_type(MonomialFactory::*)(var_type) const)
         (&MonomialFactory::operator()))

    .def("__call__",
         (value_type(MonomialFactory::*)(const BooleMonomial&) const)
         (&MonomialFactory::operator()))
   
    .def("__call__",
         (value_type(MonomialFactory::*)(const exp_type&) const)
         (&MonomialFactory::operator()))

    .def("__call__",
         (value_type(MonomialFactory::*)(const exp_type&, const ring_type&) const)
         (&MonomialFactory::operator()));
  
}
