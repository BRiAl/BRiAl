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
#include <polybori.h>
#include <polybori/pbori_defs.h>
#include <polybori/groebner/groebner_alg.h>
#include <polybori/groebner/interpolate.h>
#include "out_helper.h"
using namespace boost::python;
using namespace std;
USING_NAMESPACE_PBORI
USING_NAMESPACE_PBORIGB
#include "Poly_wrapper.h"
#include "strategy_wrapper.h"
#include <polybori/routines/pbori_algo.h>

static BoolePolynomial multiply_fast(const BoolePolynomial& p, const BoolePolynomial& q){
    typedef CommutativeCacheManager<CCacheTypes::multiply_recursive>
      cache_mgr_type;

    return dd_multiply<true>(cache_mgr_type(p.ring()), 
                             p.navigation(), q.navigation(),
                             BoolePolynomial(p.ring())); 
    
}
static BoolePolynomial multiply_traditionally(const BoolePolynomial& p, const BoolePolynomial& q){
    typedef CommutativeCacheManager<CCacheTypes::multiply_recursive>
      cache_mgr_type;

    return dd_multiply<false>(cache_mgr_type(p.ring()), 
                              p.navigation(), q.navigation(),
                              BoolePolynomial(p.ring()) ); 
    
}
static BoolePolynomial poly_power(const BoolePolynomial& p, int n){
  if (n==0) return BooleMonomial(p.ring());
  return p;
}
static void print_polynomial(const BoolePolynomial & p){
  p.print(cout);
}


static BooleSet poly_diagram_as_set(const Polynomial& p){
    return p.diagram();
}

void export_poly(){
  typedef BoolePolynomial::ring_type ring_type;
  ring_type dummy_ring(1);
  BoolePolynomial::var_type var(0, dummy_ring);
  BoolePolynomial::monom_type monom(dummy_ring);

  BoolePolynomial::set_type  (BoolePolynomial::*set)(void) const =
    &BoolePolynomial::set;

  boost::python::class_<BoolePolynomial>("Polynomial", 
    "Construct a BoolePolynomial object in the given Boolean polynomial ring.",
   boost::python::init <const ring_type &>())

  .def(init<const BoolePolynomial &>())
  .def(init<const BoolePolynomial::navigator &, const ring_type &>())
  .def(init <bool, const ring_type &>())
  .def(init <int, const ring_type &>())
  .def(init<const BooleSet &>())
  .def(init<const BooleVariable &>())
  .def(init<const BooleMonomial &>())
  .def("__hash__", &BoolePolynomial::hash, "Fast hash code, based on the \
pointer to the underlying ZDD node. \nIt may vary from runtime to runtime.")
  .def("stable_hash", &BoolePolynomial::stableHash, "Reproducible hash code")
  .def("__len__", &BoolePolynomial::length, "Number of terms")
  .def("terms", 
       range(&BoolePolynomial::orderedBegin, &BoolePolynomial::orderedEnd))
  .def("__pow__",poly_power)
  //.def(self+=self)
  //.def(self-=self)
  //.def(self*=self)
  //.def(self/self)
  //.def(self/=self)
  //.def(self+=var)
  //.def(self-=var)
  //.def((BoolePolynomial)var*self)
  //.def(monom*self)
  .def(self*monom)
  .def(self+var)
  .def(self-var)
  .def(var+self)
  .def(var-self)
  .def(self+monom)
  .def(self-monom)
  .def(monom+self)
  .def(monom-self)
  //.def(self+=monom)
  //.def(self-=monom)
  //.def(self*=BoolePolynomial())
  //.def(self*=monom)
  .def(monom*self)
  //.def(var*self)
  //.def(self*var)
  .def(self*self)
  //.def(self*=monom)
  //.def(self*=var)
  .def(self*var)
    //  .def(self/var)
  .def(self/int())
  .def(self/monom)
  .def(self/self)
  .def(self%var)
  .def(self%monom)
  .def(self%self)
  .def(int()+self)
  .def(int()-self)
  .def(self+int())
  .def(self-int())
  .def(int()*self)
  .def(self*int())
  //.def(self+=int())
  //.def(self-=int())
  //.def(self* var)
  .def(self+var)
  .def(self-var)
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
  .def("lead_divisors", &BoolePolynomial::leadDivisors, "Divisors of leading term")
  .def("lead", &BoolePolynomial::lead, "Leading term with respect to current ordering")
  .def("lex_lead", &BoolePolynomial::lexLead, "Lexicographical leading term")

  .def("first_reducible_by", &BoolePolynomial::firstReducibleBy)

  .def("lead_deg", &BoolePolynomial::leadDeg, "Degree of the leading term")
  .def("lex_lead_deg", &BoolePolynomial::lexLeadDeg, 
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
  .def("ring", &BoolePolynomial::ring,
       return_internal_reference<>(),
       "Get corresponding ring")
  .def("multiply_fast", multiply_fast)
  .def("multiply_traditionally", multiply_traditionally)
  .def("navigation", &BoolePolynomial::navigation, 
       "Navigate through underlying ZDD structure")
  .def("elength", &BoolePolynomial::eliminationLength, "Elimination length")
  .def("has_constant_part", &BoolePolynomial::hasConstantPart,
       "Check, whether Polynomials owns constant term")
    //  .def("__len__", &BoolePolynomial::length, "Number of terms")
  .def("__str__", streamable_as_str<BoolePolynomial>)
  .def("__repr__", streamable_as_str<BoolePolynomial>)
  .def("map_every_x_to_x_plus_one",map_every_x_to_x_plus_one, 
       "Replace every variable x by x + 1")
  .def("zeros_in",zeros, 
  "Get BooleSet representing the zeros of the Polynomial")
  .def("__cmp__", &BoolePolynomial::compare, "Comparison of Boolean polynomials");

  def("spoly",&spoly, "Compute s-Polynomial between two Polynomials");
  
  boost::python::class_<PolynomialFactory>("PolynomialFactory", 
    "Constructing BoolePolynomial objects in a predefined ring.",
   boost::python::init <const ring_type &>())

    .def("__call__",
         (Polynomial(PolynomialFactory::*)() const)
         (&PolynomialFactory::operator()))

    .def("__call__",
         (Polynomial(PolynomialFactory::*)(const Polynomial&) const)
         (&PolynomialFactory::operator()))
  
    .def("__call__",
         (Polynomial(PolynomialFactory::*)(const Variable&) const)
         (&PolynomialFactory::operator()))
  
    .def("__call__",
         (Polynomial(PolynomialFactory::*)(const Monomial&) const)
         (&PolynomialFactory::operator()))

    .def("__call__",
         (Polynomial(PolynomialFactory::*)(const BooleExponent&) const)
         (&PolynomialFactory::operator()))

    .def("__call__",
         (Polynomial(PolynomialFactory::*)(BooleConstant) const)
         (&PolynomialFactory::operator()))

    .def("__call__",
         (Polynomial(PolynomialFactory::*)(const Polynomial::navigator&) const)
         (&PolynomialFactory::operator()));


}
