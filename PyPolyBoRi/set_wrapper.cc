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
#include "CDDInterface.h"
#include "polybori.h"
using namespace boost::python;
using namespace std;
USING_NAMESPACE_PBORI
#include "set_wrapper.h"
void changeAssign(BooleSet& c, BooleSet::idx_type idx){
  c.changeAssign(idx);
}



void export_bset(){
  boost::python::class_<BooleSet>("BooleSet")
  .def(boost::python::init <const BooleSet&>())
  .def(boost::python::init <const CTypes::dd_type&>())
  .def(boost::python::init <const BooleSet::navigator& >())
  .def(boost::python::init <>())
 

  .def("__len__", &BooleSet::length)
  .def("__iter__", range(&BooleSet::begin, &BooleSet::end))
  .def("product", &BooleSet::product)
  .def("unateProduct", &BooleSet::unateProduct)
  //.def("diagram", &BooleSet::diagram)
  //.def("weakDiv", &BooleSet::weakDiv)
  //.def("divide", &BooleSet::divide)
  .def("weakDivide", &BooleSet::weakDivide)
  .def("divide", &BooleSet::divide)
  //.def("printCover", &BooleSet::printCover)
  .def("diff", &BooleSet::diff)
  //.def("diffConst", &BooleSet::diffConst)
  .def("subset1",&BooleSet::subset1)
  .def("subset0",&BooleSet::subset0)
  .def("change", &BooleSet::change)
  //.def("changeAssign", changeAssign)
  .def("empty", &BooleSet::emptiness)
  .def("nNodes", &BooleSet::nNodes)
  .def("nSupport", &BooleSet::nSupport)
  .def("union",&BooleSet::unite)
  .def("navigation", &BooleSet::navigation)
  .def("__contains__", &BooleSet::owns)
  .def("intersect", &BooleSet::intersect);

}
