#include <boost/python.hpp>
#include "polybori.h"

using namespace boost::python;
USING_NAMESPACE_PBORI
#define WRAP_ALSO_CUDD 1

BOOST_PYTHON_MODULE(PyPolyBoRi){
  

  boost::python::class_<BoolePolyRing>("Ring")
    //.def(boost::python::init <>())
    .def(boost::python::init <BoolePolyRing::size_type>())
//#ifdef WRAP_ALSO_CUUD
  .def("var", &BoolePolyRing::variable,with_custodian_and_ward_postcall<0,1>())
//#endif
  .def("nVars", &BoolePolyRing::nvariables);
  boost::python::class_<ZDD>("ZDD")
  .def(boost::python::init <const ZDD&>())
  .def(boost::python::init <>())
  .def(self + self)
  .def(self | self)
  .def(self * self)
  .def(self & self)
  .def(self += self)
  .def(self |= self)
  .def(self *= self)
  .def(self &= self)
  .def(self==self)
  .def(self!=self)
  .def(self<=self)
  .def(self>=self)
  .def(self<self)
  .def(self>self)
  .def(self-self)
  .def(self-=self)
  .def("printToStdout", &ZDD::print)
  .def("count", &ZDD::Count)
  .def("product", &ZDD::Product)
  .def("unateProduct", &ZDD::UnateProduct)
  .def("weakDiv", &ZDD::WeakDiv)
  .def("divide", &ZDD::Divide)
  .def("weakDivF", &ZDD::WeakDivF)
  .def("divideF", &ZDD::DivideF)
  .def("printCover", &ZDD::PrintCover)
  .def("diff", &ZDD::Diff)
  .def("diffConst", &ZDD::DiffConst)
  .def("subset1",&ZDD::Subset1)
  .def("subset0",&ZDD::Subset0)
  .def("change", &ZDD::Change)
  .def("union",&ZDD::Union)
  .def("intersect", &ZDD::Intersect)
  .def("ite", &ZDD::Ite)
  .def("printMinterm", &ZDD::PrintMinterm);
  
  }
/*


BDD PortToBdd() const;
*/