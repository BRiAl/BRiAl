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

  typedef boost::python::init<const BooleEnv::ring_type&,
    const BooleEnv::ring_type& , const std::vector<Polynomial>&  >
    constructor_type;

  boost::python::class_<FGLMStrategy > ("FGLMStrategy", "FGLM Strategy",
                                        constructor_type())
    .def("main", &FGLMStrategy::main);

}

