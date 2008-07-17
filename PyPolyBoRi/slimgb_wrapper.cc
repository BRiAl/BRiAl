/*
 *  slimgb.cc
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 02.05.06.
 *  Copyright 2006 The PolyBoRi Team. See LICENSE file.
 *
 */

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include "slimgb_wrapper.h"
#include <iostream>
#include <vector>

#include <nf.h>

#include <groebner_alg.h>
using namespace boost::python;
USING_NAMESPACE_PBORI
USING_NAMESPACE_PBORIGB

void export_slimgb(){

  class_ <std::vector<BoolePolynomial> >("BoolePolynomialVector")
    .def(vector_indexing_suite<std::vector<BoolePolynomial> >())
    .def(self==self);
  class_ <std::vector<int> >("IntVector")
    .def(self==self)
    .def(vector_indexing_suite<std::vector<int> >());
  def("parallel_reduce", parallel_reduce);
 /* def("red_tail", red_tail);*/
  def("nf3", nf3);
  def("mod_mon_set", mod_mon_set);
  def("ll_red_nf_redsb",ll_red_nf);
  def("ll_red_nf_noredsb",ll_red_nf_noredsb);
  def("recursively_insert",recursively_insert);
  def("add_up_polynomials",add_up_polynomials);
  def("mod_mon_set",mod_mon_set);
}

