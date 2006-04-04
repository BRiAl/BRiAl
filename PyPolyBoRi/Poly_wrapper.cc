/*
 *  Poly_wrapper.cc
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 04.04.06.
 *  Copyright 2006 Mathematisches Forschungsinstitut Oberwolfach. All rights reserved.
 *
 */
#include <boost/python.hpp>
#include <iostream>
#include "polybori.h"
#include "pbori_defs.h"
using namespace boost::python;
using namespace std;
USING_NAMESPACE_PBORI
#include "Poly_wrapper.h"
void print_polynomial(const BoolePolynomial & p){
  p.print(cout);
}
void export_poly(){

  boost::python::class_<BoolePolynomial>("Polynomial")
  .def(init<>())
  .def(init<const BoolePolynomial &>())
  .def(init<const CTypes::dd_type &>())
  .def(init<const BooleVariable &>())
  .def(self+=self)
  .def(self*=self)
  .def(self/self)
  .def(self/=self)
  .def(self+=BooleVariable())
  .def(self*=BooleVariable())
  .def(self*self)
  .def(self==self)
  .def(self!=self)
  .def(self+self)
  .def("deg", &BoolePolynomial::deg)
  .def("lead", &BoolePolynomial::lead)
  
  .def("lmDeg", &BoolePolynomial::lmDeg)
  .def("nNodes", &BoolePolynomial::nNodes)
  .def("nVars", &BoolePolynomial::nUsedVariables)
  .def("totalDegree", &BoolePolynomial::totalDeg)
  //wrap usedVariables
  .def("toStdOut", &print_polynomial);
  def("spoly",&spoly);
  
}