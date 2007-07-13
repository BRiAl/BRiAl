#include <boost/python.hpp>
#include <polybori.h>


//#include <iostream>
//#include "polybori.h"
//#include "pbori_defs.h"
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


class VariableIndexException{
    
};
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
void export_variable_block(){
    def("VariableBlock",variable_block);
    boost::python::class_<VariableBlock<true> >("VariableBlockTrue")
    .def(init<const VariableBlock<true>&>())    
    .def(init<idx_type,idx_type,idx_type>())
    .def("__call__",&VariableBlock<true>::get);
    boost::python::register_exception_translator<
              VariableIndexException>(translator);
    
    boost::python::class_<VariableBlock<false> >("VariableBlockFalse")
        .def(init<const VariableBlock<false>&>())    
        .def(init<idx_type,idx_type,idx_type>())
        .def("__call__",&VariableBlock<false>::get);
        boost::python::register_exception_translator<
                  VariableIndexException>(translator);
}
