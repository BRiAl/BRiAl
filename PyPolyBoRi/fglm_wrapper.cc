// 
//  fglm_wrapper.cc
//  PolyBoRi
//  
//  Created by Michael Brickenstein on 2008-11-13.
//  Copyright 2008 The PolyBoRi Team.
// 
#include <fglm.h>
void export_fglm(){
    boost::python::class_<ReductionStrategy,boost::python::bases<vector<PolyEntry> > > ("ReductionStrategy")
            .def(init<>())
            .def(init<BooleEnv::ring_type, BooleEnv::ring_type, std::vector<Polynomial>());

}

