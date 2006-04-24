/*
 *  monomial_wrapper.cc
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 21.04.06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "monomial_wrapper.h"

#include <boost/python.hpp>
#include <boost/python/tuple.hpp>
#include <boost/python/iterator.hpp>
#include <iostream>
#include <vector>
#include "polybori.h"
#include "pbori_defs.h"
using namespace boost::python;
using namespace std;
USING_NAMESPACE_PBORI
#include "Poly_wrapper.h"
//static void print_monomial(const BooleMonomial & p){
//  p.print(cout);
//}
//static int poly_hash(const BooleMonomial& p){
//  return p.lmHash();
//}
//static void plot(const BooleMonomial& p, const char* c){
//  p.prettyPrint(c);
//}
static boost::python::tuple mon2tuple(const BooleMonomial& m ){
  return tuple(m);
}
void export_monomial(){
  BooleMonomial::dd_type&  (BooleMonomial::*diagram)(void) = &BooleMonomial::diagram;
 // bool (BooleMonomial::*redv)(void) = &BooleMonomial::diagram;
   
    
  
      
  boost::python::class_<BooleMonomial>("Monomial")
  .def(init<>())
  .def(init<const BooleMonomial &>())
  .def(init<const BooleVariable &>())
  .def(boost::python::init<bool>())
  .def("__iter__", range(&BooleMonomial::begin, &BooleMonomial::end))
  //.def("__iter__", boost::python::iterator<BooleMonomial>())
  .def("__hash__", &BooleMonomial::hash)
  //.def("__len__", &BooleMonomial::length)
  //.def(self+=self)
  .def(self*=self)
  .def(self/self)
  .def(self/=self)
  //.def("tuple",mon2tuple)
  //.def(self+=BooleVariable())
  //.def(self*=BooleVariable())
  //.def(self* BooleVariable())
  //.def(self+BooleVariable())
  .def(self*self)
  .def(self==self)
  .def(self==bool())
  .def(self!=self)
  .def(self!=bool())
  .def(self+self)
  //.def("isOne", &BooleMonomial::isOne)
  
  .def("deg", &BooleMonomial::deg)
  .def("divisors", &BooleMonomial::divisors)
  .def("multiples", &BooleMonomial::multiples)
  //.def("reducibleBy", &BooleMonomial::reducibleBy)
  
  //.def("lmDeg", &BooleMonomial::lmDeg)

  //.def("totalDegree", &BooleMonomial::totalDeg)
  .def("diagram", diagram,return_internal_reference<1>());
  //.def("reducibleBy", &BooleMonomial::reducibleBy, red_overloads());
  //wrap usedVariables
  //.def("toStdOut", &print_monomial);

  
}
