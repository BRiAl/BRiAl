#include <boost/python.hpp>
#include <iostream>
#include "polybori.h"
#include "pbori_defs.h"
#include "dd_wrapper.h"
#include "Poly_wrapper.h"
#include "navigator_wrap.h"
#include "variable_block.h"
//#include "pairs.h"
#include "strategy_wrapper.h"
#include "monomial_wrapper.h"
#include "set_wrapper.h"
#include "out_helper.h"
#include "test_util.h"
#ifdef HAVE_NTL
#include "ntl_wrapper.h"
#endif
#ifdef HAVE_M4RI
#define PACKED 1
#include "../M4RI/packedmatrix.h"
#include "../M4RI/grayflex.h"
#endif
using namespace boost::python;
using namespace std;

USING_NAMESPACE_PBORI
#define WRAP_ALSO_CUDD 1

void print_variable(const BooleVariable & p){
  ((const BoolePolynomial&) p).print(cout);
}
bool have_degree_order(){
    return BoolePolyRing::isDegreeOrder();
}

BOOST_PYTHON_MODULE(PyPolyBoRi){
  
  BoolePolyRing r;
  #ifdef HAVE_M4RI
  buildAllCodes();
  
  setupPackingMasks();
  #endif
   //workaround for having a current_ring
  implicitly_convertible<BooleVariable,BooleMonomial>();
  implicitly_convertible<BooleVariable,BoolePolynomial>();
  implicitly_convertible<BooleMonomial,BoolePolynomial>();
  implicitly_convertible<int,BoolePolynomial>();
  implicitly_convertible<BooleSet,CTypes::dd_type>();
  implicitly_convertible<BoolePolynomial,CTypes::dd_type>();
  implicitly_convertible<BoolePolynomial,BooleSet>();
  implicitly_convertible<CTypes::dd_type,BoolePolynomial>();
  implicitly_convertible<BooleSet,BoolePolynomial>();
  implicitly_convertible<CTypes::dd_type,BooleSet>();
  def("change_ordering",&BoolePolyRing::changeOrdering);
  export_dd();
  export_poly();
  export_nav();
  export_test_util();
  #ifdef HANVE_NTL
  export_ntl();
  #endif
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
  def("have_degree_order", have_degree_order);
  boost::python::class_<BooleVariable>("Variable")
  .def(init<const BooleVariable &>())
  .def(init<BooleVariable::idx_type>())
  .def(self*self)
  .def(self+self)
  .def(self+int())
  .def(int()+self)
  .def(self==self)
  .def("__str__", streamable_as_str<BooleVariable>)
  .def("__repr__", streamable_as_str<BooleVariable>)
  .def("__hash__", &BooleVariable::index)

  
    .def("toStdOut", print_variable);
  export_strategy();
  export_monomial();
  export_bset();
  export_variable_block();
   }
#ifdef PB_STATIC_PROFILING_VERSION
int main(int argc,char* argv[]){
  Py_Initialize();
  initPyPolyBoRi();
  PyRun_SimpleString("from sys import path");
  PyRun_SimpleString("path.append('.')");
  PyRun_SimpleString("path.append('../pyroot')");

  PyRun_SimpleString("import toprofile");
  Py_Finalize();
  return 0;
}
#endif
/*


BDD PortToBdd() const;
*/
