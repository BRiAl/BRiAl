/*
 *  navigator_wrap.cc
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 06.04.06.
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


static CCuddNavigator then_branch(const CCuddNavigator & nav){
  CCuddNavigator res=nav;
  res.incrementThen();
  return res;
}
static CCuddNavigator else_branch(const CCuddNavigator & nav){
  CCuddNavigator res=nav;
  res.incrementElse();
  return res;
}
static CCuddNavigator::value_type value(const CCuddNavigator & nav){
  return *nav;
}
void export_nav(){
  boost::python::class_<CCuddNavigator>("CCuddNavigator")
  .def(init<>())
  .def(init<const CCuddNavigator &>())
  .def("thenBranch", then_branch)
  .def("elseBranch", else_branch)
  .def(self==self)
  .def(self!=self)
  .def("value",value)
  .def("constant", &CCuddNavigator::isConstant)
  .def("terminalOne", &CCuddNavigator::terminalValue)
  .def("__hash__", &CCuddNavigator::hash)
  .def("valid", &CCuddNavigator::isValid);
  def("one", &BoolePolyRing::ringOne);
  def("zero", &BoolePolyRing::ringZero);
  def("nVars", &BoolePolyRing::nRingVariables);
}
