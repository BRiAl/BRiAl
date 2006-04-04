/*
 *  dd_wrapper.cpp
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
#include "dd_wrapper.h"
void changeAssign(CTypes::dd_type& c, CTypes::dd_type::idx_type idx){
  c.changeAssign(idx);
}
void export_dd(){
  boost::python::class_<CTypes::dd_type>("DD")
  .def(boost::python::init <const CTypes::dd_type&>())
  .def(boost::python::init <>())
  /*.def(self + self)
  .def(self | self)
  .def(self * self)
  .def(self & self)
  .def(self += self)
  .def(self |= self)
  .def(self *= self)
  .def(self &= self)
  .def(self==self)
  .def(self!=self)
  .def(self<=self)
  .def(self>=self)
  .def(self<self)
  .def(self>self)*/
  //.def(self-self)
  //.def(self-=self)
  //.def("printToStdout", &CTypes::dd_type::print)
  //.def("count", &CTypes::dd_type::count)
  .def("product", &CTypes::dd_type::product)
  .def("unateProduct", &CTypes::dd_type::unateProduct)
  //.def("weakDiv", &CTypes::dd_type::weakDiv)
  //.def("divide", &CTypes::dd_type::divide)
  //.def("weakDivF", &CTypes::dd_type::weakDivF)
  //.def("divideF", &CTypes::dd_type::divideF)
  //.def("printCover", &CTypes::dd_type::printCover)
  .def("diff", &CTypes::dd_type::diff)
  //.def("diffConst", &CTypes::dd_type::diffConst)
  .def("subset1",&CTypes::dd_type::subset1)
  .def("subset0",&CTypes::dd_type::subset0)
  .def("change", &CTypes::dd_type::change)
  .def("changeAssign", changeAssign)
  
  .def("nNodes", &CTypes::dd_type::nNodes)
  .def("nSupport", &CTypes::dd_type::nSupport)
  //.def("union",&CTypes::dd_type::unite)
  
  .def("intersect", &CTypes::dd_type::intersect);
  //.def("ite", &CTypes::dd_type::ite)
  //.def("printMinterm", &CTypes::dd_type::printMinterm);
}