/*
 *  Poly_wrapper.cc
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 04.04.06.
 *  Copyright 2006 The PolyBoRi Team. See LICENSE file.
 *
 */
#include <boost/python.hpp>
#include <iostream>
#include "polybori.h"
#include "pbori_defs.h"
#include "groebner_alg.h"
#include "interpolate.h"
#include "out_helper.h"
using namespace boost::python;
using namespace std;
USING_NAMESPACE_PBORI
USING_NAMESPACE_PBORIGB
#include "Poly_wrapper.h"
#include "strategy_wrapper.h"
#include "pbori_algo.h"

static BoolePolynomial multiply_fast(const BoolePolynomial& p, const BoolePolynomial& q){
    typedef CommutativeCacheManager<CCacheTypes::multiply_recursive>
      cache_mgr_type;

    return dd_multiply<true>(cache_mgr_type(p.diagram().manager()), 
                             p.navigation(), q.navigation(),
                             BoolePolynomial()); 
    
}
static BoolePolynomial multiply_traditionally(const BoolePolynomial& p, const BoolePolynomial& q){
    typedef CommutativeCacheManager<CCacheTypes::multiply_recursive>
      cache_mgr_type;

    return dd_multiply<false>(cache_mgr_type(p.diagram().manager()), 
                              p.navigation(), q.navigation(),
                              BoolePolynomial() ); 
    
}
static BoolePolynomial poly_power(const BoolePolynomial& p, int n){
  if (n==0) return BooleMonomial(p.ring());
  return p;
}
static void print_polynomial(const BoolePolynomial & p){
  p.print(cout);
}

static void plot(const BoolePolynomial& p, const char* c){
  p.prettyPrint(c);
}

static BooleSet poly_diagram_as_set(const Polynomial& p){
    return p.diagram();
}

