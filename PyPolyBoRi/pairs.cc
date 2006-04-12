/*
 *  Pairs.cc
 *  PolyBoRi
 *
 *  Created by Michael Brickenstein on 12.04.06.
 *  Copyright 2006 Mathmatisches Forschungsinstitut Oberwolfach. All rights reserved.
 *
 */
#include <boost/python.hpp>
#include <iostream>
#include "polybori.h"
#include "pbori_defs.h"
using namespace boost::python;
using namespace std;
USING_NAMESPACE_PBORI

#include "pairs.h"

class Pair{
public:
  BoolePolynomial p;
  int deg;
  int length;
  Pair(const BoolePolynomial &p, int deg){
    //deg=p.deg();
    this->deg=deg;
    this->p=p;
    length=p.eliminationLength();//p.length();
      cout<<"len: "<<length<<endl;
  }
  Pair(){
    deg=0;
    length=0;
  }
  int getDeg(){
    return deg;
  }
  BoolePolynomial& getPolynomial(){
    return p;
  }
};


#if 0
bool operator<(const Pair &p1, const Pair& p2){
  return p1.deg<p2.deg;
}

bool operator<=(const Pair &p1, const Pair& p2){
  return p1.deg<=p2.deg;

}

bool operator>(const Pair &p1, const Pair& p2){
  return p1.deg>p2.deg;

}
bool operator>=(const Pair &p1, const Pair& p2){
  return p1.deg>=p2.deg;
}


bool operator<(const Pair &p1, const Pair& p2){
  return p1.length<p2.length;
}

bool operator<=(const Pair &p1, const Pair& p2){
  return p1.length<=p2.length;
  
}

bool operator>(const Pair &p1, const Pair& p2){
  return p1.length>p2.length;
  
}
bool operator>=(const Pair &p1, const Pair& p2){
  return p1.length>=p2.length;
}

#endif

bool operator<(const Pair &p1, const Pair& p2){
  if (p1.deg<p2.deg) return true;
  if (p1.deg>p2.deg) return false;
  return p1.length<p2.length;
}

bool operator<=(const Pair &p1, const Pair& p2){
  if (p1.deg<p2.deg) return true;
  if (p1.deg>p2.deg) return false;
  return p1.length<=p2.length;
  
}

bool operator>(const Pair &p1, const Pair& p2){
  if (p1.deg>p2.deg) return true;
  if (p1.deg<p2.deg) return false;
  return p1.length>p2.length;
  
}
bool operator>=(const Pair &p1, const Pair& p2){
  if (p1.deg>p2.deg) return true;
  if (p1.deg<p2.deg) return false;
  return p1.length>=p2.length;
}

void export_pairs(){
  boost::python::class_<Pair>("Pair")
  .def(init<const BoolePolynomial &, int>())
  .def("poly",&Pair::getPolynomial,return_internal_reference<1>())
  .def("deg", &Pair::getDeg)
  .def(self<self)
  .def(self>self)
  .def(self<=self)
  .def(self>=self);
}
  