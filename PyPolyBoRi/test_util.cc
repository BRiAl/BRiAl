/*
 *  strategy_wrapper.cc
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 21.04.06.
 *  Copyright 2006 The PolyBoRi Team. See LICENSE file.
 *
 */
#include <boost/python.hpp>

#include <BoolePolynomial.h>
#include <COrderedIter.h>
using namespace boost::python;
using namespace std;

//USING_NAMESPACE_PBORIGB
USING_NAMESPACE_PBORI
static unsigned long iterate_ordered(BoolePolynomial& p){
    unsigned long res=0;
    BoolePolynomial::ordered_iterator it=p.orderedBegin();
    BoolePolynomial::ordered_iterator end=p.orderedEnd();
    while(it!=end){
        res++;
        it++;
    }
    return res;
}
static unsigned long iterate_reference_ordered(BoolePolynomial& p){
    unsigned long res=0;
    BoolePolynomial::ordered_iterator it=p.orderedBegin();
    BoolePolynomial::ordered_iterator end=p.orderedEnd();
    BooleMonomial m;
    while(it!=end){
        m=*it;
        res++;
        it++;
    }
    return res;
}

static unsigned long iterate_lex(BoolePolynomial& p){
    unsigned long res=0;
    BoolePolynomial::const_iterator it=p.begin();
    BoolePolynomial::const_iterator end=p.end();
    while(it!=end){
        res++;
        it++;
    }
    return res;
}


void export_test_util(){

  def("test_iterate_lex", iterate_lex);
  def("test_iterate_reference_ordered",iterate_reference_ordered);
  def("test_iterate_ordered",iterate_ordered);

}