void export_poly(){

  BoolePolynomial::set_type  (BoolePolynomial::*set)(void) const =
    &BoolePolynomial::set;

//  const BoolePolynomial::set_type&  (BoolePolynomial::*set)(void) const =
//    &BoolePolynomial::set;

  boost::python::class_<BoolePolynomial>("Polynomial", 
                                         "Construct a BoolePolynomial object\
   in the given Boolean polynomial ring.")
  .def(init<>())
  .def(init<const BoolePolynomial &>())
  .def(init<const BoolePolynomial::navigator &, 
       const BoolePolynomial::ring_type &>())
  .def(boost::python::init <int, BooleRing>())
  .def(init<const CTypes::dd_type &>())
  .def(init<const BooleVariable &>())
  .def(init<const BooleMonomial &>())
  .def(boost::python::init<bool>())
  .def(boost::python::init<int>())
  .def("__hash__", &BoolePolynomial::hash, "Fast hash code, based on the \
pointer to the underlying ZDD node. \nIt may vary from runtime to runtime.")
  .def("stableHash", &BoolePolynomial::stableHash, "Reproducible hash code")
  .def("__len__", &BoolePolynomial::length, "Number of terms")
  .def("terms", 
       range(&BoolePolynomial::orderedBegin, &BoolePolynomial::orderedEnd))
  .def("__pow__",poly_power)
  //.def(self+=self)
  //.def(self-=self)
  //.def(self*=self)
  //.def(self/self)
  //.def(self/=self)
  //.def(self+=BooleVariable())
  //.def(self-=BooleVariable())
  //.def((BoolePolynomial)BooleVariable()*self)
  //.def(BooleMonomial()*self)
  .def(self*BooleMonomial())
  .def(self+BooleVariable())
  .def(self-BooleVariable())
  .def(BooleVariable()+self)
  .def(BooleVariable()-self)
  .def(self+BooleMonomial())
  .def(self-BooleMonomial())
  .def(BooleMonomial()+self)
  .def(BooleMonomial()-self)
  //.def(self+=BooleMonomial())
  //.def(self-=BooleMonomial())
  //.def(self*=BoolePolynomial())
  //.def(self*=BooleMonomial())
  .def(BooleMonomial()*self)
  //.def(BooleVariable()*self)
  //.def(self*BooleVariable())
  .def(self*self)
  //.def(self*=BooleMonomial())
  //.def(self*=BooleVariable())
  .def(self*BooleVariable())
    //  .def(self/BooleVariable())
  .def(self/int())
  .def(self/BooleMonomial())
  .def(self/self)
  .def(self%BooleVariable())
  .def(self%BooleMonomial())
  .def(self%self)
  .def(int()+self)
  .def(int()-self)
  .def(self+int())
  .def(self-int())
  //.def(self+=int())
  //.def(self-=int())
  //.def(self* BooleVariable())
  .def(self+BooleVariable())
  .def(self-BooleVariable())
  //.def(self*self)
  .def(self==self)
  .def(self!=int())
  .def(self==int())
  .def(self==bool())
  .def(self!=self)
  .def(self!=bool())
  .def(self+self)
  .def(self-self)
  .def("translate_indices", translate_indices)
  .def("is_zero", &BoolePolynomial::isZero, "Test if Polynomial is zero")
  .def("is_one", &BoolePolynomial::isOne, "Test if Polynomial one")

  .def("deg", &BoolePolynomial::deg, "Polynomial degree")
  .def("lm_divisors", &BoolePolynomial::lmDivisors, "Divisors of leading term")
  .def("lead", &BoolePolynomial::lead, "Leading term with respect to current ordering")
  .def("lex_lead", &BoolePolynomial::lexLead, "Lexicographical leading term")

  .def("reducible_by", &BoolePolynomial::reducibleBy)

  .def("lm_deg", &BoolePolynomial::lmDeg, "Degree of the leading term")
  .def("lex_lm_deg", &BoolePolynomial::lexLmDeg, 
       "Degree of the lexicographical leading term")
  .def("constant", &BoolePolynomial::isConstant, 
       "Test, whether Polynomial os constant")
  .def("n_nodes", &BoolePolynomial::nNodes, 
       "Number of diagram nodes in the underlying ZDD structure")
  .def("n_variables", &BoolePolynomial::nUsedVariables, 
       "Number of variables occurring in Polynomial")
  .def("vars_as_monomial",&BoolePolynomial::usedVariables, 
       "Variables occurring in Polynomial")
  .def("total_degree", &BoolePolynomial::totalDeg, "Total Polynomial degree")
  .def("graded_part", &BoolePolynomial::gradedPart, "Get part of given degree")
  //.def("diagram", diagram, return_internal_reference<1>())
  //.def("diagram",poly_diagram_as_set)
  .def("set", set, "Convert to BooleSet")
  .def("ring", &BoolePolynomial::ring, "Get corresponding ring")
  .def("multiply_fast", multiply_fast)
  .def("multiply_traditionally", multiply_traditionally)
  .def("navigation", &BoolePolynomial::navigation, 
       "Navigate through underlying ZDD structure")
  .def("elength", &BoolePolynomial::eliminationLength, "Elimination length")
  .def("has_constant_part", &BoolePolynomial::hasConstantPart,
       "Check, whether Polynomials owns constant term")
  .def("plot",plot)
    //  .def("__len__", &BoolePolynomial::length, "Number of terms")
  .def("__str__", streamable_as_str<BoolePolynomial>)
  .def("__repr__", streamable_as_str<BoolePolynomial>)
  .def("map_every_x_to_x_plus_one",map_every_x_to_x_plus_one, 
       "Replace every variable x by x + 1")
  .def("zeros_in",zeros, 
  "Get BooleSet representing the zeros of the Polynomial");
  //wrap usedVariables
  def("spoly",&spoly, "Compute s-Polynomial between two Polynomials");
  
  //implicitly_convertible<BooleVariable,BooleMonomial>();
  //implicitly_convertible<BooleVariable,BoolePolynomial>();
  
}
