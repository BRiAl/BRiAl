/*  Copyright (c) 2005-2007 by The #+PolyBoRi Team */

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
#include "misc_wrapper.h"
#include "set_wrapper.h"
#include "out_helper.h"
#include "test_util.h"
#include "fglm_wrapper.h"
#ifdef HAVE_NTL
#include "ntl_wrapper.h"
#endif
#ifdef HAVE_M4RI
#define PACKED 1
extern "C"{
#include "m4ri/packedmatrix.h"
#include "m4ri/grayflex.h"
}

#endif
using namespace boost::python;
using namespace std;

USING_NAMESPACE_PBORI
//#define EXPORT __attribute__((visibility("default")))
#define WRAP_ALSO_CUDD 1

static BooleMonomial var_power(const BooleVariable& p, int n){
  if (n==0) return BooleMonomial(p.ring());
  return p;
}

void print_variable(const BooleVariable & p){
  ((const BoolePolynomial&) p).print(cout);
}
bool have_degree_order(){
  return BooleEnv::ordering().isDegreeOrder();
}


BoolePolynomial
ring_var(const BooleRing& ring, BooleVariable::idx_type idx) {
  return ring.variable(idx);
}
BoolePolynomial ring_one(const BooleRing& ring) {
  return ring.one();
}
BoolePolynomial ring_zero(const BooleRing& ring) {
  return ring.zero();
}

static void translator_pborierror(const PBoRiError & err) {
    PyErr_SetString(PyExc_ValueError, err.text());
}

static void 
translator_pboridivisionbyzero(const PBoRiGenericError<CTypes::division_by_zero>
                               & err) {  
  PyErr_SetString(PyExc_ZeroDivisionError, err.text());
}
static BooleMonomial used_var(const BooleVariable& v){
    return v;
}
static BoolePolynomial::navigator nav(const BoolePolynomial& p){
    return p.navigation();
}
//EXPORT
BOOST_PYTHON_MODULE(PyPolyBoRi){
  
  BoolePolyRing r;

  #ifdef HAVE_M4RI
  m4ri_build_all_codes();
  //m4ri_init();
  //setupPackingMasks();
  #endif
   //workaround for having a current_ring
  implicitly_convertible<BooleVariable,BooleMonomial>();
  implicitly_convertible<BooleVariable,BoolePolynomial>();
  implicitly_convertible<BooleMonomial,BoolePolynomial>();
  implicitly_convertible<int,BooleConstant>();
  implicitly_convertible<BoolePolynomial,BooleSet>();
  implicitly_convertible<BooleSet,BoolePolynomial>();
  def("change_ordering",&BooleEnv::changeOrdering);

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
  def("get_order_code",&BooleEnv::getOrderCode);  
  def("print_ring_info", &BooleEnv::printInfo);
  
  boost::python::class_<BooleRing>("BooleRing", "Boolean ring")
    .def(boost::python::init <BooleRing::size_type>())
    
    .def("var", ring_var, "i-th ring Variable")
    .def("one", ring_one, "Polynomial one")
    .def("zero", ring_zero, "Polynomial zero")

    .def("n_variables", &BooleRing::nVariables, "Number of ring variables");
  boost::python::class_<BoolePolyRing,boost::python::bases<BooleRing> >("Ring", "Boolean polynomial ring")
    //.def(boost::python::init <>())
    .def("set",&BooleEnv::set, "Activate current Ring")
    .def(boost::python::init <BoolePolyRing::size_type>())
    
    .def(boost::python::init <BoolePolyRing::size_type, int>(
         "Construct a Boolean polynomial ring with the following parameters:\n\
            n -- number of variables (integer)\n\
            order -- term ordering (optinal), one of the following values \n\
                lp: lexicographical ordering (default)\n\
                dlex: degree lexicographical ordering\n\
                dp_asc: degree reverse lexicographical ordering \
with inverted variable order\n\
                block_dp_asc: Block ordering with blocks consisting of dp_asc\n\
                block_dlex: Block ordering with blocks consisting of dlex\n") );

  
  def("append_ring_block", &BooleEnv::appendBlock, 
      "Append integer, which marks start of next block (for block orderings)");
  def("have_degree_order", have_degree_order, 
      "Determines, whether ring ordering is a degree ordering");

  boost::python::class_<BooleConstant>("BooleConstant", 
                                       "Boolean constant value")
    .def(init<const BooleConstant &>())
    .def(init<int>("Convert integer to Boolean value"))
    .def(init<bool>("Convert bool to Boolean value"))
    .def("__str__", streamable_as_str<BooleConstant>)
    .def("__repr__", streamable_as_str<BooleConstant>);
  ;
  boost::python::class_<BooleVariable>("Variable", "Boolean Variable")
  .def(init<const BooleVariable &>())
  .def(init<BooleVariable::idx_type>("Get Variable of given index"))
  .def(self*self)
  .def(self/self)
  .def(self*BooleMonomial())
  .def(self/BooleMonomial())
  .def(BooleMonomial()*self)
  .def(self*int())
  .def(self/int())
  .def(int()*self)
  .def(self+self)
  .def(self+int())
  .def(int()+self)
  .def(self>self)
  .def(self>=self)
  .def(self<self)
  .def(self<=self)
  .def(self==self)
  .def("navigation", nav)
  .def("vars_as_monomial",&used_var, 
       "Variables occurring in Polynomial")
  .def("__str__", streamable_as_str<BooleVariable>)
  .def("__repr__", streamable_as_str<BooleVariable>)
  .def("__hash__", &BooleVariable::hash)
  .def("stable_hash", &BooleMonomial::stableHash, "Reproducible hash code")
  .def("__pow__", var_power)
  .def("index", &BooleVariable::index, "Variable position in the ring")
  .def("set",&BooleVariable::set, "Convert to BooleSet")
  .def("ring", &BooleVariable::ring, "Get corresponding ring");
  boost::python::register_exception_translator<PBoRiError>(translator_pborierror);
  typedef PBoRiGenericError<CTypes::division_by_zero> pbori_div_by_zero;
  boost::python::register_exception_translator<pbori_div_by_zero>(translator_pboridivisionbyzero);

  export_strategy();
  export_fglm();
  export_monomial();
  export_bset();
  export_variable_block();
  export_misc();
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
