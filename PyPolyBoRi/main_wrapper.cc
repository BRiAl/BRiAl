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
void print_variable(const BooleVariable & p){
  ((const BoolePolynomial&) p).print(cout);
}

BOOST_PYTHON_MODULE(PyPolyBoRi){
  
  BoolePolyRing r; //workaround for having a current_ring
  boost::python::class_<BoolePolyRing>("Ring")
    //.def(boost::python::init <>())
    .def(boost::python::init <BoolePolyRing::size_type>())
//#ifdef WRAP_ALSO_CUUD
  .def("var", &BoolePolyRing::variable,with_custodian_and_ward_postcall<0,1>())
  .def("one", &BoolePolyRing::one,with_custodian_and_ward_postcall<0,1>())
//#endif
  .def("nVars", &BoolePolyRing::nVariables);
  boost::python::class_<CTypes::dd_type>("DD")
  .def(boost::python::init <const CTypes::dd_type&>())
  .def(boost::python::init <>())
  /*.def(self + self)
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
  .def(self>self)*/
  //.def(self-self)
  //.def(self-=self)
  //.def("printToStdout", &CTypes::dd_type::print)
  //.def("count", &CTypes::dd_type::count)
  .def("product", &CTypes::dd_type::product)
  .def("unateProduct", &CTypes::dd_type::unateProduct)
  //.def("weakDiv", &CTypes::dd_type::weakDiv)
  //.def("divide", &CTypes::dd_type::divide)
  //.def("weakDivF", &CTypes::dd_type::weakDivF)
  //.def("divideF", &CTypes::dd_type::divideF)
  //.def("printCover", &CTypes::dd_type::printCover)
  .def("diff", &CTypes::dd_type::diff)
  //.def("diffConst", &CTypes::dd_type::diffConst)
  .def("subset1",&CTypes::dd_type::subset1)
  .def("subset0",&CTypes::dd_type::subset0)
  .def("change", &CTypes::dd_type::change)
  .def("nNodes", &CTypes::dd_type::nNodes)
  //.def("union",&CTypes::dd_type::union)
  .def("intersect", &CTypes::dd_type::intersect);
  //.def("ite", &CTypes::dd_type::ite)
  //.def("printMinterm", &CTypes::dd_type::printMinterm);
  boost::python::class_<BooleVariable>("Variable")
  .def(init<const BooleVariable &>())
  .def(init<BooleVariable::idx_type>())
  .def("toStdOut", &print_variable);
  boost::python::class_<BoolePolynomial>("Polynomial")
  .def(init<>())
  .def(init<const BoolePolynomial &>())
  .def(init<const CTypes::dd_type &>())
  .def(init<const BooleVariable &>())
  .def(self+=self)
  .def(self*=self)
  .def(self/self)
  .def(self/=self)
  .def(self+=BooleVariable())
  .def(self*=BooleVariable())
  .def(self*self)
  .def(self+self)
  .def("deg", &BoolePolynomial::totalDeg)
  .def("lmDeg", &BoolePolynomial::deg)
  .def("nNodes", &BoolePolynomial::nNodes)
  .def("totalDegree", &BoolePolynomial::nUsedVariables)
  //wrap usedVariables
  .def("toStdOut", &print_polynomial);
  def("spoly",&spoly);
  }
/*


BDD PortToBdd() const;
*/
