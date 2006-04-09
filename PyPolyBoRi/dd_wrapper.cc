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
#include "dd_wrapper.h"
void changeAssign(CTypes::dd_type& c, CTypes::dd_type::idx_type idx){
  c.changeAssign(idx);
}

typedef CTypes::dd_type dd_type;
typedef CTypes::manager_reference manager_reference;
static dd_type one_path(const dd_type& m_dd){
  dd_type leadterm;

  if (m_dd.emptiness())
  leadterm = m_dd;
  else {
    leadterm = manager_reference(m_dd).blank();
    dd_type::first_iterator start(m_dd.firstBegin()), finish(m_dd.firstEnd());
    
    while (start != finish){
      leadterm.changeAssign(*start);
      ++start;
    }
  }



  return leadterm;
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
  .def("__len__", &dd_type::length)
  .def("product", &CTypes::dd_type::product)
  .def("unateProduct", &CTypes::dd_type::unateProduct)
  //.def("weakDiv", &CTypes::dd_type::weakDiv)
  //.def("divide", &CTypes::dd_type::divide)
  .def("weakDivide", &CTypes::dd_type::weakDivide)
  .def("divide", &dd_type::divide)
  //.def("printCover", &CTypes::dd_type::printCover)
  .def("diff", &CTypes::dd_type::diff)
  //.def("diffConst", &CTypes::dd_type::diffConst)
  .def("subset1",&CTypes::dd_type::subset1)
  .def("subset0",&CTypes::dd_type::subset0)
  .def("change", &CTypes::dd_type::change)
  .def("changeAssign", changeAssign)
  .def("empty", &dd_type::emptiness)
  .def("nNodes", &CTypes::dd_type::nNodes)
  .def("nSupport", &CTypes::dd_type::nSupport)
  .def("union",&CTypes::dd_type::unite)
  .def("navigation", &dd_type::navigation)
  .def("intersect", &CTypes::dd_type::intersect)
  .def("onePath", one_path);
  //.def("ite", &CTypes::dd_type::ite)
  //.def("printMinterm", &CTypes::dd_type::printMinterm);
}