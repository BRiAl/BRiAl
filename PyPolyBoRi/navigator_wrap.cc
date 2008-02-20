/*
 *  navigator_wrap.cc
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 06.04.06.
 *  Copyright 2006 The PolyBoRi Team. All rights reserved.
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
  .def(init<const CCuddNavigator &>("Iterator-like object, which allows to \
navigate through ZDD-based data structures,"))
  .def("thenBranch", then_branch, "Following then-edge of thr root node")
  .def("elseBranch", else_branch, "Following else-edge of thr root node")
  .def(self==self)
  .def(self!=self)
  .def("value",value)
  .def("constant", &CCuddNavigator::isConstant, "Terminal node test")
    .def("terminalOne", &CCuddNavigator::isTerminated, "Check for terminal 1")
  .def("__hash__", &CCuddNavigator::hash, "Fast hash code, based on the \
pointer to the underlying ZDD node. \nIt may vary from runtime to runtime.")
  .def("valid", &CCuddNavigator::isValid, 
       "If navigator is constant, this checks for terminal one");
  def("one", &BooleEnv::one, "Constant one of the active ring");
  def("zero", &BooleEnv::zero, "Constant zero of the active ring");
  def("number_of_variables", &BooleEnv::nVariables, 
      "Number of variables in the active ring");
}
