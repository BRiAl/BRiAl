/*  Copyright (c) 2005-2007 by The PolyBoRi Team */

#include <boost/python.hpp>
#include <polybori.h>
#include "variable_block.h"
#include <boost/type_traits.hpp>
//#include <iostream>
//#include "polybori.h"
//#include "polybori/pbori_defs.h"
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
template <bool reverse> class VariableBlock {
public:
    idx_type size;
    idx_type start_index;
    idx_type offset;
    BooleVariable get(idx_type i){
        if UNLIKELY((i>=start_index+size)||(i<start_index)){
            throw VariableIndexException();
        }
        return BooleVariable(reverse?offset+start_index+size-1-i:i-start_index+offset );
    }
    VariableBlock(idx_type size, idx_type start_index,idx_type offset){
        this->size=size;
        this->start_index=start_index;
        this->offset=offset;
        
    }
    VariableBlock(){}
};
boost::python::object variable_block(idx_type size,idx_type start_index,idx_type offset,bool reverse){
    if (reverse) return boost::python::object(VariableBlock<true>(size,start_index,offset));
    else return boost::python::object(VariableBlock<false>(size,start_index,offset));
    
}

template <class ValueType, class StringType>
void export_variable_block_bool(ValueType, StringType str) {

  boost::python::class_<VariableBlock<ValueType::value> >(str)
    .def(init<const VariableBlock<ValueType::value>&>())    
    .def(init<idx_type,idx_type,idx_type>())
    .def("__call__",&VariableBlock<ValueType::value>::get);
    boost::python::register_exception_translator<
              VariableIndexException>(translator);

}

void export_variable_block_init(){
    def("VariableBlock",variable_block);
}

// Note: Wrapping the def(...) leads to better performance (for unknown reasons)
void export_variable_block(){
  export_variable_block_init();
  export_variable_block_bool(boost::true_type(), "VariableBlockTrue");
  export_variable_block_bool(boost::false_type(), "VariableBlockFalse");

}
