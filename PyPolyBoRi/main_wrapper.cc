#include <boost/python.hpp>
#include "polybori.h"

using namespace boost::python;
USING_NAMESPACE_PBORI

BOOST_PYTHON_MODULE(PyPolyBoRi){
  

  boost::python::class_<BoolePolyRing>("BoolePolyRing")
    //.def(boost::python::init <>())
    .def(boost::python::init <BoolePolyRing::size_type>())
  .def("nVariables", &BoolePolyRing::nvariables);
  }