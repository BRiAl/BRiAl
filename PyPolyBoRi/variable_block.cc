/*  Copyright (c) 2005-2007 by The PolyBoRi Team */

#include <boost/python.hpp>
#include <polybori.h>
#include <polybori/VariableBlock.h>

#include "variable_block.h"
#include <boost/type_traits.hpp>

using namespace boost::python;
using namespace std;


USING_NAMESPACE_PBORI

#ifdef __GNUC__
#define LIKELY(expression) (__builtin_expect(!!(expression), 1))
#define UNLIKELY(expression) (__builtin_expect(!!(expression), 0))
#else
#define LIKELY(expression) (expression)
#define UNLIKELY(expression) (expression)
#endif

typedef CTypes::idx_type idx_type;



static void translator(VariableIndexException const& x) {
    PyErr_SetString( PyExc_IndexError, "Wrong VariableIndex");
}

boost::python::object
variable_block(idx_type size,idx_type start_index,idx_type offset,bool reverse){
  if (reverse) return
    boost::python::object(VariableBlock<true>(size,start_index,offset, BooleEnv::ring()));
    else return boost::python::object(VariableBlock<false>(size,start_index,offset, BooleEnv::ring()));
    
}
boost::python::object
ring_variable_block(const BoolePolyRing& ring) {
  return boost::python::object(VariableBlock<false>(ring));
    
}

template <class ValueType, class StringType>
void export_variable_block_bool(ValueType, StringType str) {

  boost::python::class_<VariableBlock<ValueType::value> >(str,
                                                          "polybori_VariableBlock",
                                                          init<const BoolePolyRing&>())
    .def(init<const VariableBlock<ValueType::value>&>())    
    .def(init<idx_type,idx_type,idx_type, BoolePolyRing>())
    .def("__call__",&VariableBlock<ValueType::value>::operator());
    boost::python::register_exception_translator<
              VariableIndexException>(translator);

}

void export_variable_block_init(){
    def("VariableBlock",variable_block);
    def("ring_variable_block", ring_variable_block);
}

// Note: Wrapping the def(...) leads to better performance (for unknown reasons)
void export_variable_block(){
  export_variable_block_init();
  export_variable_block_bool(boost::true_type(), "VariableBlockTrue");
  export_variable_block_bool(boost::false_type(), "VariableBlockFalse");

}
