#include <boost/python.hpp>
#include <iostream>
#include "polybori.h"
#include "pbori_defs.h"
#include "dd_wrapper.h"
#include "Poly_wrapper.h"
#include "navigator_wrap.h"

//#include "pairs.h"
#include "strategy_wrapper.h"
#include "monomial_wrapper.h"
#include "set_wrapper.h"
#include "out_helper.h"
using namespace boost::python;
using namespace std;

USING_NAMESPACE_PBORI
#define WRAP_ALSO_CUDD 1

void print_variable(const BooleVariable & p){
  ((const BoolePolynomial&) p).print(cout);
}

BOOST_PYTHON_MODULE(PyPolyBoRi){
  
  BoolePolyRing r; //workaround for having a current_ring
  implicitly_convertible<BooleVariable,BooleMonomial>();
  implicitly_convertible<BooleVariable,BoolePolynomial>();
  def("change_ordering",&BoolePolyRing::changeOrdering);
  export_dd();
  export_poly();
  export_nav();
  enum_<COrderEnums::ordercodes>("OrderCode")
    .value("lp", COrderEnums::lp)
    .value("dlex", COrderEnums::dlex)
    .value("dp_asc",COrderEnums::dp_asc)
    .value("block_dp_asc",COrderEnums::block_dp_asc)
    .value("block_dlex",COrderEnums::block_dlex)
    ;
  
  /*  struct COrderEnums {
  /// Codes For orderings
  enum ordercodes {
    lp,
    dlex,
    dp_asc
  };
};*/
  def("get_order_code",&BoolePolyRing::getOrderCode);  
  def("print_ring_info", &BoolePolyRing::printInfo);
  boost::python::class_<BoolePolyRing>("Ring")
    //.def(boost::python::init <>())
    .def(boost::python::init <BoolePolyRing::size_type>())
    .def(boost::python::init <BoolePolyRing::size_type, int>())
//#ifdef WRAP_ALSO_CUUD
  .def("var", &BoolePolyRing::variable)
  .def("one", &BoolePolyRing::one)
//#endif
  .def("nVars", &BoolePolyRing::nVariables);
  def("append_ring_block", &BoolePolyRing::appendRingBlock);
 
  boost::python::class_<BooleVariable>("Variable")
  .def(init<const BooleVariable &>())
  .def(init<BooleVariable::idx_type>())
  .def(self*self)
  .def(self+self)
  .def("__str__", streamable_as_str<BooleVariable>)
  .def("__repr__", streamable_as_str<BooleVariable>)
  .def(self+int())
  .def(int()+self)
    .def("toStdOut", print_variable);
  export_strategy();
  export_monomial();
  export_bset();
   }
#ifdef PB_STATIC_PROFILING_VERSION
int main(int argc,char* argv[]){
  Py_Initialize();
  initPyPolyBoRi();
  PyRun_SimpleString("from sys import path");
  PyRun_SimpleString("path.append('.')");
  PyRun_SimpleString("path.append('../testsuite/py')");
  PyRun_SimpleString("import toprofile");
  Py_Finalize();
  return 0;
}
#endif
/*


BDD PortToBdd() const;
*/
