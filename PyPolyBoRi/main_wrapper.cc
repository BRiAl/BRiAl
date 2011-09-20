/*  Copyright (c) 2005-2007 by The #+PolyBoRi Team */

#include <boost/python.hpp>
#include <iostream>
#include <polybori.h>
#include <polybori/pbori_defs.h>
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
#include <m4ri/m4ri.h>
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


// void print_ring_info() {
//   BooleEnv::print(std::cout);
// }

BoolePolynomial
ring_var(const BoolePolyRing& ring, BooleVariable::idx_type idx) {
  return ring.variable(idx);
}
BoolePolynomial ring_one(const BoolePolyRing& ring) {
  return ring.one();
}
BoolePolynomial ring_zero(const BoolePolyRing& ring) {
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


BoolePolynomial coerce(const BoolePolyRing& ring, const BoolePolynomial& poly){
  return ring.coerce(poly);
}

bool has_degree_order(const BoolePolyRing& ring) {
  return ring.ordering().isDegreeOrder();
}

BoolePolyRing::ordercode_type
get_order_code(const BoolePolyRing& ring) {
  return ring.ordering().getOrderCode();
} 

void
append_block(const BoolePolyRing& ring,
             BoolePolyRing::idx_type next_block_start) {
  return ring.ordering().appendBlock(next_block_start);
} 

BoolePolyRing::order_type::block_iterator
ring_block_begin(const BoolePolyRing& ring) {
  return ring.ordering().blockBegin();
}

BoolePolyRing::order_type::block_iterator
ring_block_end(const BoolePolyRing& ring) {
  return ring.ordering().blockEnd();
}

void export_main() {
  
  BoolePolyRing r(10);
  BooleMonomial monom(r);

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
  implicitly_convertible<BooleConstant,int>();
  implicitly_convertible<BoolePolynomial,BooleSet>();
  implicitly_convertible<BooleSet,BoolePolynomial>();

  export_poly();
  export_nav();
  export_test_util();
  #ifdef HAVE_NTL
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
  //  def("print_ring_info", print_ring_info);


  implicitly_convertible<CCheckedIdx::idx_type, CCheckedIdx>();

  boost::python::class_<BoolePolyRing>("Ring", "Boolean polynomial ring",
                                       boost::python::init <const BoolePolyRing&>()) 
    .def("n_variables", &BoolePolyRing::nVariables, "Number of ring variables")
    .def("__hash__", &BoolePolyRing::hash)
    .def("id", &BoolePolyRing::id)
    .def("var", ring_var, "i-th ring Variable")
    .def("variable", &BoolePolyRing::variable, "i-th ring Variable")
    .def("one", ring_one, "Polynomial one")
    .def("zero", ring_zero, "Polynomial zero")
    .def(boost::python::init <int>())
    .def(boost::python::init <BoolePolyRing::size_type>())
    .def("clone", &BoolePolyRing::clone, "copies also variable name vector in a \
    new one, so somewhat deeper copy function")
    .def("coerce", coerce, "Map polynomial in this ring, if possible.")
    .def("__call__", coerce, "Map polynomial in this ring, if possible.")
    .def(boost::python::init <BoolePolyRing::size_type, int>(
         "Construct a Boolean polynomial ring with the following parameters:\n\
            n -- number of variables (integer)\n\
            order -- term ordering (optinal), one of the following values \n\
                lp: lexicographical ordering (default)\n\
                dlex: degree lexicographical ordering\n\
                dp_asc: degree reverse lexicographical ordering \
with inverted variable order\n\
                block_dp_asc: Block ordering with blocks consisting of dp_asc\n\
                block_dlex: Block ordering with blocks consisting of dlex\n") )

    .def("has_degree_order", has_degree_order,
        "Determines, whether ring ordering is a degree ordering")
    .def("set_variable_name",&BoolePolyRing::setVariableName)
    .def("change_ordering",&BoolePolyRing::changeOrdering)
    .def("get_order_code", get_order_code)
    .def("append_block", append_block,
      "Append integer, which marks the index of the start of the next block (for block orderings)")
    .def("blocks", range(ring_block_begin, ring_block_end));
  

  boost::python::class_<BooleConstant>("BooleConstant", 
                                       "Boolean constant value")
    .def(init<const BooleConstant &>())
    .def(init<int>("Convert integer to Boolean value"))
    .def(init<bool>("Convert bool to Boolean value"))
    .def(self+self)
    .def(self-self)
    .def(self*self)
    .def("__str__", streamable_as_str<BooleConstant>)
    .def("__repr__", streamable_as_str<BooleConstant>)
    .def("variables", range(&BooleConstant::variableBegin, &BooleConstant::variableEnd))
    .def("deg", &BooleConstant::deg)
    .def("is_one", &BooleConstant::isOne)
    .def("is_zero", &BooleConstant::isZero)
    .def("is_constant", &BooleConstant::isConstant)
    .def("has_constant_part", &BooleConstant::hasConstantPart);

  boost::python::class_<BooleVariable>("Variable", "Boolean Variable", init<const BoolePolyRing&>())
  .def(init<const BooleVariable &>())
  .def(init<const BoolePolyRing&>("Get first Variable in a given ring"))
  .def(init<BooleVariable::idx_type, 
       const BoolePolyRing&>("Get Variable of given index in a given ring"))
  .def(self*self)
  .def(self/self)
  .def(self*monom)
  .def(self/monom)
  .def(monom*self)
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
  .def("lead", lead_wrap)
  .def("lex_lead", lex_lead_wrap)
  .def("lex_lead_deg", lex_lead_deg_wrap)
  .def("lead_deg", lead_deg_wrap)
  .def("has_constant_part", has_constant_part_wrap)
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
  .def("ring", &BooleVariable::ring,
       return_internal_reference<>(),
       "Get corresponding ring");

  boost::python::class_<WeakRingPtr>("WeakRingRef",
				     "Weak reference to Boolean polynomial ring",
				     boost::python::init <const BoolePolyRing&>()) 
    .def("deref", &WeakRingPtr::operator*, "Get strong reference")
    .def("is_valid", &WeakRingPtr::operator bool,
	 "Check whether pointer is not dead");


  boost::python::register_exception_translator<PBoRiError>(translator_pborierror);
  typedef PBoRiGenericError<CTypes::division_by_zero> pbori_div_by_zero;
  boost::python::register_exception_translator<pbori_div_by_zero>(translator_pboridivisionbyzero);

}
