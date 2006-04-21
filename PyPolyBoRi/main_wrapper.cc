#include <boost/python.hpp>
#include <iostream>
#include "polybori.h"
#include "pbori_defs.h"
#include "dd_wrapper.h"
#include "Poly_wrapper.h"
#include "navigator_wrap.h"

//#include "pairs.h"
#include "strategy_wrapper.h"

using namespace boost::python;
using namespace std;
USING_NAMESPACE_PBORI
#define WRAP_ALSO_CUDD 1

void print_variable(const BooleVariable & p){
  ((const BoolePolynomial&) p).print(cout);
}

BOOST_PYTHON_MODULE(PyPolyBoRi){
  
  BoolePolyRing r; //workaround for having a current_ring
  export_dd();
  export_poly();
  export_nav();
  def("print_ring_info", &BoolePolyRing::printInfo);
  boost::python::class_<BoolePolyRing>("Ring")
    //.def(boost::python::init <>())
    .def(boost::python::init <BoolePolyRing::size_type>())
//#ifdef WRAP_ALSO_CUUD
  .def("var", &BoolePolyRing::variable)
  .def("one", &BoolePolyRing::one)
//#endif
  .def("nVars", &BoolePolyRing::nVariables);
 
  boost::python::class_<BooleVariable>("Variable")
  .def(init<const BooleVariable &>())
  .def(init<BooleVariable::idx_type>())
    .def("toStdOut", print_variable);
  export_strategy();
   }
/*


BDD PortToBdd() const;
*/
