// 
//  fglm_wrapper.cc
//  PolyBoRi
//  
//  Created by Michael Brickenstein on 2008-11-13.
//  Copyright 2008 The PolyBoRi Team.
// 

#include <boost/python.hpp>
#include <polybori/groebner/fglm.h>
USING_NAMESPACE_PBORI
USING_NAMESPACE_PBORIGB
void export_fglm(){
    boost::python::class_<FGLMStrategy > ("FGLMStrategy")
         .def(boost::python::init<>())
            .def(boost::python::init<const BooleEnv::ring_type& , const BooleEnv::ring_type& , const std::vector<Polynomial>&  >())
            .def("main", &FGLMStrategy::main);

}

