/*
 *  Poly_wrapper.cc
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 04.04.06.
 *  Copyright 2006 The PolyBoRi Team. All rights reserved.
 *
 */
#include <boost/python.hpp>
#include <iostream>
#include "polybori.h"
#include "pbori_defs.h"
#include "groebner_alg.h"
#include "interpolate.h"
#include "out_helper.h"
using namespace boost::python;
using namespace std;
USING_NAMESPACE_PBORI
USING_NAMESPACE_PBORIGB
#include "Poly_wrapper.h"
static BoolePolynomial poly_power(const BoolePolynomial& p, int n){
    if (n==0) return BooleMonomial();
    return p;
}
static void print_polynomial(const BoolePolynomial & p){
  p.print(cout);
}
static int poly_hash(const BoolePolynomial& p){
  return p.lmHash();
}
static void plot(const BoolePolynomial& p, const char* c){
  p.prettyPrint(c);
}

static BooleSet poly_diagram_as_set(const Polynomial& p){
    return p.diagram();
}
void export_poly(){

  BoolePolynomial::set_type  (BoolePolynomial::*set)(void) const =
    &BoolePolynomial::set;

//  const BoolePolynomial::set_type&  (BoolePolynomial::*set)(void) const =
//    &BoolePolynomial::set;


  boost::python::class_<BoolePolynomial>("Polynomial")
  .def(init<>())
  .def(init<const BoolePolynomial &>())
  .def(init<const BoolePolynomial::navigator &>())
  .def(init<const CTypes::dd_type &>())
  .def(init<const BooleVariable &>())
  .def(init<const BooleMonomial &>())
  .def(boost::python::init<bool>())
  .def(boost::python::init<int>())
  .def("__hash__", poly_hash)
  .def("__len__", &BoolePolynomial::length)
  .def("__iter__", range(&BoolePolynomial::orderedBegin, &BoolePolynomial::orderedEnd))
  .def("__pow__",poly_power)
  .def(self+=self)
  //.def(self*=self)
  //.def(self/self)
  //.def(self/=self)
  .def(self+=BooleVariable())
  //.def((BoolePolynomial)BooleVariable()*self)
  //.def(BooleMonomial()*self)
  .def(self*BooleMonomial())
  .def(self+BooleVariable())
  .def(BooleVariable()+self)
  .def(self+BooleMonomial())
  .def(BooleMonomial()+self)
  .def(self+=BooleMonomial())
  .def(self*=BoolePolynomial())
  .def(self*=BooleMonomial())
  .def(BooleMonomial()*self)
  //.def(BooleVariable()*self)
  //.def(self*BooleVariable())
  .def(self*self)
  .def(self*=BooleMonomial())
  .def(self*=BooleVariable())
  .def(self*BooleVariable())
  .def(self/BooleVariable())
  .def(self/BooleMonomial())
  .def(self%BooleVariable())
  .def(self%BooleMonomial())
  .def(self%self)
  .def(int()+self)
  .def(self+int())
  .def(self+=int())
  //.def(self* BooleVariable())
  .def(self+BooleVariable())
  //.def(self*self)
  .def(self==self)
  .def(self==bool())
  .def(self!=self)
  .def(self!=bool())
  .def(self+self)
  .def("isZero", &BoolePolynomial::isZero)
  .def("isOne", &BoolePolynomial::isOne)

  .def("deg", &BoolePolynomial::deg)
  .def("lmDivisors", &BoolePolynomial::lmDivisors)
  .def("lead", &BoolePolynomial::lead)
  .def("lexLead", &BoolePolynomial::lexLead)
  .def("firstTerm", &BoolePolynomial::firstTerm)
  .def("reducibleBy", &BoolePolynomial::reducibleBy)

  .def("lmDeg", &BoolePolynomial::lmDeg)
  .def("lexLmDeg", &BoolePolynomial::lexLmDeg)
  .def("constant", &BoolePolynomial::isConstant)
  .def("nNodes", &BoolePolynomial::nNodes)
  .def("nVars", &BoolePolynomial::nUsedVariables)
  .def("vars",&BoolePolynomial::usedVariables)
  .def("totalDegree", &BoolePolynomial::totalDeg)
  .def("gradedPart", &BoolePolynomial::gradedPart)
  //.def("diagram", diagram, return_internal_reference<1>())
  //.def("diagram",poly_diagram_as_set)
  .def("set", set)
  .def("navigation", &BoolePolynomial::navigation)
  .def("elength", &BoolePolynomial::eliminationLength)
  .def("hasConstantPart", &BoolePolynomial::hasConstantPart)
  .def("plot",plot)
  .def("__len__", &BoolePolynomial::length)
  .def("__str__", streamable_as_str<BoolePolynomial>)
  .def("__repr__", streamable_as_str<BoolePolynomial>)
  .def("mapEveryXToXPlusOne",map_every_x_to_x_plus_one)
  .def("zeroesIn",zeroes)
  //wrap usedVariables
  .def("toStdOut", &print_polynomial);
  def("spoly",&spoly);
  
  //implicitly_convertible<BooleVariable,BooleMonomial>();
  //implicitly_convertible<BooleVariable,BoolePolynomial>();
  
}
