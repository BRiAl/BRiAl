#include <boost/python.hpp>
#include <iostream>
#include "polybori.h"
using namespace boost::python;
using namespace std;
USING_NAMESPACE_PBORI
#define WRAP_ALSO_CUDD 1
void print_polynomial(const BoolePolynomial & p){
  p.print(cout);
}
BOOST_PYTHON_MODULE(PyPolyBoRi){
  

  boost::python::class_<BoolePolyRing>("Ring")
    //.def(boost::python::init <>())
    .def(boost::python::init <BoolePolyRing::size_type>())
//#ifdef WRAP_ALSO_CUUD
  .def("var", &BoolePolyRing::variable,with_custodian_and_ward_postcall<0,1>())
//#endif
  .def("nVars", &BoolePolyRing::nVariables);
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
  boost::python::class_<BooleVariable>("Variable")
  .def(init<const BooleVariable &>())
  .def(init<BooleVariable::idx_type>());
  boost::python::class_<BoolePolynomial>("Polynomial")
  .def(init<>())
  .def(init<const BoolePolynomial &>())
  .def(self+=self)
  .def(self*=self)
  .def("deg", &BoolePolynomial::totalDegree)
  .def("lmDeg", &BoolePolynomial::deg)
  .def("nNodes", &BoolePolynomial::nNodes)
  .def("totalDegree", &BoolePolynomial::nUsedVariables)
  //wrap usedVariables
  //.def("toStdOut", &BoolePolynomial::print)
  
  ;
  
  }
/*


BDD PortToBdd() const;
*/
