/*  Copyright (c) 2005-2007 by The PolyBoRi Team */

#include <boost/python.hpp>
#include <NTL/GF2.h>
#include <NTL/mat_GF2.h>
// using namespace boost::python;
NTL_CLIENT

static long gauss_(mat_GF2& m){
    return gauss(m);
}
static void setitem(mat_GF2& self, boost::python::tuple t,int v){
    int i=boost::python::extract<int>(t[0]);
    int j=boost::python::extract<int>(t[1]);
    self[i][j]=v;
}
static int getitem(const mat_GF2& self, boost::python::tuple t){
    int i=boost::python::extract<int>(t[0]);
    int j=boost::python::extract<int>(t[1]);
    if (IsZero(self[i][j])) return 0;
    else return 1;
}
static mat_GF2* create (int i,int j){
    return new mat_GF2(INIT_SIZE,i,j);
}
void export_ntl(){
  def("createMatGF2", create, boost::python::return_value_policy<boost::python::manage_new_object>());
  boost::python::class_<mat_GF2>("MatGF2")
  .def(boost::python::init<>())
  
  .def("gauss",gauss_)
  .def("__getitem__", getitem)
  .def("__setitem__",setitem);
}
