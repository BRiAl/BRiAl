/*  Copyright (c) 2005-2007 by The PolyBoRi Team */

#include <boost/python.hpp>
#include <polybori.h>
#include <polybori/VariableBlock.h>
#include <polybori/factories/VariableFactory.h>

#include "variable_block.h"
#include <boost/type_traits.hpp>

using namespace boost::python;
using namespace std;


USING_NAMESPACE_PBORI


static void translator(VariableIndexException const& x) {
    PyErr_SetString( PyExc_IndexError, "Wrong VariableIndex");
}

// Note: Wrapping the def(...) leads to better performance (for unknown reasons)
void export_variable_block(){

  typedef VariableBlock::idx_type idx_type;
  boost::python::class_<VariableBlock>("VariableBlock",
                       "Polybori's VariableBlock",
                       init<idx_type, idx_type, idx_type, bool,
                            const BoolePolyRing&>())
                                         
  .def(init<const VariableBlock&>())    
  .def("__call__", &VariableBlock::operator());

  register_exception_translator<VariableIndexException>(translator);


  typedef VariableFactory::value_type value_type;
  boost::python::class_<VariableFactory>("VariableFactory",
                       "Curry variables with given ring",
                       init<const BoolePolyRing&>())

    .def("__call__",
         (value_type(VariableFactory::*)() const)
         (&VariableFactory::operator()))

    .def("__call__",
         (value_type(VariableFactory::*)(idx_type) const)
         (&VariableFactory::operator()))
  
    .def("__call__",
         (value_type(VariableFactory::*)(idx_type,
                                         const value_type::ring_type&) const)
         (&VariableFactory::operator()));
}
